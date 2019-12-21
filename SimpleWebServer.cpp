#include "stdafx.h"
#include "SimpleWebServer.h"
#include "iFlyConnect.h"
#include "MenuReader.h"
#include "TextReader.h"

#include "gsxStruct.h"

#include "SimulatorDataStruct.h"

#define KS_METAR_SERVER "tgftp.nws.noaa.gov"
#define KS_METAR_PATH "/data/observations/metar/stations/"
#define KS_TAF_PATH "/data/forecasts/taf/stations/"

#include <winsock2.h>
#include <stdio.h>
#include <string.h>

#include <set>
#include <map>
#include <iostream>

#include <Wt/WServer>
#include <Wt/WSignal>
#include <Wt/WString>

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>

#include "boost/date_time/posix_time/posix_time.hpp"

#include <chrono>

using namespace Wt;

using namespace boost::posix_time;

iFlyConnect iflyconnect;
MenuReader menureader;
TextReader textreader;

SimpleWebServer::SimpleWebServer(WServer& server)
	: server_(server),
	stop_(false)
{
	tempDepMetarString = "";
	tempDepTafString = "";
	tempArrMetarString = "";
	tempArrTafString = "";
	tempInfoString = "( -> )";
	acDepIcao = "";
	acArrIcao = "";
	stateDoor1 = 0;
	stateDoor2 = 0;
	stateDoor3 = 0;
	stateGroundPower = 0;
	stateGroundAir = 0;

	thread_ = boost::thread(boost::bind(&SimpleWebServer::run, this));
}

SimpleWebServer::~SimpleWebServer()
{
	stop_ = true;
	thread_.join();
}

bool SimpleWebServer::connectWtClient(Client *client, const ChatEventCallback& handleEvent)
{
	boost::recursive_mutex::scoped_lock lock(mutex_);

	if (clients_.count(client) == 0) {
		ClientInfo clientInfo;

		clientInfo.sessionId = WApplication::instance()->sessionId();
		clientInfo.eventCallback = handleEvent;

		clients_[client] = clientInfo;

		getInitialState();

		return true;
	}
	else
		return false;
}

bool SimpleWebServer::disconnectWtClient(Client *client)
{
	boost::recursive_mutex::scoped_lock lock(mutex_);

	return clients_.erase(client) == 1;
}

void SimpleWebServer::run()
{
	/*
	* This method simulates changes to the data that happen in a background
	* thread.
	*/

	for (;;) {
		boost::this_thread::sleep(boost::posix_time::seconds(1));

		if (stop_)
		{
			return;
		}

		boost::recursive_mutex::scoped_lock lock(mutex_);

		iflyconnect.MainProcess();

		if (iflyconnect.GetStatus()== TRUE)
		{
			if (simulatorStatus == 0)
			{
				simulatorStatus = 1;
				postEvent(ChatEvent(ChatEvent::Status, 1, 1));
			}

			iflyconnect.GetSimulatorData(&webuisimulatordata);

			SendGSXMenuTextToWidget();
			SendGsxTextTextToWidget();

			SendSimulatorDoorDataToWidget();
			SendSimulatorGroundDataToWidget();
			SendSimulatorIcaoDataToWidget();
			SendSimulatorPostitionDataToWidget();
			SendSimulatorLastWPTDataToWidget();
			SendSimulatorActualWPTDataToWidget();
			SendSimulatorNextWPTDataToWidget();
			SendSimulatorDestinationDataToWidget();
			SendSimulatorOooiDataToWidget();
		}
		else
		{
			if (simulatorStatus == 1)
			{
				simulatorStatus = 0;
				postEvent(ChatEvent(ChatEvent::Status, 0, 0));
			}
		}
	}
}

