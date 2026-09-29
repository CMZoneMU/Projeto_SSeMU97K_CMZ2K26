#pragma once

#include "User.h"

#define PROTOCOL_NAME "Pre Season 0.97x KOR"
#define PROTOCOL_CODE1 0x10
#define PROTOCOL_CODE2 0x15
#define PROTOCOL_CODE3 0x11
#define PROTOCOL_CODE4 0x1D

#define SET_NUMBERHB(x) ((BYTE)((DWORD)(x)>>(DWORD)8))
#define SET_NUMBERLB(x) ((BYTE)((DWORD)(x)&0xFF))
#define SET_NUMBERHW(x) ((WORD)((DWORD)(x)>>(DWORD)16))
#define SET_NUMBERLW(x) ((WORD)((DWORD)(x)&0xFFFF))
#define SET_NUMBERHDW(x) ((DWORD)((QWORD)(x)>>(QWORD)32))
#define SET_NUMBERLDW(x) ((DWORD)((QWORD)(x)&0xFFFFFFFF))

#define MAKE_NUMBERW(x,y) ((WORD)(((BYTE)((y)&0xFF))|((BYTE)((x)&0xFF)<<8)))
#define MAKE_NUMBERDW(x,y) ((DWORD)(((WORD)((y)&0xFFFF))|((WORD)((x)&0xFFFF)<<16)))
#define MAKE_NUMBERQW(x,y) ((QWORD)(((DWORD)((y)&0xFFFFFFFF))|((DWORD)((x)&0xFFFFFFFF)<<32)))

#define MAKE_PACKET(x) (BYTE*)&x,x.header.size

//**********************************************//
//************ Packet Base *********************//
//**********************************************//

struct PBMSG_HEAD
{
	void set(BYTE head,BYTE size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->head = head;
	}

	void setE(BYTE head,BYTE size) // OK
	{
		this->type = 0xC3;
		this->size = size;
		this->head = head;
	}

	BYTE type;
	BYTE size;
	BYTE head;
};

struct PSBMSG_HEAD
{
	void set(BYTE head,BYTE subh,BYTE size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	void setE(BYTE head,BYTE subh,BYTE size) // OK
	{
		this->type = 0xC3;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	BYTE type;
	BYTE size;
	BYTE head;
	BYTE subh;
};

struct PWMSG_HEAD
{
	void set(BYTE head,WORD size) // OK
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
	}

	void setE(BYTE head,WORD size) // OK
	{
		this->type = 0xC4;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
	}

	BYTE type;
	BYTE size[2];
	BYTE head;
};

struct PSWMSG_HEAD
{
	void set(BYTE head,BYTE subh,WORD size) // OK
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
		this->subh = subh;
	}

	void setE(BYTE head,BYTE subh,WORD size) // OK
	{
		this->type = 0xC4;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
		this->subh = subh;
	}

	BYTE type;
	BYTE size[2];
	BYTE head;
	BYTE subh;
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_CHAT_RECV
{
	PBMSG_HEAD header; // C1:00
	char name[10];
	char message[60];
};

struct PMSG_CHAT_WHISPER_RECV
{
	PBMSG_HEAD header; // C1:02
	char name[10];
	char message[60];
};

struct PMSG_MAIN_CHECK_RECV
{
	PBMSG_HEAD header; // C1:03
	DWORD key;
};

struct PMSG_LIVE_CLIENT_RECV
{
	PBMSG_HEAD header; // C1:0E
	DWORD TickCount;
	WORD PhysiSpeed;
	WORD MagicSpeed;
};

struct PMSG_POSITION_RECV
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE3]
	BYTE x;
	BYTE y;
};

struct PMSG_ACTION_RECV
{
	PBMSG_HEAD header; // C1:18
	BYTE dir;
	BYTE action;
	BYTE index[2];
};

struct PMSG_EVENT_REMAIN_TIME_RECV
{
	PBMSG_HEAD header; // C1:91
	BYTE EventType;
	BYTE ItemLevel;
};

struct PMSG_PET_ITEM_COMMAND_RECV
{
	PBMSG_HEAD header; // C1:A7
	BYTE type;
	BYTE command;
	BYTE index[2];
};

