#include "pch.h"

#include "Resource.h"

#include "WebUiServer.h"

#include "iFlyConnect.h"
#include "MenuReader.h"
#include "TextReader.h"

#define KS_METAR_SERVER "tgftp.nws.noaa.gov"
#define KS_METAR_PATH "/data/observations/metar/stations/"
#define KS_TAF_PATH "/data/forecasts/taf/stations/"

using namespace Wt;
using namespace boost::posix_time;
using namespace std::chrono;

iFlyConnect iflyconnect;
MenuReader menureader;
TextReader textreader;

WebUiServer::WebUiServer(Wt::WServer& server)
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

   thread_ = boost::thread(boost::bind(&WebUiServer::run, this));

}

WebUiServer::~WebUiServer()
{
    stop_ = true;
    //thread_.join();
}

bool WebUiServer::connectClient(Client *client,
			       const WebUiEventCallback& handleEvent)
{
  std::unique_lock<std::recursive_mutex> lock(mutex_);

  if (clients_.count(client) == 0) {
    ClientInfo clientInfo;
  
    clientInfo.sessionId = Wt::WApplication::instance()->sessionId();
    clientInfo.eventCallback = handleEvent;

    clients_[client] = clientInfo;

	getInitialState();

    return true;
  } else
    return false;
}

bool WebUiServer::disconnectClient(Client *client)
{
  std::unique_lock<std::recursive_mutex> lock(mutex_);

  return clients_.erase(client) == 1;
}

void WebUiServer::postWebUiEvent(const WebUiEvent& event)
{
  std::unique_lock<std::recursive_mutex> lock(mutex_);

  Wt::WApplication *app = Wt::WApplication::instance();

  for (ClientMap::const_iterator i = clients_.begin(); i != clients_.end();
       ++i) {

      server_.post(i->second.sessionId,
          std::bind(i->second.eventCallback, event));
  }
}

void WebUiServer::run()
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

		std::unique_lock<std::recursive_mutex> lock(mutex_);

		iflyconnect.MainProcess();

		if (iflyconnect.GetStatus() == TRUE)
		{
			if (simulatorStatus == 0)
			{
				simulatorStatus = 1;
				postWebUiEvent(WebUiEvent(WebUiEvent::Status, 1, 1));
			}

			iflyconnect.GetSimulatorData(&webuisimulatordata_);

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
				postWebUiEvent(WebUiEvent(WebUiEvent::Status, 0, 0));
			}
		}
	}
}

void WebUiServer::getInitialState()
{
	postWebUiEvent(WebUiEvent(WebUiEvent::Status, simulatorStatus));

	postWebUiEvent(WebUiEvent(WebUiEvent::GroundPower, stateGroundPower));
	postWebUiEvent(WebUiEvent(WebUiEvent::GroundAir, stateGroundAir));

	postWebUiEvent(WebUiEvent(WebUiEvent::Door, 1, stateDoor1));
	postWebUiEvent(WebUiEvent(WebUiEvent::Door, 2, stateDoor2));
	postWebUiEvent(WebUiEvent(WebUiEvent::Door, 3, stateDoor3));

	postWebUiEvent(WebUiEvent(WebUiEvent::WxrMetarDep, tempDepMetarString));
	postWebUiEvent(WebUiEvent(WebUiEvent::WxrTafDep, tempDepTafString));
	postWebUiEvent(WebUiEvent(WebUiEvent::WxrMetarArr, tempArrMetarString));
	postWebUiEvent(WebUiEvent(WebUiEvent::WxrTafArr, tempArrTafString));

	postWebUiEvent(WebUiEvent(WebUiEvent::LatLon, currentLat, currentLon));

	postWebUiEvent(WebUiEvent(WebUiEvent::FlightInfo, tempInfoString));
	postWebUiEvent(WebUiEvent(WebUiEvent::Alt, currentAlt));
	postWebUiEvent(WebUiEvent(WebUiEvent::OnGround, currentOnGround));
	postWebUiEvent(WebUiEvent(WebUiEvent::Speed, currentGS));
	postWebUiEvent(WebUiEvent(WebUiEvent::Temperature, currentTemp));

	postWebUiEvent(WebUiEvent(WebUiEvent::Oooi, currentOooiOut, currentOooiOff, currentOooiOn, currentOooiIn));

	postWebUiEvent(WebUiEvent(WebUiEvent::Destination, currentDestinationETA, currentDestinationFuel, currentDestinationDTG));

	postWebUiEvent(WebUiEvent(WebUiEvent::LastWPT, currentLastName, currentLastAltitude, currentLastATA, currentLastFuel));
	postWebUiEvent(WebUiEvent(WebUiEvent::ActualWPT, currentActualName, currentActualETA, currentActualFuel, currentActualDTG));
	postWebUiEvent(WebUiEvent(WebUiEvent::NextWPT, currentNextName, currentNextETA, currentNextFuel, currentNextDTG));

}

