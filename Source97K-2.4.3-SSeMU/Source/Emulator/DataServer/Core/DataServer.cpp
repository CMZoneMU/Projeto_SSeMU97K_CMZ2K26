#include "stdafx.h"
#include "resource.h"
#include "DataServer.h"
#include "AllowableIpList.h"
#include "BadSyntax.h"
#include "GuildManager.h"
#include "ItemManager.h"
#include "MiniDump.h"
#include "QueryManager.h"
#include "ServerDisplayer.h"
#include "SocketManager.h"
#include "Util.h"

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) // OK
{
	CMiniDump::Start();

	LoadString(hInstance,IDS_APP_TITLE,szTitle,MAX_LOADSTRING);
	LoadString(hInstance,IDC_DATASERVER,szWindowClass,MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if(InitInstance(hInstance,nCmdShow) == 0)
	{
		return 0;
	}

	char buff[256];

	wsprintf(buff,DATASERVER_VERSION,PROTOCOL_NAME);

	SetWindowText(hWnd,buff);

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if(WSAStartup(MAKEWORD(2,2),&wsa) == 0)
	{
		char DataServerODBC[32] = {0};

		char DataServerUSER[32] = {0};

		char DataServerPASS[32] = {0};

		GetPrivateProfileString("DataServerInfo","DataServerODBC","",DataServerODBC,sizeof(DataServerODBC),".\\DataServer.ini");

		GetPrivateProfileString("DataServerInfo","DataServerUSER","",DataServerUSER,sizeof(DataServerUSER),".\\DataServer.ini");

		GetPrivateProfileString("DataServerInfo","DataServerPASS","",DataServerPASS,sizeof(DataServerPASS),".\\DataServer.ini");

		WORD DataServerPort = GetPrivateProfileInt("DataServerInfo","DataServerPort",55960,".\\DataServer.ini");

		wsprintf(buff,"DataServer_EX0%d_%s",97,DataServerODBC);

		if(CreateMutex(0,0,buff) == 0 || GetLastError() == ERROR_ALREADY_EXISTS)
		{
			LogAdd(LOG_RED,"Could not open DataServer (Already Exists) [%s]", DataServerODBC);
		}
		else
		{
			if(gQueryManager.Connect(DataServerODBC,DataServerUSER,DataServerPASS) == 0)
			{
				LogAdd(LOG_RED,"Could not connect to database (ODBC Error)");
			}
			else
			{
				if(gItemManager.Connect(DataServerODBC,DataServerUSER,DataServerPASS) == false)
				{
					LogAdd(LOG_RED,"Could not connect to database (ODBC Error)");
				}
				else
				{
					if(gSocketManager.Start(DataServerPort) == 0)
					{
						gQueryManager.Disconnect();
					}
					else
					{
						gAllowableIpList.Load("AllowableIpList.txt");

						gBadSyntax.Load("BadSyntax.txt");

						gGuildManager.Init();

						SetTimer(hWnd,TIMER_30000,30000,0);
					}
				}
			}
		}
	}
	else
	{
		LogAdd(LOG_RED,"WSAStartup() failed with error: %d",WSAGetLastError());
	}

	SetTimer(hWnd,TIMER_2000,2000,0);

	HACCEL hAccelTable = LoadAccelerators(hInstance,(LPCTSTR)IDC_DATASERVER);

	MSG msg;

	while(GetMessage(&msg,0,0,0) != 0)
	{
		if(TranslateAccelerator(msg.hwnd,hAccelTable,&msg) == 0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	CMiniDump::Clean();

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) // OK
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance,(LPCTSTR)IDI_DATASERVER);
	wcex.hCursor = LoadCursor(0,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = (LPCSTR)IDC_DATASERVER;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance,(LPCTSTR)IDI_DATASERVER);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance,int nCmdShow) // OK
{
	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, 0, 600, 600, 0, 0, hInstance, 0);

	if(hWnd == 0)
	{
		return 0;
	}

	HWND hWndStatusBar;
	hWndStatusBar = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)IDC_STATUSBAR, hInstance, NULL);
	ShowWindow(hWndStatusBar, SW_HIDE);

	int iQueueBarWidths[] = { 135, 235 ,-1 };

	SendMessage(hWndStatusBar, SB_SETPARTS, 3, (LPARAM)iQueueBarWidths);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	return 1;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_ABOUT:
					DialogBox(hInst,(LPCTSTR)IDD_ABOUTBOX,hWnd,(DLGPROC)About);
					break;
				case IDM_EXIT:
					if(CheckServersOpened() != 0)
					{
						MessageBox(hWnd, "Can not close data server with opened gameservers, close all gameservers first!", "Danger of data loss!", MB_OK | MB_ICONEXCLAMATION);
						break;
					}

					if(MessageBox(hWnd,"Are you sure to terminate DataServer?","Ask terminate server",MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}
					break;
				default:
					return DefWindowProc(hWnd,message,wParam,lParam);
			}
			break;
		case WM_TIMER:
			switch(wParam)
			{
				case TIMER_2000:
					gServerDisplayer.Run();
					break;
				case TIMER_30000:
					gItemManager.SaveItemCount();
					break;
				default:
					break;
			}
			break;
		case WM_CLOSE:
			if(CheckServersOpened() != 0)
			{
				MessageBox(hWnd, "Can not close DataServer with opened GameServer, close all GameServers first!", "Danger of data loss!", MB_OK | MB_ICONEXCLAMATION);
				break;
			}

			if (MessageBox(hWnd, "Are you sure to terminate DataServer?", "Ask terminate server", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT hPaintStruct;
			HDC hdc = BeginPaint(hWnd,&hPaintStruct);
			HDC hMemDC = CreateCompatibleDC(hdc);
			// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Load TopBar bitmap with DIB fallback and clean GDI release
			HBITMAP hBmp = (HBITMAP)LoadImage(hInst,MAKEINTRESOURCE(IDB_BITMAP1),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
			if(hBmp == 0) hBmp = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));
			HBITMAP OldBmp = (HBITMAP)SelectObject(hMemDC,hBmp);
			BitBlt(hdc,0,0,594,80,hMemDC,0,0,SRCCOPY);
			SelectObject(hMemDC,OldBmp);
			DeleteObject(hBmp);
			DeleteDC(hMemDC);
			EndPaint(hWnd,&hPaintStruct);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd,message,wParam,lParam);
	}

	return 0;
}

LRESULT CALLBACK About(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_INITDIALOG:
			return 1;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg,LOWORD(wParam));
				return 1;
			}
			break;
	}

	return 0;
}
