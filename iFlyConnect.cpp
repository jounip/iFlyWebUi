#include "pch.h"

#include "iFlyConnect.h"

#include "Resource.h"

#include "Fsuipc/IPCuser64.h"

#include "SDK737/SDK.h"
#include "SDK737/SDK_CDU.h"
#include "SDK737/SDK_Failure.h"
#include "SDK737/Hotkey.h"

#include "SimulatorDataStruct.h"

#include "SimConnect/SimConnect.h"

#include <string>
#include <strsafe.h> 

extern HWND hWnd;

// define _iflytest when testing without simulator
//#define _IFLYTEST


iFlyConnect::iFlyConnect()
{
	interface_status = ID_INTERFACE_NOT_OK;
	hSimConnect = NULL;
	pBufSDK = NULL;
	hMapFileSDK = NULL;
	pBufSDK2 = NULL;
	hMapFileSDK2 = NULL;
}


iFlyConnect::~iFlyConnect()
{
	CloseInterface();
}


void CALLBACK iFlyConnect::MyDispatchProcRD(SIMCONNECT_RECV *pData, DWORD cbData, void *pContext)
{
	iFlyConnect *pThis = reinterpret_cast<iFlyConnect*>(pContext);
	pThis->Process(pData, cbData);
}


void iFlyConnect::Process(SIMCONNECT_RECV *pData, DWORD cbData)
{
	switch (pData->dwID)
	{
	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
	{
		SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;

		switch (pObjData->dwRequestID)
		{
		case REQUEST_1:
		{
			DWORD ObjectID = pObjData->dwObjectID;
			scStruct *pS = (scStruct*)&pObjData->dwData;

			if (pS->door1 > 0) { _door1 = 1; }
			else { _door1 = 0; }
			if (pS->door2 > 0) { _door2 = 1; }
			else { _door2 = 0; }
			if (pS->door3 > 0) { _door3 = 1; }
			else { _door3 = 0; }

			_alt = pS->alt;

			break;
		}

		default:
			break;
		}
		break;
	}


	case SIMCONNECT_RECV_ID_QUIT:
	{
		interface_status = ID_SIMCONNECT_NOT_OK;
		break;
	}

	default:
		break;
	}
}

void iFlyConnect::MainProcess()
{
	if (interface_status == ID_INTERFACE_OK)
	{
		#ifdef _IFLYTEST
		// do here stuff for testing mode
		#else
		// do simconnect stuff
		SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, this);

		if (interface_status == ID_INTERFACE_OK)
		{
			// copy from SDK to readable struct
			if (pShareMemSDK2 && pBufSDK2)
			{
				CopyMemory((PVOID)pShareMemSDK2, pBufSDK2, sizeof(struct ShareMemorySDK2));
			}
			else
			{
				interface_status = ID_IFLY_SDK_NOT_OK;
			}
			if (pShareMemSDK && pBufSDK)
			{
				CopyMemory((PVOID)pShareMemSDK, pBufSDK, sizeof(struct ShareMemorySDK));
			}
			else
			{
				interface_status = ID_IFLY_SDK2_NOT_OK;
			}

		}
		if (interface_status == ID_INTERFACE_OK)
		{
			ifly_status = pShareMemSDK2->IFLY737NG_STATE;
			if (ifly_status == 0)
			{
				interface_status = ID_IFLY_NOT_OK;
			}
			else
			{
				fsuipc_status = FSUIPC_Process(&dwResult);
				if (fsuipc_status != 0 && dwResult>0)
				{
					interface_status = ID_FSUIPC_NOT_OK;
				}
			}
		}
		if (interface_status != ID_INTERFACE_OK)
		{
			SendMessage(hWnd, ID_INTERFACE_NOT_OK, interface_status, 0);
		}
		#endif _IFLYTEST
	}
	else
	{
		#ifdef _IFLYTEST
		// do here stuff for testing mode
		interface_status = ID_INTERFACE_OK;
		#else
		CloseInterface();
		interface_status = OpenInterface();
		#endif _IFLYTEST

		if (interface_status == ID_INTERFACE_OK)
		{
			SendMessage(hWnd, ID_INTERFACE_OK, interface_status, 0);
		}
		else
		{
			SendMessage(hWnd, ID_INTERFACE_NOT_OK, interface_status, 0);
		}
		
	}
}