void SimpleWebServer::getInitialState()
{
	postEvent(ChatEvent(ChatEvent::Status, simulatorStatus));

	postEvent(ChatEvent(ChatEvent::GroundPower, stateGroundPower));
	postEvent(ChatEvent(ChatEvent::GroundAir, stateGroundAir));

	postEvent(ChatEvent(ChatEvent::Door, 1, stateDoor1));
	postEvent(ChatEvent(ChatEvent::Door, 2, stateDoor2));
	postEvent(ChatEvent(ChatEvent::Door, 3, stateDoor3));

	postEvent(ChatEvent(ChatEvent::WxrMetarDep, tempDepMetarString));
	postEvent(ChatEvent(ChatEvent::WxrTafDep, tempDepTafString));
	postEvent(ChatEvent(ChatEvent::WxrMetarArr, tempArrMetarString));
	postEvent(ChatEvent(ChatEvent::WxrTafArr, tempArrTafString));

	postEvent(ChatEvent(ChatEvent::LatLon, currentLat, currentLon));

	postEvent(ChatEvent(ChatEvent::FlightInfo, tempInfoString));
	postEvent(ChatEvent(ChatEvent::Alt, currentAlt));
	postEvent(ChatEvent(ChatEvent::OnGround, currentOnGround));
	postEvent(ChatEvent(ChatEvent::Speed, currentGS));
	postEvent(ChatEvent(ChatEvent::Temperature, currentTemp));

	postEvent(ChatEvent(ChatEvent::Oooi, currentOooiOut, currentOooiOff, currentOooiOn, currentOooiIn));

	postEvent(ChatEvent(ChatEvent::Destination, currentDestinationETA, currentDestinationFuel, currentDestinationDTG));

	postEvent(ChatEvent(ChatEvent::LastWPT, currentLastName, currentLastAltitude, currentLastATA, currentLastFuel));
	postEvent(ChatEvent(ChatEvent::ActualWPT, currentActualName, currentActualETA, currentActualFuel, currentActualDTG));
	postEvent(ChatEvent(ChatEvent::NextWPT, currentNextName, currentNextETA, currentNextFuel, currentNextDTG));

}