void WebUiServer::SendGsxTextTextToWidget()
{
	static int _timer;
	static bool _new;

	currentGsxTextText = textreader.GetTextItem();

	if (currentGsxTextText != "")
	{
		//postWebUiEvent(WebUiEvent(WebUiEvent::GsxTextText, currentGsxTextText));
		_timer = 0;
		_new = TRUE;
	}
	else
	{
		_timer++;
		if (_timer > 4 && _new == TRUE)
		{
			//postWebUiEvent(WebUiEvent(WebUiEvent::GsxTextText, ""));
			_new = FALSE;
		}
	}
}
void WebUiServer::SendGSXMenuTextToWidget()
{
	gsxlinesStruct _gsxmenu;
	bool _newdata;

	_newdata = menureader.GetMenuItems(&_gsxmenu);

	if (_newdata)
	{
		//currentGsxMenuSubject = _gsxmenu.txtMenuSubject; postWebUiEvent(WebUiEvent(WebUiEvent::GsxMenuText, currentGsxMenuSubject, 0, 0));
		//currentGsxMenuText1 = _gsxmenu.txtMenuItem1; postWebUiEvent(WebUiEvent(WebUiEvent::GsxMenuText, currentGsxMenuText1, 1, 0));
		//currentGsxMenuText2 = _gsxmenu.txtMenuItem2; postWebUiEvent(WebUiEvent(WebUiEvent::GsxMenuText, currentGsxMenuText2, 2, 0));
		//currentGsxMenuText3 = _gsxmenu.txtMenuItem3; postWebUiEvent(WebUiEvent(WebUiEvent::GsxMenuText, currentGsxMenuText3, 3, 0));
		//currentGsxMenuText4 = _gsxmenu.txtMenuItem4; postWebUiEvent(WebUiEvent(WebUiEvent::GsxMenuText, currentGsxMenuText4, 4, 0));
		//currentGsxMenuText5 = _gsxmenu.txtMenuItem5; postWebUiEvent(WebUiEvent(WebUiEvent::GsxMenuText, currentGsxMenuText5, 5, 0));
		//currentGsxMenuText6 = _gsxmenu.txtMenuItem6; postWebUiEvent(WebUiEvent(WebUiEvent::GsxMenuText, currentGsxMenuText6, 6, 0));
		//currentGsxMenuText7 = _gsxmenu.txtMenuItem7; postWebUiEvent(WebUiEvent(WebUiEvent::GsxMenuText, currentGsxMenuText7, 7, 0));
		//currentGsxMenuText8 = _gsxmenu.txtMenuItem8; postWebUiEvent(WebUiEvent(WebUiEvent::GsxMenuText, currentGsxMenuText8, 8, 0));
		//currentGsxMenuText9 = _gsxmenu.txtMenuItem9; postWebUiEvent(WebUiEvent(WebUiEvent::GsxMenuText, currentGsxMenuText9, 9, 0));
		//currentGsxMenuText0 = _gsxmenu.txtMenuItem10; postWebUiEvent(WebUiEvent(WebUiEvent::GsxMenuText, currentGsxMenuText0, 10, 0));
	}

}
void WebUiServer::SendSimulatorLastWPTDataToWidget()
{
	if (webuisimulatordata_.LastWPT_name != "")
	{
		currentLastName = webuisimulatordata_.LastWPT_name;
		currentLastAltitude = webuisimulatordata_.LastWPT_Altitude;

		if (webuisimulatordata_.LastWPT_ATA > -1)
		{
			ptime t(second_clock::universal_time());
			currentLastATA = to_simple_string(t.time_of_day());
		}
		else
		{
			currentLastATA = "N/A";
		}
		currentLastFuel = webuisimulatordata_.LastWPT_Fuel;

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
		postWebUiEvent(WebUiEvent(WebUiEvent::LastWPT, currentLastName, currentLastAltitude, currentLastATA, currentLastFuel));
		prevLastName = currentLastName;
		prevLastAltitude = currentLastAltitude;
		prevLastATA = currentLastATA;
		prevLastFuel = currentLastFuel;
	}
}