struct PMSG_PET_ITEM_INFO_RECV
{
	PBMSG_HEAD header; // C1:A9
	BYTE type;
	BYTE flag;
	BYTE slot;
};

struct PMSG_FRIEND_MESSAGE_RECV
{
	PWMSG_HEAD header; // C1:C5
	DWORD guid;
	char name[10];
	char subject[60];
	BYTE dir;
	BYTE action;
	WORD size;
	char text[1000];
};

struct PMSG_MOVE_RECV
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE1]
	BYTE x;
	BYTE y;
	BYTE path[8];
};

struct PMSG_CONNECT_ACCOUNT_RECV
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

struct PMSG_CLOSE_CLIENT_RECV
{
	PSBMSG_HEAD header; // C1:F1:02
	BYTE type;
};

struct PMSG_CHARACTER_CREATE_RECV
{
	PSBMSG_HEAD header; // C1:F3:01
	char name[10];
	BYTE Class;
};

struct PMSG_CHARACTER_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:F3:02
	char name[10];
	char PersonalCode[10];
};

struct PMSG_CHARACTER_INFO_RECV
{
	PSBMSG_HEAD header; // C1:F3:03
	char name[10];
};

struct PMSG_LEVEL_UP_POINT_RECV
{
	PSBMSG_HEAD header; // C1:F3:06
	BYTE type;
};

struct PMSG_OPTION_DATA_RECV
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C1:F3:30
	BYTE SkillKey[10];
	BYTE GameOption;
	BYTE QKey;
	BYTE WKey;
	BYTE EKey;
	BYTE ChatWindow;
	#pragma pack()
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_CHAT_SEND
{
	PBMSG_HEAD header; // C1:00
	char name[10];
	char message[60];
};

struct PMSG_CHAT_TARGET_SEND
{
	PBMSG_HEAD header; // C1:01
	BYTE index[2];
	char message[60];
};

struct PMSG_CHAT_WHISPER_SEND
{
	PBMSG_HEAD header; // C1:02
	char name[10];
	char message[60];
};

struct PMSG_MAIN_CHECK_SEND
{
	PBMSG_HEAD header; // C1:03
	WORD key;
};

struct PMSG_EVENT_STATE_SEND
{
	PBMSG_HEAD header; // C1:0B
	BYTE state;
	BYTE event;
};

struct PMSG_SERVER_MSG_SEND
{
	PBMSG_HEAD header; // C1:0C
	BYTE MsgNumber;
};

struct PMSG_WEATHER_SEND
{
	PBMSG_HEAD header; // C1:0F
	BYTE weather;
};

struct PMSG_DAMAGE_SEND
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE2]
	BYTE index[2];
	BYTE damage[2];
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewCurHP;
	DWORD ViewDamageHP;
	#endif
};

struct PMSG_POSITION_SEND
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE3]
	BYTE index[2];
	BYTE x;
	BYTE y;
};

struct PMSG_USER_DIE_SEND
{
	PBMSG_HEAD header; // C1:17
	BYTE index[2];
	BYTE skill;
	BYTE killer[2];
};

struct PMSG_ACTION_SEND
{
	PBMSG_HEAD header; // C1:18
	BYTE index[2];
	BYTE dir;
	BYTE action;
	BYTE target[2];
};

struct PMSG_LIFE_SEND
{
	PBMSG_HEAD header; // C1:26
	BYTE type;
	BYTE life[2];
	BYTE flag;
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewHP;
	#endif
};

struct PMSG_MANA_SEND
{
	PBMSG_HEAD header; // C1:27
	BYTE type;
	BYTE mana[2];
	BYTE bp[2];
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewMP;
	DWORD ViewBP;
	#endif
};

struct PMSG_ITEM_SPECIAL_TIME_SEND
{
	PBMSG_HEAD header; // C3:29
	BYTE number;
	WORD time;
};

struct PMSG_MAP_ATTR_SEND
{
	PBMSG_HEAD header; // C1:46
	BYTE type;
	BYTE attr;
	BYTE flag;
	BYTE count;
};

struct PMSG_MAP_ATTR
{
	BYTE x;
	BYTE y;
	BYTE tx;
	BYTE ty;
};

