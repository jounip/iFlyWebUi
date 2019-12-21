// iFlyWebUi.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "iFlyWebUi.h"
#include <string>

#include "INIReader.h"

#include "WebApplication.h"
#include "SimpleWebServer.h"

#include <Wt/WApplication>


#define MAX_LOADSTRING 100

using namespace Wt;

WApplication *createApplication(const WEnvironment& env,
	SimpleWebServer& server)
{
	return new WebApplication(env, server);
}

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HWND hButton = NULL;
HWND hEditBox;
HWND hWnd;

LPARAM prevLogText;

BOOL ServerRunning = FALSE;

// edit box size
int nLength;

std::string gsxMenuPath;
std::string gsxTextPath;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	INIReader reader("iFlyWebUi.ini");

	if (reader.ParseError() < 0) {
		MessageBox(0, "loading iFlyWebUi.ini failed!", "Error!", MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	gsxMenuPath = reader.Get("WideFS", "Menu", "");
	gsxTextPath = reader.Get("WideFS", "Text", "");

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IFLYWEBUI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IFLYWEBUI));

	//C++11 initializer list

	//location of wt_config.xml.If unspecified,
	//WT_CONFIG_XML is searched in the environment,
	//if it does not exist then the compiled - in
	//default (/ etc / wt / wt_config.xml) is tried.If
	//the default does not exist, we revert to
	//default values for all parameters.

	const char * const v[9] =
	{
		__argv[0],
		"--docroot", ".",
		"--http-port", "8090",
		"--http-address", "0.0.0.0",
		"--config", "wt_config.xml"
	};

	// Add WT Start Stuff here
	Wt::WServer server(7, const_cast<char**>(v), WTHTTP_CONFIGURATION);
	SimpleWebServer iFlyServer(server);

	/*
	* We add one entry point for the full-window application,
	*/
	server.addEntryPoint(Wt::Application,
		boost::bind(createApplication, _1,
		boost::ref(iFlyServer)));

	if (server.start())
	{
		SendMessage(hWnd, ID_WITTY_OK, ID_WITTY_OK, 0);
		ServerRunning = TRUE;
	}
	else
	{
		SendMessage(hWnd, ID_INTERFACE_NOT_OK, ID_WITTY_NOT_OK, 0);
		ServerRunning = FALSE;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if (ServerRunning)
	{
		server.stop();
	}

	return (int) msg.wParam;
}

// Private Logger(LPARAM logtext)
// Logs all the logtest to application window

void Logger(LPARAM logtext)
{
	if (logtext != prevLogText)
	{
		nLength = GetWindowTextLength(hEditBox);
		SendMessage(hEditBox, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
		SendMessage(hEditBox, EM_REPLACESEL, (WPARAM)FALSE, logtext);

		nLength = GetWindowTextLength(hEditBox);
		SendMessage(hEditBox, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
		SendMessage(hEditBox, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)"\r\n");

		prevLogText = logtext;
	}
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IFLYWEBUI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_IFLYWEBUI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 532, 320, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(0, "Window Creation Failed!", "Error!", MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	Logger((LPARAM)"Status : BASIC SYSTEMS OK");
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void QuitRequested()
{
	Logger((LPARAM)"Status : SHUTTING DOWN");
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:

		hButton = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", "Exit!", WS_CHILDWINDOW | BS_PUSHBUTTON
			| WS_VISIBLE, 396, 210, 75, 26, hWnd, (HMENU)IDC_BUTTON, hInst, NULL);

		hEditBox = CreateWindowEx(WS_EX_STATICEDGE, "EDIT", "",
			WS_CHILDWINDOW | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER | WS_VSCROLL | WS_DISABLED | WS_VISIBLE, 11, 36, 342, 210, hWnd, (HMENU)IDC_MAIN_EDIT, hInst, NULL);

		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			QuitRequested();
			DestroyWindow(hWnd);
			break;
		case IDC_BUTTON:
		{
			QuitRequested();
			DestroyWindow(hWnd);
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case ID_INTERFACE_NOT_OK:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case ID_IFLY_NOT_OK:
			Logger((LPARAM)"Status : IFLY NOT OK");
			break;
		case ID_IFLY_SDK2_NOT_OK:
			Logger((LPARAM)"Status : IFLY CDU SDK NOT OK");
			break;
		case ID_IFLY_SDK_NOT_OK:
			Logger((LPARAM)"Status : IFLY SDK NOT OK");
			break;
		case ID_FSUIPC_NOT_OK:
			Logger((LPARAM)"Status : FSUIPC NOT OK");
			break;
		case ID_WITTY_NOT_OK:
			Logger((LPARAM)"Status : WT NOT STARTED");
			break;
		case ID_SIMCONNECT_NOT_OK:
			Logger((LPARAM)"Status : SIMCONNECT NOT OK");
			break;
		default:
			Logger((LPARAM)"Status : INTERFACE NOT OK");
			break;
		}
		break;
	case ID_THREAD_EXIT:
		Logger((LPARAM)"Status : iFly737 THREAD EXIT");
		break;
	case ID_WITTY_OK:
		Logger((LPARAM)"Status : WT STARTED");
		break;
	case ID_INTERFACE_OK:
		Logger((LPARAM)"Status : ALL INTERFACES OK");
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
		QuitRequested();
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