void WebUiServer::SendSimulatorActualWPTDataToWidget()
{
	if (webuisimulatordata_.ActWPT_name != "")
	{
		currentActualName = webuisimulatordata_.ActWPT_name;

		if (webuisimulatordata_.ActWPT_ETA > -1)
		{
			if (tempActWPT_ETA != webuisimulatordata_.ActWPT_ETA)
			{
				tempActWPT_ETA = webuisimulatordata_.ActWPT_ETA;
				ptime t(second_clock::universal_time());
				time_duration actual_eta_seconds = boost::posix_time::seconds(webuisimulatordata_.ActWPT_ETA);
				auto actual_eta = t + actual_eta_seconds;
				currentActualETA = to_simple_string(actual_eta.time_of_day());
			}
		}
		else
		{
			currentActualETA = "N/A";
		}
		currentActualFuel = webuisimulatordata_.ActWPT_Fuel;
		currentActualDTG = webuisimulatordata_.ActWPT_DTG;
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
		postWebUiEvent(WebUiEvent(WebUiEvent::ActualWPT, currentActualName, currentActualETA, currentActualFuel, currentActualDTG));
		prevActualName = currentActualName;
		prevActualETA = currentActualETA;
		prevActualFuel = currentActualFuel;
		prevActualDTG = currentActualDTG;
	}
}

void WebUiServer::SendSimulatorNextWPTDataToWidget()
{
	if (webuisimulatordata_.NextWPT_name != "")
	{
		currentNextName = webuisimulatordata_.NextWPT_name;

		if (webuisimulatordata_.NextWPT_ETA > -1)
		{
			if (tempNextWPT_ETA != webuisimulatordata_.ActWPT_ETA)
			{
				tempNextWPT_ETA = webuisimulatordata_.ActWPT_ETA;
				ptime t(second_clock::universal_time());
				time_duration next_eta_seconds = boost::posix_time::seconds(webuisimulatordata_.NextWPT_ETA);
				auto next_eta = t + next_eta_seconds;
				currentNextETA = to_simple_string(next_eta.time_of_day());
			}
		}
		else
		{
			currentNextETA = "N/A";
		}
		currentNextFuel = webuisimulatordata_.NextWPT_Fuel;
		currentNextDTG = webuisimulatordata_.NextWPT_DTG;
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
		postWebUiEvent(WebUiEvent(WebUiEvent::NextWPT, currentNextName, currentNextETA, currentNextFuel, currentNextDTG));
		prevNextName = currentNextName;
		prevNextETA = currentNextETA;
		prevNextFuel = currentNextFuel;
		prevNextDTG = currentNextDTG;
	}
}