void SimpleWebServer::SendGsxTextTextToWidget()
{
	static int _timer;
	static bool _new;

	currentGsxTextText = textreader.GetTextItem();

	if (currentGsxTextText != "")
	{
		postEvent(ChatEvent(ChatEvent::GsxTextText, currentGsxTextText));
		_timer = 0;
		_new = TRUE;
	}
	else
	{
		_timer++;
		if (_timer > 4 && _new == TRUE)
		{
			postEvent(ChatEvent(ChatEvent::GsxTextText, ""));
			_new = FALSE;
		}
	}
}
void SimpleWebServer::SendGSXMenuTextToWidget()
{
	gsxlinesStruct _gsxmenu;
	bool _newdata;
	
	_newdata = menureader.GetMenuItems(&_gsxmenu);

	if (_newdata)
	{
		currentGsxMenuSubject = _gsxmenu.txtMenuSubject; postEvent(ChatEvent(ChatEvent::GsxMenuText, currentGsxMenuSubject, 0, 0));
		currentGsxMenuText1 = _gsxmenu.txtMenuItem1; postEvent(ChatEvent(ChatEvent::GsxMenuText, currentGsxMenuText1,1,0));
		currentGsxMenuText2 = _gsxmenu.txtMenuItem2; postEvent(ChatEvent(ChatEvent::GsxMenuText, currentGsxMenuText2, 2, 0));
		currentGsxMenuText3 = _gsxmenu.txtMenuItem3; postEvent(ChatEvent(ChatEvent::GsxMenuText, currentGsxMenuText3, 3, 0));
		currentGsxMenuText4 = _gsxmenu.txtMenuItem4; postEvent(ChatEvent(ChatEvent::GsxMenuText, currentGsxMenuText4, 4, 0));
		currentGsxMenuText5 = _gsxmenu.txtMenuItem5; postEvent(ChatEvent(ChatEvent::GsxMenuText, currentGsxMenuText5, 5, 0));
		currentGsxMenuText6 = _gsxmenu.txtMenuItem6; postEvent(ChatEvent(ChatEvent::GsxMenuText, currentGsxMenuText6, 6, 0));
		currentGsxMenuText7 = _gsxmenu.txtMenuItem7; postEvent(ChatEvent(ChatEvent::GsxMenuText, currentGsxMenuText7, 7, 0));
		currentGsxMenuText8 = _gsxmenu.txtMenuItem8; postEvent(ChatEvent(ChatEvent::GsxMenuText, currentGsxMenuText8, 8, 0));
		currentGsxMenuText9 = _gsxmenu.txtMenuItem9; postEvent(ChatEvent(ChatEvent::GsxMenuText, currentGsxMenuText9, 9, 0));
		currentGsxMenuText0 = _gsxmenu.txtMenuItem10; postEvent(ChatEvent(ChatEvent::GsxMenuText, currentGsxMenuText0, 10, 0));
	}

}
void SimpleWebServer::SendSimulatorLastWPTDataToWidget()
{
	if (webuisimulatordata.LastWPT_name != "")
	{
		currentLastName = webuisimulatordata.LastWPT_name;
		currentLastAltitude = webuisimulatordata.LastWPT_Altitude;

		if (webuisimulatordata.LastWPT_ATA > -1)
		{
			ptime t(second_clock::universal_time());
			currentLastATA = to_simple_string(t.time_of_day());
		}
		else
		{
			currentLastATA = "N/A";
		}
		currentLastFuel = webuisimulatordata.LastWPT_Fuel;

	}
	else
	{
		currentLastName = "N/A";
		currentLastAltitude = 0;
		currentLastATA = "00:00:00";
		currentLastFuel = 0;
	}

	if (prevLastName != currentLastName || prevLastAltitude != currentLastAltitude || prevLastFuel != currentLastFuel)
	{
		postEvent(ChatEvent(ChatEvent::LastWPT, currentLastName, currentLastAltitude, currentLastATA, currentLastFuel));
		prevLastName = currentLastName;
		prevLastAltitude = currentLastAltitude;
		prevLastATA = currentLastATA;
		prevLastFuel = currentLastFuel;
	}
}

void SimpleWebServer::SendSimulatorActualWPTDataToWidget()
{
	if (webuisimulatordata.ActWPT_name != "")
	{
		currentActualName = webuisimulatordata.ActWPT_name;

		if (webuisimulatordata.ActWPT_ETA > -1)
		{
			if (tempActWPT_ETA != webuisimulatordata.ActWPT_ETA)
			{
				tempActWPT_ETA = webuisimulatordata.ActWPT_ETA;
				ptime t(second_clock::universal_time());
				time_duration actual_eta_seconds = seconds(webuisimulatordata.ActWPT_ETA);
				auto actual_eta = t + actual_eta_seconds;
				currentActualETA = to_simple_string(actual_eta.time_of_day());
			}
		}
		else
		{
			currentActualETA = "N/A";
		}
		currentActualFuel = webuisimulatordata.ActWPT_Fuel;
		currentActualDTG = webuisimulatordata.ActWPT_DTG;
	}
	else
	{
		currentActualName = "N/A";
		currentActualETA = "00:00:00";
		currentActualFuel = 0;
		currentActualDTG = 0;
	}

	if (prevActualName != currentActualName || prevActualETA != currentActualETA || prevActualFuel != currentActualFuel || prevActualDTG != currentActualDTG)
	{
		postEvent(ChatEvent(ChatEvent::ActualWPT, currentActualName, currentActualETA, currentActualFuel, currentActualDTG));
		prevActualName = currentActualName;
		prevActualETA = currentActualETA;
		prevActualFuel = currentActualFuel;
		prevActualDTG = currentActualDTG;
	}
}

