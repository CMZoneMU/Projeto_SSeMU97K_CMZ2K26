#include "stdafx.h"
#include "Attack.h"
#include "Protocol.h"
#include "ChaosMix.h"
#include "GoldenArcher.h"
#include "HackCheck.h"
#include "HealthBar.h"
#include "Language.h"
#include "Notice.h"
#include "PacketManager.h"
#include "Patchs.h"
#include "PeriodicItem.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Reconnect.h"
#include "ServerList.h"
#include "Shop.h"
#include "Skill.h"
#include "WindowTime.h"
#include "OptionsMenu.h"

void InitProtocol()
{
	SetCompleteHook(0xE9, 0x00439505, &HookProtocol);
}

_declspec(naked) void HookProtocol()
{
	static DWORD jmpBack = 0x0043950B;

	_asm
	{
		Pushad;
		Push Ebp;
		Push Esi;
		Call ProtocolCoreEx;
		Popad;
		Mov Edx, Dword Ptr Ds : [0x7E11DCC] ;
		Jmp jmpBack;
	}
}

void __stdcall ProtocolCoreEx(BYTE head, BYTE* lpMsg)
{
	switch (head)
	{
		case 0x02:
			if (m_bWhisperSound && m_SoundOnOff)
			{
				PlayBuffer(25, 0, 0);
			}
			break;
		case 0x15:
			GCDamageRecv((PMSG_DAMAGE_RECV*)lpMsg);
			break;
		case 0xDE:
			GCCharacterCreationEnableRecv((PMSG_CHARACTER_CREATION_ENABLE_RECV*)lpMsg);
			break;
		case 0x17:
			GCUserDieRecv((PMSG_USER_DIE_RECV*)lpMsg);
			break;
		case 0x1E:
			GCDurationSkillAttackRecv((PMSG_DURATION_SKILL_ATTACK_RECV*)lpMsg);
			break;
		case 0x24:
			gChaosMix.Clear();
			break;
		case 0x26:
			GCLifeRecv((PMSG_LIFE_RECV*)lpMsg);
			break;
		case 0x27:
			GCManaRecv((PMSG_MANA_RECV*)lpMsg);
			break;
		case 0x87:
			gChaosMix.Clear();
			break;
		case 0x88:
			gChaosMix.GCChaosMixRateRecv((PMSG_CHAOS_MIX_RATE_RECV*)lpMsg);
			break;
		case 0x94:
			GoldenArcherCountRecv((PMSG_GOLDEN_ARCHER_COUNT_RECV*)lpMsg);
			break;
		case 0x9C:
			GCRewardExperienceRecv((PMSG_REWARD_EXPERIENCE_RECV*)lpMsg);
			break;
		case 0xA3:
			GCQuestRewardRecv((PMSG_QUEST_REWARD_RECV*)lpMsg);
			break;
		case 0xD5:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					GCPeriodicItemInit();
					break;
				case 0x01:
					GCPeriodicItemRecv((PMSG_PERIODIC_ITEM_RECV*)lpMsg);
					break;
				case 0x02:
					GCPeriodicItemDeleteRecv((PMSG_PERIODIC_ITEM_DELETE_RECV*)lpMsg);
					break;
			}
			break;
		case 0xF1:
			switch(((lpMsg[0]== 0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					GCConnectClientRecv((PMSG_CONNECT_CLIENT_RECV*)lpMsg);
					break;
				case 0x01:
					GCConnectAccountRecv((PMSG_CONNECT_ACCOUNT_RECV*)lpMsg);
					break;
				case 0x02:
					GCCloseClientRecv((PMSG_CLOSE_CLIENT_RECV*)lpMsg);
					break;
			}
			break;
		case 0xF3:
			switch(((lpMsg[0]== 0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					GCCharacterListRecv((PMSG_CHARACTER_LIST_RECV*)lpMsg);
					break;
				case 0x03:
					GCCharacterInfoRecv((PMSG_CHARACTER_INFO_RECV*)lpMsg);
					break;
				case 0x04:
					GCCharacterRegenRecv((PMSG_CHARACTER_REGEN_RECV*)lpMsg);
					break;
				case 0x05:
					GCLevelUpRecv((PMSG_LEVEL_UP_RECV*)lpMsg);
					break;
				case 0x06:
					GCLevelUpPointRecv((PMSG_LEVEL_UP_POINT_RECV*)lpMsg);
					break;
				case 0x07:
					GCMonsterDamageRecv((PMSG_MONSTER_DAMAGE_RECV*)lpMsg);
					break;
				case 0xE0:
					GCNewCharacterInfoRecv((PMSG_NEW_CHARACTER_INFO_RECV*)lpMsg);
					break;
				case 0xE1:
					GCNewCharacterCalcRecv((PMSG_NEW_CHARACTER_CALC_RECV*)lpMsg);
					break;
				case 0xE2:
					GCHealthBarRecv((PMSG_NEW_HEALTH_BAR_RECV*)lpMsg);
					break;
				case 0xE4:
					GCNewMessageRecv((PMSG_NEW_MESSAGE_RECV*)lpMsg);
					break;
				case 0xE8:
					GCWindowNameRecv((PMSG_WINDOW_NAME_RECV*)lpMsg);
					break;
				case 0xEA:
					CCServerNameRecv((PMSG_SERVER_NAME_RECV*)lpMsg);
					break;
				case 0xEB:
					GCCustomAttackStatusRecv((PMSG_CUSTOM_ATTACK_STATUS_RECV*)lpMsg);
					break;
				case 0xEC:
					GCItemStackListRecv((PMSG_ITEM_LIST_INFO_RECV*)lpMsg);
					break;
				case 0xED:
					GCItemValueListRecv((PMSG_ITEM_LIST_INFO_RECV*)lpMsg);
					break;
				case 0xEF:
					GCEventTimeListRecv((PMSG_EVENT_TIME_LIST_RECV*)lpMsg);
					break;
			}
			break;
	}
}

void GCWindowNameRecv(PMSG_WINDOW_NAME_RECV* lpMsg) // OK
{
	strcpy_s(WindowName,lpMsg->title);
}

void CCServerNameRecv(PMSG_SERVER_NAME_RECV* lpMsg) // OK
{
	memset(ServerName,0,sizeof(ServerName));

	for (int n = 0; n<lpMsg->count; n++)
	{
		PMSG_SERVER_NAME* lpInfo = (PMSG_SERVER_NAME*)(((BYTE*)lpMsg) + sizeof(PMSG_SERVER_NAME_RECV) + (sizeof(PMSG_SERVER_NAME)*n));

		memcpy(ServerName[lpInfo->index],lpInfo->Name,sizeof(ServerName[lpInfo->index]));
	}
}

void GCDamageRecv(PMSG_DAMAGE_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (ViewIndex == aIndex)
	{
		ViewCurHP = lpMsg->ViewCurHP;
	}

	ViewDamageHP = lpMsg->ViewDamageHP;
}

void GCCharacterCreationEnableRecv(PMSG_CHARACTER_CREATION_ENABLE_RECV* lpMsg) // OK
{
	SetDword(0x00522933+1,(lpMsg->result==3)?0:1001); // Magic Gladiator Create Enable

	SetExperienceTable(lpMsg->CharacterMaxLevel,lpMsg->ExperienceMultiplierConstA,lpMsg->ExperienceMultiplierConstB,lpMsg->CharacterDeleteMaxLevel);
}

void GCUserDieRecv(PMSG_USER_DIE_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (ViewIndex == aIndex)
	{
		ViewCurHP = 0;
	}
}

void GCLifeRecv(PMSG_LIFE_RECV* lpMsg) // OK
{
	if(lpMsg->type == 0xFE)
	{
		ViewMaxHP = lpMsg->ViewHP;
	}

	if(lpMsg->type == 0xFF)
	{
		ViewCurHP = ((ViewCurHP==0)?ViewCurHP:lpMsg->ViewHP);
	}
}

void GCManaRecv(PMSG_MANA_RECV* lpMsg) // OK
{
	if(lpMsg->type == 0xFE)
	{
		ViewMaxMP = lpMsg->ViewMP;
		ViewMaxBP = lpMsg->ViewBP;
	}

	if(lpMsg->type == 0xFF)
	{
		ViewCurMP = lpMsg->ViewMP;
		ViewCurBP = lpMsg->ViewBP;
	}
}

void GCRewardExperienceRecv(PMSG_REWARD_EXPERIENCE_RECV* lpMsg)
{
	ViewDamageHP = lpMsg->ViewDamageHP;
	ViewExperience = lpMsg->ViewExperience;
	ViewNextExperience = lpMsg->ViewNextExperience;
}

void GCQuestRewardRecv(PMSG_QUEST_REWARD_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]) & 0x7FFF;

	if(ViewIndex == aIndex)
	{
		ViewPoint = lpMsg->ViewPoint;
	}
}

void GCConnectClientRecv(PMSG_CONNECT_CLIENT_RECV* lpMsg)
{
	ServerCode = lpMsg->ServerCode;

	ViewIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	PMSG_HARDWAREID_INFO_SEND pMsg;

	pMsg.header.set(0xF3,0x09,sizeof(pMsg));

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 98 (2.5.7) - Encrypt HardwareId before sending
	char* pHwid = GetHardwareId();

	if(pHwid != 0)
	{
		PacketArgumentEncrypt((BYTE*)pMsg.HardwareId,(BYTE*)pHwid,sizeof(pMsg.HardwareId));
	}
	else
	{
		memset(pMsg.HardwareId,0,sizeof(pMsg.HardwareId));
	}

	DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GCConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV* lpMsg)
{
	ReconnectOnConnectAccount(lpMsg->result);
}

void GCCloseClientRecv(PMSG_CLOSE_CLIENT_RECV* lpMsg)
{
	ReconnectOnCloseClient(lpMsg->result);
}

void GCCharacterListRecv(PMSG_CHARACTER_LIST_RECV* lpMsg)
{
	ReconnectOnCharacterList();
}

void GCCharacterInfoRecv(PMSG_CHARACTER_INFO_RECV* lpMsg)
{
	ReconnectOnCharacterInfo();

	ViewReset = lpMsg->ViewReset;
	ViewPoint = lpMsg->ViewPoint;
	ViewCurHP = lpMsg->ViewCurHP;
	ViewMaxHP = lpMsg->ViewMaxHP;
	ViewCurMP = lpMsg->ViewCurMP;
	ViewMaxMP = lpMsg->ViewMaxMP;
	ViewCurBP = lpMsg->ViewCurBP;
	ViewMaxBP = lpMsg->ViewMaxBP;
	ViewStrength = lpMsg->ViewStrength;
	ViewDexterity = lpMsg->ViewDexterity;
	ViewVitality = lpMsg->ViewVitality;
	ViewEnergy = lpMsg->ViewEnergy;
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Set AccountLevel in Character Info
	ViewAccountLevel = lpMsg->ViewAccountLevel;
	ViewFruitPoint = lpMsg->FruitPoint;
	ViewMaxFruitPoint = lpMsg->MaxFruitPoint;

	STRUCT_DECRYPT;

	if(((*(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0B)) & 7) == 3)
	{
		MemoryCpy(0x00559F60,gProtect.m_MainInfo.m_DevilSquareEntryLevelSpecial,sizeof(gProtect.m_MainInfo.m_DevilSquareEntryLevelSpecial));
	}
	else
	{
		MemoryCpy(0x00559F60, gProtect.m_MainInfo.m_DevilSquareEntryLevelCommon, sizeof(gProtect.m_MainInfo.m_DevilSquareEntryLevelCommon));
	}

	STRUCT_ENCRYPT;
}

void GCCharacterRegenRecv(PMSG_CHARACTER_REGEN_RECV* lpMsg)
{
	ViewCurHP = lpMsg->ViewCurHP;
	ViewCurMP = lpMsg->ViewCurMP;
	ViewCurBP = lpMsg->ViewCurBP;
}

void GCLevelUpRecv(PMSG_LEVEL_UP_RECV* lpMsg)
{
	ViewLevel = lpMsg->Level;
	ViewPoint = lpMsg->ViewPoint;
	ViewMaxHP = lpMsg->ViewMaxHP;
	ViewMaxMP = lpMsg->ViewMaxMP;
	ViewMaxBP = lpMsg->ViewMaxBP;
	ViewExperience = lpMsg->ViewExperience;
	ViewNextExperience = lpMsg->ViewNextExperience;
	ViewFruitPoint = lpMsg->FruitPoint;
	ViewMaxFruitPoint = lpMsg->MaxFruitPoint;
}

void GCLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg)
{
	if(lpMsg->result >= 16 && lpMsg->result <= 20)
	{
		ViewPoint = lpMsg->ViewPoint;
		ViewMaxHP = lpMsg->ViewMaxHP;
		ViewMaxMP = lpMsg->ViewMaxMP;
		ViewMaxBP = lpMsg->ViewMaxBP;
		ViewStrength = lpMsg->ViewStrength;
		ViewDexterity = lpMsg->ViewDexterity;
		ViewVitality = lpMsg->ViewVitality;
		ViewEnergy = lpMsg->ViewEnergy;
	}
}

