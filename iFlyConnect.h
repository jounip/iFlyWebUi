#pragma once

#include "SimConnect/SimConnect.h"

#include "SimulatorDataStruct.h"

#include <string>

using namespace std;

class iFlyConnect
{
public:
	iFlyConnect();
	~iFlyConnect();
	void MainProcess(void);

	void GetSimulatorData(WebUiSimulatorData *);
	bool GetStatus();

	void ToggleGroundPower();
	void ToggleGroundAir();
	void ToggleDoor(int);
	void TogglePushBack(int);
	void SendVirtualKey(long);

	static void CALLBACK MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
	void Process(SIMCONNECT_RECV *pData, DWORD cbData);

private:
	DWORD OpenInterface(void);
	void CloseInterface(void);
	int ReadStatus(void);

	BOOL fsuipc_status;
	INT interface_status;
	INT ifly_status;
	DWORD dwResult;

	int _door1 = 0;
	int _door2 = 0;
	int _door3 = 0;

	double _alt = 0;

	// fsuipc keypress command
	long _command = 1070;

	// SimConnect Global Variables
	HANDLE hSimConnect = NULL;
	HRESULT hr_simconnect;

	static enum EVENT_ID {
		EVENT_DOOR,
		EVENT_PUSHBACK,
		EVENT_SEL1,
		EVENT_SEL2,
		EVENT_SEL3,
	};

	static enum DATA_DEFINE_ID {
		DEFINITION_1,
	};

	static enum DATA_REQUEST_ID {
		REQUEST_1,
	};

	struct scStruct
	{
		double door1;
		double door2;
		double door3;
		double alt;
	};
};

