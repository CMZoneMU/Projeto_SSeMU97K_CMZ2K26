#include "stdafx.h"
#include "Attack.h"
#include "ChaosMix.h"
#include "Camera.h"
#include "CustomChangeClass.h"
#include "Effect.h"
#include "EventEntryLevel.h"
#include "Font.h"
#include "Fog.h"
#include "GoldenArcher.h"
#include "HackCheck.h"
#include "HealthBar.h"
#include "ItemMove.h"
#include "ItemToolTip.h"
#include "Language.h"
#include "OptionsMenu.h"
#include "WeaponView.h"
#include "LoadModels.h"
#include "Map.h"
#include "Monster.h"
#include "PacketManager.h"
#include "Patchs.h"
#include "PeriodicItem.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Protocol.h"
#include "Reconnect.h"
#include "Resolution.h"
#include "ServerList.h"
#include "Shop.h"
#include "Skill.h"
#include "Sound.h"
#include "Texture.h"
#include "TrayMode.h"
#include "Window.h"
#include "WindowTime.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

HHOOK HookKB,HookMS,HookDW;
HINSTANCE hins;

LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam) // OK
{
	if(nCode == HC_ACTION)
	{
		if(((DWORD)lParam & (1 << 30)) != 0 && ((DWORD)lParam & (1 << 31)) != 0 && GetForegroundWindow() == g_hWnd)
		{
			if(SceneFlag == 5 && wParam == 72)
			{
				WindowButtonToggle();
			}
			else if(gProtect.m_MainInfo.KeyCodeAutoAttackSwitch != 0 && wParam == gProtect.m_MainInfo.KeyCodeAutoAttackSwitch)
			{
				AutoAttackToggle();
			}
			else if(gProtect.m_MainInfo.KeyCodeHealthBarSwitch != 0 && wParam == gProtect.m_MainInfo.KeyCodeHealthBarSwitch)
			{
				HealthBarToggle();
			}
			else if(gProtect.m_MainInfo.KeyCodeCamera3DSwitch != 0 && wParam == gProtect.m_MainInfo.KeyCodeCamera3DSwitch)
			{
				gCamera.Toggle();
			}
			else if(gProtect.m_MainInfo.KeyCodeCamera3DRestore != 0 && wParam == gProtect.m_MainInfo.KeyCodeCamera3DRestore)
			{
				gCamera.Restore();
			}
			else if(gProtect.m_MainInfo.KeyCodeTrayModeSwitch != 0 && wParam == gProtect.m_MainInfo.KeyCodeTrayModeSwitch)
			{
				gTrayMode.Toggle();
			}
			// Update CMZ 17 (3.1.7) 28-09-26 - Toggle Custom Change Class window
			else if(SceneFlag == 5 && !InputEnable && wParam == 'J')
			{
				gCustomChangeClass.Toggle();
			}
		}
	}

	return CallNextHookEx(HookKB,nCode,wParam,lParam);
}

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam) // OK
{
	if(nCode == HC_ACTION)
	{
		MOUSEHOOKSTRUCTEX* HookStruct =(MOUSEHOOKSTRUCTEX*)lParam;

		if(GetForegroundWindow() == g_hWnd)
		{
			switch(wParam)
			{
				case WM_MOUSEMOVE:
					gCamera.Move(HookStruct);
					break;
				case WM_MBUTTONDOWN:
					gCamera.SetIsMove(1);
					gCamera.SetCursorX(HookStruct->pt.x);
					gCamera.SetCursorY(HookStruct->pt.y);
					break;
				case WM_MBUTTONUP:
					gCamera.SetIsMove(0);
					break;
				case WM_MOUSEWHEEL:
					gCamera.Zoom(HookStruct);
					break;
			}
		}
	}

	return CallNextHookEx(HookMS,nCode,wParam,lParam);
}

SHORT WINAPI KeysProc(int nCode) // OK
{
	if(GetForegroundWindow() != g_hWnd)
	{
		return 0;
	}

	return GetAsyncKeyState(nCode);
}