void GCMonsterDamageRecv(PMSG_MONSTER_DAMAGE_RECV* lpMsg)
{
	ViewCurHP = lpMsg->ViewCurHP;
	ViewDamageHP = lpMsg->ViewDamageHP;
}

void GCNewCharacterInfoRecv(PMSG_NEW_CHARACTER_INFO_RECV* lpMsg)
{
	ViewLevel = lpMsg->Level;
	ViewReset = lpMsg->ViewReset;
	ViewPoint = lpMsg->ViewPoint;
	ViewCurHP = lpMsg->ViewCurHP;
	ViewMaxHP = lpMsg->ViewMaxHP;
	ViewCurMP = lpMsg->ViewCurMP;
	ViewMaxMP = lpMsg->ViewMaxMP;
	ViewCurBP = lpMsg->ViewCurBP;
	ViewMaxBP = lpMsg->ViewMaxBP;
	ViewStrength = lpMsg->ViewStrength;
	ViewDexterity = lpMsg->ViewDexterity;
	ViewVitality = lpMsg->ViewVitality;
	ViewEnergy = lpMsg->ViewEnergy;
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Set AccountLevel in New Character Info
	ViewAccountLevel = lpMsg->ViewAccountLevel;
	ViewExperience = lpMsg->Experience;
	ViewNextExperience = lpMsg->NextExperience;
}