DWORD iFlyConnect::OpenInterface(void)
{
#ifdef _IFLYTEST
	// do here stuff for testing mode
	return ID_INTERFACE_OK;
#else
	if (SUCCEEDED(hr_simconnect = SimConnect_Open(&hSimConnect, "WebUi Simconnect proc", NULL, 0, 0, 0)))
	{
		hr_simconnect = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_DOOR, "TOGGLE_AIRCRAFT_EXIT");
		hr_simconnect = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_PUSHBACK, "TOGGLE_PUSHBACK");
		hr_simconnect = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SEL1, "SELECT_1");
		hr_simconnect = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SEL2, "SELECT_2");
		hr_simconnect = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SEL3, "SELECT_3");

		// Set up the data definition, but do not yet do anything with it 
		hr_simconnect = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "EXIT OPEN:0", "percent over 100", SIMCONNECT_DATATYPE_FLOAT64);
		hr_simconnect = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "EXIT OPEN:1", "percent over 100", SIMCONNECT_DATATYPE_FLOAT64);
		hr_simconnect = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "EXIT OPEN:2", "percent over 100", SIMCONNECT_DATATYPE_FLOAT64);
		hr_simconnect = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "PLANE ALTITUDE", "feet", SIMCONNECT_DATATYPE_FLOAT64);

		// Now the sim is running, request information on the user aircraft 
		hr_simconnect = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_1, DEFINITION_1, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);

		FSUIPC_Close(); // Closing when it wasn't open is okay, so this is safe here
		if (FSUIPC_Open(SIM_ANY, &dwResult))
		{
			// Okay, fsuipc is open, let's open IFly
			hMapFileSDK = OpenFileMapping(
				FILE_MAP_READ,				// read/write access
				FALSE,						// do not inherit the name
				szNameSDK);					// name of mapping object

			if (hMapFileSDK == NULL)
			{
				return ID_IFLY_SDK_NOT_OK;
			}
			else
			{
				pBufSDK = (LPTSTR)MapViewOfFile(hMapFileSDK,    // handle to mapping object
					FILE_MAP_READ,						// read/write permission
					0,
					0,
					BUF_SIZE_SDK);
				if (pBufSDK == NULL)
				{
					UnmapViewOfFile(pBufSDK);
					pBufSDK = NULL;
					CloseHandle(hMapFileSDK);
					hMapFileSDK = NULL;
					return ID_IFLY_SDK_NOT_OK;
				}
				else
				{
					hMapFileSDK2 = OpenFileMapping(
						FILE_MAP_READ,   // read/write access
						FALSE,                 // do not inherit the name
						szNameSDK2);               // name of mapping object

					if (hMapFileSDK2 == NULL)
					{
						return ID_IFLY_SDK2_NOT_OK;
					}
					else
					{
						pBufSDK2 = (LPTSTR)MapViewOfFile(hMapFileSDK2,    // handle to mapping object
							FILE_MAP_READ,  // read/write permission
							0,
							0,
							BUF_SIZE_SDK2);
						if (pBufSDK2 == NULL)
						{
							UnmapViewOfFile(pBufSDK);
							pBufSDK = NULL;
							CloseHandle(hMapFileSDK);
							hMapFileSDK = NULL;
							UnmapViewOfFile(pBufSDK2);
							pBufSDK2 = NULL;
							CloseHandle(hMapFileSDK2);
							hMapFileSDK2 = NULL;
							return ID_IFLY_SDK2_NOT_OK;
						}
						else
						{
							iFly737NG_Init_SendMessage();
							CopyMemory((PVOID)pShareMemSDK2, pBufSDK2, sizeof(struct ShareMemorySDK2));
							ifly_status = pShareMemSDK2->IFLY737NG_STATE;
							if (ifly_status == 1)
							{
								return ID_INTERFACE_OK;
							}
							else
							{
								return ID_IFLY_NOT_OK;
							}
						}
					}
				}
			}
		}
		else
		{
			return ID_FSUIPC_NOT_OK;
		}
	}
	else
	{
		return ID_SIMCONNECT_NOT_OK;
	}
#endif _IFLYTEST
}

void iFlyConnect::CloseInterface(void)
{
#ifdef _IFLYTEST
	// do here stuff for testing mode
	
#else
	if (hSimConnect)
	{
		hr_simconnect = SimConnect_Close(hSimConnect);
		hSimConnect = NULL;
	}
	if (pBufSDK)
	{
		UnmapViewOfFile(pBufSDK);
		pBufSDK = NULL;
	}
	if (hMapFileSDK)
	{
		CloseHandle(hMapFileSDK);
		hMapFileSDK = NULL;
	}
	if (pBufSDK2)
	{
		UnmapViewOfFile(pBufSDK2);
		pBufSDK2 = NULL;
	}
	if (hMapFileSDK2)
	{
		CloseHandle(hMapFileSDK2);
		hMapFileSDK2 = NULL;
	}
	hMapFileSDK2 = NULL;

	FSUIPC_Close(); // Closing when it wasn't open is okay, so this is safe here
#endif _IFLYTEST
}