void WINAPI ReduceConsumeProc() // OK
{
	// Disabled to prevent purging RAM to disk and causing stutter / freezing
}

extern "C" _declspec(dllexport) void _cdecl EntryProc()
{
	timeBeginPeriod(1);

	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 98 (2.5.7) - Change ServerInfo.sse to Data/Custom/MainInfo/ServerInfo.cmz
	if(gProtect.ReadMainFile("Data\\Custom\\MainInfo\\ServerInfo.cmz") != 0)
	{
		gProtect.CheckLauncher();

		gProtect.CheckInstance();

		gProtect.CheckClientFile();

		InitPatchs();

		InitHackCheck();

		InitProtocol();

		InitEffect();

		InitGoldenArcher();

		//InitLanguage();
		
		gLanguage.Init();
		
		gWeaponView.Init();

		gOptionsMenu.Init();

		InitPrintPlayer();

		InitReconnect();

		gResolution.Init();

		InitItemMove();

		InitItemToolTip();

		gChaosMix.Init();

		InitMap();

		InitTexture();

		InitMonster();

		// Update 21 Kayito -> 97K - Fix Texturas (Zeus)
		gLoadModels.Init();
		// Fim Update 21 Kayito -> 97K

		InitServerList();

		InitSkill();

		InitShop();

		InitFog();

		InitFont();

		InitEventEntryLevel();

		InitWindowTime();

		// Update CMZ 17 (3.1.7) 28-09-26 - Init Custom Change Class Interface
		gCustomChangeClass.Init();

		gSound.Init();

		SetDword(0x00552398,(DWORD)&KeysProc);

		SetCompleteHook(0xE8,0x004BC0AE,&DrawNewHealthBar);

		gCustomEffect.Load(gProtect.m_MainInfo.CustomEffectInfo);

		gCustomFog.Load(gProtect.m_MainInfo.CustomFogInfo);

		gCustomItem.Load(gProtect.m_MainInfo.CustomItemInfo);

		gCustomMap.Load(gProtect.m_MainInfo.CustomMapInfo);

		gCustomMessage.LoadEng(gProtect.m_MainInfo.EngCustomMessageInfo);

		gCustomMessage.LoadPor(gProtect.m_MainInfo.PorCustomMessageInfo);

		gCustomMessage.LoadSpn(gProtect.m_MainInfo.SpnCustomMessageInfo);

		gCustomMonster.Load(gProtect.m_MainInfo.CustomMonsterInfo);

		gCustomTooltip.Load(gProtect.m_MainInfo.CustomTooltipInfo);

		gSkillAttackSpeed.Load(gProtect.m_MainInfo.SkillAttackSpeedInfo);

		gPacketManager.LoadEncryptionKey("Data\\Enc1.dat");

		gPacketManager.LoadDecryptionKey("Data\\Dec2.dat");

		HookKB = SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,hins,GetCurrentThreadId());

		HookMS = SetWindowsHookEx(WH_MOUSE,MouseProc,hins,GetCurrentThreadId());

		// CreateThread(0,0,(LPTHREAD_START_ROUTINE)ReduceConsumeProc,0,0,0);

		

		#if(DEBUG_CONSOLE == 1)

		if(AllocConsole() == 0)
		{
			ErrorMessageBox("Could not open AllocConsole()");
			return;
		}

		SetConsoleTitleA("SSeMU || Debugger");

		DeleteMenu(GetSystemMenu(GetConsoleWindow(),0),SC_CLOSE,MF_BYCOMMAND);

		#endif

		gProtect.CheckPluginFile();
	}
	else
	{
		ErrorMessageBox("Could not load Data\\Custom\\MainInfo\\ServerInfo.cmz!");
		ExitProcess(0);
	}
}
BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved) // OK
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			timeBeginPeriod(1);
			hins = (HINSTANCE)hModule;
			gWindow.WindowModeLoad(hins);
			gTrayMode.Init(hins);
			break;
		case DLL_PROCESS_DETACH:
			timeEndPeriod(1);
			break;
	}

	return 1;
}