void SimpleWebServer::SendSimulatorNextWPTDataToWidget()
{
	if (webuisimulatordata.NextWPT_name != "")
	{
		currentNextName = webuisimulatordata.NextWPT_name;

		if (webuisimulatordata.NextWPT_ETA > -1)
		{
			if (tempNextWPT_ETA != webuisimulatordata.ActWPT_ETA)
			{
				tempNextWPT_ETA = webuisimulatordata.ActWPT_ETA;
				ptime t(second_clock::universal_time());
				time_duration next_eta_seconds = seconds(webuisimulatordata.NextWPT_ETA);
				auto next_eta = t + next_eta_seconds;
				currentNextETA = to_simple_string(next_eta.time_of_day());
			}
		}
		else
		{
			currentNextETA = "N/A";
		}
		currentNextFuel = webuisimulatordata.NextWPT_Fuel;
		currentNextDTG = webuisimulatordata.NextWPT_DTG;
	}
	else
	{
		currentNextName = "N/A";
		currentNextETA = "00:00:00";
		currentNextFuel = 0;
		currentNextDTG = 0;
	}

	if (prevNextName != currentNextName || prevNextETA != currentNextETA || prevNextFuel != currentNextFuel || prevNextDTG != currentNextDTG)
	{
		postEvent(ChatEvent(ChatEvent::NextWPT, currentNextName, currentNextETA, currentNextFuel, currentNextDTG));
		prevNextName = currentNextName;
		prevNextETA = currentNextETA;
		prevNextFuel = currentNextFuel;
		prevNextDTG = currentNextDTG;
	}
}

void SimpleWebServer::SendSimulatorDestinationDataToWidget()
{
	if (webuisimulatordata.DestinationAirport_Avail == TRUE)
	{
		if (webuisimulatordata.DestinationAirport_Coord_ETA > -1)
		{
			if (tempDestinationWPT_ETA != webuisimulatordata.DestinationAirport_Coord_ETA)
			{
				tempDestinationWPT_ETA = webuisimulatordata.DestinationAirport_Coord_ETA;
				ptime t(second_clock::universal_time());
				time_duration next_eta_seconds = seconds(webuisimulatordata.DestinationAirport_Coord_ETA);
				auto next_eta = t + next_eta_seconds;
				currentDestinationETA = to_simple_string(next_eta.time_of_day());
			}
		}
		else
		{
			currentDestinationETA = "N/A";
		}
		currentDestinationFuel = webuisimulatordata.DestinationAirport_Coord_Fuel;
		currentDestinationDTG = webuisimulatordata.DestinationAirport_Coord_DTG;

	}
	else
	{
		currentDestinationETA = "00:00:00";
		currentDestinationFuel = 0;
		currentDestinationDTG = 0;
	}

	if (prevDestinationETA != currentDestinationETA || prevDestinationFuel != currentDestinationFuel || prevDestinationDTG != currentDestinationDTG)
	{
		postEvent(ChatEvent(ChatEvent::Destination, currentDestinationETA, currentDestinationFuel, currentDestinationDTG));
		prevDestinationETA = currentDestinationETA;
		prevDestinationFuel = currentDestinationFuel;
		prevDestinationDTG = currentDestinationDTG;
	}
}