void iFlyConnect::GetSimulatorData(WebUiSimulatorData *_simdata)
{
#ifdef _IFLYTEST
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
#else
	_simdata->Plane_Location_Lat = pShareMemSDK->Plane_Location_Lat;
	_simdata->Plane_Location_Lon = pShareMemSDK->Plane_Location_Lon;
	_simdata->IS_FMC_PREFLIGHT_COMPLETED = pShareMemSDK->IS_FMC_PREFLIGHT_COMPLETED;
	_simdata->Parking_Brake_Lever_Status = pShareMemSDK->Parking_Brake_Lever_Status;
	_simdata->DestinationAirport_Avail = pShareMemSDK->DestinationAirport_Avail;
	_simdata->DestinationAirport_Coord_DTG = pShareMemSDK->DestinationAirport_Coord_DTG;
	_simdata->DestinationAirport_Coord_ETA = pShareMemSDK->DestinationAirport_Coord_ETA;
	_simdata->DestinationAirport_Coord_Fuel = pShareMemSDK->DestinationAirport_Coord_Fuel;
	_simdata->FLT_NO = pShareMemSDK->FLT_NO;
	_simdata->ICAO_DEST_AIRPORT = pShareMemSDK->ICAO_DEST_AIRPORT;
	_simdata->ICAO_ORIGIN_AIRPORT = pShareMemSDK->ICAO_ORIGIN_AIRPORT;
	_simdata->LastWPT_name = pShareMemSDK->LastWPT_name;
	_simdata->LastWPT_Altitude = pShareMemSDK->LastWPT_Altitude;
	_simdata->LastWPT_ATA = pShareMemSDK->LastWPT_ATA;
	_simdata->LastWPT_Fuel = pShareMemSDK->LastWPT_Fuel;
	_simdata->ActWPT_name = pShareMemSDK->ActWPT_name;
	_simdata->ActWPT_DTG = pShareMemSDK->ActWPT_DTG;
	_simdata->ActWPT_ETA = pShareMemSDK->ActWPT_ETA;
	_simdata->ActWPT_Fuel = pShareMemSDK->ActWPT_Fuel;
	_simdata->NextWPT_name = pShareMemSDK->NextWPT_name;
	_simdata->NextWPT_DTG = pShareMemSDK->NextWPT_DTG;
	_simdata->NextWPT_ETA = pShareMemSDK->NextWPT_ETA;
	_simdata->NextWPT_Fuel = pShareMemSDK->NextWPT_Fuel;
	_simdata->Ambient_Temperature = pShareMemSDK->Ambient_Temperature;
	_simdata->Airspeed_GS = pShareMemSDK->Airspeed_GS;
	_simdata->AIR_GND = pShareMemSDK->AIR_GND;
	_simdata->GroundSupply_Power = pShareMemSDK->GroundSupply_Power;
	_simdata->GroundSupply_Air = pShareMemSDK->GroundSupply_Air;
	_simdata->IFLY737NG_STATE = pShareMemSDK->IFLY737NG_STATE;
	_simdata->Door1 = _door1;
	_simdata->Door2 = _door2;
	_simdata->Door3 = _door3;
	_simdata->Altitude = _alt;
#endif _IFLYTEST
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
	if (interface_status == ID_INTERFACE_OK && pShareMemSDK && pBufSDK)
	{
		if (pShareMemSDK->GRD_POWER_AVAILABLE_Light_Status == 1)
		{
			iFly737NG_SendMessage(KEY_COMMAND_ELECTRICAL_GROUND_SUPPLY_OFF, 0);
		}
		else
		{
			iFly737NG_SendMessage(KEY_COMMAND_ELECTRICAL_GROUND_SUPPLY_ON, 0);
		}
	}
}

void iFlyConnect::ToggleGroundAir()
{
	if (interface_status == ID_INTERFACE_OK && pShareMemSDK && pBufSDK)
	{
		if (pShareMemSDK->GroundSupply_Air == 1)
		{
			iFly737NG_SendMessage(KEY_COMMAND_AIRSYSTEM_GROUND_SUPPLY_ON, 0);
		}
		else
		{
			iFly737NG_SendMessage(KEY_COMMAND_AIRSYSTEM_GROUND_SUPPLY_OFF, 0);

		}
	}
}

void iFlyConnect::ToggleDoor(int _door)
{
	if (hr_simconnect == S_OK)
	{
		if (_door == ID_WEBUI_BUTTON_DOOR1)
		{
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_DOOR, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_SEL1, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (_door == ID_WEBUI_BUTTON_DOOR2)
		{
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_DOOR, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_SEL2, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (_door == ID_WEBUI_BUTTON_DOOR3)
		{
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_DOOR, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_SEL3, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
	}
}

void iFlyConnect::TogglePushBack(int _turn)
{
	if (hr_simconnect == S_OK)
	{
		if (_turn == ID_WEBUI_BUTTON_PUSHBACK1)
		{
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_PUSHBACK, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_SEL1, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (_turn == ID_WEBUI_BUTTON_PUSHBACK2)
		{
			// should this check that if PB is ongoing and turning it should stop and start again? Now it just stops
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_PUSHBACK, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (_turn == ID_WEBUI_BUTTON_PUSHBACK3)
		{
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_PUSHBACK, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_SEL2, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (_turn == ID_WEBUI_BUTTON_PUSHBACK4)
		{
			// add check here to stop only if PB ongoing
			hr_simconnect = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_PUSHBACK, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
	}
}

void iFlyConnect::SendVirtualKey(long _virtualkey)
{


	if (fsuipc_status == 0)
	{
		// send key to simulator
		fsuipc_status = FSUIPC_Write(0x3114, 4, &_virtualkey, &dwResult);
		fsuipc_status = FSUIPC_Write(0x3110, 4, &_command, &dwResult);
	}
}
