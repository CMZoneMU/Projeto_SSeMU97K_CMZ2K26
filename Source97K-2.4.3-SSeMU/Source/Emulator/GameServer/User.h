#pragma once

#include "Effect.h"
#include "Item.h"
#include "Skill.h"

#define MAX_OBJECT 10000
#define MAX_OBJECT_MONSTER 8000
#define MAX_OBJECT_USER 1000

#define OBJECT_START_MONSTER 0
#define OBJECT_START_USER (MAX_OBJECT-MAX_OBJECT_USER)

#define MAX_SELF_DEFENSE 5
#define MAX_HIT_DAMAGE 40
#define MAX_VIEWPORT 75
#define MAX_MONEY 2000000000
#define MAX_CHARACTER_LEVEL 1000
#define MAX_ACCOUNT_LEVEL 4
#define MAX_DAMAGE_REDUCTION 6
#define MAX_MESSAGE_SIZE 255
#define MAX_CHAT_MESSAGE_SIZE 59
#define MAX_ROAD_PATH_TABLE 16
#define MAX_MONSTER_SEND_MSG 20
#define MAX_MONSTER_SEND_ATTACK_MSG 30

#define OBJECT_RANGE(x) (((x)<0)?0:((x)>=MAX_OBJECT)?0:1)
#define OBJECT_MONSTER_RANGE(x) (((x)<OBJECT_START_MONSTER)?0:((x)>=MAX_OBJECT_MONSTER)?0:1)
#define OBJECT_SUMMON_RANGE(x) (((x)<MAX_OBJECT_MONSTER)?0:((x)>=OBJECT_START_USER)?0:1)
#define OBJECT_USER_RANGE(x) (((x)<OBJECT_START_USER)?0:((x)>=MAX_OBJECT)?0:1)
#define GET_MAX_RESISTANCE(x,y,z) (((((x)>(y))?(x):(y))>(z))?(((x)>(y))?(x):(y)):(z))
#define GET_MAX_WORD_VALUE(x) (((x)>65000)?65000:((WORD)(x)))
#define CHECK_RANGE(x,y) (((x)<0)?0:((x)>=(y))?0:1)

static const int gGameServerMaxAccount[5] = { ((12<<2)+2),((10<<7)-250-30),((10<<7)-200-80),((10<<7)-180-100),((10<<7)-80-200)};

enum eObjectAuthority
{
	AUTHORITY_USER = 1,
	AUTHORITY_INVISIBLE = 2,
	AUTHORITY_GAME_MASTER = 3,
	AUTHORITY_ADMINISTRATOR = 4,
};

enum eObjectConnectState
{
	OBJECT_OFFLINE = 0,
	OBJECT_CONNECTED = 1,
	OBJECT_LOGGED = 2,
	OBJECT_ONLINE = 3,
};

enum eObjectType
{
	OBJECT_NONE = 0,
	OBJECT_USER = 1,
	OBJECT_MONSTER = 2,
	OBJECT_NPC = 3,
	OBJECT_ITEM = 5,
};

enum eObjectState
{
	OBJECT_EMPTY = 0,
	OBJECT_CREATE = 1,
	OBJECT_PLAYING = 2,
	OBJECT_DYING = 4,
	OBJECT_DIECMD = 8,
	OBJECT_DIED = 16,
	OBJECT_DELCMD = 32,
};

enum eInterfaceType
{
	INTERFACE_NONE = 0,
	INTERFACE_TRADE = 1,
	INTERFACE_PARTY = 2,
	INTERFACE_SHOP = 3,
	INTERFACE_GUILD = 4,
	INTERFACE_GUILD_CREATE = 5,
	INTERFACE_WAREHOUSE = 6,
	INTERFACE_CHAOS_BOX = 7,
	INTERFACE_PERSONAL_SHOP = 8,
	INTERFACE_GOLDEN_ARCHER = 9,
	INTERFACE_QUEST = 10,
	INTERFACE_TRANSFER = 11,
	INTERFACE_COMMON = 12,
	INTERFACE_TRAINER = 13,
	INTERFACE_GUILD_REQUEST = 14,
};