void SimpleWebServer::SendSimulatorPostitionDataToWidget()
{
	currentLat = webuisimulatordata.Plane_Location_Lat;
	currentLon = webuisimulatordata.Plane_Location_Lon;
	currentAlt = (int)webuisimulatordata.Altitude;
	currentOnGround = webuisimulatordata.AIR_GND;
	currentTemp = (int)webuisimulatordata.Ambient_Temperature;
	currentGS = (int)webuisimulatordata.Airspeed_GS;

	if (prevLat != currentLat || prevLon != currentLon)
	{
		postEvent(ChatEvent(ChatEvent::LatLon, currentLat, currentLon));
		prevLat = currentLat;
		prevLon = currentLon;
	}

	if (prevTemp != currentTemp)
	{
		postEvent(ChatEvent(ChatEvent::Temperature, currentTemp));
		prevTemp = currentTemp;
	}

	if (prevGS != currentGS)
	{
		postEvent(ChatEvent(ChatEvent::Speed, currentGS));
		prevGS = currentGS;
	}


	if (prevAlt != currentAlt)
	{
		postEvent(ChatEvent(ChatEvent::Alt, currentAlt));
		prevAlt = currentAlt;
	}

	if (prevOnGround != currentOnGround)
	{
		postEvent(ChatEvent(ChatEvent::OnGround,currentOnGround));
		prevOnGround = currentOnGround;
	}
}
void SimpleWebServer::SendSimulatorDoorDataToWidget()
{
	stateDoor1 = webuisimulatordata.Door1;
	if (stateDoor1 != prevDoor1)
	{
		postEvent(ChatEvent(ChatEvent::Door, 1, stateDoor1));
		prevDoor1 = stateDoor1;
	}

	stateDoor2 = webuisimulatordata.Door2;
	if (stateDoor2 != prevDoor2)
	{
		postEvent(ChatEvent(ChatEvent::Door, 2, stateDoor2));
		prevDoor2 = stateDoor2;
	}
	stateDoor3 = webuisimulatordata.Door3;

	if (stateDoor3 != prevDoor3)
	{
		postEvent(ChatEvent(ChatEvent::Door, 3, stateDoor3));
		prevDoor3 = stateDoor3;
	}
}

void SimpleWebServer::SendSimulatorGroundDataToWidget()
{
	stateGroundPower = webuisimulatordata.GroundSupply_Power;
	stateGroundAir = webuisimulatordata.GroundSupply_Air;

	if (prevGroundPower != stateGroundPower)
	{
		postEvent(ChatEvent(ChatEvent::GroundPower, stateGroundPower));
		prevGroundPower = stateGroundPower;
	}

	if (prevGroundAir != stateGroundAir)
	{
		postEvent(ChatEvent(ChatEvent::GroundAir, stateGroundAir));
		prevGroundAir = stateGroundAir;
	}
}

void SimpleWebServer::SendSimulatorIcaoDataToWidget()
{
	tempIcao = webuisimulatordata.ICAO_ORIGIN_AIRPORT;
	if (tempIcao != acDepIcao)
	{
		acDepIcao = tempIcao;

		tempInfoString = acFlightNumber + "  (" + acDepIcao + " - " + acArrIcao + " )";
		tempInfoStringWt = tempInfoString;
		postEvent(ChatEvent(ChatEvent::FlightInfo, tempInfoStringWt));

		if (tempIcao.length() == 4)
		{
			wxrStatus = GetWeather(1, acDepIcao, tempWeather);
			tempDepMetarString = tempWeather;
			postEvent(ChatEvent(ChatEvent::WxrMetarDep, tempDepMetarString));

			wxrStatus = GetWeather(2, acDepIcao, tempWeather);
			tempDepTafString = tempWeather;
			postEvent(ChatEvent(ChatEvent::WxrTafDep, tempDepTafString));
		}
	}

	tempIcao = webuisimulatordata.ICAO_DEST_AIRPORT;
	if (tempIcao != acArrIcao)
	{
		acArrIcao = tempIcao;

		tempInfoString = acFlightNumber + " ( " + acDepIcao + " - " + acArrIcao + " )";
		tempInfoStringWt = tempInfoString;
		postEvent(ChatEvent(ChatEvent::FlightInfo, tempInfoStringWt));

		if (tempIcao.length() == 4)
		{
			wxrStatus = GetWeather(1, acArrIcao, tempWeather);
			tempArrMetarString = tempWeather;
			postEvent(ChatEvent(ChatEvent::WxrMetarArr, tempArrMetarString));

			wxrStatus = GetWeather(2, acArrIcao, tempWeather);
			tempArrTafString = tempWeather;
			postEvent(ChatEvent(ChatEvent::WxrTafArr, tempArrTafString));
		}
	}

	tempFlightNumber = webuisimulatordata.FLT_NO;
	if (tempFlightNumber != acFlightNumber)
	{
		acFlightNumber = tempFlightNumber;

		tempInfoString = acFlightNumber + " ( " + acDepIcao + " - " + acArrIcao + " )";
		tempInfoStringWt = tempInfoString;
		postEvent(ChatEvent(ChatEvent::FlightInfo, tempInfoStringWt));
	}
}

