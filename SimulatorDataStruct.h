#ifndef _WEBUISIMULATORDATA_H_
#define _WEBUISIMULATORDATA_H_

#include <string>

struct WebUiSimulatorData
{
	double     Plane_Location_Lat;
	double     Plane_Location_Lon;

	bool		IS_FMC_PREFLIGHT_COMPLETED;
	int        Parking_Brake_Lever_Status;

	bool		DestinationAirport_Avail;
	double     DestinationAirport_Coord_DTG;//DTG from present position to the Destination Airport
	double     DestinationAirport_Coord_ETA;//ETA to the Destination Airport
	double     DestinationAirport_Coord_Fuel;//predicted fuel remaining at the Destination Airport

	std::string      FLT_NO;//the same as display at CDU page
	std::string      ICAO_DEST_AIRPORT;
	std::string      ICAO_ORIGIN_AIRPORT;

	std::string      LastWPT_name;//waypoint ID
	int        LastWPT_Altitude;//feet
	int        LastWPT_ATA;//second since 00:00:00, LastWPT_ATA=-1:unavailable, Example:LastWPT_ATA=0--->00:00:00    LastWPT_ATA=1--->00:00:01    LastWPT_ATA=3600--->01:00:00
	int        LastWPT_Fuel;//kgs, LastWPT_Fuel=-1:unavailable

	std::string      ActWPT_name;
	double     ActWPT_DTG;//nm
	int        ActWPT_ETA;
	int        ActWPT_Fuel;

	std::string      NextWPT_name;
	double     NextWPT_DTG;//nm
	int        NextWPT_ETA;
	int        NextWPT_Fuel;

	double     Ambient_Temperature;

	double     Airspeed_GS;

	BOOL       AIR_GND;//0:AIR    1:GND

	int        GroundSupply_Power;//0:unavailable    1:available,not connected    2:available, connected
	int        GroundSupply_Air;//0:unavailable    1:available,not connected    2:available, connected

//--------------------From CDU SDK---------------------------------

	int IFLY737NG_STATE;//IFLY737NG_NOT_RUNNING or IFLY737NG_RUNNING

//--------------------From SimConnect Data-------------------------

	int Door1;
	int Door2;
	int Door3;
	double Altitude;
};
#endif