enum eActionType
{
	#if(GAMESERVER_UPDATE>=99)
	ACTION_ATTACK1 = 120,
	ACTION_SIT1 = 128,
	ACTION_POSE1 = 129,
	ACTION_HEALING1 = 130,
	#else
	ACTION_ATTACK1 = 100,
	ACTION_SIT1 = 108,
	ACTION_POSE1 = 109,
	ACTION_HEALING1 = 110,
	#endif
};

enum eAttributeType
{
	ATTRIBUTE_NONE = 0,
	ATTRIBUTE_SPECIAL = 1,
	ATTRIBUTE_MONSTER = 2,
	ATTRIBUTE_SUMMON = 3,
	ATTRIBUTE_SUMMONED = 4,
	ATTRIBUTE_INVASION = 5,
	ATTRIBUTE_KALIMA = 6,
};

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 97 (2.5.5) - Standardize damage types and properties
enum eDamageType
{
	DAMAGE_TYPE_NORMAL = 0,
	DAMAGE_TYPE_IGNORE = 1,
	DAMAGE_TYPE_EXE = 2,
	DAMAGE_TYPE_CRITICAL = 3,
	DAMAGE_TYPE_REFLECT = 4,
	DAMAGE_TYPE_POISON = 5,
};

enum eDamageProp
{
	DAMAGE_PROP_DOUBLE = 64,
	DAMAGE_PROP_COMBO = 128,
};

enum eDamageReductionType
{
	DAMAGE_REDUCTION_EXCELLENT_ITEM = 0,
	DAMAGE_REDUCTION_SET_ITEM = 1,
};

enum eEmotionType
{
	EMOTION_REST = 0,
	EMOTION_ATTACK = 1,
	EMOTION_MOVE = 2,
	EMOTION_ESCAPE = 3,
};

enum eExperienceType
{
	EXPERIENCE_COMMON = 0,
	EXPERIENCE_PARTY = 1,
	EXPERIENCE_QUEST = 2,
	EXPERIENCE_DEVIL_SQUARE = 3,
	EXPERIENCE_BLOOD_CASTLE = 4,
	EXPERIENCE_CHAOS_CASTLE = 5,
};

enum eViewState
{
	VIEW_STATE_NONE = 0,
	VIEW_STATE_TELEPORT = 1,
	VIEW_STATE_ACTION_1 = 2,
	VIEW_STATE_ACTION_2 = 3,
	VIEW_STATE_ACTION_3 = 4,
};

struct MESSAGE_STATE_MACHINE
{
	MESSAGE_STATE_MACHINE() // OK
	{
		this->Clear();
	}

	void Clear() // OK
	{
		this->MsgCode = -1;
		this->SendUser = -1;
		this->MsgTime = 0;
		this->SubCode = 0;
	}

	int MsgCode;
	int SendUser;
	int MsgTime;
	int SubCode;
};

struct MESSAGE_STATE_MACHINE_COMMON
{
	MESSAGE_STATE_MACHINE_COMMON() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_MACHINE;

		for(int n=0;n < MAX_MONSTER_SEND_MSG;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_MACHINE& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_MACHINE* CommonStruct;
	MESSAGE_STATE_MACHINE* ObjectStruct[MAX_MONSTER_SEND_MSG];
};

struct MESSAGE_STATE_MACHINE_HEADER
{
	MESSAGE_STATE_MACHINE_HEADER() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_MACHINE_COMMON;

		for(int n=0;n < MAX_OBJECT;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_MACHINE_COMMON& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_MACHINE_COMMON* CommonStruct;
	MESSAGE_STATE_MACHINE_COMMON* ObjectStruct[MAX_OBJECT];
};

struct MESSAGE_STATE_ATTACK_MACHINE
{
	MESSAGE_STATE_ATTACK_MACHINE() // OK
	{
		this->Clear();
	}

