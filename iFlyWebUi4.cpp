// iFlyWebUi4.cpp : Defines the entry point for the application.
//

#include "pch.h"

#include "iFlyWebUi4.h"

#include "inih/INIReader.h"

#include "WebUiServer.h"
#include "WebUiApplication.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

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
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env,
    WebUiServer& server)
{
    return Wt::cpp14::make_unique<ChatApplication>(env, server);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    INIReader reader("iFlyWebUi.ini");

    if (reader.ParseError() < 0) {
        MessageBox(0, "loading iFlyWebUi.ini failed!", "Error!", MB_ICONSTOP | MB_OK);
        return FALSE;
    }

    gsxMenuPath = reader.Get("WideFS", "Menu", "");
    gsxTextPath = reader.Get("WideFS", "Text", "");

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_IFLYWEBUI4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IFLYWEBUI4));

    MSG msg;

    const char* const v[7] =
    {
        "iFlyWebUi",
        "--docroot", ".",
        "--http-port", "8123",
        "--http-address", "0.0.0.0"
    };

    Wt::WServer server(7, const_cast<char**>(v), WTHTTP_CONFIGURATION);
    WebUiServer chatServer(server);

    /*
     * We add two entry points: one for the full-window application,
     * and one for a widget that can be integrated in another page.
     */
    server.addEntryPoint(Wt::EntryPointType::Application,
        std::bind(createApplication, std::placeholders::_1,
            std::ref(chatServer)));

    if (server.start())
    {
        SendMessage(hWnd, ID_WITTY_OK, 0, 0);
        ServerRunning = TRUE;
    }
    else
    {
        SendMessage(hWnd, ID_WITTY_NOT_OK, 0, 0);
        ServerRunning = FALSE;
    }

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IFLYWEBUI4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_IFLYWEBUI4);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 532, 320, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      MessageBox(0, "Window Creation Failed!", "Error!", MB_ICONSTOP | MB_OK);
      return FALSE;
   }

   Logger((LPARAM)"Status : WINDOW EXECUTABLE STARTED OK");

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
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;

    switch (message)
    {
    case WM_CREATE:
        {
            hButton = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", "Exit!", WS_CHILDWINDOW | BS_PUSHBUTTON
                | WS_VISIBLE, 396, 210, 75, 26, hWnd, (HMENU)IDC_BUTTON, hInst, NULL);

            hEditBox = CreateWindowEx(WS_EX_STATICEDGE, "EDIT", "",
                WS_CHILDWINDOW | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER | WS_VSCROLL | WS_DISABLED | WS_VISIBLE, 11, 36, 342, 210, hWnd, (HMENU)IDC_MAIN_EDIT, hInst, NULL);

        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDC_BUTTON:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
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
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        QuitRequested();
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
