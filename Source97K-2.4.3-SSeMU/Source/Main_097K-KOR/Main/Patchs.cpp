#include "stdafx.h"
#include "Attack.h"
#include "ItemMove.h"
#include "Notice.h"
#include "Patchs.h"
#include "Protect.h"
#include "Protocol.h"

char WindowName[128];
int g_IncreaseFPSSwitch = 1;

void InitPatchs()
{
	SetByte(0x0042C417, 0xE8); // Fix autobuff view to others

	SetByte(0x00558EA8, 0xA0); // Accent

	SetByte(0x00406F36, 0xEB); // Crack return

	SetByte(0x00406F5F, 0xEB); // Crack error messagebox

	SetByte(0x00406F9B, 0xEB); // Crack error messagebox

	SetByte(0x0041ECB5, 0xEB); // Crack mu.exe

	SetByte(0x0041ED25, 0xEB); // Crack config.ini

	SetByte(0x0041ED5E, 0xEB); // Crack config.ini

	SetByte(0x0041EFB5, 0xEB); // Crack gg init

	SetByte(0x0053D90B, 0xEB); // Crack gameguard

	SetByte(0x0055961C, (gProtect.m_MainInfo.ClientVersion[0] + 1)); // Version

	SetByte(0x0055961D, (gProtect.m_MainInfo.ClientVersion[2] + 2)); // Version

	SetByte(0x0055961E, (gProtect.m_MainInfo.ClientVersion[3] + 3)); // Version

	SetByte(0x0055961F, (gProtect.m_MainInfo.ClientVersion[5] + 4)); // Version

	SetByte(0x00559620, (gProtect.m_MainInfo.ClientVersion[6] + 5)); // Version

	SetWord(0x005615BC, (gProtect.m_MainInfo.IpAddressPort)); // IpAddressPort

	SetDword(0x005267B2, (DWORD)gProtect.m_MainInfo.ScreenShotPath); //Screenshot

	MemoryCpy(0x00558ED8, gProtect.m_MainInfo.IpAddress, sizeof(gProtect.m_MainInfo.IpAddress)); // IpAddress

	MemoryCpy(0x00559624, gProtect.m_MainInfo.ClientSerial, sizeof(gProtect.m_MainInfo.ClientSerial)); // ClientSerial

	MemorySet(0x004127B0, 0x90, 5); // Remove MuError.log

	SetByte(0x0041F0AC, ANTIALIASED_QUALITY); //Font Quality

	SetByte(0x0041F0ED, ANTIALIASED_QUALITY); //Font Quality

	SetByte(0x0041F12E, ANTIALIASED_QUALITY); //Font Quality

	SetCompleteHook(0xE9, 0x00524146, 0x00524231); // Remove Select Character Texts

	SetWord(0x00444B76, 0x19EB); //-> Uniria & Dinorant Reflect

	g_IncreaseFPSSwitch = GetPrivateProfileInt("Config", "IncreaseFPSSwitch", GetPrivateProfileInt("Graphics", "IncreaseFPSSwitch", 1, ".\\Config.ini"), ".\\Config.ini");

	if (g_IncreaseFPSSwitch != 0)
	{
		// 30 FPS Fluido (32 ms / ~31.25 FPS em vez de 40 ms / 25 FPS)
		// DecIDA_Main 97.11 WebZen.c Linha 205228: while (dword_5616B8 >= 32)
		SetByte(0x00525D57, 0x20);

		// DecIDA_Main 97.11 WebZen.c Linha 205533: dword_5616B8 -= 32
		SetByte(0x00526713, 0x20);
	}

	// Troca GetTickCount() por timeGetTime() no inicio da medicao do frame (0x0052696D: call ds:[00552404])
	// Garante resolucao de 1ms em vez dos 15.6ms do TickCount que travavam o jogo em 24 FPS
	SetDword(0x0052696F, 0x00552404);

	// Hook de Frame Wait de alta precisao e Reducao de CPU (0x00526A3F a 0x00526A6E = 47 bytes)
	// Substitui o loop busy-wait por suspensao hibrida (Sleep(1) + Sleep(0)), reduzindo CPU para < 2% e cravando 31.25 FPS
	SetCompleteHook(0xE9, 0x00526A3F, &HookFrameWait);
	MemorySet(0x00526A3F + 5, 0x90, 42);


	// CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ReduceRam, 0, 0, 0); // Disabled to prevent constant RAM flushing


	// Fix move cursor NPC
	SetByte(0x00430B9F, 0x90);
	MemorySet(0x00430BBD, 0x90, 0x7);

	// Fix move cursor TRADE
	SetByte(0x004335FC, 0x90);
	SetByte(0x00433621, 0x90);
	MemorySet(0x00433635, 0x90, 0x6);

	// Fix move cursor
	SetByte(0x00492EBD, 0x90);
	MemorySet(0x00492EDB, 0x90, 0x7);

	// Fix Shop NPC Closing send to server
	SetCompleteHook(0xE9, 0x004CBB15, &FixShopNpcClose);

	// Fix Drop Zen
	MemorySet(0x0042F2DF, 0x90, 0x7);

	// Fix Trade Zen Over 50000000
	SetByte(0x00515BF7, 0xEB);

	SetCompleteHook(0xE8, 0x0042B33D, &IgnoreRandomStuck);

	SetCompleteHook(0xE9, 0x00483AC5, &FixChasingAttackMovement);

	// Decrypt BMD
	SetCompleteHook(0xE9, 0x004424B4, &DecBMD);

	// EncTerrain%d.map
	SetByte(0x0050E5F9, 0xEB);
	SetDword(0x0050E623 + 1, (DWORD)"Data\\%s\\EncTerrain%d.map");
	// EncTerrain%d.att
	SetDword(0x0050E648 + 1, (DWORD)"Data\\%s\\EncTerrain%d.att");
	// EncTerrain%d.obj
	SetByte(0x0050E663, 0xEB);
	SetDword(0x0050E68D + 1, (DWORD)"Data\\%s\\EncTerrain%d.obj");

	// Decrypt MAP
	SetCompleteHook(0xE8, 0x0050E636, &OpenTerrainMapping);

	// Decrypt ATT
	SetCompleteHook(0xE8, 0x0050E658, &OpenTerrainAttribute);

	// Decrypt OBJ
	SetCompleteHook(0xE8, 0x0050E6A0, &OpenObjectsEnc);

	SetCompleteHook(0xE8, 0x00525D4C, &CalcFPS);

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.4) - Hide right dragon (Menu04) when right windows are open
	SetCompleteHook(0xE8, 0x004BD332, &RenderRightDragon);

	// --- Render path principal (gameplay normal) ---
	// Patch 1: Transparência do corpo do item a partir de +14 (era +12)
	SetByte(0x0050565F, 0x0D);

	// Patch 2: Alinha o bloco do brilho dourado (overlay 0x240) até +13
	SetByte(0x005056E3, 0x0E);

	// --- Render path secundário (troca de mapa / recarregar personagem) ---
	// Patch 3: Transparência no carregamento de mapa
	SetByte(0x004F825F, 0x0D);

	// Patch 4: Texture lookup path A
	SetByte(0x004EF126, 0x0D);

	// Patch 5: Texture lookup path B
	SetByte(0x004EF451, 0x0D);

	// Update 88: Correção do NPC 255 (Phantom Soldier / Lumen the Barmaid / Garçonete de Lorencia)
	SetByte(0x004689AB, 0xE9);
	SetDword(0x004689AB + 1, 0x000028BB);
}