	void Clear() // OK
	{
		this->MsgCode = -1;
		this->SendUser = -1;
		this->MsgTime = 0;
		this->SubCode = 0;
		this->SubCode2 = 0;
	}

	int MsgCode;
	int SendUser;
	int MsgTime;
	int SubCode;
	int SubCode2;
};

struct MESSAGE_STATE_ATTACK_MACHINE_COMMON
{
	MESSAGE_STATE_ATTACK_MACHINE_COMMON() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_ATTACK_MACHINE;

		for(int n=0;n < MAX_MONSTER_SEND_ATTACK_MSG;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_ATTACK_MACHINE& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_ATTACK_MACHINE* CommonStruct;
	MESSAGE_STATE_ATTACK_MACHINE* ObjectStruct[MAX_MONSTER_SEND_ATTACK_MSG];
};

struct MESSAGE_STATE_ATTACK_MACHINE_HEADER
{
	MESSAGE_STATE_ATTACK_MACHINE_HEADER() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_ATTACK_MACHINE_COMMON;

		for(int n=0;n < MAX_OBJECT;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_ATTACK_MACHINE_COMMON& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_ATTACK_MACHINE_COMMON* CommonStruct;
	MESSAGE_STATE_ATTACK_MACHINE_COMMON* ObjectStruct[MAX_OBJECT];
};

struct EFFECT_OPTION
{
	int AddPhysiDamage;
	int AddMagicDamage;
	int AddDefense;
	int AddDamageReduction;
	int AddCriticalDamage;
	int MulMaxHP;
	int DivDefense;
};

struct ACTION_STATE
{
	bool Attack;
	bool Move;
	BYTE Emotion;
	BYTE EmotionCount;

	void Reset()
	{
		this->Attack = false;
		this->Move = false;
		this->Emotion = EMOTION_REST;
		this->EmotionCount = 0;
	}

