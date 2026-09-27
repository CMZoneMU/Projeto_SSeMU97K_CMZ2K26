#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define PROTECT_START _asm _emit 0xEB _asm _emit 0x0E _asm _emit 0x14 _asm _emit 0x28 _asm _emit 0xCD _asm _emit 0xDB _asm _emit 0x50 _asm _emit 0x63 _asm _emit 0xAF _asm _emit 0x22 _asm _emit 0x75 _asm _emit 0xEE _asm _emit 0xB5 _asm _emit 0xD8 _asm _emit 0x43 _asm _emit 0x72
#define PROTECT_FINAL _asm _emit 0xEB _asm _emit 0x0E _asm _emit 0x37 _asm _emit 0xF2 _asm _emit 0xCD _asm _emit 0xDB _asm _emit 0x50 _asm _emit 0x63 _asm _emit 0xAF _asm _emit 0x22 _asm _emit 0x75 _asm _emit 0xEE _asm _emit 0xB5 _asm _emit 0xD8 _asm _emit 0x85 _asm _emit 0x61

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 99 (2.5.8) - Update title bar version
#define GAMESERVER_VERSION "[GameServer97k] [%s] (%s) (Update 99 - Ver: 2.5.8)"

#ifndef GAMESERVER_UPDATE
#define GAMESERVER_UPDATE 99
#endif

#ifndef GAMESERVER_EXTRA
#define GAMESERVER_EXTRA 1
#endif

#ifndef ENCRYPT_STATE
#define ENCRYPT_STATE 1
#endif

// System Include
#include <windows.h>
#include <winsock2.h>
#include <mswSock.h>
#include <commctrl.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <map>
#include <vector>
#include <queue>
#include <random>
#include <Rpc.h>
#include <algorithm>
#include <string>
#include <atltime.h>
#include <mutex>
#include <atomic>
#include <dbghelp.h>
#include <Psapi.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <shellapi.h>
#include "..\\..\\..\\Util\\lua\\include\\lua.hpp"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"..\\..\\..\\Util\\lua\\lua52.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")

typedef unsigned __int64 QWORD;