void SimpleWebServer::SendSimulatorOooiDataToWidget()
{
	// check here what state flight is and update OOOI times when necessary
	if (flightState == FLIGHTSTATE_PREFLIGHT && webuisimulatordata.Parking_Brake_Lever_Status == 0 && webuisimulatordata.IS_FMC_PREFLIGHT_COMPLETED == TRUE && webuisimulatordata.Door1 == 0)
	{
		flightState = FLIGHTSTATE_PB_OR_TAXI_OUT;
		ptime t(second_clock::universal_time());
		currentOooiOut = to_simple_string(t.time_of_day());
	}

	if (flightState == FLIGHTSTATE_PB_OR_TAXI_OUT && webuisimulatordata.AIR_GND == FALSE)
	{
		flightState = FLIGHTSTATE_ON_AIR;
		ptime t(second_clock::universal_time());
		currentOooiOff = to_simple_string(t.time_of_day());
	}

	if (flightState == FLIGHTSTATE_ON_AIR && webuisimulatordata.AIR_GND == TRUE)
	{
		flightState = FLIGHTSTATE_TAXI_IN;
		ptime t(second_clock::universal_time());
		currentOooiOn = to_simple_string(t.time_of_day());
	}

	// bounced landing.. huh
	if (flightState == FLIGHTSTATE_TAXI_IN && webuisimulatordata.AIR_GND == FALSE)
	{
		flightState = FLIGHTSTATE_ON_AIR;
		currentOooiOn = "N/A";
	}

	if (flightState == FLIGHTSTATE_TAXI_IN && webuisimulatordata.Parking_Brake_Lever_Status == 1)
	{
		flightState = FLIGHTSTATE_POSTFLIGHT;
		ptime t(second_clock::universal_time());
		currentOooiIn = to_simple_string(t.time_of_day());
	}

	if (flightState == FLIGHTSTATE_POSTFLIGHT && webuisimulatordata.Door1 == 1)
	{
		flightState = FLIGHTSTATE_COMPLETED;
	}

	if (flightState == FLIGHTSTATE_COMPLETED && webuisimulatordata.Door1 == 0)
	{
		flightState = FLIGHTSTATE_PREFLIGHT;
		currentOooiOut = "N/A";
		currentOooiOff = "N/A";
		currentOooiOn = "N/A";
		currentOooiIn = "N/A";
	}


	// if changed then send all OOOI info to ui
	if (currentOooiOut != prevOooiOut || currentOooiOff != prevOooiOff || currentOooiOn != prevOooiOn || currentOooiIn != prevOooiIn)
	{
		postEvent(ChatEvent(ChatEvent::Oooi, currentOooiOut, currentOooiOff, currentOooiOn, currentOooiIn));
		prevOooiOut = currentOooiOut;
		prevOooiOff = currentOooiOff;
		prevOooiOn = currentOooiOn;
		prevOooiIn = currentOooiIn;
	}
}

void SimpleWebServer::postEvent(const ChatEvent& event)
{
	boost::recursive_mutex::scoped_lock lock(mutex_);

	WApplication *app = WApplication::instance();

	for (ClientMap::const_iterator i = clients_.begin(); i != clients_.end();
		++i) {
		/*
		* For other uses, we post it to their session. By posting the
		* event, we avoid dead-lock scenarios, race conditions, and
		* delivering the event to a session that is just about to be
		* terminated.
		*/
		server_.post(i->second.sessionId,
			boost::bind(i->second.eventCallback, event));
	}
}