	void Update()
	{
		if(this->EmotionCount > 0)
		{
			this->EmotionCount--;
		}
		else
		{
			this->Emotion = EMOTION_REST;
		}
	}
};

struct VIEWPORT_STRUCT
{
	#pragma pack(1)
	BYTE state;
	short index;
	BYTE type;
	#pragma pack()
};

struct HIT_DAMAGE_STRUCT
{
	int index;
	int damage;
	DWORD time;
};

struct INTERFACE_STATE
{
	DWORD use:2;
	DWORD state:4;
	DWORD type:10;
};

struct ATTACK_STRUCT
{
	int Started;
	int Offline;
	int AutoBuff;
	int SkillIndex;
	int StartMap;
	int StartX;
	int StartY;
	DWORD AttackTime;
	DWORD PotionTime;
	DWORD OnlineTime;
	int TargetIndex;
};

struct PICK_STRUCT 
{
	int Started;
	int ItemCount;
	char ItemList[20][20];
	int PickJewel;
	int PickSet;
	int PickExc;
	int PickMoney;
	DWORD OnlineTime;
};

struct OBJECTSTRUCT
{
	int Index;
	int Connected;
	char LoginMessageSend;
	char LoginMessageCount;
	char CloseCount;
	char CloseType;
	BOOL EnableDelCharacter;
	struct PER_SOCKET_CONTEXT* PerSocketContext;
	SOCKET Socket;
	bool ClientVerify;
	char IpAddr[16];
	char HardwareId[45];
	DWORD AutoSaveTime;
	DWORD ConnectTickCount;
	DWORD ClientTickCount;
	DWORD ServerTickCount;
	DWORD TimeCount;
	WORD Type;
	BYTE Live;
	char Account[11];
	char Name[11];
	char Password[11];
	char MonsterName[32];
	char PersonalCode[14];
	WORD Class;
	BYTE DBClass;
	BYTE ClassCode;
	BYTE ChangeUp;
	short Level;
	int LevelUpPoint;
	WORD FruitPoint;
	DWORD Experience;
	DWORD NextExperience;
	DWORD Money;
	int Strength;
	int Dexterity;
	int Vitality;
	int Energy;
	int Leadership;
	float Life;
	float MaxLife;
	float ScriptMaxLife;
	float Mana;
	float MaxMana;
	DWORD ChatLimitTime;
	BYTE ChatLimitTimeSec;
	int AddStrength;
	int AddDexterity;
	int AddVitality;
	int AddEnergy;
	int AddLeadership;
	int BP;
	int MaxBP;
	int AddBP;
	float VitalityToLife;
	float EnergyToMana;
	BYTE PKLevel;
	int PKCount;
	int PKTime;
	short X;
	short Y;
	BYTE Dir;
	BYTE Map;
	BYTE DeathMap;
	short DeathX;
	short DeathY;
	int AddLife;
	int AddMana;
	BYTE StartX;
	BYTE StartY;
	short OldX;
	short OldY;
	short TX;
	short TY;
	short MTX;
	short MTY;
	int PathCount;
	int PathCur;
	char PathStartEnd;
	short PathOri[15];
	short PathX[15];
	short PathY[15];
	char PathDir[15];
	DWORD PathTime;
	DWORD Authority;
	DWORD Penalty;
	ACTION_STATE ActionState;
	BYTE ActionNumber;
	DWORD CurActionTime;
	DWORD NextActionTime;
	DWORD DelayActionTime;
	DWORD State;
	BYTE Rest;
	char ViewState;
	WORD ViewSkillState;
	DWORD LastAttackTime;
	DWORD TeleportTime;
	char Teleport;
	char KillerType;
	char DieRegen;
	char RegenOk;
	BYTE RegenMapNumber;
	BYTE RegenMapX;
	BYTE RegenMapY;
	DWORD RegenTime;
	DWORD MaxRegenTime;
	short PosNum;
	char DelayLevel;
	int DrinkSpeed;
	DWORD DrinkSpeedLastTime;
	int DrinkDamage;
	DWORD DrinkDamageLastTime;
	DWORD MonsterDeleteTime;
	// Update 88 2.4.6 -> 97K - Suporte a SpecialBag em CustomMonster.txt
	bool CustomMonsterDrop;
	char KalimaGateExist;
	int KalimaGateIndex;
	char KalimaGateEnterCount;
	int KalimaGateLevel;
	OBJECTSTRUCT* AttackObj;
	bool AttackerKilled;
	int SelfDefense[MAX_SELF_DEFENSE];
	DWORD SelfDefenseTime[MAX_SELF_DEFENSE];
	DWORD MySelfDefenseTime;
	int PartyNumber;
	int PartyTargetUser;
	struct GUILD_INFO* Guild;
	int GuildIndex;
	int SummonIndex;
	int Change;
	short TargetNumber;
	short TargetShopNumber;
	short ShopNumber;
	short LastAttackerID;
	int PhysiDamageMin;
	int PhysiDamageMax;
	int MagicDamageMin;
	int MagicDamageMax;
	int PhysiDamageMaxLeft;
	int PhysiDamageMinLeft;
	int PhysiDamageMaxRight;
	int PhysiDamageMinRight;
	int DKDamageMultiplierRate;
	int DLDamageMultiplierRate;
	int AttackSuccessRate;
	int PhysiSpeed;
	int MagicSpeed;
	int Defense;
	int MagicDefense;
	int DefenseSuccessRate;
	short MoveSpeed;
	short MoveRange;
	short AttackRange;
	short AttackType;
	short ViewRange;
	short Attribute;
	short ItemRate;
	short MoneyRate;
	CSkill* SkillBackup;
	CSkill* Skill;
	WORD MultiSkillIndex;
	WORD MultiSkillCount;
	#if(GAMESERVER_UPDATE>=99)
	BYTE CharSet[13];
	#else
	BYTE CharSet[11];
	#endif
	BYTE Resistance[MAX_RESISTANCE_TYPE];
	VIEWPORT_STRUCT* VpPlayer;
	VIEWPORT_STRUCT* VpPlayer2;
	VIEWPORT_STRUCT* VpPlayerItem;
	int VPCount;
	int VPCount2;
	int VPCountItem;
	HIT_DAMAGE_STRUCT* HitDamage;
	INTERFACE_STATE Interface;
	DWORD InterfaceTime;
	char Transaction;
	CItem* Inventory;
	CItem* Inventory1;
	CItem* Inventory2;
	BYTE* InventoryMap;
	BYTE* InventoryMap1;
	BYTE* InventoryMap2;
	CItem* Trade;
	BYTE* TradeMap;
	int TradeMoney;
	bool TradeOk;
	CItem* Warehouse;
	BYTE* WarehouseMap;
	char WarehouseCount;
	short WarehousePW;
	BYTE WarehouseLock;
	int WarehouseMoney;
	int WarehouseSave;
	CItem* ChaosBox;
	BYTE* ChaosBoxMap;
	int ChaosMoney;
	int ChaosSuccessRate;
	int ChaosLock;
	DWORD Option;
	int ChaosCastleBlowTime;
	int DuelUserReserved;
	int DuelUserRequested;
	int DuelUser;
	BYTE DuelScore;
	DWORD DuelTickCount;
	bool PShopOpen;
	bool PShopTransaction;
	bool PShopItemChange;
	bool PShopRedrawAbs;
	char PShopText[36];
	bool PShopWantDeal;
	int PShopDealerIndex;
	char PShopDealerName[10];
	CRITICAL_SECTION PShopTrade;
	int VpPShopPlayer[MAX_VIEWPORT];
	int VpPShopPlayerCount;
	int LoadWarehouse;
	BYTE Quest[50];
	bool SendQuestInfo;
	int CheckLifeTime;
	BYTE LastTeleportTime;
	BYTE SkillNovaState;
	BYTE SkillNovaCount;
	DWORD SkillNovaTime;
	BYTE ReqWarehouseOpen;
	bool IsFullSetItem;
	WORD SkillSummonPartyTime;
	BYTE SkillSummonPartyMap;
	BYTE SkillSummonPartyX;
	BYTE SkillSummonPartyY;
	bool IsChaosMixCompleted;
	bool SkillLongSpearChange;
	DWORD CharSaveTime;
	CEffect* Effect;
	DWORD* SkillDelay;
	DWORD* HackPacketDelay;
	DWORD* HackPacketCount;
	int WarehouseNumber;
	int AccountLevel;
	char AccountExpireDate[20];
	char AutoPartyPassword[11];
	// Update 90 2.4.8 -> 97K - EventFlag e EventLevel para rastreamento de eventos locais
	BYTE EventFlag;
	BYTE EventLevel;
	// Update 91 2.4.9 -> 97K - Rastreamento individual de movimento para deteccao de speed hack
	DWORD MoveTime;
	short LastX;
	short LastY;
	int AutoAddPointCount;
	int AutoAddPointStats[5];
	int AutoResetEnable;
	int AutoResetStats[5];
	int CommandManagerTransaction[1];
	int GoldenArcherTransaction[2];
	DWORD TradeOkTime;
	DWORD PotionTime;
	DWORD ComboTime;
	DWORD HPAutoRecuperationTime;
	DWORD MPAutoRecuperationTime;
	DWORD BPAutoRecuperationTime;
	int Reset;
	int MasterReset;
	int ChangeSkin;
	EFFECT_OPTION EffectOption;
	int ArmorSetBonus;
	int SkillDamageBonus;
	int DoubleDamageRate;
	int IgnoreDefenseRate;
	int CriticalDamageRate;
	int CriticalDamage;
	int ExcellentDamageRate;
	int ExcellentDamage;
	int MoneyAmountDropRate;
	int HPRecovery;
	int MPRecovery;
	int BPRecovery;
	int HPRecoveryRate;
	int MPRecoveryRate;
	int BPRecoveryRate;
	int HPRecoveryCount;
	int MPRecoveryCount;
	int BPRecoveryCount;
	int MPConsumptionRate;
	int BPConsumptionRate;
	int ShieldDamageReduction;
	int ShieldDamageReductionTime;
	int DamageReduction[MAX_DAMAGE_REDUCTION];
	int DamageReflect;
	int HuntHP;
	int HuntMP;
	BYTE KillCount;
	int RegenType;
	DWORD LastCheckTick;
	DWORD CommandCheckTime[200];
	DWORD HackSkillDelay[618];
	DWORD HackSkillCount[618];
	WORD BCCount;
	WORD CCCount;
	WORD DSCount;
	BYTE Permission[50];
	ATTACK_STRUCT Attack;
	PICK_STRUCT Pick;
	int InvasionIndex;
	bool WindowTimeOpen;
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.6) - Security cooldown timer for class change
	DWORD CustomChangeClassTime;
};

struct OBJECTSTRUCT_HEADER
{
	OBJECTSTRUCT_HEADER() // OK
	{
		this->CommonStruct = new OBJECTSTRUCT;

		for(int n=0;n < MAX_OBJECT;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	OBJECTSTRUCT& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	OBJECTSTRUCT* CommonStruct;
	OBJECTSTRUCT* ObjectStruct[MAX_OBJECT];
};

typedef OBJECTSTRUCT* LPOBJ;

extern OBJECTSTRUCT_HEADER gObj;

extern MESSAGE_STATE_MACHINE_HEADER gSMMsg;

extern MESSAGE_STATE_ATTACK_MACHINE_HEADER gSMAttackProcMsg;

extern DWORD gLevelExperience[MAX_CHARACTER_LEVEL+1];

extern DWORD gPetExperience[MAX_CHARACTER_LEVEL+1];

//**************************************************************************//
// OBJECT MAIN FUNCTIONS ***************************************************//
//**************************************************************************//
void gObjEventRunProc();
void gObjViewportProc();
void gObjFirstProc();
void gObjCloseProc();
void gObjCountProc();
void gObjAccountLevelProc();
//**************************************************************************//
// OBJECT BASE FUNCTIONS ***************************************************//
//**************************************************************************//
void gObjInit();
void gObjAllDisconnect();
void gObjOfflineDisconnect();
void gObjSetExperienceTable();
void gObjCharZeroSet(int aIndex);
void gObjClearPlayerOption(LPOBJ lpObj);
void gObjClearSpecialOption(LPOBJ lpObj);
void gObjCalcExperience(LPOBJ lpObj);
bool gObjAllocData(int aIndex);
void gObjFreeData(int aIndex);
short gObjAddSearch(SOCKET socket,char* IpAddress);
short gObjAdd(SOCKET socket,char* IpAddress,int aIndex);
short gObjDel(int aIndex);
LPOBJ gObjFind(char* name);
int gObjCalcDistance(LPOBJ lpObj,LPOBJ lpTarget);
int gObjCalcDistance(LPOBJ lpObj,int x,int y);
bool gObjGetRandomFreeLocation(int map,int* ox,int* oy,int tx,int ty,int count);
bool gObjGetRandomMonsterLocation(int map,int* ox,int* oy,int tx,int ty,int count);
//**************************************************************************//
// OBJECT CHECK FUNCTIONS **************************************************//
//**************************************************************************//
bool gObjIsConnected(int aIndex);
bool gObjIsConnectedGP(int aIndex);
bool gObjIsConnectedGS(int aIndex);
bool gObjIsNameValid(int aIndex,char* name);
bool gObjIsAccountValid(int aIndex,char* account);
bool gObjIsChangeSkin(int aIndex);
bool gObjCheckMaxMoney(int aIndex,DWORD AddMoney);
bool gObjCheckPersonalCode(int aIndex,char* PersonalCode);
bool gObjCheckResistance(LPOBJ lpObj,int type);
bool gObjCheckTeleportArea(int aIndex,int x,int y);
bool gObjCheckMapTile(LPOBJ lpObj,int type);
//**************************************************************************//
// ITEM TRANSACTION FUNCTIONS **********************************************//
//**************************************************************************//
bool gObjFixInventoryPointer(int aIndex);
void gObjSetInventory1Pointer(LPOBJ lpObj);
void gObjSetInventory2Pointer(LPOBJ lpObj);
bool gObjInventoryTransaction(int aIndex);
bool gObjInventoryCommit(int aIndex);
bool gObjInventoryRollback(int aIndex);
//**************************************************************************//
// VIEWPORT FUNCTIONS ******************************************************//
//**************************************************************************//
void gObjSetViewport(int aIndex,int state);
void gObjClearViewport(LPOBJ lpObj);
void gObjViewportListProtocolDestroy(LPOBJ lpObj);
void gObjViewportListProtocolCreate(LPOBJ lpObj);
void gObjViewportListProtocol(int aIndex);
void gObjViewportListDestroy(int aIndex);
void gObjViewportListCreate(int aIndex);
//**************************************************************************//
// USER FUNCTIONS **********************************************************//
//**************************************************************************//
void gObjSetKillCount(int aIndex,int type);
void gObjTeleportMagicUse(int aIndex,int x,int y);
void gObjInterfaceCheckTime(LPOBJ lpObj);
void gObjSkillNovaCheckTime(LPOBJ lpObj);
void gObjPKDownCheckTime(LPOBJ lpObj,int time,int type);
void gObjUserDie(LPOBJ lpObj,LPOBJ lpTarget);
void gObjPlayerDiePenalty(LPOBJ lpObj,LPOBJ lpTarget);
void gObjPlayerKiller(LPOBJ lpObj,LPOBJ lpTarget);
int gObjMoveGate(int aIndex,int gate);
void gObjTeleport(int aIndex,int map,int x,int y);
void gObjSummonAlly(LPOBJ lpObj,int map,int x,int y);
void gObjSkillUseProc(LPOBJ lpObj);
void gObjUserKill(int aIndex);
int gObjInventorySearchSerialNumber(LPOBJ lpObj,DWORD serial);
int gObjWarehouseSearchSerialNumber(LPOBJ lpObj,DWORD serial);
void gObjAddMsgSend(LPOBJ lpObj,int MsgCode,int SendUser,int SubCode);
void gObjAddMsgSendDelay(LPOBJ lpObj,int MsgCode,int SendUser,int MsgTimeDelay,int SubCode);
void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj,int MsgCode,int SendUser,int MsgTimeDelay,int SubCode,int SubCode2);
//**************************************************************************//
// OTHERS FUNCTIONS ********************************************************//
//**************************************************************************//
void gObjSecondProc();
void gObjDelayLifeCheck(int aIndex);
bool gObjBackSpring(LPOBJ lpObj,LPOBJ lpTarget);
bool gObjBackSpring2(LPOBJ lpObj,LPOBJ lpTarget,int count);
bool gObjIsSelfDefense(LPOBJ lpObj,int aTargetIndex);
void gObjCheckSelfDefense(LPOBJ lpObj,int aTargetIndex);
void gObjTimeCheckSelfDefense(LPOBJ lpObj);
void gObjSetPosition(int aIndex,int x,int y);
void gObjUseDrink(LPOBJ lpObj,int level);
// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 101 (2.6.1) - Assinatura ampliada para nivel e index do item
void gObjCustomLogPlusChaosMix(LPOBJ lpObj,int index,int level);
bool gObjCheckAutoParty(LPOBJ lpObj,LPOBJ lpTarget);
void gObjEventEntrySaveSend(LPOBJ lpObj,int type);

extern int gObjCount;
extern int gObjMonCount;
extern int gObjCallMonCount;
extern int gObjTotalUser;
extern int gObjTotalMonster;
extern int gObjTotalOffline;
extern int gCloseMsg;
extern int gCloseMsgTime;
extern int gCloseServer;