void CalcFPS() // OK
{
	((void(*)())0x0043FD70)();

	if(SceneFlag == 5)
	{
		ScrollNotice();

		RightClickMove();

		static DWORD dwLastTitleUpdate = 0;
		if (GetTickCount() - dwLastTitleUpdate >= 500)
		{
			dwLastTitleUpdate = GetTickCount();

			char szTitle[256];
			char* baseTitle = (WindowName[0] == 0) ? gProtect.m_MainInfo.WindowName : WindowName;

			sprintf_s(szTitle, "%s || FPS: %.0f", baseTitle, FPS);
			SetWindowText(g_hWnd, szTitle);
		}

		if(CustomAttack != 0)
		{
			if((Attacking != -1 || MouseRButton || MouseRButtonPush) && !MouseOnWindow)
			{
				CustomAttack = 0;

				PBMSG_HEAD pMsg;

				pMsg.set(0x04,sizeof(pMsg));

				DataSend((BYTE*)&pMsg,pMsg.size);
			}
		}
	}
	else
	{
		ClearNotice();
		CustomAttack = 0;
		WindowName[0] = 0;
		SetWindowText(g_hWnd,gProtect.m_MainInfo.WindowName);
	}
}

DWORD WINAPI FrameWait(DWORD dwStartTime)
{
	DWORD dwTarget = (g_IncreaseFPSSwitch != 0) ? 32 : 40;
	DWORD dwElapsed = timeGetTime() - dwStartTime;

	while (dwElapsed < dwTarget)
	{
		DWORD dwRemaining = dwTarget - dwElapsed;

		if (dwRemaining > 2)
		{
			Sleep(1);
		}
		else
		{
			Sleep(0);
		}

		dwElapsed = timeGetTime() - dwStartTime;
	}

	return dwElapsed;
}