void WebUiServer::SendSimulatorDestinationDataToWidget()
{
	if (webuisimulatordata_.DestinationAirport_Avail == TRUE)
	{
		if (webuisimulatordata_.DestinationAirport_Coord_ETA > -1)
		{
			if (tempDestinationWPT_ETA != webuisimulatordata_.DestinationAirport_Coord_ETA)
			{
				tempDestinationWPT_ETA = webuisimulatordata_.DestinationAirport_Coord_ETA;
				ptime t(second_clock::universal_time());
				time_duration next_eta_seconds = boost::posix_time::seconds((int)webuisimulatordata_.DestinationAirport_Coord_ETA);
				auto next_eta = t + next_eta_seconds;
				currentDestinationETA = to_simple_string(next_eta.time_of_day());
			}
		}
		else
		{
			currentDestinationETA = "N/A";
		}
		currentDestinationFuel = webuisimulatordata_.DestinationAirport_Coord_Fuel;
		currentDestinationDTG = webuisimulatordata_.DestinationAirport_Coord_DTG;

	}
	else
	{
		currentDestinationETA = "00:00:00";
		currentDestinationFuel = 0;
		currentDestinationDTG = 0;
	}

	if (prevDestinationETA != currentDestinationETA || prevDestinationFuel != currentDestinationFuel || prevDestinationDTG != currentDestinationDTG)
	{
		postWebUiEvent(WebUiEvent(WebUiEvent::Destination, currentDestinationETA, currentDestinationFuel, currentDestinationDTG));
		prevDestinationETA = currentDestinationETA;
		prevDestinationFuel = currentDestinationFuel;
		prevDestinationDTG = currentDestinationDTG;
	}
}

void WebUiServer::SendSimulatorPostitionDataToWidget()
{
	currentLat = webuisimulatordata_.Plane_Location_Lat;
	currentLon = webuisimulatordata_.Plane_Location_Lon;
	currentAlt = (int)webuisimulatordata_.Altitude;
	currentOnGround = webuisimulatordata_.AIR_GND;
	currentTemp = (int)webuisimulatordata_.Ambient_Temperature;
	currentGS = (int)webuisimulatordata_.Airspeed_GS;

	if (prevLat != currentLat || prevLon != currentLon)
	{
		postWebUiEvent(WebUiEvent(WebUiEvent::LatLon, currentLat, currentLon));
		prevLat = currentLat;
		prevLon = currentLon;
	}

	if (prevTemp != currentTemp)
	{
		postWebUiEvent(WebUiEvent(WebUiEvent::Temperature, currentTemp));
		prevTemp = currentTemp;
	}

	if (prevGS != currentGS)
	{
		postWebUiEvent(WebUiEvent(WebUiEvent::Speed, currentGS));
		prevGS = currentGS;
	}


	if (prevAlt != currentAlt)
	{
		postWebUiEvent(WebUiEvent(WebUiEvent::Alt, currentAlt));
		prevAlt = currentAlt;
	}

	if (prevOnGround != currentOnGround)
	{
		postWebUiEvent(WebUiEvent(WebUiEvent::OnGround, currentOnGround));
		prevOnGround = currentOnGround;
	}
}
void WebUiServer::SendSimulatorDoorDataToWidget()
{
	stateDoor1 = webuisimulatordata_.Door1;
	if (stateDoor1 != prevDoor1)
	{
		postWebUiEvent(WebUiEvent(WebUiEvent::Door, 1, stateDoor1));
		prevDoor1 = stateDoor1;
	}

	stateDoor2 = webuisimulatordata_.Door2;
	if (stateDoor2 != prevDoor2)
	{
		postWebUiEvent(WebUiEvent(WebUiEvent::Door, 2, stateDoor2));
		prevDoor2 = stateDoor2;
	}
	stateDoor3 = webuisimulatordata_.Door3;

	if (stateDoor3 != prevDoor3)
	{
		postWebUiEvent(WebUiEvent(WebUiEvent::Door, 3, stateDoor3));
		prevDoor3 = stateDoor3;
	}
}

void WebUiServer::SendSimulatorGroundDataToWidget()
{
	stateGroundPower = webuisimulatordata_.GroundSupply_Power;
	stateGroundAir = webuisimulatordata_.GroundSupply_Air;

	if (prevGroundPower != stateGroundPower)
	{
		postWebUiEvent(WebUiEvent(WebUiEvent::GroundPower, stateGroundPower));
		prevGroundPower = stateGroundPower;
	}

	if (prevGroundAir != stateGroundAir)
	{
		postWebUiEvent(WebUiEvent(WebUiEvent::GroundAir, stateGroundAir));
		prevGroundAir = stateGroundAir;
	}
}