void SimpleWebServer::sendButtonPress(int pressButtonValue)
{
	if (stop_)
	{
		return;
	}

	boost::recursive_mutex::scoped_lock lock(mutex_);

	if (pressButtonValue == ID_WEBUI_TIMER_TIMEOUT)
	{
		if (acDepIcao.length() == 4)
		{
			wxrStatus = GetWeather(1, acDepIcao, tempWeather);
			tempDepMetarString = tempWeather;
			postEvent(ChatEvent(ChatEvent::WxrMetarDep, tempDepMetarString));

			wxrStatus = GetWeather(2, acDepIcao, tempWeather);
			tempDepTafString = tempWeather;
			postEvent(ChatEvent(ChatEvent::WxrTafDep, tempDepTafString));
		}

		if (acArrIcao.length() == 4)
		{
			wxrStatus = GetWeather(1, acArrIcao, tempWeather);
			tempArrMetarString = tempWeather;
			postEvent(ChatEvent(ChatEvent::WxrMetarArr, tempArrMetarString));

			wxrStatus = GetWeather(2, acArrIcao, tempWeather);
			tempArrTafString = tempWeather;
			postEvent(ChatEvent(ChatEvent::WxrTafArr, tempArrTafString));
		}
	}



	if (pressButtonValue == ID_WEBUI_BUTTON_GROUND_POWER)
	{
		iflyconnect.ToggleGroundPower();
	}

	if (pressButtonValue == ID_WEBUI_BUTTON_GROUND_AIR)
	{
		iflyconnect.ToggleGroundAir();
	}

	if (pressButtonValue == ID_WEBUI_BUTTON_DOOR1)
	{
		iflyconnect.ToggleDoor(ID_WEBUI_BUTTON_DOOR1);
	}

	if (pressButtonValue == ID_WEBUI_BUTTON_DOOR2)
	{
		iflyconnect.ToggleDoor(ID_WEBUI_BUTTON_DOOR2);
	}

	if (pressButtonValue == ID_WEBUI_BUTTON_DOOR3)
	{
		iflyconnect.ToggleDoor(ID_WEBUI_BUTTON_DOOR3);
	}

	if (pressButtonValue == ID_WEBUI_BUTTON_PUSHBACK1)
	{
		iflyconnect.TogglePushBack(ID_WEBUI_BUTTON_PUSHBACK1);
	}

	if (pressButtonValue == ID_WEBUI_BUTTON_PUSHBACK2)
	{
		iflyconnect.TogglePushBack(ID_WEBUI_BUTTON_PUSHBACK2);
	}

	if (pressButtonValue == ID_WEBUI_BUTTON_PUSHBACK3)
	{
		iflyconnect.TogglePushBack(ID_WEBUI_BUTTON_PUSHBACK3);
	}

	if (pressButtonValue == ID_WEBUI_BUTTON_PUSHBACK4)
	{
		iflyconnect.TogglePushBack(ID_WEBUI_BUTTON_PUSHBACK4);
	}

	if (pressButtonValue == ID_WEBUI_BUTTON_RESET_OOOI)
	{
		currentOooiOut="N/A";
		currentOooiOff = "N/A";
		currentOooiOn = "N/A";
		currentOooiIn = "N/A";

		flightState = FLIGHTSTATE_PREFLIGHT;

		postEvent(ChatEvent(ChatEvent::Oooi, currentOooiOut, currentOooiOff, currentOooiOn, currentOooiIn));
		prevOooiOut = currentOooiOut;
		prevOooiOff = currentOooiOff;
		prevOooiOn = currentOooiOn;
		prevOooiIn = currentOooiIn;

	}

	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_0)	{ iflyconnect.SendVirtualKey(48); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_1)	{ iflyconnect.SendVirtualKey(49); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_2)	{ iflyconnect.SendVirtualKey(50); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_3)	{ iflyconnect.SendVirtualKey(51); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_4)	{ iflyconnect.SendVirtualKey(52); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_5)	{ iflyconnect.SendVirtualKey(53); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_6)	{ iflyconnect.SendVirtualKey(54); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_7)	{ iflyconnect.SendVirtualKey(55); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_8)	{ iflyconnect.SendVirtualKey(56); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_9)	{ iflyconnect.SendVirtualKey(57); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_F12)	{ iflyconnect.SendVirtualKey(123); }

	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_PROC1) { StartExternalProcess(1); }

}