__declspec(naked) void HookFrameWait()
{
	static DWORD JmpBack = 0x00526A6E;

	__asm
	{
		Mov Eax, Dword Ptr Ss:[Ebp-0x158]
		Push Eax
		Call FrameWait
		Mov Dword Ptr Ss:[Ebp-0x160], Eax
		Jmp [JmpBack]
	}
}


void ReduceRam(LPVOID lpThreadParameter)
{
	// Function disabled to prevent RAM paging stutter
}

__declspec(naked) void FixShopNpcClose()
{
	static DWORD jmpBack = 0x004CBB1A;
	static BYTE InterfaceID;

	_asm
	{
		Pushad;
		Mov Bl, Byte Ptr Ds : [Eax] ;
		Mov InterfaceID, Bl;
	}

	if (InterfaceID == 0xFE) // Case Shop
	{
		static PBMSG_HEAD pMsg;

		pMsg.set(0x31, sizeof(pMsg));

		DataSend((BYTE*)&pMsg, pMsg.size);
	}

	_asm
	{
		Popad;
		Mov Bl, [Eax + 1];
		Inc Bl;
		Jmp jmpBack;
	}
}

void IgnoreRandomStuck(DWORD c, DWORD Damage)
{
	if (*(WORD*)(c + 2) != 390)
	{
		((void(_cdecl*)(DWORD c, DWORD Damage)) 0x00444B60)(c, Damage);
	}
}

_declspec(naked) void FixChasingAttackMovement()
{
	static DWORD jmpBack = 0x00483ACC;
	static DWORD SendMove = 0x00491C40;

	_asm
	{
		Mov Byte Ptr[Ebp + 0x2ED], 5;
		Push Ebp;
		Push Ebp;
		Call SendMove;
		Add Esp, 0x8;
		Jmp jmpBack;
	}
}

_declspec(naked) void DecBMD()
{
	static DWORD jmpBack = 0x004424BA;
	static int DataSize;
	static BYTE* Data;

	_asm
	{
		Mov DataSize, Edi;
		Mov Data, Ebx;
		Pushad;
	}

	if (Data[3] == 0xC)
	{
		Decrypt(&Data[4], &Data[8], *((long*)(Data + 4)));
	}

	_asm
	{
		Popad;
		Mov Cl, [Ebx + 3];
		Lea Esi, [Ebx + 4];
		Jmp jmpBack;
	}
}

int OpenTerrainMapping(char* FileName)
{
	((void(*)()) 0x004F6C60)();

	FILE* fp;

	if (fopen_s(&fp, FileName, "rb") != 0)
	{
		return -1;
	}

	fseek(fp, 0, SEEK_END);

	int EncBytes = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	BYTE* EncData = new BYTE[EncBytes];

	fread(EncData, 1, EncBytes, fp);

	fclose(fp);

	BYTE* Data = new BYTE[EncBytes];

	Decrypt(Data, EncData, EncBytes);

	delete[] EncData;

	int DataPtr = 0;

	BYTE Version = Data[DataPtr];

	DataPtr += 1;

	int iMapNumber = Data[DataPtr];

	DataPtr += 1;

	memcpy((BYTE*)0x080BB2B4, Data + DataPtr, 256 * 256);

	DataPtr += 256 * 256;

	memcpy((BYTE*)0x080AB2B4, Data + DataPtr, 256 * 256);

	DataPtr += 256 * 256;

	for (int i = 0; i < 256 * 256; i++)
	{
		BYTE Alpha;

		Alpha = *((BYTE*)(Data + DataPtr));

		DataPtr += 1;

		*(float*)(0x0834B608 + (i * 4)) = (float)Alpha / 255.0f;
	}

	delete[] Data;

	fclose(fp);

	return iMapNumber;
}

