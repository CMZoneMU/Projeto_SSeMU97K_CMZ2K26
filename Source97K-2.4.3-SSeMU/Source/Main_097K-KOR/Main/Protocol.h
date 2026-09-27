#pragma once

#include "ProtocolDefines.h"

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_DAMAGE_RECV
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE2] // 0x15
	BYTE index[2];
	BYTE damage[2];
	// Extra
	DWORD ViewCurHP;
	DWORD ViewDamageHP;
};

struct PMSG_CHARACTER_CREATION_ENABLE_RECV
{
	PBMSG_HEAD header; // C1:DE
	BYTE flag;
	BYTE result;
	//EXTRA
	WORD ExperienceMultiplierConstA;
	WORD ExperienceMultiplierConstB;
	WORD CharacterMaxLevel;
	WORD CharacterDeleteMaxLevel;
};

struct PMSG_USER_DIE_RECV
{
	PBMSG_HEAD header; // C1:17
	BYTE index[2];
	BYTE skill;
	BYTE killer[2];
};

struct PMSG_LIFE_RECV
{
	PBMSG_HEAD header; // C1:26
	BYTE type;
	BYTE life[2];
	BYTE flag;
	// Extra
	DWORD ViewHP;
};

struct PMSG_MANA_RECV
{
	PBMSG_HEAD header; // C1:27
	BYTE type;
	BYTE mana[2];
	BYTE bp[2];
	// Extra
	DWORD ViewMP;
	DWORD ViewBP;
};

struct PMSG_REWARD_EXPERIENCE_RECV
{
	PBMSG_HEAD header; // C1:9C
	BYTE index[2];
	WORD experience[2];
	BYTE damage[2];
	// Extra
	DWORD ViewDamageHP;
	DWORD ViewExperience;
	DWORD ViewNextExperience;
};

struct PMSG_QUEST_REWARD_RECV
{
	PBMSG_HEAD header; // C1:A3
	BYTE index[2];
	BYTE QuestReward;
	BYTE QuestAmount;
	// Extra
	DWORD ViewPoint;
};

struct PMSG_CONNECT_CLIENT_RECV
{
	PSBMSG_HEAD header; // C1:F1:00
	BYTE result;
	BYTE index[2];
	BYTE ClientVersion[5];
	//EXTRA
	WORD ServerCode;
};

struct PMSG_CONNECT_ACCOUNT_RECV
{
	PSBMSG_HEAD header; // C1:F1:01
	BYTE result;
};

struct PMSG_CLOSE_CLIENT_RECV
{
	PSBMSG_HEAD header; // C1:F1:02
	BYTE result;
};

struct PMSG_CHARACTER_LIST_RECV
{
	PSBMSG_HEAD header; // C1:F3:00
	BYTE count;
};

struct PMSG_CHARACTER_LIST
{
	BYTE slot;
	char Name[10];
	WORD Level;
	BYTE CtlCode;
	BYTE CharSet[11];
};

struct PMSG_CHARACTER_INFO_RECV
{
	PSBMSG_HEAD header; // C3:F3:03
	BYTE X;
	BYTE Y;
	BYTE Map;
	BYTE Dir;
	DWORD Experience;
	DWORD NextExperience;
	WORD LevelUpPoint;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	WORD Life;
	WORD MaxLife;
	WORD Mana;
	WORD MaxMana;
	WORD BP;
	WORD MaxBP;
	DWORD Money;
	BYTE PKLevel;
	BYTE CtlCode;
	WORD FruitPoint;
	WORD MaxFruitPoint;
	// Extra
	DWORD ViewReset;
	DWORD ViewPoint;
	DWORD ViewCurHP;
	DWORD ViewMaxHP;
	DWORD ViewCurMP;
	DWORD ViewMaxMP;
	DWORD ViewCurBP;
	DWORD ViewMaxBP;
	DWORD ViewStrength;
	DWORD ViewDexterity;
	DWORD ViewVitality;
	DWORD ViewEnergy;
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Add AccountLevel to Character Info
	DWORD ViewAccountLevel;
};

struct PMSG_CHARACTER_REGEN_RECV
{
	PSBMSG_HEAD header; // C3:F3:04
	BYTE X;
	BYTE Y;
	BYTE Map;
	BYTE Dir;
	WORD Life;
	WORD Mana;
	WORD BP;
	DWORD Experience;
	DWORD Money;
	// Extra
	DWORD ViewCurHP;
	DWORD ViewCurMP;
	DWORD ViewCurBP;
};

struct PMSG_LEVEL_UP_RECV
{
	PSBMSG_HEAD header; // C1:F3:05
	WORD Level;
	WORD LevelUpPoint;
	WORD MaxLife;
	WORD MaxMana;
	WORD MaxBP;
	WORD FruitPoint;
	WORD MaxFruitPoint;
	// Extra
	DWORD ViewPoint;
	DWORD ViewMaxHP;
	DWORD ViewMaxMP;
	DWORD ViewMaxBP;
	DWORD ViewExperience;
	DWORD ViewNextExperience;
};

struct PMSG_LEVEL_UP_POINT_RECV
{
	PSBMSG_HEAD header; // C1:F3:06
	BYTE result;
	WORD MaxLifeAndMana;
	WORD MaxBP;
	// Extra
	DWORD ViewPoint;
	DWORD ViewMaxHP;
	DWORD ViewMaxMP;
	DWORD ViewMaxBP;
	DWORD ViewStrength;
	DWORD ViewDexterity;
	DWORD ViewVitality;
	DWORD ViewEnergy;
};