void WebUiServer::SendSimulatorIcaoDataToWidget()
{
	tempIcao = webuisimulatordata_.ICAO_ORIGIN_AIRPORT;
	if (tempIcao != acDepIcao)
	{
		acDepIcao = tempIcao;

		tempInfoString = acFlightNumber + "  (" + acDepIcao + " - " + acArrIcao + " )";
		tempInfoStringWt = tempInfoString;
		postWebUiEvent(WebUiEvent(WebUiEvent::FlightInfo, tempInfoStringWt));

		if (tempIcao.length() == 4)
		{
			wxrStatus = GetWeather(1, acDepIcao, tempWeather);
			tempDepMetarString = tempWeather;
			postWebUiEvent(WebUiEvent(WebUiEvent::WxrMetarDep, tempDepMetarString));

			wxrStatus = GetWeather(2, acDepIcao, tempWeather);
			tempDepTafString = tempWeather;
			postWebUiEvent(WebUiEvent(WebUiEvent::WxrTafDep, tempDepTafString));
		}
	}

	tempIcao = webuisimulatordata_.ICAO_DEST_AIRPORT;
	if (tempIcao != acArrIcao)
	{
		acArrIcao = tempIcao;

		tempInfoString = acFlightNumber + " ( " + acDepIcao + " - " + acArrIcao + " )";
		tempInfoStringWt = tempInfoString;
		postWebUiEvent(WebUiEvent(WebUiEvent::FlightInfo, tempInfoStringWt));

		if (tempIcao.length() == 4)
		{
			wxrStatus = GetWeather(1, acArrIcao, tempWeather);
			tempArrMetarString = tempWeather;
			postWebUiEvent(WebUiEvent(WebUiEvent::WxrMetarArr, tempArrMetarString));

			wxrStatus = GetWeather(2, acArrIcao, tempWeather);
			tempArrTafString = tempWeather;
			postWebUiEvent(WebUiEvent(WebUiEvent::WxrTafArr, tempArrTafString));
		}
	}

	tempFlightNumber = webuisimulatordata_.FLT_NO;
	if (tempFlightNumber != acFlightNumber)
	{
		acFlightNumber = tempFlightNumber;

		tempInfoString = acFlightNumber + " ( " + acDepIcao + " - " + acArrIcao + " )";
		tempInfoStringWt = tempInfoString;
		postWebUiEvent(WebUiEvent(WebUiEvent::FlightInfo, tempInfoStringWt));
	}
}

void WebUiServer::SendSimulatorOooiDataToWidget()
{
	// check here what state flight is and update OOOI times when necessary
	if (flightState == FLIGHTSTATE_PREFLIGHT && webuisimulatordata_.Parking_Brake_Lever_Status == 0 && webuisimulatordata_.IS_FMC_PREFLIGHT_COMPLETED == TRUE && webuisimulatordata_.Door1 == 0)
	{
		flightState = FLIGHTSTATE_PB_OR_TAXI_OUT;
		ptime t(second_clock::universal_time());
		currentOooiOut = to_simple_string(t.time_of_day());
	}

	if (flightState == FLIGHTSTATE_PB_OR_TAXI_OUT && webuisimulatordata_.AIR_GND == FALSE)
	{
		flightState = FLIGHTSTATE_ON_AIR;
		ptime t(second_clock::universal_time());
		currentOooiOff = to_simple_string(t.time_of_day());
	}

	if (flightState == FLIGHTSTATE_ON_AIR && webuisimulatordata_.AIR_GND == TRUE)
	{
		flightState = FLIGHTSTATE_TAXI_IN;
		ptime t(second_clock::universal_time());
		currentOooiOn = to_simple_string(t.time_of_day());
	}

	// bounced landing.. huh
	if (flightState == FLIGHTSTATE_TAXI_IN && webuisimulatordata_.AIR_GND == FALSE)
	{
		flightState = FLIGHTSTATE_ON_AIR;
		currentOooiOn = "N/A";
	}

	if (flightState == FLIGHTSTATE_TAXI_IN && webuisimulatordata_.Parking_Brake_Lever_Status == 1)
	{
		flightState = FLIGHTSTATE_POSTFLIGHT;
		ptime t(second_clock::universal_time());
		currentOooiIn = to_simple_string(t.time_of_day());
	}

	if (flightState == FLIGHTSTATE_POSTFLIGHT && webuisimulatordata_.Door1 == 1)
	{
		flightState = FLIGHTSTATE_COMPLETED;
	}

	if (flightState == FLIGHTSTATE_COMPLETED && webuisimulatordata_.Door1 == 0)
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
		postWebUiEvent(WebUiEvent(WebUiEvent::Oooi, currentOooiOut, currentOooiOff, currentOooiOn, currentOooiIn));
		prevOooiOut = currentOooiOut;
		prevOooiOff = currentOooiOff;
		prevOooiOn = currentOooiOn;
		prevOooiIn = currentOooiIn;
	}
}