void GCNewCharacterCalcRecv(PMSG_NEW_CHARACTER_CALC_RECV* lpMsg)
{
	ViewPoint = lpMsg->ViewPoint;
	ViewCurHP = lpMsg->ViewCurHP;
	ViewMaxHP = lpMsg->ViewMaxHP;
	ViewCurMP = lpMsg->ViewCurMP;
	ViewMaxMP = lpMsg->ViewMaxMP;
	ViewCurBP = lpMsg->ViewCurBP;
	ViewMaxBP = lpMsg->ViewMaxBP;
	ViewStrength = lpMsg->ViewStrength;
	ViewDexterity = lpMsg->ViewDexterity;
	ViewVitality = lpMsg->ViewVitality;
	ViewEnergy = lpMsg->ViewEnergy;
	ViewAddStrength = lpMsg->ViewAddStrength;
	ViewAddDexterity = lpMsg->ViewAddDexterity;
	ViewAddVitality = lpMsg->ViewAddVitality;
	ViewAddEnergy = lpMsg->ViewAddEnergy;
	ViewPhysiSpeed = lpMsg->ViewPhysiSpeed;
	ViewMagicSpeed = lpMsg->ViewMagicSpeed;
	ViewFruitPoint = lpMsg->FruitPoint;
	ViewMaxFruitPoint = lpMsg->MaxFruitPoint;
}