struct PMSG_PARTY_ITEM_INFO_SEND
{
	PBMSG_HEAD header; // C1:47
	BYTE index[2];
	WORD ItemInfo;
	BYTE level;
};

struct PMSG_EFFECT_INFO_SEND
{
	PBMSG_HEAD header; // C1:48
	BYTE index[2];
	BYTE effect;
};

struct PMSG_MONSTER_SKILL_SEND
{
	PBMSG_HEAD header; // C1:69
	BYTE skill;
	WORD index;
	WORD target;
};

struct PMSG_EVENT_REMAIN_TIME_SEND
{
	PBMSG_HEAD header; // C1:91
	BYTE EventType;
	BYTE RemainTimeH;
	BYTE EnteredUser;
	BYTE RemainTimeL;
};

struct PMSG_TIME_COUNT_SEND
{
	PBMSG_HEAD header; // C1:92
	BYTE type;
};

struct PMSG_REWARD_EXPERIENCE_SEND
{
	PBMSG_HEAD header; // C1:9C
	BYTE index[2];
	WORD experience[2];
	BYTE damage[2];
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewDamageHP;
	DWORD ViewExperience;
	DWORD ViewNextExperience;
	#endif
};

struct PMSG_PET_ITEM_INFO_SEND
{
	PBMSG_HEAD header; // C1:A9
	BYTE type;
	BYTE flag;
	BYTE slot;
	BYTE level;
	UINT experience;
};

struct PMSG_KILL_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:B8:01
	BYTE count;
};

struct PMSG_MOVE_SEND
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE1]
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE dir;
};

struct PMSG_ELEMENTAL_DAMAGE_SEND
{
	PBMSG_HEAD header; // C1:D8
	BYTE index[2];
	BYTE attribute;
	DWORD damage;
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewCurHP;
	#endif
};

struct PMSG_CHARACTER_CREATION_ENABLE_SEND
{
	PBMSG_HEAD header; // C1:DE
	BYTE flag;
	BYTE result;
	#if(GAMESERVER_EXTRA==1)
	WORD ExperienceMultiplierConstA;
	WORD ExperienceMultiplierConstB;
	WORD CharacterMaxLevel;
	WORD CharacterDeleteMaxLevel;
	#endif
};

struct PMSG_LIFE_UPDATE_SEND
{
	PSBMSG_HEAD header; // C1:EC:10
	BYTE index[2];
	BYTE MaxHP[4];
	BYTE CurHP[4];
};

struct PMSG_CHARACTER_ATTACK_SPEED_SEND
{
	PSBMSG_HEAD header; // C1:EC:30
	DWORD PhysiSpeed;
	DWORD MagicSpeed;
};

struct PMSG_ENTER_EVENT_MAP_ERROR_SEND
{
	PSBMSG_HEAD header; // C1:EF:09
	DWORD result;
};

struct PMSG_CONNECT_CLIENT_SEND
{
	PSBMSG_HEAD header; // C1:F1:00
	BYTE result;
	BYTE index[2];
	BYTE ClientVersion[5];
	#if(GAMESERVER_EXTRA==1)
	WORD ServerCode;
	#endif
};

struct PMSG_CONNECT_ACCOUNT_SEND
{
	PSBMSG_HEAD header; // C1:F1:01
	BYTE result;
};

struct PMSG_CLOSE_CLIENT_SEND
{
	PSBMSG_HEAD header; // C1:F1:02
	BYTE result;
};

struct PMSG_CHARACTER_LIST_SEND
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

struct PMSG_CHARACTER_CREATE_SEND
{
	PSBMSG_HEAD header; // C1:F3:01
	BYTE result;
	char name[10];
	BYTE slot;
	BYTE equipment[24];
};

struct PMSG_CHARACTER_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:F3:02
	BYTE result;
};

struct PMSG_CHARACTER_INFO_SEND
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
	#if(GAMESERVER_EXTRA==1)
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
	// Update CMZ 15 (3.1.5) 26-09-26 - Add AccountLevel to Character Info
	DWORD ViewAccountLevel;
	#endif
};