void WebUiServer::sendButtonPress(int pressButtonValue)
{
    if (stop_)
    {
        return;
    }

    std::unique_lock<std::recursive_mutex> lock(mutex_);

	if (pressButtonValue == ID_WEBUI_TIMER_TIMEOUT)
	{
		if (acDepIcao.length() == 4)
		{
			wxrStatus = GetWeather(1, acDepIcao, tempWeather);
			tempDepMetarString = tempWeather;
			postWebUiEvent(WebUiEvent(WebUiEvent::WxrMetarDep, tempDepMetarString));

			wxrStatus = GetWeather(2, acDepIcao, tempWeather);
			tempDepTafString = tempWeather;
			postWebUiEvent(WebUiEvent(WebUiEvent::WxrTafDep, tempDepTafString));
		}

		if (acArrIcao.length() == 4)
		{
			wxrStatus = GetWeather(1, acArrIcao, tempWeather);
			tempArrMetarString = tempWeather;
			postWebUiEvent(WebUiEvent(WebUiEvent::WxrMetarArr, tempArrMetarString));

			wxrStatus = GetWeather(2, acArrIcao, tempWeather);
			tempArrTafString = tempWeather;
			postWebUiEvent(WebUiEvent(WebUiEvent::WxrTafArr, tempArrTafString));
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
		currentOooiOut = "N/A";
		currentOooiOff = "N/A";
		currentOooiOn = "N/A";
		currentOooiIn = "N/A";

		flightState = FLIGHTSTATE_PREFLIGHT;

		postWebUiEvent(WebUiEvent(WebUiEvent::Oooi, currentOooiOut, currentOooiOff, currentOooiOn, currentOooiIn));
		prevOooiOut = currentOooiOut;
		prevOooiOff = currentOooiOff;
		prevOooiOn = currentOooiOn;
		prevOooiIn = currentOooiIn;

	}

	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_0) { iflyconnect.SendVirtualKey(48); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_1) { iflyconnect.SendVirtualKey(49); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_2) { iflyconnect.SendVirtualKey(50); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_3) { iflyconnect.SendVirtualKey(51); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_4) { iflyconnect.SendVirtualKey(52); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_5) { iflyconnect.SendVirtualKey(53); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_6) { iflyconnect.SendVirtualKey(54); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_7) { iflyconnect.SendVirtualKey(55); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_8) { iflyconnect.SendVirtualKey(56); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_9) { iflyconnect.SendVirtualKey(57); }
	if (pressButtonValue == ID_WEBUI_BUTTON_VIRTUAL_KEY_F12) { iflyconnect.SendVirtualKey(123); }

}

int WebUiServer::GetWeather(int _wxrType, std::string& _icao, std::string& _resultWxr)
{
	SOCKET udtSocket;
	SOCKADDR_IN udtAddress;
	WSADATA udtWsa;
	LPHOSTENT udtHost;
	_TCHAR acTmp[1024];
	_TCHAR acData[1024];
	_TCHAR acResult[1024];
	_TCHAR* pcStart;
	_TCHAR* pcEnd;

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

	char* cstr = new char[_icao.length() + 1];
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