int OpenTerrainAttribute(char* FileName)
{
	FILE* fp;

	if (fopen_s(&fp, FileName, "rb") != 0)
	{
		char Text[256];

		sprintf_s(Text, "%s file not found.", FileName);

		MessageBox(g_hWnd, Text, NULL, MB_OK);

		SendMessage(g_hWnd, WM_DESTROY, 0, 0);

		return  -1;
	}

	fseek(fp, 0, SEEK_END);

	int EncBytes = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	BYTE* EncData = new BYTE[EncBytes];

	fread(EncData, EncBytes, 1, fp);

	BYTE* byBuffer = new BYTE[EncBytes];

	Decrypt(byBuffer, EncData, EncBytes);

	delete[] EncData;

	if (EncBytes != 131076 && EncBytes != 65540)
	{
		delete[] byBuffer;

		return -1;
	}

	bool extAtt = (EncBytes == 131076);

	PacketArgumentEncrypt(byBuffer, byBuffer, EncBytes);

	BYTE Version = byBuffer[0];

	int iMap = byBuffer[1];

	BYTE Width = byBuffer[2];

	BYTE Height = byBuffer[3];

	if (extAtt == false)
	{
		memcpy((BYTE*)0x0838BC70, &byBuffer[4], 256 * 256);
	}
	else
	{
		BYTE TWall[256 * 256 * sizeof(WORD)];

		memcpy(TWall, &byBuffer[4], 256 * 256 * sizeof(WORD));

		for (int i = 0; i < 256 * 256; ++i)
		{
			*(BYTE*)(0x0838BC70 + i) = TWall[i * 2];
		}
	}

	delete[] byBuffer;

	bool Error = false;

	if (Version != 0 || Width != 255 || Height != 255)
	{
		Error = true;
	}

	switch (World)
	{
		case MAP_LORENCIA:
		{
			if (*(BYTE*)(0x0838BC70 + (123 * 256 + 135)) != 5)
			{
				Error = true;
			}

			break;
		}

		case MAP_DUNGEON:
		{
			if (*(BYTE*)(0x0838BC70 + (120 * 256 + 227)) != 4)
			{
				Error = true;
			}

			break;
		}

		case MAP_DEVIAS:
		{
			if (*(BYTE*)(0x0838BC70 + (55 * 256 + 208)) != 5)
			{
				Error = true;
			}

			break;
		}

		case MAP_NORIA:
		{
			if (*(BYTE*)(0x0838BC70 + (119 * 256 + 186)) != 5)
			{
				Error = true;
			}

			break;
		}

		case MAP_LOST_TOWER:
		{
			if (*(BYTE*)(0x0838BC70 + (75 * 256 + 193)) != 5)
			{
				Error = true;
			}

			break;
		}
	}

	for (int i = 0; i < 256 * 256; i++)
	{
		if (*(BYTE*)(0x0838BC70 + i) >= 128)
		{
			Error = true;
		}
	}

	if (Error)
	{
		((void(*)()) 0x004F6CB0)();

		return -1;
	}

	fclose(fp);

	return iMap;
}

int OpenObjectsEnc(char* FileName)
{
	FILE* fp;

	if (fopen_s(&fp, FileName, "rb") != 0)
	{
		char Text[256];

		sprintf_s(Text, "%s file not found.", FileName);

		MessageBox(g_hWnd, Text, NULL, MB_OK);

		SendMessage(g_hWnd, WM_DESTROY, 0, 0);

		return -1;
	}

	fseek(fp, 0, SEEK_END);

	int EncBytes = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	BYTE* EncData = new BYTE[EncBytes];

	fread(EncData, 1, EncBytes, fp);

	fclose(fp);

	BYTE* Data = new BYTE[EncBytes];

	Decrypt(Data, EncData, EncBytes);

	delete[] EncData;

	int DataPtr = 0;

	BYTE Version = Data[DataPtr];

	DataPtr += 1;

	int iMapNumber = Data[DataPtr];

	DataPtr += 1;

	short Count = *((short*)(Data + DataPtr));

	DataPtr += 2;

	for (int i = 0; i < Count; i++)
	{
		float Position[3];

		float Angle[3];

		short Type = *((short*)(Data + DataPtr));

		DataPtr += 2;

		memcpy(Position, Data + DataPtr, sizeof(float) * 3);

		DataPtr += sizeof(float) * 3;

		memcpy(Angle, Data + DataPtr, sizeof(float) * 3);

		DataPtr += sizeof(float) * 3;

		float Scale = *((float*)(Data + DataPtr));

		DataPtr += 4;

		((DWORD(__cdecl*)(int Type, float Position[3], float Angle[3], float Scale)) 0x004FF5A0)(Type, Position, Angle, Scale); // CreateObject
	}

	delete[] Data;

	fclose(fp);

	return iMapNumber;
}

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.4) - Check if any right-side window is open
static bool CheckRightWindowsOpen()
{
	if ((*(BYTE*)0x07EAA114 == 1) || // Guild ("G")
		(*(BYTE*)0x07EAA115 == 1) || // Party ("P")
		(*(BYTE*)0x07EAA116 == 1) || // Character ("C")
		(*(BYTE*)0x07EAA117 == 1) || // Inventory ("V")
		(*(BYTE*)0x07EAA119 == 1) || // Warehouse (Bau)
		(*(BYTE*)0x07EAA11A == 1) || // Chaos Machine
		(*(BYTE*)0x07EAA11B == 1) || // Trade
		(*(BYTE*)0x007EAA130 == 1))  // Event Time Window
	{
		return true;
	}

	return false;
}

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.4) - Render right dragon (Menu04) only when windows are closed
void __cdecl RenderRightDragon(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	if (CheckRightWindowsOpen())
	{
		return;
	}

	RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
}