void SimpleWebServer::StartExternalProcess(int _key)
{


	if (startup_status1_>0)
	{
		PostThreadMessage(pi.dwThreadId, WM_CLOSE, NULL, NULL);
		startup_status1_ = 0;
	}
	else
	{
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		startup_status1_ = CreateProcess(NULL, "E:\\AS_P3Dv4\\AS_P3Dv4.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	}


}
int SimpleWebServer::GetWeather(int _wxrType, std::string& _icao, std::string& _resultWxr)
{

	SOCKET udtSocket;
	SOCKADDR_IN udtAddress;
	WSADATA udtWsa;
	LPHOSTENT udtHost;
	_TCHAR acTmp[1024];
	_TCHAR acData[1024];
	_TCHAR acResult[1024];
	_TCHAR *pcStart;
	_TCHAR *pcEnd;

	acTmp[0] = _T('\0');
	acData[0] = _T('\0');

	int _receivedBytes;

	if (stop_)
	{
		return(-9);
	}

	if (WSAStartup(MAKEWORD(1, 0), &udtWsa) != 0)
	{
		return(-1);
	}

	udtHost = gethostbyname(KS_METAR_SERVER);
	if (!udtHost)
	{
		return(-2);
	}

	udtSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (udtSocket == INVALID_SOCKET)
	{
		return(-3);
	}

	if (!udtHost->h_addr_list)
	{
		return(-4);
	}

	udtAddress.sin_addr = *((LPIN_ADDR)*udtHost->h_addr_list);
	udtAddress.sin_family = AF_INET;
	udtAddress.sin_port = htons(80);

	if (connect(udtSocket, (LPSOCKADDR)&udtAddress, sizeof(udtAddress)) == SOCKET_ERROR)
	{
		return(-5);
	}

	if (_icao.length() != 4)
	{
		_icao = "EFHK";
	}

	char * cstr = new char[_icao.length() + 1];
	std::strcpy(cstr, _icao.c_str());

	switch (_wxrType)
	{
	case 1:
	{
		wsprintf(acTmp, "GET http://" KS_METAR_SERVER KS_METAR_PATH "%hs.TXT HTTP/1.0\n\n", cstr);
		break;
	}
	case 2:
	{
		wsprintf(acTmp, "GET http://" KS_METAR_SERVER KS_TAF_PATH "%hs.TXT HTTP/1.0\n\n", cstr);
		break;
	}
	break;
	}

	delete[] cstr;


	if (send(udtSocket, acTmp, lstrlen(acTmp), 0) == SOCKET_ERROR)
	{
		return(-6);
	}

	_receivedBytes = recv(udtSocket, acData, sizeof(acData), 0);

	if (_receivedBytes == SOCKET_ERROR)
	{
		return(-7);
	}

	closesocket(udtSocket);
	WSACleanup();

	//Read only received bytes
	lstrcpyn(acResult, acData, _receivedBytes);

	//Remove HTTP headers
	pcStart = strstr(acResult, "\r\n\r\n");
	if (pcStart)
	{
		lstrcpyn(acResult, pcStart + 4, acResult - pcStart + 4);
	}

	//Remove Datetime
	pcStart = strstr(acResult, "\n");
	if (pcStart)
	{
		lstrcpyn(acResult, pcStart + 1, acResult - pcStart + 1);
	}

	_resultWxr = acResult;
	return(0);
}