void GCHealthBarRecv(PMSG_NEW_HEALTH_BAR_RECV* lpMsg)
{
	ClearNewHealthBar();

	for(int n=0;n < lpMsg->count;n++)
	{
		PMSG_NEW_HEALTH_RECV* lpInfo = (PMSG_NEW_HEALTH_RECV*)(((BYTE*)lpMsg)+sizeof(PMSG_NEW_HEALTH_BAR_RECV)+(sizeof(PMSG_NEW_HEALTH_RECV)*n));

		InsertNewHealthBar(lpInfo->index,lpInfo->type,lpInfo->rate);
	}
}

void GCNewMessageRecv(PMSG_NEW_MESSAGE_RECV* lpMsg) // OK
{
	if(lpMsg->type == 0)
	{
		((int(*)(char*))0x0051D6F0)(lpMsg->message);
	}
	else if(lpMsg->type == 1)
	{
		((void*(*)(char*,char*,int))0x00480620)("",lpMsg->message,lpMsg->color);
	}
	else if(lpMsg->type == 2)
	{
		NoticeAdd(lpMsg->color,lpMsg->message);
	}
}

void DataSend(BYTE* lpMsg, DWORD size)
{
	BYTE EncBuff[2048];

	if (gPacketManager.AddData(lpMsg,size) != 0 && gPacketManager.ExtractPacket(EncBuff) != 0)
	{
		BYTE send[2048];

		memcpy(send,EncBuff,size);

		if (EncBuff[0] == 0xC3 || EncBuff[0] == 0xC4)
		{
			if (EncBuff[0] == 0xC3)
			{
				BYTE save = EncBuff[1];

				PACKET_DECRYPT;

				EncBuff[1] = (*(BYTE*)(MAIN_PACKET_SERIAL))++;

				PACKET_ENCRYPT;

				size = gPacketManager.Encrypt(&send[2],&EncBuff[1],(size-1))+2;

				EncBuff[1] = save;

				send[0] = 0xC3;
				send[1] = LOBYTE(size);
			}
			else
			{
				BYTE save = EncBuff[2];

				EncBuff[2] = (*(BYTE*)(MAIN_PACKET_SERIAL))++;

				size = gPacketManager.Encrypt(&send[3],&EncBuff[2],(size-2))+3;

				EncBuff[2] = save;

				send[0] = 0xC4;
				send[1] = HIBYTE(size);
				send[2] = LOBYTE(size);
			}
		}

		MySend(pSocket,send,size,0);
	}
}