#include "stdafx.h"
#include "iFlyConnect.h"

#include "Resource.h"

#include "SimulatorDataStruct.h"


#include <string>
#include <strsafe.h> 

extern HWND hWnd;

// define _iflytest when testing without simulator
#define _IFLYTEST


iFlyConnect::iFlyConnect()
{
	interface_status = ID_INTERFACE_NOT_OK;
}


iFlyConnect::~iFlyConnect()
{
}


void iFlyConnect::MainProcess()
{
	if (interface_status == ID_INTERFACE_OK)
	{
		// do here stuff for testing mode
	}
	else
	{
		// do here stuff for testing mode
		interface_status = ID_INTERFACE_OK;
		
	}
}


DWORD iFlyConnect::OpenInterface(void)
{
	// do here stuff for testing mode
	return ID_INTERFACE_OK;
}

void iFlyConnect::CloseInterface(void)
{

}

void iFlyConnect::GetSimulatorData(WebUiSimulatorData *_simdata)
{
	// do here stuff for testing mode
	_simdata->Plane_Location_Lat = 60.307736;
	_simdata->Plane_Location_Lon = 24.881802;
	_simdata->IS_FMC_PREFLIGHT_COMPLETED = 0;
	_simdata->Parking_Brake_Lever_Status = 1;
	_simdata->DestinationAirport_Avail = TRUE;
	_simdata->DestinationAirport_Coord_DTG = 345.78901453;
	_simdata->DestinationAirport_Coord_ETA = 1456;
	_simdata->DestinationAirport_Coord_Fuel = 1900;
	_simdata->FLT_NO = "TEST123";
	_simdata->ICAO_DEST_AIRPORT = "EFHK";
	_simdata->ICAO_ORIGIN_AIRPORT = "EFOU";
	_simdata->LastWPT_name = "PING";
	_simdata->LastWPT_Altitude = 29543;
	_simdata->LastWPT_ATA = 3431;
	_simdata->LastWPT_Fuel = 3432;
	_simdata->ActWPT_name = "PONG";
	_simdata->ActWPT_DTG = 23.456;
	_simdata->ActWPT_ETA = 56;
	_simdata->ActWPT_Fuel = 2345;
	_simdata->NextWPT_name = "PANG";
	_simdata->NextWPT_DTG = 4.5678;
	_simdata->NextWPT_ETA = 12;
	_simdata->NextWPT_Fuel = 1234;
	_simdata->Ambient_Temperature = -38;
	_simdata->Airspeed_GS = 567;
	_simdata->AIR_GND = 1;
	_simdata->GroundSupply_Power = 0;
	_simdata->GroundSupply_Air = 1;
	_simdata->IFLY737NG_STATE = 1;
	_simdata->Door1 = 1;
	_simdata->Door2 = 0;
	_simdata->Door3 = 0;
	_simdata->Altitude = 32081;
}

bool iFlyConnect::GetStatus(void)
{
	if (interface_status == ID_INTERFACE_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void iFlyConnect::ToggleGroundPower()
{

}

void iFlyConnect::ToggleGroundAir()
{

}

void iFlyConnect::ToggleDoor(int _door)
{

}

void iFlyConnect::TogglePushBack(int _turn)
{

}

void iFlyConnect::SendVirtualKey(long _virtualkey)
{

}