struct PMSG_CHARACTER_REGEN_SEND
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
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewCurHP;
	DWORD ViewCurMP;
	DWORD ViewCurBP;
	#endif
};

struct PMSG_LEVEL_UP_SEND
{
	PSBMSG_HEAD header; // C1:F3:05
	WORD Level;
	WORD LevelUpPoint;
	WORD MaxLife;
	WORD MaxMana;
	WORD MaxBP;
	WORD FruitPoint;
	WORD MaxFruitPoint;
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewPoint;
	DWORD ViewMaxHP;
	DWORD ViewMaxMP;
	DWORD ViewMaxBP;
	DWORD ViewExperience;
	DWORD ViewNextExperience;
	#endif
};

struct PMSG_LEVEL_UP_POINT_SEND
{
	PSBMSG_HEAD header; // C1:F3:06
	BYTE result;
	WORD MaxLifeAndMana;
	WORD MaxBP;
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewPoint;
	DWORD ViewMaxHP;
	DWORD ViewMaxMP;
	DWORD ViewMaxBP;
	DWORD ViewStrength;
	DWORD ViewDexterity;
	DWORD ViewVitality;
	DWORD ViewEnergy;
	#endif
};

struct PMSG_MONSTER_DAMAGE_SEND
{
	PSBMSG_HEAD header; // C1:F3:07
	BYTE damage[2];
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewCurHP;
	DWORD ViewDamageHP;
	#endif
};

struct PMSG_PK_LEVEL_SEND
{
	PSBMSG_HEAD header; // C1:F3:08
	BYTE index[2];
	BYTE PKLevel;
};

struct PMSG_SUMMON_LIFE_SEND
{
	PSBMSG_HEAD header; // C1:F3:20
	BYTE life;
};

struct PMSG_TIME_VIEW_SEND
{
	PSBMSG_HEAD header; // C1:F3:22
	WORD time;
};

struct PMSG_OPTION_DATA_SEND
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C1:F3:30
	BYTE SkillKey[10];
	BYTE GameOption;
	BYTE QKey;
	BYTE WKey;
	BYTE EKey;
	BYTE ChatWindow;
	#pragma pack()
};

struct PMSG_SERVER_COMMAND_SEND
{
	PSBMSG_HEAD header; // C1:F3:40
	BYTE type;
	BYTE x;
	BYTE y;
};

struct PMSG_NEW_CHARACTER_INFO_SEND
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
	//EXTRA
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
	// Update CMZ 15 (3.1.5) 26-09-26 - Add AccountLevel to New Character Info
	DWORD ViewAccountLevel;
};

struct PMSG_NEW_CHARACTER_CALC_SEND
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

struct PMSG_NEW_HEALTH_BAR_SEND
{
	PSWMSG_HEAD header; // C2:F3:E2
	BYTE count;
};

struct PMSG_NEW_HEALTH_BAR
{
	WORD index;
	BYTE type;
	BYTE hprate;
};

struct PMSG_NEW_MESSAGE_SEND
{
	PSBMSG_HEAD header; // C1:F3:[E4:E5]
	BYTE type;
	BYTE color;
	char message[128];
};

struct PMSG_WINDOW_NAME_SEND
{
	PSBMSG_HEAD header; // C1:F3:E8
	char title[128];
};

//**********************************************//
//**********************************************//
//**********************************************//

