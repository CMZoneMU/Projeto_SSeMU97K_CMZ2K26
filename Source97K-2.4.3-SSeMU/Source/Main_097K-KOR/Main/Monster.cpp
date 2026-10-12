#include "stdafx.h"
#include "Monster.h"
#include "CustomMonster.h"
#include "Texture.h"
#include "Offset.h"
#include "Util.h"
#include "LoadModels.h"

void InitMonster() // OK
{
	SetWord(0x0044AE16+2,0xFFFF); // Monster Kill

	SetCompleteHook(0xE8,0x00429CF6,&CreateMonster);

	SetCompleteHook(0xE8,0x0042A288,&CreateMonster);

	SetCompleteHook(0xE8,0x0042A592,&CreateMonster);

	SetCompleteHook(0xE8,0x0047D0F0,&CreateMonster);

	SetCompleteHook(0xE8,0x004C07C2,&CreateMonster);
}

DWORD CreateMonster(int index,int x,int y,int key) // OK
{
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 107 (2.6.6) - Support monster index up to 512 from viewport packet
	int highBit = 0;
	_asm
	{
		push eax
		mov eax, ebx
		cmp eax, 0x00100000
		jb SKIP_HIGH
		cmp eax, 0x7FFFFFFF
		ja SKIP_HIGH
		movzx eax, byte ptr [ebx+9]
		and eax, 0x03
		mov highBit, eax
	SKIP_HIGH:
		pop eax
	}

	if(highBit != 0)
	{
		index |= (highBit << 8);
	}

	CUSTOM_MONSTER_INFO* lpInfo = gCustomMonster.GetInfoByIndex(index);

	if(lpInfo != 0)
	{
		// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 107 (2.6.6) - Safe model index above MAX_MODELS to prevent collision with items
		int aIndex = MAX_MODELS + lpInfo->Index;

		DWORD o = gLoadModels.GetModels() + (188 * aIndex);

		if(lpInfo->Type == 0 || lpInfo->Type == 2)
		{
			if(*(short*)(o + 38) <= 0)
			{
				char path[MAX_PATH] = {0};

				wsprintf(path,"Data\\%s",lpInfo->FolderPath);

				gLoadModels.MyAccessModel(aIndex,path,lpInfo->ModelName,-1);

				for(int n = 0;n < *(short*)(o + 38);++n)
				{
					*(float*)(*(DWORD*)(o + 48) + 16 * n + 4) = 0.25f;
				}
			}
		}
		else
		{
			if(*(short*)(o + 38) <= 0  && *(short*)(o + 36) <= 0)
			{
				char path[MAX_PATH] = {0};

				wsprintf(path,"Data\\%s",lpInfo->FolderPath);

				gLoadModels.MyAccessModel(aIndex,path,lpInfo->ModelName,-1);

				if(*(short*)(o + 36) > 0)
				{
					*(float*)(*(DWORD*)(o + 48) + 4) = 0.25f;
					*(float*)(*(DWORD*)(o + 48) + 20) = 0.2f;
					*(float*)(*(DWORD*)(o + 48) + 36) = 0.34f;
					*(float*)(*(DWORD*)(o + 48) + 52) = 0.33f;
					*(float*)(*(DWORD*)(o + 48) + 68) = 0.33f;
					*(float*)(*(DWORD*)(o + 48) + 84) = 0.5f;
					*(float*)(*(DWORD*)(o + 48) + 100) = 0.55f;
					*(bool*)(*(DWORD*)(o + 48) + 96) = true;
				}
			}
		}

		if(*(short*)(o + 36) > 0)
		{
			gLoadModels.MyOpenTexture(aIndex,lpInfo->FolderPath,GL_NEAREST, GL_TRUE);
		}

		DWORD pCharacter = ((DWORD(*)(int,int,BYTE,BYTE,float))0x0045BFA0)(key,((lpInfo->Type>1)?390:aIndex),x,y,0.0f);

		if(pCharacter)
		{
			memcpy((DWORD*)(pCharacter + 449),lpInfo->Name,sizeof(lpInfo->Name));

			*(DWORD*)(pCharacter + 747) = ((lpInfo->Effect!=-1)?lpInfo->Effect:index);

			*(BYTE*)(pCharacter + 132) = ((lpInfo->Type == 0 || lpInfo->Type == 2)?4:2);

			*(float*)(pCharacter + 12) = lpInfo->Scale;

			*(BYTE*)(pCharacter + 8) = 0;

			*(short*)(pCharacter + 784) = *(short*)(0x05826CA0);

			if(lpInfo->Type >= 2)
			{
				*(BYTE*)(pCharacter + 444) = lpInfo->NpcClass;

				*(BYTE*)(pCharacter + 746) = lpInfo->PKLevel;

				for(int n=0;n < 5;n++)
				{
					*(WORD*)(pCharacter + 504 + 24 * n) = (lpInfo->NpcClass & 7) + 912 + 8 * n;
				}

				for(int n=0;n < MAX_SKIN_SLOT;n++)
				{
					if(lpInfo->SkinInfo[n].Use == true)
					{
						*(WORD*)(pCharacter + 504 + 24 * n) = lpInfo->SkinInfo[n].ItemIndex+ITEM_BASE_MODEL;
						*(BYTE*)(pCharacter + 506 + 24 * n) = lpInfo->SkinInfo[n].ItemLevel;
						*(BYTE*)(pCharacter + 507 + 24 * n) = lpInfo->SkinInfo[n].NewOption;
					}
				}

				((void(*)(DWORD))0x0045C050)(pCharacter);
			}

			return pCharacter;
		}
	}

	return ((DWORD(*)(int,int,int,int,int))0x0045CCF0)(index,x,y,key,0);
}