struct PMSG_MONSTER_DAMAGE_RECV
{
	PSBMSG_HEAD header; // C1:F3:07
	BYTE damage[2];
	// Extra
	DWORD ViewCurHP;
	DWORD ViewDamageHP;
};

struct PMSG_NEW_CHARACTER_INFO_RECV
{
	PSBMSG_HEAD header; // C1:F3:E0
	WORD Level;
	WORD LevelUpPoint;
	DWORD Experience;
	DWORD NextExperience;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	WORD Life;
	WORD MaxLife;
	WORD Mana;
	WORD MaxMana;
	WORD BP;
	WORD MaxBP;
	// Extra
	DWORD ViewReset;
	DWORD ViewPoint;
	DWORD ViewCurHP;
	DWORD ViewMaxHP;
	DWORD ViewCurMP;
	DWORD ViewMaxMP;
	DWORD ViewCurBP;
	DWORD ViewMaxBP;
	DWORD ViewStrength;
	DWORD ViewDexterity;
	DWORD ViewVitality;
	DWORD ViewEnergy;
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Add AccountLevel to New Character Info
	DWORD ViewAccountLevel;
};

struct PMSG_NEW_CHARACTER_CALC_RECV
{
	PSBMSG_HEAD header; // C1:F3:E1
	DWORD ViewPoint;
	DWORD ViewCurHP;
	DWORD ViewMaxHP;
	DWORD ViewCurMP;
	DWORD ViewMaxMP;
	DWORD ViewCurBP;
	DWORD ViewMaxBP;
	DWORD ViewStrength;
	DWORD ViewDexterity;
	DWORD ViewVitality;
	DWORD ViewEnergy;
	DWORD ViewAddStrength;
	DWORD ViewAddDexterity;
	DWORD ViewAddVitality;
	DWORD ViewAddEnergy;
	DWORD ViewPhysiSpeed;
	DWORD ViewMagicSpeed;
	DWORD FruitPoint;
	DWORD MaxFruitPoint;
};

struct PMSG_NEW_HEALTH_BAR_RECV
{
	PSWMSG_HEAD header; // C2:F3:E2
	BYTE count;
};

struct PMSG_NEW_HEALTH_RECV
{
	WORD index;
	BYTE type;
	BYTE rate;
};

struct PMSG_NEW_MESSAGE_RECV
{
	PSBMSG_HEAD header; // C1:F3:[E4:E5]
	BYTE type;
	BYTE color;
	char message[128];
};

struct PMSG_WINDOW_NAME_RECV
{
	PSBMSG_HEAD header; // C1:F3:E8
	char title[128];
};

struct PMSG_SERVER_NAME_RECV
{
	PSWMSG_HEAD header; // C1:F3:EA
	BYTE count;
};

struct PMSG_SERVER_NAME
{
	WORD index;
	char Name[32];
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_CONNECT_ACCOUNT_SEND
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C3:F1:01
	char account[10];
	char password[10];
	DWORD TickCount;
	BYTE ClientVersion[5];
	BYTE ClientSerial[16];
	#pragma pack()
};

struct PMSG_CHARACTER_LIST_SEND
{
	PSBMSG_HEAD header; // C1:F3:00
};

struct PMSG_CHARACTER_INFO_SEND
{
	PSBMSG_HEAD header; // C1:F3:03
	char name[10];
};

struct PMSG_HARDWAREID_INFO_SEND
{
	PSBMSG_HEAD header; // C1:F3:09
	char HardwareId[45];
};

//**********************************************//
//**********************************************//
//**********************************************//

void InitProtocol();

static void HookProtocol();

void __stdcall ProtocolCoreEx(BYTE head, BYTE* lpMsg);

void GCWindowNameRecv(PMSG_WINDOW_NAME_RECV* lpMsg);

void CCServerNameRecv(PMSG_SERVER_NAME_RECV* lpMsg);

void GCDamageRecv(PMSG_DAMAGE_RECV* lpMsg);

void GCCharacterCreationEnableRecv(PMSG_CHARACTER_CREATION_ENABLE_RECV* lpMsg);

void GCUserDieRecv(PMSG_USER_DIE_RECV* lpMsg);

void GCLifeRecv(PMSG_LIFE_RECV* lpMsg);

void GCManaRecv(PMSG_MANA_RECV* lpMsg);

void GCRewardExperienceRecv(PMSG_REWARD_EXPERIENCE_RECV* lpMsg);

void GCQuestRewardRecv(PMSG_QUEST_REWARD_RECV* lpMsg);

void GCConnectClientRecv(PMSG_CONNECT_CLIENT_RECV* lpMsg);

void GCConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV* lpMsg);

void GCCloseClientRecv(PMSG_CLOSE_CLIENT_RECV* lpMsg);

void GCCharacterListRecv(PMSG_CHARACTER_LIST_RECV* lpMsg);

void GCCharacterInfoRecv(PMSG_CHARACTER_INFO_RECV* lpMsg);

void GCCharacterRegenRecv(PMSG_CHARACTER_REGEN_RECV* lpMsg);

void GCLevelUpRecv(PMSG_LEVEL_UP_RECV* lpMsg);

void GCLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg);

void GCMonsterDamageRecv(PMSG_MONSTER_DAMAGE_RECV* lpMsg);

void GCNewCharacterInfoRecv(PMSG_NEW_CHARACTER_INFO_RECV* lpMsg);

void GCNewCharacterCalcRecv(PMSG_NEW_CHARACTER_CALC_RECV* lpMsg);

void GCHealthBarRecv(PMSG_NEW_HEALTH_BAR_RECV* lpMsg);

void GCNewMessageRecv(PMSG_NEW_MESSAGE_RECV* lpMsg);

void DataSend(BYTE* lpMsg, DWORD size);