void ProtocolCore(BYTE head,BYTE* lpMsg,int size,int aIndex,int encrypt,int serial);
void CGChatRecv(PMSG_CHAT_RECV* lpMsg,int aIndex);
void CGChatWhisperRecv(PMSG_CHAT_WHISPER_RECV* lpMsg,int aIndex);
void CGMainCheckRecv(PMSG_MAIN_CHECK_RECV* lpMsg,int aIndex);
void CGWindowsEventOpen(int aIndex);
void CGLiveClientRecv(PMSG_LIVE_CLIENT_RECV* lpMsg,int aIndex);
void CGPositionRecv(PMSG_POSITION_RECV* lpMsg,int aIndex);
void CGActionRecv(PMSG_ACTION_RECV* lpMsg,int aIndex);
void CGEventRemainTimeRecv(PMSG_EVENT_REMAIN_TIME_RECV* lpMsg,int aIndex);
void CGPetItemCommandRecv(PMSG_PET_ITEM_COMMAND_RECV* lpMsg,int aIndex);
void CGPetItemInfoRecv(PMSG_PET_ITEM_INFO_RECV* lpMsg,int aIndex);
void CGMoveRecv(PMSG_MOVE_RECV* lpMsg,int aIndex);
void CGConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV* lpMsg,int aIndex);
void CGCloseClientRecv(PMSG_CLOSE_CLIENT_RECV* lpMsg,int aIndex);
void CGCharacterListRecv(int aIndex);
void CGCharacterCreateRecv(PMSG_CHARACTER_CREATE_RECV* lpMsg,int aIndex);
void CGCharacterDeleteRecv(PMSG_CHARACTER_DELETE_RECV* lpMsg,int aIndex);
void CGCharacterInfoRecv(PMSG_CHARACTER_INFO_RECV* lpMsg,int aIndex);
void CGLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg,int aIndex);
void CGCharacterMoveViewportEnableRecv(int aIndex);
void CGOptionDataRecv(PMSG_OPTION_DATA_RECV* lpMsg,int aIndex);
void GCChatTargetSend(LPOBJ lpObj,int aIndex,char* message);
void GCChatWhisperSend(int aIndex,char* name,char* message);
void GCMainCheckSend(int aIndex);
void GCEventStateSend(int aIndex,BYTE state,BYTE event);
void GCEventStateSendToAll(int Map,BYTE state,BYTE event);
void GCServerMsgSend(int aIndex,BYTE msg);
void GCWeatherSend(int aIndex,BYTE weather);
void GCDamageSend(int aIndex,int bIndex,BYTE flag,int damage,int type);
void GCUserDieSend(LPOBJ lpObj,int aIndex,int skill,int bIndex);
void GCActionSend(LPOBJ lpObj,BYTE action,int aIndex,int bIndex);
void GCMoneySend(int aIndex,DWORD money);
void GCLifeSend(int aIndex,BYTE type,int life);
void GCManaSend(int aIndex,BYTE type,int mana,int bp);
void GCItemUseSpecialTimeSend(int aIndex,BYTE number,int time);
void GCMapAttrSend(int aIndex,BYTE type,BYTE attr,BYTE flag,BYTE count,PMSG_MAP_ATTR* lpInfo);
void GCPartyItemInfoSend(int aIndex,CItem* lpItem);
void GCEffectInfoSend(int aIndex,BYTE effect);
void GCGuildMasterQuestionSend(int aIndex);
void GCMonsterSkillSend(LPOBJ lpObj,LPOBJ lpTarget,int skill);
void GCMonsterDieSend(int aIndex,int bIndex,int experience,int damage,BYTE flag);
void GCRewardExperienceSend(int aIndex,int experience);
void GCPetItemInfoSend(int aIndex,BYTE type,BYTE flag,BYTE slot,BYTE level,DWORD experience,BYTE durability);
void GCCharacterCreationEnableSend(int aIndex,BYTE flag,BYTE result);
void GCConnectClientSend(int aIndex,BYTE result);
void GCConnectAccountSend(int aIndex,BYTE result);
void GCConnectAccountSend(int aIndex,BYTE result,SOCKET socket);
void GCCloseClientSend(int aIndex,BYTE result);
void GCCharacterRegenSend(LPOBJ lpObj);
void GCLevelUpSend(LPOBJ lpObj);
void GCMonsterDamageSend(int aIndex,int damage);
void GCPKLevelSend(int aIndex,int PKLevel);
void GCSummonLifeSend(int aIndex,int life,int MaxLife);
void GCTimeViewSend(int aIndex,int time);
void GCServerCommandSend(int aIndex,int type,int x,int y,int Viewport);
void GCNewCharacterInfoSend(LPOBJ lpObj);
void GCNewCharacterCalcSend(LPOBJ lpObj);
void GCNewHealthBarSend(LPOBJ lpObj);
void GCNewMessageSend(int aIndex,int type,int color,char* message,...);
void GCNewMessageSendToAll(int type,int color,char* message,...);
void GCWindowsNameSend(int aIndex,char* message,...);