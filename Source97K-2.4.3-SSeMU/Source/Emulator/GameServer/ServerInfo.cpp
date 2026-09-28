// ServerInfo.cpp: implementation of the CServerInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerInfo.h"
#include "BlackList.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CommandManager.h"
#include "CommandRequirement.h"
#include "ConnectMember.h"
#include "CustomArena.h"
#include "CustomAttack.h"
#include "CustomBuyVip.h"
#include "CustomChangeClass.h"
#include "CustomDailyReward.h"
#include "CustomEventDrop.h"
#include "CustomGift.h"
#include "CustomHealthBar.h"
#include "CustomLukeHelper.h"
#include "CustomMonster.h"
#include "CustomMonsterKillCount.h"
#include "CustomPick.h"
#include "DamageTable.h"
#include "DefaultClassInfo.h"
#include "DefaultClassFreebies.h"
#include "DevilSquare.h"
#include "DropEvent.h"
#include "EffectManager.h"
#include "EventEntryLevel.h"
#include "EventSpawnStage.h"
#include "ExcellentOptionRate.h"
#include "ExperienceTable.h"
#include "Filter.h"
#include "GameMaster.h"
#include "Gate.h"
#include "GoldenArcher.h"
#include "HackPacketCheck.h"
#include "HackSkillSpeedCheck.h"
#include "InvasionManager.h"
#include "ItemBagManager.h"
#include "ItemDrop.h"
#include "ItemLevel.h"
#include "ItemMoneyDrop.h"
#include "ItemMove.h"
#include "ItemOption.h"
#include "ItemOptionRate.h"
#include "ItemStack.h"
#include "ItemValue.h"
#include "MapManager.h"
#include "MapRateInfo.h"
#include "Message.h"
#include "MonsterManager.h"
#include "MonsterMove.h"
#include "MonsterSetBase.h"
#include "Move.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "PacketManager.h"
#include "Path.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "QuestReward.h"
#include "ResetLimit.h"
#include "ResetTable.h"
#include "ShopManager.h"
#include "SkillDamage.h"
#include "SkillEnabler.h"
#include "SkillHitBox.h"
#include "SkillManager.h"
#include "SkillUseArea.h"
#include "Util.h"
#include "ScriptLoader.h"

CServerInfo gServerInfo;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerInfo::CServerInfo() // OK
{

}

CServerInfo::~CServerInfo() // OK
{

}

void CServerInfo::ReadInit() // OK
{
	gPath.SetMainPath(MAIN_PATH);

	this->ReadBlackListInfo();

	this->ReadChaosMixInfo();

	this->ReadCharacterInfo();

	this->ReadCommandInfo();

	this->ReadCommonInfo();

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 99 (2.5.8) - Leitura das configuracoes de multi-idioma
	this->ReadLangInfo();

	this->ReadCustomInfo();

	this->ReadEventInfo();

	this->ReadEventItemBagInfo();

	this->ReadHackInfo();

	this->ReadItemInfo();

	this->ReadMapInfo();

	this->ReadMonsterInfo();

	this->ReadMoveInfo();

	this->ReadQuestInfo();

	this->ReadScriptInfo();

	this->ReadShopInfo();

	this->ReadSkillInfo();

	this->ReadUtilInfo();
}

void CServerInfo::ReadBlackListInfo() // OK
{
	gBlackList.Load(gPath.GetFullPath("BlackList.txt"));
}

void CServerInfo::ReadChaosMixInfo() // OK
{
	this->ReadChaosMixInfo("GameServerInfo",".\\Data\\GameServerInfo - ChaosMix.dat");
}

void CServerInfo::ReadCharacterInfo() // OK
{
	this->ReadCharacterInfo("GameServerInfo",".\\Data\\GameServerInfo - Character.dat");

	gDefaultClassInfo.Load(gPath.GetFullPath("Character\\DefaultClassInfo.txt"));

	gDefaultClassFreebies.Load(gPath.GetFullPath("Character\\DefaultClassFreebies.txt"));
}

void CServerInfo::ReadCommandInfo() // OK
{
	this->ReadCommandInfo("GameServerInfo",".\\Data\\GameServerInfo - Command.dat");

	gCommandManager.Load(gPath.GetFullPath("CommandManager.txt"));

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 96 (2.5.4) - Carregamento de CommandRequirement.txt
	gCommandRequirement.Load(gPath.GetFullPath("CommandRequirement.txt"));
}

void CServerInfo::ReadCommonInfo() // OK
{
	this->ReadCommonInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat");

	gConnectMember.Load(gPath.GetFullPath("ConnectMember.txt"));

	gMapManager.Load(gPath.GetFullPath("MapManager.txt"));

	gMapRateInfo.Load(gPath.GetFullPath("MapRateInfo.txt"));
}

void CServerInfo::ReadCustomInfo() // OK
{
	gCustomArena.ReadCustomArenaInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomAttack.ReadCustomAttackInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomBuyVip.ReadCustomBuyVipInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomChangeClass.ReadCustomChangeClassInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomEventDrop.ReadCustomEventDropInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomGift.ReadCustomGiftInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomLukeHelper.ReadCustomLukeHelperInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomMonsterKillCount.ReadCustomMonsterKillCountInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomPick.ReadCustomPickInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomArena.Load(gPath.GetFullPath("Custom\\CustomArena.txt"));

	gCustomAttack.Load(gPath.GetFullPath("Custom\\CustomAttack.txt"));

	gCustomBuyVip.Load(gPath.GetFullPath("Custom\\CustomBuyVip.txt"));

	gCustomDailyReward.Load(gPath.GetFullPath("Custom\\CustomDailyReward.txt"));

	gCustomEventDrop.Load(gPath.GetFullPath("Custom\\CustomEventDrop.txt"));

	gCustomGift.Load(gPath.GetFullPath("Custom\\CustomGift.txt"));

	gCustomHealthBar.Load(gPath.GetFullPath("Custom\\CustomHealthBar.txt"));

	gCustomLukeHelper.Load(gPath.GetFullPath("Custom\\CustomLukeHelper.txt"));

	gCustomMonster.Load(gPath.GetFullPath("Custom\\CustomMonster.txt"));

	gCustomMonsterKillCount.Load(gPath.GetFullPath("Custom\\CustomMonsterKillCount.txt"));

	gCustomPick.Load(gPath.GetFullPath("Custom\\CustomPick.txt"));
}

void CServerInfo::ReadEventInfo() // OK
{
	this->ReadEventInfo("GameServerInfo",".\\Data\\GameServerInfo - Event.dat");

	gBloodCastle.Load(gPath.GetFullPath("Event\\BloodCastle.dat"));

	gBonusManager.Load(gPath.GetFullPath("Event\\BonusManager.dat"));

	gDevilSquare.Load(gPath.GetFullPath("Event\\DevilSquare.dat"));

	gDropEvent.Load(gPath.GetFullPath("Event\\DropEvent.dat"));

	gEventEntryLevel.Load(gPath.GetFullPath("Event\\EventEntryLevel.dat"));

	gEventSpawnStage.Load(gPath.GetFullPath("Event\\EventStageSpawn.dat"));

	gGoldenArcher.Load(gPath.GetFullPath("Event\\GoldenArcher.dat"));

	gInvasionManager.Load(gPath.GetFullPath("Event\\InvasionManager.dat"));
}

void CServerInfo::ReadEventItemBagInfo() // OK
{
	gItemBagManager.Load(gPath.GetFullPath("EventItemBagManager.txt"));

	gItemBagManager.LoadEventItemBag();
}

void CServerInfo::ReadHackInfo() // OK
{
	this->ReadHackInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat");
	
	gHackPacketCheck.Load(gPath.GetFullPath("Hack\\HackPacketCheck.txt"));

	gHackSkillSpeedCheck.Load(gPath.GetFullPath("Hack\\HackSkillSpeedCheck.txt"));

	gPacketManager.LoadEncryptionKey(gPath.GetFullPath("Hack\\Enc2.dat"));

	gPacketManager.LoadDecryptionKey(gPath.GetFullPath("Hack\\Dec1.dat"));
}

void CServerInfo::ReadItemInfo() // OK
{
	this->ReadItemInfo("GameServerInfo",".\\Data\\GameServerInfo - Item.dat");

	gItemManager.Load(gPath.GetFullPath("Item\\Item.txt"));

	gExcellentOptionRate.Load(gPath.GetFullPath("Item\\ExcellentOptionRate.txt"));

	gItemDrop.Load(gPath.GetFullPath("Item\\ItemDrop.txt"));

	gItemLevel.Load(gPath.GetFullPath("Item\\ItemLevel.txt"));

	gItemMoneyDrop.Load(gPath.GetFullPath("Item\\ItemMoneyDrop.txt"));

	gItemMove.Load(gPath.GetFullPath("Item\\ItemMove.txt"));

	gItemOption.Load(gPath.GetFullPath("Item\\ItemOption.txt"));

	gItemOptionRate.Load(gPath.GetFullPath("Item\\ItemOptionRate.txt"));

	gItemStack.Load(gPath.GetFullPath("Item\\ItemStack.txt"));

	gItemValue.Load(gPath.GetFullPath("Item\\ItemValue.txt"));
}

void CServerInfo::ReadMapInfo() // OK
{
	FILE* file;

	char path[256] = {0};

	char temp[256] = {0};

	for(int n=0;n < MAX_MAP;n++)
	{
		wsprintf(temp,"Terrain\\terrain%d.att",(n+1));

		if(fopen_s(&file,gPath.GetFullPath(temp),"r") == 0)
		{
			strcpy_s(path,temp);
			fclose(file);
		}

		gMap[n].Load(gPath.GetFullPath(path),n);
	}
}

void CServerInfo::ReadMonsterInfo() // OK
{
	gMonsterManager.Load(gPath.GetFullPath("Monster\\MonsterList.txt"));

	gMonsterMove.Load(gPath.GetFullPath("Monster\\MonsterMove.txt"));

	gMonsterSetBase.LoadSpawn();

	gShopManager.ReloadShop();
}

void CServerInfo::ReadMoveInfo() // OK
{
	gGate.Load(gPath.GetFullPath("Move\\Gate.txt"));

	gMove.Load(gPath.GetFullPath("Move\\Move.txt"));
}

void CServerInfo::ReadQuestInfo() // OK
{
	gQuest.Load(gPath.GetFullPath("Quest\\Quest.txt"));

	gQuestObjective.Load(gPath.GetFullPath("Quest\\QuestObjective.txt"));

	gQuestReward.Load(gPath.GetFullPath("Quest\\QuestReward.txt"));
}

void CServerInfo::ReadScriptInfo() // OK
{
	gScriptLoader.OnShutScript();

	gScriptLoader.Load(gPath.GetFullPath("Script\\ScriptMain.lua"));

	this->m_CalcCharacter.Load(gPath.GetFullPath("Script\\Character\\CalcCharacter.lua"));
}

void CServerInfo::ReadShopInfo() // OK
{
	gShopManager.Load(gPath.GetFullPath("ShopManager.txt"));

	gShopManager.ReloadShop();

	gShopManager.ReloadShopInterface();
}

void CServerInfo::ReadSkillInfo() // OK
{
	this->ReadSkillInfo("GameServerInfo",".\\Data\\GameServerInfo - Skill.dat");

	gEffectManager.Load(gPath.GetFullPath("Skill\\EffectList.txt"));

	gSkillDamage.Load(gPath.GetFullPath("Skill\\SkillDamage.txt"));

	gSkillHitBox.Load(gPath.GetFullPath("Skill\\SkillElect.hit"));

	gSkillEnabler.Load(gPath.GetFullPath("Skill\\SkillEnabler.txt"));

	gSkillManager.Load(gPath.GetFullPath("Skill\\SkillList.txt"));

	gSkillUseArea.Load(gPath.GetFullPath("Skill\\SkillUseArea.txt"));
}

void CServerInfo::ReadUtilInfo() // OK
{
	gDamageTable.Load(gPath.GetFullPath("Util\\DamageTable.txt"));

	gExperienceTable.Load(gPath.GetFullPath("Util\\ExperienceTable.txt"));

	gFilter.Load(gPath.GetFullPath("Util\\Filter.txt"));

	gGameMaster.Load(gPath.GetFullPath("Util\\GameMaster.txt"));

	gResetLimit.Load(gPath.GetFullPath("Util\\ResetLimit.txt"));

	gResetTable.Load(gPath.GetFullPath("Util\\ResetTable.txt"));
}

void CServerInfo::WriteItemInfo() // OK
{
	gItemManager.GetItemBmd(gPath.GetFullPath("Item\\Item.bmd"));
}

void CServerInfo::ReloadMonsterInfo() // OK
{
	for(int n=0;n < MAX_OBJECT_MONSTER;n++)
	{
		if(gObj[n].Type != OBJECT_MONSTER && gObj[n].Type != OBJECT_NPC)
		{
			continue;
		}

		gObjDel(n);
	}

	gObjMonCount = 0;

	this->ReadMonsterInfo();

	gMonsterManager.SetMonsterData();
}

//->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void CServerInfo::ReadStartupInfo(char* section,char* path) // OK
{
	char buff[256] = {0};

	GetPrivateProfileString(section,"ServerName","",this->m_ServerName,sizeof(this->m_ServerName),path);

	this->m_ServerCode = GetPrivateProfileInt(section,"ServerCode",0,path);

	this->m_ServerLock = GetPrivateProfileInt(section,"ServerLock",0,path);

	this->m_ServerPort = GetPrivateProfileInt(section,"ServerPort",0,path);

	wsprintf(this->m_ServerMutex,"MUTEX_%d",this->m_ServerCode);

	GetPrivateProfileString(section,"ServerVersion","",buff,sizeof(buff),path);

	this->m_ServerVersion[0] = buff[0];

	this->m_ServerVersion[1] = buff[2];

	this->m_ServerVersion[2] = buff[3];

	this->m_ServerVersion[3] = buff[5];

	this->m_ServerVersion[4] = buff[6];

	this->m_ServerVersion[5] = 0;

	GetPrivateProfileString(section,"ServerSerial","",this->m_ServerSerial,sizeof(this->m_ServerSerial),path);

	this->m_ServerMaxUserNumber = GetPrivateProfileInt(section,"ServerMaxUserNumber",0,path);

	this->m_ServerMaxUserNumber = ((this->m_ServerMaxUserNumber>gGameServerMaxAccount[4]) ? gGameServerMaxAccount[4] : this->m_ServerMaxUserNumber);

	this->m_ServerEncDecKey1 = GetPrivateProfileInt(section,"ServerEncDecKey1",0,path);

	this->m_ServerEncDecKey2 = GetPrivateProfileInt(section,"ServerEncDecKey2",0,path);

	GetPrivateProfileString(section,"DataServerAddress","",this->m_DataServerAddress,sizeof(this->m_DataServerAddress),path);
	this->m_DataServerPort = GetPrivateProfileInt(section,"DataServerPort",0,path);

	GetPrivateProfileString(section,"JoinServerAddress","",this->m_JoinServerAddress,sizeof(this->m_JoinServerAddress),path);
	this->m_JoinServerPort = GetPrivateProfileInt(section,"JoinServerPort",0,path);

	GetPrivateProfileString(section,"ConnectServerAddress","",this->m_ConnectServerAddress,sizeof(this->m_ConnectServerAddress),path);
	this->m_ConnectServerPort = GetPrivateProfileInt(section,"ConnectServerPort",0,path);

	this->m_WriteChatLog = GetPrivateProfileInt(section,"WriteChatLog",0,path);
	this->m_WriteCommandLog = GetPrivateProfileInt(section,"WriteCommandLog",0,path);
	this->m_WriteTradeLog = GetPrivateProfileInt(section,"WriteTradeLog",0,path);
	this->m_WriteConnectLog = GetPrivateProfileInt(section,"WriteConnectLog",0,path);
	this->m_WriteHackLog = GetPrivateProfileInt(section,"WriteHackLog",0,path);
	this->m_WriteChaosMixLog = GetPrivateProfileInt(section,"WriteChaosMixLog",0,path);
	this->m_WriteScriptLog = GetPrivateProfileInt(section,"WriteScriptLog",0,path);
	this->m_WritePenaltyLog = GetPrivateProfileInt(section,"WritePenaltyLog",0,path);

	this->m_ExperienceMultiplierConstA = GetPrivateProfileInt(section,"ExperienceMultiplierConstA",0,path);
	this->m_ExperienceMultiplierConstB = GetPrivateProfileInt(section,"ExperienceMultiplierConstB",0,path);

	this->m_MaxLevel = GetPrivateProfileInt(section,"MaxLevel",0,path);
	this->m_MaxLevel = ((this->m_MaxLevel > MAX_CHARACTER_LEVEL) ? MAX_CHARACTER_LEVEL : this->m_MaxLevel);
}

void CServerInfo::ReadChaosMixInfo(char* section,char* path) // OK
{
	this->m_ChaosItemMixRate[0] = GetPrivateProfileInt(section,"ChaosItemMixRate_AL0",0,path);
	this->m_ChaosItemMixRate[1] = GetPrivateProfileInt(section,"ChaosItemMixRate_AL1",0,path);
	this->m_ChaosItemMixRate[2] = GetPrivateProfileInt(section,"ChaosItemMixRate_AL2",0,path);
	this->m_ChaosItemMixRate[3] = GetPrivateProfileInt(section,"ChaosItemMixRate_AL3",0,path);

	this->m_DevilSquareMixRate1[0] = GetPrivateProfileInt(section,"DevilSquareMixRate1_AL0",0,path);
	this->m_DevilSquareMixRate1[1] = GetPrivateProfileInt(section,"DevilSquareMixRate1_AL1",0,path);
	this->m_DevilSquareMixRate1[2] = GetPrivateProfileInt(section,"DevilSquareMixRate1_AL2",0,path);
	this->m_DevilSquareMixRate1[3] = GetPrivateProfileInt(section,"DevilSquareMixRate1_AL3",0,path);

	this->m_DevilSquareMixRate2[0] = GetPrivateProfileInt(section,"DevilSquareMixRate2_AL0",0,path);
	this->m_DevilSquareMixRate2[1] = GetPrivateProfileInt(section,"DevilSquareMixRate2_AL1",0,path);
	this->m_DevilSquareMixRate2[2] = GetPrivateProfileInt(section,"DevilSquareMixRate2_AL2",0,path);
	this->m_DevilSquareMixRate2[3] = GetPrivateProfileInt(section,"DevilSquareMixRate2_AL3",0,path);
	
	this->m_DevilSquareMixRate3[0] = GetPrivateProfileInt(section,"DevilSquareMixRate3_AL0",0,path);
	this->m_DevilSquareMixRate3[1] = GetPrivateProfileInt(section,"DevilSquareMixRate3_AL1",0,path);
	this->m_DevilSquareMixRate3[2] = GetPrivateProfileInt(section,"DevilSquareMixRate3_AL2",0,path);
	this->m_DevilSquareMixRate3[3] = GetPrivateProfileInt(section,"DevilSquareMixRate3_AL3",0,path);

	this->m_DevilSquareMixRate4[0] = GetPrivateProfileInt(section,"DevilSquareMixRate4_AL0",0,path);
	this->m_DevilSquareMixRate4[1] = GetPrivateProfileInt(section,"DevilSquareMixRate4_AL1",0,path);
	this->m_DevilSquareMixRate4[2] = GetPrivateProfileInt(section,"DevilSquareMixRate4_AL2",0,path);
	this->m_DevilSquareMixRate4[3] = GetPrivateProfileInt(section,"DevilSquareMixRate4_AL3",0,path);

	this->m_PlusCommonItemLevelMixRate[0][0] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate1_AL0",0,path);
	this->m_PlusCommonItemLevelMixRate[0][1] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate1_AL1",0,path);
	this->m_PlusCommonItemLevelMixRate[0][2] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate1_AL2",0,path);
	this->m_PlusCommonItemLevelMixRate[0][3] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate1_AL3",0,path);

	this->m_PlusExcSetItemLevelMixRate[0][0] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate1_AL0",0,path);
	this->m_PlusExcSetItemLevelMixRate[0][1] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate1_AL1",0,path);
	this->m_PlusExcSetItemLevelMixRate[0][2] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate1_AL2",0,path);
	this->m_PlusExcSetItemLevelMixRate[0][3] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate1_AL3",0,path);

	this->m_PlusCommonItemLevelMixRate[1][0] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate2_AL0",0,path);
	this->m_PlusCommonItemLevelMixRate[1][1] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate2_AL1",0,path);
	this->m_PlusCommonItemLevelMixRate[1][2] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate2_AL2",0,path);
	this->m_PlusCommonItemLevelMixRate[1][3] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate2_AL3",0,path);

	this->m_PlusExcSetItemLevelMixRate[1][0] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate2_AL0",0,path);
	this->m_PlusExcSetItemLevelMixRate[1][1] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate2_AL1",0,path);
	this->m_PlusExcSetItemLevelMixRate[1][2] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate2_AL2",0,path);
	this->m_PlusExcSetItemLevelMixRate[1][3] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate2_AL3",0,path);

	this->m_PlusCommonItemLevelMixRate[2][0] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate3_AL0",0,path);
	this->m_PlusCommonItemLevelMixRate[2][1] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate3_AL1",0,path);
	this->m_PlusCommonItemLevelMixRate[2][2] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate3_AL2",0,path);
	this->m_PlusCommonItemLevelMixRate[2][3] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate3_AL3",0,path);

	this->m_PlusExcSetItemLevelMixRate[2][0] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate3_AL0",0,path);
	this->m_PlusExcSetItemLevelMixRate[2][1] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate3_AL1",0,path);
	this->m_PlusExcSetItemLevelMixRate[2][2] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate3_AL2",0,path);
	this->m_PlusExcSetItemLevelMixRate[2][3] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate3_AL3",0,path);

	this->m_PlusCommonItemLevelMixRate[3][0] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate4_AL0",0,path);
	this->m_PlusCommonItemLevelMixRate[3][1] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate4_AL1",0,path);
	this->m_PlusCommonItemLevelMixRate[3][2] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate4_AL2",0,path);
	this->m_PlusCommonItemLevelMixRate[3][3] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate4_AL3",0,path);

	this->m_PlusExcSetItemLevelMixRate[3][0] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate4_AL0",0,path);
	this->m_PlusExcSetItemLevelMixRate[3][1] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate4_AL1",0,path);
	this->m_PlusExcSetItemLevelMixRate[3][2] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate4_AL2",0,path);
	this->m_PlusExcSetItemLevelMixRate[3][3] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate4_AL3",0,path);

	this->m_DinorantMixRate[0] = GetPrivateProfileInt(section,"DinorantMixRate_AL0",0,path);
	this->m_DinorantMixRate[1] = GetPrivateProfileInt(section,"DinorantMixRate_AL1",0,path);
	this->m_DinorantMixRate[2] = GetPrivateProfileInt(section,"DinorantMixRate_AL2",0,path);
	this->m_DinorantMixRate[3] = GetPrivateProfileInt(section,"DinorantMixRate_AL3",0,path);

	this->m_FruitMixRate[0] = GetPrivateProfileInt(section,"FruitMixRate_AL0",0,path);
	this->m_FruitMixRate[1] = GetPrivateProfileInt(section,"FruitMixRate_AL1",0,path);
	this->m_FruitMixRate[2] = GetPrivateProfileInt(section,"FruitMixRate_AL2",0,path);
	this->m_FruitMixRate[3] = GetPrivateProfileInt(section,"FruitMixRate_AL3",0,path);

	this->m_Wing2MixRate[0] = GetPrivateProfileInt(section,"Wing2MixRate_AL0",0,path);
	this->m_Wing2MixRate[1] = GetPrivateProfileInt(section,"Wing2MixRate_AL1",0,path);
	this->m_Wing2MixRate[2] = GetPrivateProfileInt(section,"Wing2MixRate_AL2",0,path);
	this->m_Wing2MixRate[3] = GetPrivateProfileInt(section,"Wing2MixRate_AL3",0,path);

	this->m_BloodCastleMixRate1[0] = GetPrivateProfileInt(section,"BloodCastleMixRate1_AL0",0,path);
	this->m_BloodCastleMixRate1[1] = GetPrivateProfileInt(section,"BloodCastleMixRate1_AL1",0,path);
	this->m_BloodCastleMixRate1[2] = GetPrivateProfileInt(section,"BloodCastleMixRate1_AL2",0,path);
	this->m_BloodCastleMixRate1[3] = GetPrivateProfileInt(section,"BloodCastleMixRate1_AL3",0,path);

	this->m_BloodCastleMixRate2[0] = GetPrivateProfileInt(section,"BloodCastleMixRate2_AL0",0,path);
	this->m_BloodCastleMixRate2[1] = GetPrivateProfileInt(section,"BloodCastleMixRate2_AL1",0,path);
	this->m_BloodCastleMixRate2[2] = GetPrivateProfileInt(section,"BloodCastleMixRate2_AL2",0,path);
	this->m_BloodCastleMixRate2[3] = GetPrivateProfileInt(section,"BloodCastleMixRate2_AL3",0,path);

	this->m_BloodCastleMixRate3[0] = GetPrivateProfileInt(section,"BloodCastleMixRate3_AL0",0,path);
	this->m_BloodCastleMixRate3[1] = GetPrivateProfileInt(section,"BloodCastleMixRate3_AL1",0,path);
	this->m_BloodCastleMixRate3[2] = GetPrivateProfileInt(section,"BloodCastleMixRate3_AL2",0,path);
	this->m_BloodCastleMixRate3[3] = GetPrivateProfileInt(section,"BloodCastleMixRate3_AL3",0,path);

	this->m_BloodCastleMixRate4[0] = GetPrivateProfileInt(section,"BloodCastleMixRate4_AL0",0,path);
	this->m_BloodCastleMixRate4[1] = GetPrivateProfileInt(section,"BloodCastleMixRate4_AL1",0,path);
	this->m_BloodCastleMixRate4[2] = GetPrivateProfileInt(section,"BloodCastleMixRate4_AL2",0,path);
	this->m_BloodCastleMixRate4[3] = GetPrivateProfileInt(section,"BloodCastleMixRate4_AL3",0,path);

	this->m_BloodCastleMixRate5[0] = GetPrivateProfileInt(section,"BloodCastleMixRate5_AL0",0,path);
	this->m_BloodCastleMixRate5[1] = GetPrivateProfileInt(section,"BloodCastleMixRate5_AL1",0,path);
	this->m_BloodCastleMixRate5[2] = GetPrivateProfileInt(section,"BloodCastleMixRate5_AL2",0,path);
	this->m_BloodCastleMixRate5[3] = GetPrivateProfileInt(section,"BloodCastleMixRate5_AL3",0,path);

	this->m_BloodCastleMixRate6[0] = GetPrivateProfileInt(section,"BloodCastleMixRate6_AL0",0,path);
	this->m_BloodCastleMixRate6[1] = GetPrivateProfileInt(section,"BloodCastleMixRate6_AL1",0,path);
	this->m_BloodCastleMixRate6[2] = GetPrivateProfileInt(section,"BloodCastleMixRate6_AL2",0,path);
	this->m_BloodCastleMixRate6[3] = GetPrivateProfileInt(section,"BloodCastleMixRate6_AL3",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_BloodCastleMixRate7[0] = GetPrivateProfileInt(section,"BloodCastleMixRate7_AL0",0,path);
	this->m_BloodCastleMixRate7[1] = GetPrivateProfileInt(section,"BloodCastleMixRate7_AL1",0,path);
	this->m_BloodCastleMixRate7[2] = GetPrivateProfileInt(section,"BloodCastleMixRate7_AL2",0,path);
	this->m_BloodCastleMixRate7[3] = GetPrivateProfileInt(section,"BloodCastleMixRate7_AL3",0,path);
	#endif

	this->m_Wing1MixRate[0] = GetPrivateProfileInt(section,"Wing1MixRate_AL0",0,path);
	this->m_Wing1MixRate[1] = GetPrivateProfileInt(section,"Wing1MixRate_AL1",0,path);
	this->m_Wing1MixRate[2] = GetPrivateProfileInt(section,"Wing1MixRate_AL2",0,path);
	this->m_Wing1MixRate[3] = GetPrivateProfileInt(section,"Wing1MixRate_AL3",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_PetMixRate[0] = GetPrivateProfileInt(section,"PetMixRate_AL0",0,path);
	this->m_PetMixRate[1] = GetPrivateProfileInt(section,"PetMixRate_AL1",0,path);
	this->m_PetMixRate[2] = GetPrivateProfileInt(section,"PetMixRate_AL2",0,path);
	this->m_PetMixRate[3] = GetPrivateProfileInt(section,"PetMixRate_AL3",0,path);
	#endif
}

void CServerInfo::ReadCharacterInfo(char* section,char* path) // OK
{
	this->m_CalcLuaScriptSwitch = GetPrivateProfileInt(section,"CalcLuaScriptSwitch",0,path);

	this->m_DuelDamageRate = GetPrivateProfileInt(section,"DuelDamageRate",0,path);

	this->m_CustomArenaDamageRate = GetPrivateProfileInt(section,"CustomArenaDamageRate",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_ChaosCastleDamageRate = GetPrivateProfileInt(section,"ChaosCastleDamageRate",0,path);
	#endif

	this->m_GeneralDamageRatePvP = GetPrivateProfileInt(section,"GeneralDamageRatePvP",0,path);
	this->m_GeneralDamageRatePvM = GetPrivateProfileInt(section,"GeneralDamageRatePvM",0,path);

	this->m_ReflectDamageRatePvP = GetPrivateProfileInt(section,"ReflectDamageRatePvP",0,path);
	this->m_ReflectDamageRatePvM = GetPrivateProfileInt(section,"ReflectDamageRatePvM",0,path);

	this->m_DamageRatePvP[CLASS_DW] = GetPrivateProfileInt(section,"DWDamageRatePvP",0,path);
	this->m_DamageRatePvP[CLASS_DK] = GetPrivateProfileInt(section,"DKDamageRatePvP",0,path);
	this->m_DamageRatePvP[CLASS_FE] = GetPrivateProfileInt(section,"FEDamageRatePvP",0,path);
	this->m_DamageRatePvP[CLASS_MG] = GetPrivateProfileInt(section,"MGDamageRatePvP",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_DamageRatePvP[CLASS_DL] = GetPrivateProfileInt(section,"DLDamageRatePvP",0,path);
	#endif

	this->m_DamageRatePvM[CLASS_DW] = GetPrivateProfileInt(section,"DWDamageRatePvM",0,path);
	this->m_DamageRatePvM[CLASS_DK] = GetPrivateProfileInt(section,"DKDamageRatePvM",0,path);
	this->m_DamageRatePvM[CLASS_FE] = GetPrivateProfileInt(section,"FEDamageRatePvM",0,path);
	this->m_DamageRatePvM[CLASS_MG] = GetPrivateProfileInt(section,"MGDamageRatePvM",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_DamageRatePvM[CLASS_DL] = GetPrivateProfileInt(section,"DLDamageRatePvM",0,path);
	#endif

	this->m_DamageRateTo[CLASS_DW][CLASS_DW] = GetPrivateProfileInt(section,"DWDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_DW][CLASS_DK] = GetPrivateProfileInt(section,"DWDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_DW][CLASS_FE] = GetPrivateProfileInt(section,"DWDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_DW][CLASS_MG] = GetPrivateProfileInt(section,"DWDamageRateToMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_DamageRateTo[CLASS_DW][CLASS_DL] = GetPrivateProfileInt(section,"DWDamageRateToDL",0,path);
	#endif

	this->m_DamageRateTo[CLASS_DK][CLASS_DW] = GetPrivateProfileInt(section,"DKDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_DK][CLASS_DK] = GetPrivateProfileInt(section,"DKDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_DK][CLASS_FE] = GetPrivateProfileInt(section,"DKDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_DK][CLASS_MG] = GetPrivateProfileInt(section,"DKDamageRateToMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_DamageRateTo[CLASS_DK][CLASS_DL] = GetPrivateProfileInt(section,"DKDamageRateToDL",0,path);
	#endif

	this->m_DamageRateTo[CLASS_FE][CLASS_DW] = GetPrivateProfileInt(section,"FEDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_FE][CLASS_DK] = GetPrivateProfileInt(section,"FEDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_FE][CLASS_FE] = GetPrivateProfileInt(section,"FEDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_FE][CLASS_MG] = GetPrivateProfileInt(section,"FEDamageRateToMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_DamageRateTo[CLASS_FE][CLASS_DL] = GetPrivateProfileInt(section,"FEDamageRateToDL",0,path);
	#endif

	this->m_DamageRateTo[CLASS_MG][CLASS_DW] = GetPrivateProfileInt(section,"MGDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_MG][CLASS_DK] = GetPrivateProfileInt(section,"MGDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_MG][CLASS_FE] = GetPrivateProfileInt(section,"MGDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_MG][CLASS_MG] = GetPrivateProfileInt(section,"MGDamageRateToMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_DamageRateTo[CLASS_MG][CLASS_DL] = GetPrivateProfileInt(section,"MGDamageRateToDL",0,path);

	this->m_DamageRateTo[CLASS_DL][CLASS_DW] = GetPrivateProfileInt(section,"DLDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_DL][CLASS_DK] = GetPrivateProfileInt(section,"DLDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_DL][CLASS_FE] = GetPrivateProfileInt(section,"DLDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_DL][CLASS_MG] = GetPrivateProfileInt(section,"DLDamageRateToMG",0,path);
	this->m_DamageRateTo[CLASS_DL][CLASS_DL] = GetPrivateProfileInt(section,"DLDamageRateToDL",0,path);
	#endif

	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_DW] = GetPrivateProfileInt(section,"DWReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_DK] = GetPrivateProfileInt(section,"DWReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_FE] = GetPrivateProfileInt(section,"DWReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_MG] = GetPrivateProfileInt(section,"DWReflectDamageRateToMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_DL] = GetPrivateProfileInt(section,"DWReflectDamageRateToDL",0,path);
	#endif

	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_DW] = GetPrivateProfileInt(section,"DKReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_DK] = GetPrivateProfileInt(section,"DKReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_FE] = GetPrivateProfileInt(section,"DKReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_MG] = GetPrivateProfileInt(section,"DKReflectDamageRateToMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_DL] = GetPrivateProfileInt(section,"DKReflectDamageRateToDL",0,path);
	#endif

	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_DW] = GetPrivateProfileInt(section,"FEReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_DK] = GetPrivateProfileInt(section,"FEReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_FE] = GetPrivateProfileInt(section,"FEReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_MG] = GetPrivateProfileInt(section,"FEReflectDamageRateToMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_DL] = GetPrivateProfileInt(section,"FEReflectDamageRateToDL",0,path);
	#endif

	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_DW] = GetPrivateProfileInt(section,"MGReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_DK] = GetPrivateProfileInt(section,"MGReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_FE] = GetPrivateProfileInt(section,"MGReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_MG] = GetPrivateProfileInt(section,"MGReflectDamageRateToMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_DL] = GetPrivateProfileInt(section,"MGReflectDamageRateToDL",0,path);

	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_DW] = GetPrivateProfileInt(section,"DLReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_DK] = GetPrivateProfileInt(section,"DLReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_FE] = GetPrivateProfileInt(section,"DLReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_MG] = GetPrivateProfileInt(section,"DLReflectDamageRateToMG",0,path);
	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_DL] = GetPrivateProfileInt(section,"DLReflectDamageRateToDL",0,path);
	#endif

	this->m_DamageStuckRate[CLASS_DW] = GetPrivateProfileInt(section,"DWDamageStuckRate",0,path);
	this->m_DamageStuckRate[CLASS_DK] = GetPrivateProfileInt(section,"DKDamageStuckRate",0,path);
	this->m_DamageStuckRate[CLASS_FE] = GetPrivateProfileInt(section,"FEDamageStuckRate",0,path);
	this->m_DamageStuckRate[CLASS_MG] = GetPrivateProfileInt(section,"MGDamageStuckRate",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_DamageStuckRate[CLASS_DL] = GetPrivateProfileInt(section,"DLDamageStuckRate",0,path);
	#endif

	this->m_DamageStuckOnPetUniria = GetPrivateProfileInt(section,"DamageStuckOnPetUniria",0,path);
	this->m_DamageStuckOnPetDinorant = GetPrivateProfileInt(section,"DamageStuckOnPetDinorant",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_DamageStuckOnPetDarkHorse = GetPrivateProfileInt(section,"DamageStuckOnPetDarkHorse",0,path);
	#endif

	this->m_DKDamageMultiplierConstA = GetPrivateProfileInt(section,"DKDamageMultiplierConstA",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_DLDamageMultiplierConstA = GetPrivateProfileInt(section,"DLDamageMultiplierConstA",0,path);
	#endif

	this->m_DKDamageMultiplierMaxRate = GetPrivateProfileInt(section,"DKDamageMultiplierMaxRate",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_DLDamageMultiplierMaxRate = GetPrivateProfileInt(section,"DLDamageMultiplierMaxRate",0,path);

	this->m_DarkSpiritRangeAttackRate = GetPrivateProfileInt(section,"DarkSpiritRangeAttackRate",0,path);

	this->m_DarkSpiritCriticalDamageRate = GetPrivateProfileInt(section,"DarkSpiritCriticalDamageRate",0,path);

	this->m_DarkSpiritExcellentDamageRate = GetPrivateProfileInt(section,"DarkSpiritExcellentDamageRate",0,path);

	this->m_DarkSpiritAttackDamageMinConstA = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMinConstA",0,path);
	this->m_DarkSpiritAttackDamageMinConstB = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMinConstB",0,path);
	this->m_DarkSpiritAttackDamageMinConstC = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMinConstC",0,path);

	this->m_DarkSpiritAttackDamageMaxConstA = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMaxConstA",0,path);
	this->m_DarkSpiritAttackDamageMaxConstB = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMaxConstB",0,path);
	this->m_DarkSpiritAttackDamageMaxConstC = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMaxConstC",0,path);

	this->m_DarkSpiritAttackSpeedConstA = GetPrivateProfileInt(section,"DarkSpiritAttackSpeedConstA",0,path);
	this->m_DarkSpiritAttackSpeedConstB = GetPrivateProfileInt(section,"DarkSpiritAttackSpeedConstB",0,path);
	this->m_DarkSpiritAttackSpeedConstC = GetPrivateProfileInt(section,"DarkSpiritAttackSpeedConstC",0,path);
	this->m_DarkSpiritAttackSpeedConstD = GetPrivateProfileInt(section,"DarkSpiritAttackSpeedConstD",0,path);

	this->m_DarkSpiritAttackSuccessRateConstA = GetPrivateProfileInt(section,"DarkSpiritAttackSuccessRateConstA",0,path);
	this->m_DarkSpiritAttackSuccessRateConstB = GetPrivateProfileInt(section,"DarkSpiritAttackSuccessRateConstB",0,path);
	this->m_DarkSpiritAttackSuccessRateConstC = GetPrivateProfileInt(section,"DarkSpiritAttackSuccessRateConstC",0,path);

	this->m_ComboDamageConstA[CLASS_DW] = GetPrivateProfileInt(section,"DWComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_DW] = GetPrivateProfileInt(section,"DWComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_DW] = GetPrivateProfileInt(section,"DWComboDamageConstC",0,path);
	this->m_ComboDamageConstA[CLASS_DK] = GetPrivateProfileInt(section,"DKComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_DK] = GetPrivateProfileInt(section,"DKComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_DK] = GetPrivateProfileInt(section,"DKComboDamageConstC",0,path);
	this->m_ComboDamageConstA[CLASS_FE] = GetPrivateProfileInt(section,"FEComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_FE] = GetPrivateProfileInt(section,"FEComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_FE] = GetPrivateProfileInt(section,"FEComboDamageConstC",0,path);
	this->m_ComboDamageConstA[CLASS_MG] = GetPrivateProfileInt(section,"MGComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_MG] = GetPrivateProfileInt(section,"MGComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_MG] = GetPrivateProfileInt(section,"MGComboDamageConstC",0,path);
	this->m_ComboDamageConstA[CLASS_DL] = GetPrivateProfileInt(section,"DLComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_DL] = GetPrivateProfileInt(section,"DLComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_DL] = GetPrivateProfileInt(section,"DLComboDamageConstC",0,path);

	this->m_EarthquakeDamageConstA = GetPrivateProfileInt(section,"EarthquakeDamageConstA",0,path);
	this->m_EarthquakeDamageConstB = GetPrivateProfileInt(section,"EarthquakeDamageConstB",0,path);
	this->m_EarthquakeDamageConstC = GetPrivateProfileInt(section,"EarthquakeDamageConstC",0,path);

	this->m_ElectricSparkDamageConstA = GetPrivateProfileInt(section,"ElectricSparkDamageConstA",0,path);
	this->m_ElectricSparkDamageConstB = GetPrivateProfileInt(section,"ElectricSparkDamageConstB",0,path);

	this->m_DLSkillDamageConstA = GetPrivateProfileInt(section,"DLSkillDamageConstA",0,path);
	this->m_DLSkillDamageConstB = GetPrivateProfileInt(section,"DLSkillDamageConstB",0,path);

	this->m_NovaDamageConstA = GetPrivateProfileInt(section,"NovaDamageConstA",0,path);
	this->m_NovaDamageConstB = GetPrivateProfileInt(section,"NovaDamageConstB",0,path);
	this->m_NovaDamageConstC = GetPrivateProfileInt(section,"NovaDamageConstC",0,path);
	#endif

	this->m_HPRecoveryRate[CLASS_DW] = GetPrivateProfileInt(section,"DWHPRecoveryRate",0,path);
	this->m_HPRecoveryRate[CLASS_DK] = GetPrivateProfileInt(section,"DKHPRecoveryRate",0,path);
	this->m_HPRecoveryRate[CLASS_FE] = GetPrivateProfileInt(section,"FEHPRecoveryRate",0,path);
	this->m_HPRecoveryRate[CLASS_MG] = GetPrivateProfileInt(section,"MGHPRecoveryRate",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_HPRecoveryRate[CLASS_DL] = GetPrivateProfileInt(section,"DLHPRecoveryRate",0,path);
	#endif

	this->m_MPRecoveryRate[CLASS_DW] = GetPrivateProfileInt(section,"DWMPRecoveryRate",0,path);
	this->m_MPRecoveryRate[CLASS_DK] = GetPrivateProfileInt(section,"DKMPRecoveryRate",0,path);
	this->m_MPRecoveryRate[CLASS_FE] = GetPrivateProfileInt(section,"FEMPRecoveryRate",0,path);
	this->m_MPRecoveryRate[CLASS_MG] = GetPrivateProfileInt(section,"MGMPRecoveryRate",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_MPRecoveryRate[CLASS_DL] = GetPrivateProfileInt(section,"DLMPRecoveryRate",0,path);
	#endif

	this->m_BPRecoveryRate[CLASS_DW] = GetPrivateProfileInt(section,"DWBPRecoveryRate",0,path);
	this->m_BPRecoveryRate[CLASS_DK] = GetPrivateProfileInt(section,"DKBPRecoveryRate",0,path);
	this->m_BPRecoveryRate[CLASS_FE] = GetPrivateProfileInt(section,"FEBPRecoveryRate",0,path);
	this->m_BPRecoveryRate[CLASS_MG] = GetPrivateProfileInt(section,"MGBPRecoveryRate",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_BPRecoveryRate[CLASS_DL] = GetPrivateProfileInt(section,"DLBPRecoveryRate",0,path);
	#endif

	this->m_DWPhysiDamageMinConstA = GetPrivateProfileInt(section,"DWPhysiDamageMinConstA",0,path);
	this->m_DWPhysiDamageMaxConstA = GetPrivateProfileInt(section,"DWPhysiDamageMaxConstA",0,path);
	
	this->m_DWMagicDamageMinConstA = GetPrivateProfileInt(section,"DWMagicDamageMinConstA",0,path);
	this->m_DWMagicDamageMaxConstA = GetPrivateProfileInt(section,"DWMagicDamageMaxConstA",0,path);

	this->m_DKPhysiDamageMinConstA = GetPrivateProfileInt(section,"DKPhysiDamageMinConstA",0,path);
	this->m_DKPhysiDamageMaxConstA = GetPrivateProfileInt(section,"DKPhysiDamageMaxConstA",0,path);
	
	this->m_DKMagicDamageMinConstA = GetPrivateProfileInt(section,"DKMagicDamageMinConstA",0,path);
	this->m_DKMagicDamageMaxConstA = GetPrivateProfileInt(section,"DKMagicDamageMaxConstA",0,path);

	this->m_FEPhysiDamageMinConstA = GetPrivateProfileInt(section,"FEPhysiDamageMinConstA",0,path);
	this->m_FEPhysiDamageMaxConstA = GetPrivateProfileInt(section,"FEPhysiDamageMaxConstA",0,path);

	this->m_FEPhysiDamageMinBowConstA = GetPrivateProfileInt(section,"FEPhysiDamageMinBowConstA",0,path);
	this->m_FEPhysiDamageMinBowConstB = GetPrivateProfileInt(section,"FEPhysiDamageMinBowConstB",0,path);
	
	this->m_FEPhysiDamageMaxBowConstA = GetPrivateProfileInt(section,"FEPhysiDamageMaxBowConstA",0,path);
	this->m_FEPhysiDamageMaxBowConstB = GetPrivateProfileInt(section,"FEPhysiDamageMaxBowConstB",0,path);

	this->m_FEMagicDamageMinConstA = GetPrivateProfileInt(section,"FEMagicDamageMinConstA",0,path);
	this->m_FEMagicDamageMaxConstA = GetPrivateProfileInt(section,"FEMagicDamageMaxConstA",0,path);

	this->m_MGPhysiDamageMinConstA = GetPrivateProfileInt(section,"MGPhysiDamageMinConstA",0,path);
	this->m_MGPhysiDamageMinConstB = GetPrivateProfileInt(section,"MGPhysiDamageMinConstB",0,path);

	this->m_MGPhysiDamageMaxConstA = GetPrivateProfileInt(section,"MGPhysiDamageMaxConstA",0,path);
	this->m_MGPhysiDamageMaxConstB = GetPrivateProfileInt(section,"MGPhysiDamageMaxConstB",0,path);

	this->m_MGMagicDamageMinConstA = GetPrivateProfileInt(section,"MGMagicDamageMinConstA",0,path);
	this->m_MGMagicDamageMaxConstA = GetPrivateProfileInt(section,"MGMagicDamageMaxConstA",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_DLPhysiDamageMinConstA = GetPrivateProfileInt(section,"DLPhysiDamageMinConstA",0,path);
	this->m_DLPhysiDamageMinConstB = GetPrivateProfileInt(section,"DLPhysiDamageMinConstB",0,path);

	this->m_DLPhysiDamageMaxConstA = GetPrivateProfileInt(section,"DLPhysiDamageMaxConstA",0,path);
	this->m_DLPhysiDamageMaxConstB = GetPrivateProfileInt(section,"DLPhysiDamageMaxConstB",0,path);

	this->m_DLMagicDamageMinConstA = GetPrivateProfileInt(section,"DLMagicDamageMinConstA",0,path);
	this->m_DLMagicDamageMaxConstA = GetPrivateProfileInt(section,"DLMagicDamageMaxConstA",0,path);
	#endif

	this->m_DWAttackSuccessRateConstA = GetPrivateProfileInt(section,"DWAttackSuccessRateConstA",0,path);
	this->m_DWAttackSuccessRateConstB = GetPrivateProfileInt(section,"DWAttackSuccessRateConstB",0,path);
	this->m_DWAttackSuccessRateConstC = GetPrivateProfileInt(section,"DWAttackSuccessRateConstC",0,path);
	this->m_DWAttackSuccessRateConstD = GetPrivateProfileInt(section,"DWAttackSuccessRateConstD",0,path);

	this->m_DKAttackSuccessRateConstA = GetPrivateProfileInt(section,"DKAttackSuccessRateConstA",0,path);
	this->m_DKAttackSuccessRateConstB = GetPrivateProfileInt(section,"DKAttackSuccessRateConstB",0,path);
	this->m_DKAttackSuccessRateConstC = GetPrivateProfileInt(section,"DKAttackSuccessRateConstC",0,path);
	this->m_DKAttackSuccessRateConstD = GetPrivateProfileInt(section,"DKAttackSuccessRateConstD",0,path);

	this->m_FEAttackSuccessRateConstA = GetPrivateProfileInt(section,"FEAttackSuccessRateConstA",0,path);
	this->m_FEAttackSuccessRateConstB = GetPrivateProfileInt(section,"FEAttackSuccessRateConstB",0,path);
	this->m_FEAttackSuccessRateConstC = GetPrivateProfileInt(section,"FEAttackSuccessRateConstC",0,path);
	this->m_FEAttackSuccessRateConstD = GetPrivateProfileInt(section,"FEAttackSuccessRateConstD",0,path);

	this->m_MGAttackSuccessRateConstA = GetPrivateProfileInt(section,"MGAttackSuccessRateConstA",0,path);
	this->m_MGAttackSuccessRateConstB = GetPrivateProfileInt(section,"MGAttackSuccessRateConstB",0,path);
	this->m_MGAttackSuccessRateConstC = GetPrivateProfileInt(section,"MGAttackSuccessRateConstC",0,path);
	this->m_MGAttackSuccessRateConstD = GetPrivateProfileInt(section,"MGAttackSuccessRateConstD",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_DLAttackSuccessRateConstA = GetPrivateProfileInt(section,"DLAttackSuccessRateConstA",0,path);
	this->m_DLAttackSuccessRateConstB = GetPrivateProfileInt(section,"DLAttackSuccessRateConstB",0,path);
	this->m_DLAttackSuccessRateConstC = GetPrivateProfileInt(section,"DLAttackSuccessRateConstC",0,path);
	this->m_DLAttackSuccessRateConstD = GetPrivateProfileInt(section,"DLAttackSuccessRateConstD",0,path);
	this->m_DLAttackSuccessRateConstE = GetPrivateProfileInt(section,"DLAttackSuccessRateConstE",0,path);
	#endif

	this->m_DWPhysiSpeedConstA = GetPrivateProfileInt(section,"DWPhysiSpeedConstA",0,path);
	this->m_DWMagicSpeedConstA = GetPrivateProfileInt(section,"DWMagicSpeedConstA",0,path);

	this->m_DKPhysiSpeedConstA = GetPrivateProfileInt(section,"DKPhysiSpeedConstA",0,path);
	this->m_DKMagicSpeedConstA = GetPrivateProfileInt(section,"DKMagicSpeedConstA",0,path);

	this->m_FEPhysiSpeedConstA = GetPrivateProfileInt(section,"FEPhysiSpeedConstA",0,path);
	this->m_FEMagicSpeedConstA = GetPrivateProfileInt(section,"FEMagicSpeedConstA",0,path);

	this->m_MGPhysiSpeedConstA = GetPrivateProfileInt(section,"MGPhysiSpeedConstA",0,path);
	this->m_MGMagicSpeedConstA = GetPrivateProfileInt(section,"MGMagicSpeedConstA",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_DLPhysiSpeedConstA = GetPrivateProfileInt(section,"DLPhysiSpeedConstA",0,path);
	this->m_DLMagicSpeedConstA = GetPrivateProfileInt(section,"DLMagicSpeedConstA",0,path);
	#endif

	this->m_DWDefenseSuccessRateConstA = GetPrivateProfileInt(section,"DWDefenseSuccessRateConstA",0,path);
	this->m_DKDefenseSuccessRateConstA = GetPrivateProfileInt(section,"DKDefenseSuccessRateConstA",0,path);

	this->m_FEDefenseSuccessRateConstA = GetPrivateProfileInt(section,"FEDefenseSuccessRateConstA",0,path);

	this->m_MGDefenseSuccessRateConstA = GetPrivateProfileInt(section,"MGDefenseSuccessRateConstA",0,path);

	this->m_DLDefenseSuccessRateConstA = GetPrivateProfileInt(section,"DLDefenseSuccessRateConstA",0,path);

	this->m_DWDefenseConstA = GetPrivateProfileInt(section,"DWDefenseConstA",0,path);
	this->m_DKDefenseConstA = GetPrivateProfileInt(section,"DKDefenseConstA",0,path);
	this->m_FEDefenseConstA = GetPrivateProfileInt(section,"FEDefenseConstA",0,path);
	this->m_MGDefenseConstA = GetPrivateProfileInt(section,"MGDefenseConstA",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_DLDefenseConstA = GetPrivateProfileInt(section,"DLDefenseConstA",0,path);
	#endif
}

void CServerInfo::ReadCommandInfo(char* section,char* path) // OK
{
	this->m_CommandPostGlobal = GetPrivateProfileInt(section,"CommandPostGlobal",0,path);

	this->m_CommandPostType = GetPrivateProfileInt(section,"CommandPostType",0,path);

	// Update 89 2.4.7 -> 97K - Comando /post personalizavel
	this->m_CommandPostMessage = GetPrivateProfileInt(section,"CommandPostMessage",323,path);

	this->m_CommandPKClearType = GetPrivateProfileInt(section,"CommandPKClearType",0,path);
	this->m_CommandPKClearMoney[0] = GetPrivateProfileInt(section,"CommandPKClearMoney_AL0",0,path);
	this->m_CommandPKClearMoney[1] = GetPrivateProfileInt(section,"CommandPKClearMoney_AL1",0,path);
	this->m_CommandPKClearMoney[2] = GetPrivateProfileInt(section,"CommandPKClearMoney_AL2",0,path);
	this->m_CommandPKClearMoney[3] = GetPrivateProfileInt(section,"CommandPKClearMoney_AL3",0,path);

	this->m_CommandAddPointAutoEnable[0] = GetPrivateProfileInt(section,"CommandAddPointAutoEnable_AL0",0,path);
	this->m_CommandAddPointAutoEnable[1] = GetPrivateProfileInt(section,"CommandAddPointAutoEnable_AL1",0,path);
	this->m_CommandAddPointAutoEnable[2] = GetPrivateProfileInt(section,"CommandAddPointAutoEnable_AL2",0,path);
	this->m_CommandAddPointAutoEnable[3] = GetPrivateProfileInt(section,"CommandAddPointAutoEnable_AL3",0,path);

	this->m_CommandChangeLimit[0] = GetPrivateProfileInt(section,"CommandChangeLimit_AL0",0,path);
	this->m_CommandChangeLimit[1] = GetPrivateProfileInt(section,"CommandChangeLimit_AL1",0,path);
	this->m_CommandChangeLimit[2] = GetPrivateProfileInt(section,"CommandChangeLimit_AL2",0,path);
	this->m_CommandChangeLimit[3] = GetPrivateProfileInt(section,"CommandChangeLimit_AL3",0,path);

	this->m_CommandWareNumber[0] = GetPrivateProfileInt(section,"CommandWareNumber_AL0",0,path);
	this->m_CommandWareNumber[1] = GetPrivateProfileInt(section,"CommandWareNumber_AL1",0,path);
	this->m_CommandWareNumber[2] = GetPrivateProfileInt(section,"CommandWareNumber_AL2",0,path);
	this->m_CommandWareNumber[3] = GetPrivateProfileInt(section,"CommandWareNumber_AL3",0,path);

	this->m_CommandResetType = GetPrivateProfileInt(section,"CommandResetType",0,path);

	this->m_CommandResetKeepStrength = GetPrivateProfileInt(section,"CommandResetKeepStrength",0,path);
	this->m_CommandResetKeepDexterity = GetPrivateProfileInt(section,"CommandResetKeepDexterity",0,path);
	this->m_CommandResetKeepVitality = GetPrivateProfileInt(section,"CommandResetKeepVitality",0,path);
	this->m_CommandResetKeepEnergy = GetPrivateProfileInt(section,"CommandResetKeepEnergy",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_CommandResetKeepLeadership = GetPrivateProfileInt(section,"CommandResetKeepLeadership",0,path);
	#endif

	this->m_CommandResetAutoEnable[0] = GetPrivateProfileInt(section,"CommandResetAutoEnable_AL0",0,path);
	this->m_CommandResetAutoEnable[1] = GetPrivateProfileInt(section,"CommandResetAutoEnable_AL1",0,path);
	this->m_CommandResetAutoEnable[2] = GetPrivateProfileInt(section,"CommandResetAutoEnable_AL2",0,path);
	this->m_CommandResetAutoEnable[3] = GetPrivateProfileInt(section,"CommandResetAutoEnable_AL3",0,path);

	this->m_CommandResetCheckItem[0] = GetPrivateProfileInt(section,"CommandResetCheckItem_AL0",0,path);
	this->m_CommandResetCheckItem[1] = GetPrivateProfileInt(section,"CommandResetCheckItem_AL1",0,path);
	this->m_CommandResetCheckItem[2] = GetPrivateProfileInt(section,"CommandResetCheckItem_AL2",0,path);
	this->m_CommandResetCheckItem[3] = GetPrivateProfileInt(section,"CommandResetCheckItem_AL3",0,path);

	this->m_CommandResetMove[0] = GetPrivateProfileInt(section,"CommandResetMove_AL0",0,path);
	this->m_CommandResetMove[1] = GetPrivateProfileInt(section,"CommandResetMove_AL1",0,path);
	this->m_CommandResetMove[2] = GetPrivateProfileInt(section,"CommandResetMove_AL2",0,path);
	this->m_CommandResetMove[3] = GetPrivateProfileInt(section,"CommandResetMove_AL3",0,path);

	this->m_CommandResetClearQuest[0] = GetPrivateProfileInt(section,"CommandResetClearQuest_AL0",0,path);
	this->m_CommandResetClearQuest[1] = GetPrivateProfileInt(section,"CommandResetClearQuest_AL1",0,path);
	this->m_CommandResetClearQuest[2] = GetPrivateProfileInt(section,"CommandResetClearQuest_AL2",0,path);
	this->m_CommandResetClearQuest[3] = GetPrivateProfileInt(section,"CommandResetClearQuest_AL3",0,path);

	this->m_CommandResetClearSkill[0] = GetPrivateProfileInt(section,"CommandResetClearSkill_AL0",0,path);
	this->m_CommandResetClearSkill[1] = GetPrivateProfileInt(section,"CommandResetClearSkill_AL1",0,path);
	this->m_CommandResetClearSkill[2] = GetPrivateProfileInt(section,"CommandResetClearSkill_AL2",0,path);
	this->m_CommandResetClearSkill[3] = GetPrivateProfileInt(section,"CommandResetClearSkill_AL3",0,path);

	this->m_CommandResetClearParty[0] = GetPrivateProfileInt(section,"CommandResetClearParty_AL0",0,path);
	this->m_CommandResetClearParty[1] = GetPrivateProfileInt(section,"CommandResetClearParty_AL1",0,path);
	this->m_CommandResetClearParty[2] = GetPrivateProfileInt(section,"CommandResetClearParty_AL2",0,path);
	this->m_CommandResetClearParty[3] = GetPrivateProfileInt(section,"CommandResetClearParty_AL3",0,path);

	this->m_CommandResetLevel[0] = GetPrivateProfileInt(section,"CommandResetLevel_AL0",0,path);
	this->m_CommandResetLevel[1] = GetPrivateProfileInt(section,"CommandResetLevel_AL1",0,path);
	this->m_CommandResetLevel[2] = GetPrivateProfileInt(section,"CommandResetLevel_AL2",0,path);
	this->m_CommandResetLevel[3] = GetPrivateProfileInt(section,"CommandResetLevel_AL3",0,path);

	this->m_CommandResetMoney[0] = GetPrivateProfileInt(section,"CommandResetMoney_AL0",0,path);
	this->m_CommandResetMoney[1] = GetPrivateProfileInt(section,"CommandResetMoney_AL1",0,path);
	this->m_CommandResetMoney[2] = GetPrivateProfileInt(section,"CommandResetMoney_AL2",0,path);
	this->m_CommandResetMoney[3] = GetPrivateProfileInt(section,"CommandResetMoney_AL3",0,path);

	this->m_CommandResetCount[0] = GetPrivateProfileInt(section,"CommandResetCount_AL0",0,path);
	this->m_CommandResetCount[1] = GetPrivateProfileInt(section,"CommandResetCount_AL1",0,path);
	this->m_CommandResetCount[2] = GetPrivateProfileInt(section,"CommandResetCount_AL2",0,path);
	this->m_CommandResetCount[3] = GetPrivateProfileInt(section,"CommandResetCount_AL3",0,path);

	this->m_CommandResetLimit[0] = GetPrivateProfileInt(section,"CommandResetLimit_AL0",0,path);
	this->m_CommandResetLimit[1] = GetPrivateProfileInt(section,"CommandResetLimit_AL1",0,path);
	this->m_CommandResetLimit[2] = GetPrivateProfileInt(section,"CommandResetLimit_AL2",0,path);
	this->m_CommandResetLimit[3] = GetPrivateProfileInt(section,"CommandResetLimit_AL3",0,path);

	this->m_CommandResetLimitDay[0] = GetPrivateProfileInt(section,"CommandResetLimitDay_AL0",0,path);
	this->m_CommandResetLimitDay[1] = GetPrivateProfileInt(section,"CommandResetLimitDay_AL1",0,path);
	this->m_CommandResetLimitDay[2] = GetPrivateProfileInt(section,"CommandResetLimitDay_AL2",0,path);
	this->m_CommandResetLimitDay[3] = GetPrivateProfileInt(section,"CommandResetLimitDay_AL3",0,path);

	this->m_CommandResetLimitWek[0] = GetPrivateProfileInt(section,"CommandResetLimitWek_AL0",0,path);
	this->m_CommandResetLimitWek[1] = GetPrivateProfileInt(section,"CommandResetLimitWek_AL1",0,path);
	this->m_CommandResetLimitWek[2] = GetPrivateProfileInt(section,"CommandResetLimitWek_AL2",0,path);
	this->m_CommandResetLimitWek[3] = GetPrivateProfileInt(section,"CommandResetLimitWek_AL3",0,path);

	this->m_CommandResetLimitMon[0] = GetPrivateProfileInt(section,"CommandResetLimitMon_AL0",0,path);
	this->m_CommandResetLimitMon[1] = GetPrivateProfileInt(section,"CommandResetLimitMon_AL1",0,path);
	this->m_CommandResetLimitMon[2] = GetPrivateProfileInt(section,"CommandResetLimitMon_AL2",0,path);
	this->m_CommandResetLimitMon[3] = GetPrivateProfileInt(section,"CommandResetLimitMon_AL3",0,path);

	this->m_CommandResetStartLevel[0] = GetPrivateProfileInt(section,"CommandResetStartLevel_AL0",0,path);
	this->m_CommandResetStartLevel[1] = GetPrivateProfileInt(section,"CommandResetStartLevel_AL1",0,path);
	this->m_CommandResetStartLevel[2] = GetPrivateProfileInt(section,"CommandResetStartLevel_AL2",0,path);
	this->m_CommandResetStartLevel[3] = GetPrivateProfileInt(section,"CommandResetStartLevel_AL3",0,path);

	this->m_CommandResetPoint[0] = GetPrivateProfileInt(section,"CommandResetPoint_AL0",0,path);
	this->m_CommandResetPoint[1] = GetPrivateProfileInt(section,"CommandResetPoint_AL1",0,path);
	this->m_CommandResetPoint[2] = GetPrivateProfileInt(section,"CommandResetPoint_AL2",0,path);
	this->m_CommandResetPoint[3] = GetPrivateProfileInt(section,"CommandResetPoint_AL3",0,path);

	this->m_CommandResetPointRate[CLASS_DW] = GetPrivateProfileInt(section,"CommandResetPointRateDW",0,path);
	this->m_CommandResetPointRate[CLASS_DK] = GetPrivateProfileInt(section,"CommandResetPointRateDK",0,path);
	this->m_CommandResetPointRate[CLASS_FE] = GetPrivateProfileInt(section,"CommandResetPointRateFE",0,path);
	this->m_CommandResetPointRate[CLASS_MG] = GetPrivateProfileInt(section,"CommandResetPointRateMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_CommandResetPointRate[CLASS_DL] = GetPrivateProfileInt(section,"CommandResetPointRateDL",0,path);
	#endif

	this->m_CommandMasterResetType = GetPrivateProfileInt(section,"CommandMasterResetType",0,path);

	this->m_CommandMasterResetKeepStrength = GetPrivateProfileInt(section,"CommandMasterResetKeepStrength",0,path);
	this->m_CommandMasterResetKeepDexterity = GetPrivateProfileInt(section,"CommandMasterResetKeepDexterity",0,path);
	this->m_CommandMasterResetKeepVitality = GetPrivateProfileInt(section,"CommandMasterResetKeepVitality",0,path);
	this->m_CommandMasterResetKeepEnergy = GetPrivateProfileInt(section,"CommandMasterResetKeepEnergy",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_CommandMasterResetKeepLeadership = GetPrivateProfileInt(section,"CommandMasterResetKeepLeadership",0,path);
	#endif

	this->m_CommandMasterResetCheckItem[0] = GetPrivateProfileInt(section,"CommandMasterResetCheckItem_AL0",0,path);
	this->m_CommandMasterResetCheckItem[1] = GetPrivateProfileInt(section,"CommandMasterResetCheckItem_AL1",0,path);
	this->m_CommandMasterResetCheckItem[2] = GetPrivateProfileInt(section,"CommandMasterResetCheckItem_AL2",0,path);
	this->m_CommandMasterResetCheckItem[3] = GetPrivateProfileInt(section,"CommandMasterResetCheckItem_AL3",0,path);

	this->m_CommandMasterResetMove[0] = GetPrivateProfileInt(section,"CommandMasterResetMove_AL0",0,path);
	this->m_CommandMasterResetMove[1] = GetPrivateProfileInt(section,"CommandMasterResetMove_AL1",0,path);
	this->m_CommandMasterResetMove[2] = GetPrivateProfileInt(section,"CommandMasterResetMove_AL2",0,path);
	this->m_CommandMasterResetMove[3] = GetPrivateProfileInt(section,"CommandMasterResetMove_AL3",0,path);

	this->m_CommandMasterResetClearQuest[0] = GetPrivateProfileInt(section,"CommandMasterResetClearQuest_AL0",0,path);
	this->m_CommandMasterResetClearQuest[1] = GetPrivateProfileInt(section,"CommandMasterResetClearQuest_AL1",0,path);
	this->m_CommandMasterResetClearQuest[2] = GetPrivateProfileInt(section,"CommandMasterResetClearQuest_AL2",0,path);
	this->m_CommandMasterResetClearQuest[3] = GetPrivateProfileInt(section,"CommandMasterResetClearQuest_AL3",0,path);

	this->m_CommandMasterResetClearSkill[0] = GetPrivateProfileInt(section,"CommandMasterResetClearSkill_AL0",0,path);
	this->m_CommandMasterResetClearSkill[1] = GetPrivateProfileInt(section,"CommandMasterResetClearSkill_AL1",0,path);
	this->m_CommandMasterResetClearSkill[2] = GetPrivateProfileInt(section,"CommandMasterResetClearSkill_AL2",0,path);
	this->m_CommandMasterResetClearSkill[3] = GetPrivateProfileInt(section,"CommandMasterResetClearSkill_AL3",0,path);

	this->m_CommandMasterResetClearParty[0] = GetPrivateProfileInt(section,"CommandMasterResetClearParty_AL0",0,path);
	this->m_CommandMasterResetClearParty[1] = GetPrivateProfileInt(section,"CommandMasterResetClearParty_AL0",0,path);
	this->m_CommandMasterResetClearParty[2] = GetPrivateProfileInt(section,"CommandMasterResetClearParty_AL0",0,path);
	this->m_CommandMasterResetClearParty[3] = GetPrivateProfileInt(section,"CommandMasterResetClearParty_AL0",0,path);

	this->m_CommandMasterResetLevel[0] = GetPrivateProfileInt(section,"CommandMasterResetLevel_AL0",0,path);
	this->m_CommandMasterResetLevel[1] = GetPrivateProfileInt(section,"CommandMasterResetLevel_AL1",0,path);
	this->m_CommandMasterResetLevel[2] = GetPrivateProfileInt(section,"CommandMasterResetLevel_AL2",0,path);
	this->m_CommandMasterResetLevel[3] = GetPrivateProfileInt(section,"CommandMasterResetLevel_AL3",0,path);

	this->m_CommandMasterResetReset[0] = GetPrivateProfileInt(section,"CommandMasterResetReset_AL0",0,path);
	this->m_CommandMasterResetReset[1] = GetPrivateProfileInt(section,"CommandMasterResetReset_AL1",0,path);
	this->m_CommandMasterResetReset[2] = GetPrivateProfileInt(section,"CommandMasterResetReset_AL2",0,path);
	this->m_CommandMasterResetReset[3] = GetPrivateProfileInt(section,"CommandMasterResetReset_AL3",0,path);

	this->m_CommandMasterResetMoney[0] = GetPrivateProfileInt(section,"CommandMasterResetMoney_AL0",0,path);
	this->m_CommandMasterResetMoney[1] = GetPrivateProfileInt(section,"CommandMasterResetMoney_AL1",0,path);
	this->m_CommandMasterResetMoney[2] = GetPrivateProfileInt(section,"CommandMasterResetMoney_AL2",0,path);
	this->m_CommandMasterResetMoney[3] = GetPrivateProfileInt(section,"CommandMasterResetMoney_AL3",0,path);

	this->m_CommandMasterResetCount[0] = GetPrivateProfileInt(section,"CommandMasterResetCount_AL0",0,path);
	this->m_CommandMasterResetCount[1] = GetPrivateProfileInt(section,"CommandMasterResetCount_AL1",0,path);
	this->m_CommandMasterResetCount[2] = GetPrivateProfileInt(section,"CommandMasterResetCount_AL2",0,path);
	this->m_CommandMasterResetCount[3] = GetPrivateProfileInt(section,"CommandMasterResetCount_AL3",0,path);

	this->m_CommandMasterResetLimit[0] = GetPrivateProfileInt(section,"CommandMasterResetLimit_AL0",0,path);
	this->m_CommandMasterResetLimit[1] = GetPrivateProfileInt(section,"CommandMasterResetLimit_AL1",0,path);
	this->m_CommandMasterResetLimit[2] = GetPrivateProfileInt(section,"CommandMasterResetLimit_AL2",0,path);
	this->m_CommandMasterResetLimit[3] = GetPrivateProfileInt(section,"CommandMasterResetLimit_AL3",0,path);

	this->m_CommandMasterResetLimitDay[0] = GetPrivateProfileInt(section,"CommandMasterResetLimitDay_AL0",0,path);
	this->m_CommandMasterResetLimitDay[1] = GetPrivateProfileInt(section,"CommandMasterResetLimitDay_AL1",0,path);
	this->m_CommandMasterResetLimitDay[2] = GetPrivateProfileInt(section,"CommandMasterResetLimitDay_AL2",0,path);
	this->m_CommandMasterResetLimitDay[3] = GetPrivateProfileInt(section,"CommandMasterResetLimitDay_AL3",0,path);

	this->m_CommandMasterResetLimitWek[0] = GetPrivateProfileInt(section,"CommandMasterResetLimitWek_AL0",0,path);
	this->m_CommandMasterResetLimitWek[1] = GetPrivateProfileInt(section,"CommandMasterResetLimitWek_AL1",0,path);
	this->m_CommandMasterResetLimitWek[2] = GetPrivateProfileInt(section,"CommandMasterResetLimitWek_AL2",0,path);
	this->m_CommandMasterResetLimitWek[3] = GetPrivateProfileInt(section,"CommandMasterResetLimitWek_AL3",0,path);

	this->m_CommandMasterResetLimitMon[0] = GetPrivateProfileInt(section,"CommandMasterResetLimitMon_AL0",0,path);
	this->m_CommandMasterResetLimitMon[1] = GetPrivateProfileInt(section,"CommandMasterResetLimitMon_AL1",0,path);
	this->m_CommandMasterResetLimitMon[2] = GetPrivateProfileInt(section,"CommandMasterResetLimitMon_AL2",0,path);
	this->m_CommandMasterResetLimitMon[3] = GetPrivateProfileInt(section,"CommandMasterResetLimitMon_AL3",0,path);

	this->m_CommandMasterResetStartLevel[0] = GetPrivateProfileInt(section,"CommandMasterResetStartLevel_AL0",0,path);
	this->m_CommandMasterResetStartLevel[1] = GetPrivateProfileInt(section,"CommandMasterResetStartLevel_AL1",0,path);
	this->m_CommandMasterResetStartLevel[2] = GetPrivateProfileInt(section,"CommandMasterResetStartLevel_AL2",0,path);
	this->m_CommandMasterResetStartLevel[3] = GetPrivateProfileInt(section,"CommandMasterResetStartLevel_AL3",0,path);

	this->m_CommandMasterResetStartReset[0] = GetPrivateProfileInt(section,"CommandMasterResetStartReset_AL0",0,path);
	this->m_CommandMasterResetStartReset[1] = GetPrivateProfileInt(section,"CommandMasterResetStartReset_AL1",0,path);
	this->m_CommandMasterResetStartReset[2] = GetPrivateProfileInt(section,"CommandMasterResetStartReset_AL2",0,path);
	this->m_CommandMasterResetStartReset[3] = GetPrivateProfileInt(section,"CommandMasterResetStartReset_AL3",0,path);

	this->m_CommandMasterResetPoint[0] = GetPrivateProfileInt(section,"CommandMasterResetPoint_AL0",0,path);
	this->m_CommandMasterResetPoint[1] = GetPrivateProfileInt(section,"CommandMasterResetPoint_AL1",0,path);
	this->m_CommandMasterResetPoint[2] = GetPrivateProfileInt(section,"CommandMasterResetPoint_AL2",0,path);
	this->m_CommandMasterResetPoint[3] = GetPrivateProfileInt(section,"CommandMasterResetPoint_AL3",0,path);

	this->m_CommandMasterResetPointRate[CLASS_DW] = GetPrivateProfileInt(section,"CommandMasterResetPointRateDW",0,path);
	this->m_CommandMasterResetPointRate[CLASS_DK] = GetPrivateProfileInt(section,"CommandMasterResetPointRateDK",0,path);
	this->m_CommandMasterResetPointRate[CLASS_FE] = GetPrivateProfileInt(section,"CommandMasterResetPointRateFE",0,path);
	this->m_CommandMasterResetPointRate[CLASS_MG] = GetPrivateProfileInt(section,"CommandMasterResetPointRateMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_CommandMasterResetPointRate[CLASS_DL] = GetPrivateProfileInt(section,"CommandMasterResetPointRateDL",0,path);
	#endif
}

void CServerInfo::ReadCommonInfo(char* section,char* path) // OK
{
	this->m_ServerMaxUserNumber = GetPrivateProfileInt(section,"ServerMaxUserNumber",0,path);
	this->m_ServerMaxUserNumber = ((this->m_ServerMaxUserNumber>gGameServerMaxAccount[4]) ? gGameServerMaxAccount[4] : this->m_ServerMaxUserNumber);

	this->m_MaxConnectionPerIP = GetPrivateProfileInt(section,"MaxConnectionPerIP",0,path);
	this->m_MaxConnectionPerHID = GetPrivateProfileInt(section,"MaxConnectionPerHID",0,path);

	this->m_ChaosMixPlusItemAnnounce = GetPrivateProfileInt(section,"ChaosMixPlusItemAnnounce",0,path);
	this->m_MaxItemOption = GetPrivateProfileInt(section,"MaxItemOption",0,path);
	this->m_PersonalCodeCheck = GetPrivateProfileInt(section,"PersonalCodeCheck",0,path);
	this->m_ConnectMemberCheck = GetPrivateProfileInt(section,"ConnectMemberCheck",0,path);
	this->m_TeleportAttackCheck = GetPrivateProfileInt(section,"TeleportAttackCheck",0,path);
	this->m_EffectOverwriteMode = GetPrivateProfileInt(section,"EffectOverwriteMode",0,path);
	this->m_SelfDefenseTime = GetPrivateProfileInt(section,"SelfDefenseTime",0,path);

	this->m_MonsterMaxLifeRate = GetPrivateProfileInt(section,"MonsterMaxLifeRate",0,path);
	this->m_MonsterDefenseRate = GetPrivateProfileInt(section,"MonsterDefenseRate",0,path);
	this->m_MonsterDefenseSuccessRateRate = GetPrivateProfileInt(section,"MonsterDefenseSuccessRateRate",0,path);
	this->m_MonsterPhysiDamageRate = GetPrivateProfileInt(section,"MonsterPhysiDamageRate",0,path);
	this->m_MonsterAttackSuccessRateRate = GetPrivateProfileInt(section,"MonsterAttackSuccessRateRate",0,path);
	this->m_MonsterHealthBarSwitch = GetPrivateProfileInt(section,"MonsterHealthBarSwitch",0,path);

	this->m_MonsterGetTopHitDamageUserMaxTime = GetPrivateProfileInt(section,"MonsterGetTopHitDamageUserMaxTime",0,path);

	this->m_ExperienceDeduceRate1 = GetPrivateProfileInt(section,"ExperienceDeduceRate1",0,path);
	this->m_ExperienceDeduceRate2 = GetPrivateProfileInt(section,"ExperienceDeduceRate2",0,path);
	this->m_ExperienceDeduceRate3 = GetPrivateProfileInt(section,"ExperienceDeduceRate3",0,path);
	this->m_ExperienceDeduceRate4 = GetPrivateProfileInt(section,"ExperienceDeduceRate4",0,path);
	this->m_ExperienceDeduceRate5 = GetPrivateProfileInt(section,"ExperienceDeduceRate5",0,path);
	
	this->m_DropItemRate1 = GetPrivateProfileInt(section,"DropItemRate1",0,path);
	this->m_DropItemRate2 = GetPrivateProfileInt(section,"DropItemRate2",0,path);
	this->m_DropItemRate3 = GetPrivateProfileInt(section,"DropItemRate3",0,path);
	this->m_DropItemRate4 = GetPrivateProfileInt(section,"DropItemRate4",0,path);
	this->m_DropItemRate5 = GetPrivateProfileInt(section,"DropItemRate5",0,path);
	this->m_DropItemLevel = GetPrivateProfileInt(section,"DropItemLevel",0,path);
	
	this->m_DropItemPet = GetPrivateProfileInt(section,"DropItemPet",0,path);
	this->m_DropItemWing = GetPrivateProfileInt(section,"DropItemWing",0,path);
	this->m_DropItemExc = GetPrivateProfileInt(section,"DropItemExc",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_DropItemSet = GetPrivateProfileInt(section,"DropItemSet",0,path);
	#endif

	this->m_MoneyDeduceRate = GetPrivateProfileInt(section,"MoneyDeduceRate",0,path);

	this->m_NonPK = GetPrivateProfileInt(section,"NonPK",0,path);
	this->m_PKLimitFree = GetPrivateProfileInt(section,"PKLimitFree",0,path);
	this->m_PKLimitShop = GetPrivateProfileInt(section,"PKLimitShop",0,path);
	this->m_PKLimitMove = GetPrivateProfileInt(section,"PKLimitMove",0,path);
	this->m_PKLimitMoveSummon = GetPrivateProfileInt(section,"PKLimitMoveSummon",0,path);
	this->m_PKLimitEventEntry = GetPrivateProfileInt(section,"PKLimitEventEntry",0,path);
	this->m_PKDeathAnnounce = GetPrivateProfileInt(section,"PKDeathAnnounce",0,path);
	this->m_PKDownPlusTimePoint = GetPrivateProfileInt(section,"PKDownPlusTimePoint",0,path);
	this->m_PKDownPlusKillPoint = GetPrivateProfileInt(section,"PKDownPlusKillPoint",0,path);
	this->m_PKDownRequirePoint1 = GetPrivateProfileInt(section,"PKDownRequirePoint1",0,path);
	this->m_PKDownRequirePoint2 = GetPrivateProfileInt(section,"PKDownRequirePoint2",0,path);
	this->m_PKDownRequirePoint3 = GetPrivateProfileInt(section,"PKDownRequirePoint3",0,path);
	this->m_PKDownRequirePoint4 = GetPrivateProfileInt(section,"PKDownRequirePoint4",0,path);

	this->m_TradeSwitch = GetPrivateProfileInt(section,"TradeSwitch",0,path);

	this->m_PersonalShopSwitch = GetPrivateProfileInt(section,"PersonalShopSwitch",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_DuelSwitch = GetPrivateProfileInt(section,"DuelSwitch",0,path);
	this->m_DuelAnnounceSwitch = GetPrivateProfileInt(section,"DuelAnnounceSwitch",0,path);
	this->m_DuelMaxScore = GetPrivateProfileInt(section,"DuelMaxScore",0,path);
	this->m_DuelMaxTime = GetPrivateProfileInt(section,"DuelMaxTime",0,path);
	#endif

	this->m_GuildCreateSwitch = GetPrivateProfileInt(section,"GuildCreateSwitch",0,path);
	this->m_GuildDeleteSwitch = GetPrivateProfileInt(section,"GuildDeleteSwitch",0,path);
	
	this->m_GuildCreateMinLevel[0] = GetPrivateProfileInt(section,"GuildCreateMinLevel_AL0",0,path);
	this->m_GuildCreateMinLevel[1] = GetPrivateProfileInt(section,"GuildCreateMinLevel_AL1",0,path);
	this->m_GuildCreateMinLevel[2] = GetPrivateProfileInt(section,"GuildCreateMinLevel_AL2",0,path);
	this->m_GuildCreateMinLevel[3] = GetPrivateProfileInt(section,"GuildCreateMinLevel_AL3",0,path);
	
	this->m_GuildCreateMinReset[0] = GetPrivateProfileInt(section,"GuildCreateMinReset_AL0",0,path);
	this->m_GuildCreateMinReset[1] = GetPrivateProfileInt(section,"GuildCreateMinReset_AL1",0,path);
	this->m_GuildCreateMinReset[2] = GetPrivateProfileInt(section,"GuildCreateMinReset_AL2",0,path);
	this->m_GuildCreateMinReset[3] = GetPrivateProfileInt(section,"GuildCreateMinReset_AL3",0,path);

	this->m_GuildInsertMaxUser1 = GetPrivateProfileInt(section,"GuildInsertMaxUser1",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_GuildInsertMaxUser2 = GetPrivateProfileInt(section,"GuildInsertMaxUser2",0,path);
	#endif

	// Update 89 2.4.7 -> 97K - Guild War & Battle Soccer Configuracoes
	this->m_GuildWarSwitch = GetPrivateProfileInt(section,"GuildWarSwitch",1,path);
	this->m_GuildWarAttackEnable = GetPrivateProfileInt(section,"GuildWarAttackEnable",0,path);
	this->m_GuildWarScoreMax1 = GetPrivateProfileInt(section,"GuildWarScoreMax1",20,path);
	this->m_GuildWarScoreMax2 = GetPrivateProfileInt(section,"GuildWarScoreMax2",100,path);
	this->m_GuildWarKillScore1 = GetPrivateProfileInt(section,"GuildWarKillScore1",1,path);
	this->m_GuildWarKillScore2 = GetPrivateProfileInt(section,"GuildWarKillScore2",2,path);
	this->m_GuildWarKillScore3 = GetPrivateProfileInt(section,"GuildWarKillScore3",20,path);
	this->m_GuildWarWinnerScore1 = GetPrivateProfileInt(section,"GuildWarWinnerScore1",3,path);
	this->m_GuildWarWinnerScore2 = GetPrivateProfileInt(section,"GuildWarWinnerScore2",2,path);
	this->m_GuildWarWinnerScore3 = GetPrivateProfileInt(section,"GuildWarWinnerScore3",1,path);

	this->m_PetExperienceRateDivisor = GetPrivateProfileInt(section,"PetExperienceRateDivisor",0,path);

	this->m_AddExperienceRate[0] = GetPrivateProfileInt(section,"AddExperienceRate_AL0",0,path);
	this->m_AddExperienceRate[1] = GetPrivateProfileInt(section,"AddExperienceRate_AL1",0,path);
	this->m_AddExperienceRate[2] = GetPrivateProfileInt(section,"AddExperienceRate_AL2",0,path);
	this->m_AddExperienceRate[3] = GetPrivateProfileInt(section,"AddExperienceRate_AL3",0,path);

	this->m_AddEventExperienceRate[0] = GetPrivateProfileInt(section,"AddEventExperienceRate_AL0",0,path);
	this->m_AddEventExperienceRate[1] = GetPrivateProfileInt(section,"AddEventExperienceRate_AL1",0,path);
	this->m_AddEventExperienceRate[2] = GetPrivateProfileInt(section,"AddEventExperienceRate_AL2",0,path);
	this->m_AddEventExperienceRate[3] = GetPrivateProfileInt(section,"AddEventExperienceRate_AL3",0,path);

	this->m_ItemDropTime = GetPrivateProfileInt(section,"ItemDropTime",0,path);

	this->m_ItemDropRate[0] = GetPrivateProfileInt(section,"ItemDropRate_AL0",0,path);
	this->m_ItemDropRate[1] = GetPrivateProfileInt(section,"ItemDropRate_AL1",0,path);
	this->m_ItemDropRate[2] = GetPrivateProfileInt(section,"ItemDropRate_AL2",0,path);
	this->m_ItemDropRate[3] = GetPrivateProfileInt(section,"ItemDropRate_AL3",0,path);

	this->m_MoneyDropTime = GetPrivateProfileInt(section,"MoneyDropTime",0,path);

	this->m_MoneyAmountDropRate[0] = GetPrivateProfileInt(section,"MoneyAmountDropRate_AL0",0,path);
	this->m_MoneyAmountDropRate[1] = GetPrivateProfileInt(section,"MoneyAmountDropRate_AL1",0,path);
	this->m_MoneyAmountDropRate[2] = GetPrivateProfileInt(section,"MoneyAmountDropRate_AL2",0,path);
	this->m_MoneyAmountDropRate[3] = GetPrivateProfileInt(section,"MoneyAmountDropRate_AL3",0,path);

	this->m_WeaponDurabilityRate = GetPrivateProfileInt(section,"WeaponDurabilityRate",0,path);

	this->m_ArmorDurabilityRate = GetPrivateProfileInt(section,"ArmorDurabilityRate",0,path);

	this->m_WingDurabilityRate = GetPrivateProfileInt(section,"WingDurabilityRate",0,path);

	this->m_GuardianDurabilityRate = GetPrivateProfileInt(section,"GuardianDurabilityRate",0,path);

	this->m_PendantDurabilityRate = GetPrivateProfileInt(section,"PendantDurabilityRate",0,path);

	this->m_RingDurabilityRate = GetPrivateProfileInt(section,"RingDurabilityRate",0,path);

	this->m_PetDurabilityRate = GetPrivateProfileInt(section,"PetDurabilityRate",0,path);

	this->m_TradeItemBlock = GetPrivateProfileInt(section,"TradeItemBlock",0,path);

	this->m_TradeItemBlockExc = GetPrivateProfileInt(section,"TradeItemBlockExc",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_TradeItemBlockSet = GetPrivateProfileInt(section,"TradeItemBlockSet",0,path);
	#endif

	this->m_TradeItemBlockSell = GetPrivateProfileInt(section,"TradeItemBlockSell",0,path);

	this->m_MaxLevelUp = GetPrivateProfileInt(section,"MaxLevelUp",0,path);

	this->m_MaxLevelUpEvent = GetPrivateProfileInt(section,"MaxLevelUpEvent",0,path);

	this->m_MaxStatPoint[0] = GetPrivateProfileInt(section,"MaxStatPoint_AL0",0,path);
	this->m_MaxStatPoint[1] = GetPrivateProfileInt(section,"MaxStatPoint_AL1",0,path);
	this->m_MaxStatPoint[2] = GetPrivateProfileInt(section,"MaxStatPoint_AL2",0,path);
	this->m_MaxStatPoint[3] = GetPrivateProfileInt(section,"MaxStatPoint_AL3",0,path);

	this->m_LevelUpPoint[CLASS_DW][0] = GetPrivateProfileInt(section,"DWLevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_DW][1] = GetPrivateProfileInt(section,"DWLevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_DW][2] = GetPrivateProfileInt(section,"DWLevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_DW][3] = GetPrivateProfileInt(section,"DWLevelUpPoint_AL3",0,path);

	this->m_LevelUpPoint[CLASS_DK][0] = GetPrivateProfileInt(section,"DKLevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_DK][1] = GetPrivateProfileInt(section,"DKLevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_DK][2] = GetPrivateProfileInt(section,"DKLevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_DK][3] = GetPrivateProfileInt(section,"DKLevelUpPoint_AL3",0,path);

	this->m_LevelUpPoint[CLASS_FE][0] = GetPrivateProfileInt(section,"FELevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_FE][1] = GetPrivateProfileInt(section,"FELevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_FE][2] = GetPrivateProfileInt(section,"FELevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_FE][3] = GetPrivateProfileInt(section,"FELevelUpPoint_AL3",0,path);

	this->m_LevelUpPoint[CLASS_MG][0] = GetPrivateProfileInt(section,"MGLevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_MG][1] = GetPrivateProfileInt(section,"MGLevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_MG][2] = GetPrivateProfileInt(section,"MGLevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_MG][3] = GetPrivateProfileInt(section,"MGLevelUpPoint_AL3",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_LevelUpPoint[CLASS_DL][0] = GetPrivateProfileInt(section,"DLLevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_DL][1] = GetPrivateProfileInt(section,"DLLevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_DL][2] = GetPrivateProfileInt(section,"DLLevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_DL][3] = GetPrivateProfileInt(section,"DLLevelUpPoint_AL3",0,path);
	#endif

	this->m_PlusStatPoint = GetPrivateProfileInt(section,"PlusStatPoint",0,path);

	this->m_PlusStatMinLevel = GetPrivateProfileInt(section,"PlusStatMinLevel",0,path);

	this->m_CharacterCreateSwitch = GetPrivateProfileInt(section,"CharacterCreateSwitch",0,path);

	this->m_MGCreateLevel[0] = GetPrivateProfileInt(section,"MGCreateLevel_AL0",0,path);
	this->m_MGCreateLevel[1] = GetPrivateProfileInt(section,"MGCreateLevel_AL1",0,path);
	this->m_MGCreateLevel[2] = GetPrivateProfileInt(section,"MGCreateLevel_AL2",0,path);
	this->m_MGCreateLevel[3] = GetPrivateProfileInt(section,"MGCreateLevel_AL3",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_DLCreateLevel[0] = GetPrivateProfileInt(section,"DLCreateLevel_AL0",0,path);
	this->m_DLCreateLevel[1] = GetPrivateProfileInt(section,"DLCreateLevel_AL1",0,path);
	this->m_DLCreateLevel[2] = GetPrivateProfileInt(section,"DLCreateLevel_AL2",0,path);
	this->m_DLCreateLevel[3] = GetPrivateProfileInt(section,"DLCreateLevel_AL3",0,path);
	#endif

	this->m_CharacterDeleteSwitch = GetPrivateProfileInt(section,"CharacterDeleteSwitch",0,path);
	this->m_CharacterDeleteMaxLevel = GetPrivateProfileInt(section,"CharacterDeleteMaxLevel",0,path);

	// Update 89 2.4.7 -> 97K - Reconnect Time Unificado
	this->m_ReconnectTime = GetPrivateProfileInt(section,"ReconnectTime",300,path);
	// Update 90 2.4.8 -> 97K - Aprimoramento de opcoes de Reconnect
	this->m_ReconnectAttackSwitch = GetPrivateProfileInt(section,"ReconnectAttackSwitch",0,path);
	this->m_ReconnectCommandSwitch = GetPrivateProfileInt(section,"ReconnectCommandSwitch",1,path);
	this->m_ReconnectEventSwitch = GetPrivateProfileInt(section,"ReconnectEventSwitch",0,path);
	this->m_ReconnectEventMaxTime = GetPrivateProfileInt(section,"ReconnectEventMaxTime",30,path);
	this->m_ReconnectPartySwitch = GetPrivateProfileInt(section,"ReconnectPartySwitch",1,path);
	this->m_ReconnectPickSwitch = GetPrivateProfileInt(section,"ReconnectPickSwitch",0,path);
	this->m_ReconnectOfflineSwitch = GetPrivateProfileInt(section,"ReconnectOfflineSwitch",0,path);

	this->m_PartyMoneyDistribute = GetPrivateProfileInt(section,"PartyMoneyDistribute",0,path);

	this->m_PartyDisableKillBetweenMembers = GetPrivateProfileInt(section,"PartyDisableKillBetweenMembers",0,path);

	this->m_PartyGeneralExperience[0] = GetPrivateProfileInt(section,"PartyGeneralExperience1",0,path);
	this->m_PartyGeneralExperience[1] = GetPrivateProfileInt(section,"PartyGeneralExperience2",0,path);
	this->m_PartyGeneralExperience[2] = GetPrivateProfileInt(section,"PartyGeneralExperience3",0,path);
	this->m_PartyGeneralExperience[3] = GetPrivateProfileInt(section,"PartyGeneralExperience4",0,path);
	this->m_PartyGeneralExperience[4] = GetPrivateProfileInt(section,"PartyGeneralExperience5",0,path);

	this->m_PartySpecialExperience[0] = GetPrivateProfileInt(section,"PartySpecialExperience1",0,path);
	this->m_PartySpecialExperience[1] = GetPrivateProfileInt(section,"PartySpecialExperience2",0,path);
	this->m_PartySpecialExperience[2] = GetPrivateProfileInt(section,"PartySpecialExperience3",0,path);
	this->m_PartySpecialExperience[3] = GetPrivateProfileInt(section,"PartySpecialExperience4",0,path);
	this->m_PartySpecialExperience[4] = GetPrivateProfileInt(section,"PartySpecialExperience5",0,path);

	this->m_PartyMaxGapLevel = GetPrivateProfileInt(section,"PartyMaxGapLevel",0,path);

	this->m_SoulSuccessRate[0] = GetPrivateProfileInt(section,"SoulSuccessRate_AL0",0,path);
	this->m_SoulSuccessRate[1] = GetPrivateProfileInt(section,"SoulSuccessRate_AL1",0,path);
	this->m_SoulSuccessRate[2] = GetPrivateProfileInt(section,"SoulSuccessRate_AL2",0,path);
	this->m_SoulSuccessRate[3] = GetPrivateProfileInt(section,"SoulSuccessRate_AL3",0,path);

	this->m_LifeSuccessRate[0] = GetPrivateProfileInt(section,"LifeSuccessRate_AL0",0,path);
	this->m_LifeSuccessRate[1] = GetPrivateProfileInt(section,"LifeSuccessRate_AL1",0,path);
	this->m_LifeSuccessRate[2] = GetPrivateProfileInt(section,"LifeSuccessRate_AL2",0,path);
	this->m_LifeSuccessRate[3] = GetPrivateProfileInt(section,"LifeSuccessRate_AL3",0,path);

	this->m_AddLuckSuccessRate1[0] = GetPrivateProfileInt(section,"AddLuckSuccessRate1_AL0",0,path);
	this->m_AddLuckSuccessRate1[1] = GetPrivateProfileInt(section,"AddLuckSuccessRate1_AL1",0,path);
	this->m_AddLuckSuccessRate1[2] = GetPrivateProfileInt(section,"AddLuckSuccessRate1_AL2",0,path);
	this->m_AddLuckSuccessRate1[3] = GetPrivateProfileInt(section,"AddLuckSuccessRate1_AL3",0,path);

	this->m_AddLuckSuccessRate2[0] = GetPrivateProfileInt(section,"AddLuckSuccessRate2_AL0",0,path);
	this->m_AddLuckSuccessRate2[1] = GetPrivateProfileInt(section,"AddLuckSuccessRate2_AL1",0,path);
	this->m_AddLuckSuccessRate2[2] = GetPrivateProfileInt(section,"AddLuckSuccessRate2_AL2",0,path);
	this->m_AddLuckSuccessRate2[3] = GetPrivateProfileInt(section,"AddLuckSuccessRate2_AL3",0,path);

	this->m_FruitAddPointMin = GetPrivateProfileInt(section,"FruitAddPointMin",0,path);
	this->m_FruitAddPointMax = GetPrivateProfileInt(section,"FruitAddPointMax",0,path);

	this->m_FruitAddPointSuccessRate[0] = GetPrivateProfileInt(section,"FruitAddPointSuccessRate_AL0",0,path);
	this->m_FruitAddPointSuccessRate[1] = GetPrivateProfileInt(section,"FruitAddPointSuccessRate_AL1",0,path);
	this->m_FruitAddPointSuccessRate[2] = GetPrivateProfileInt(section,"FruitAddPointSuccessRate_AL2",0,path);
	this->m_FruitAddPointSuccessRate[3] = GetPrivateProfileInt(section,"FruitAddPointSuccessRate_AL3",0,path);

	this->m_QuestMonsterItemDropParty = GetPrivateProfileInt(section,"QuestMonsterItemDropParty",0,path);
}

void CServerInfo::ReadEventInfo(char* section,char* path) // OK
{
	this->m_WindowEventSwitch = GetPrivateProfileInt(section,"WindowEventSwitch",0,path);

	this->m_BloodCastleEvent = GetPrivateProfileInt(section,"BloodCastleEvent",0,path);

	this->m_BloodCastleMaxUser = GetPrivateProfileInt(section,"BloodCastleMaxUser",0,path);

	this->m_BloodCastleMaxEntryCount[0] = GetPrivateProfileInt(section,"BloodCastleMaxEntryCount_AL0",0,path);
	this->m_BloodCastleMaxEntryCount[1] = GetPrivateProfileInt(section,"BloodCastleMaxEntryCount_AL1",0,path);
	this->m_BloodCastleMaxEntryCount[2] = GetPrivateProfileInt(section,"BloodCastleMaxEntryCount_AL2",0,path);
	this->m_BloodCastleMaxEntryCount[3] = GetPrivateProfileInt(section,"BloodCastleMaxEntryCount_AL3",0,path);

	this->m_BloodCastleNPCWithoutEntrance = GetPrivateProfileInt(section,"BloodCastleNPCWithoutEntrance",0,path);

	this->m_BonusManagerSwitch = GetPrivateProfileInt(section,"BonusManagerSwitch",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_ChaosCastleEvent = GetPrivateProfileInt(section,"ChaosCastleEvent",0,path);

	this->m_ChaosCastleMinUser = GetPrivateProfileInt(section,"ChaosCastleMinUser",0,path);

	this->m_ChaosCastleBlowUserRate = GetPrivateProfileInt(section,"ChaosCastleBlowUserRate",0,path);

	this->m_ChaosCastleMoneyRate = GetPrivateProfileInt(section,"ChaosCastleMoneyRate",0,path);

	this->m_ChaosCastleMaxEntryCount[0] = GetPrivateProfileInt(section,"ChaosCastleMaxEntryCount_AL0",0,path);
	this->m_ChaosCastleMaxEntryCount[1] = GetPrivateProfileInt(section,"ChaosCastleMaxEntryCount_AL1",0,path);
	this->m_ChaosCastleMaxEntryCount[2] = GetPrivateProfileInt(section,"ChaosCastleMaxEntryCount_AL2",0,path);
	this->m_ChaosCastleMaxEntryCount[3] = GetPrivateProfileInt(section,"ChaosCastleMaxEntryCount_AL3",0,path);
	#endif

	this->m_DevilSquareEvent = GetPrivateProfileInt(section,"DevilSquareEvent",0,path);

	this->m_DevilSquareMaxUser = GetPrivateProfileInt(section,"DevilSquareMaxUser",0,path);

	this->m_DevilSquareMaxEntryCount[0] = GetPrivateProfileInt(section,"DevilSquareMaxEntryCount_AL0",0,path);
	this->m_DevilSquareMaxEntryCount[1] = GetPrivateProfileInt(section,"DevilSquareMaxEntryCount_AL1",0,path);
	this->m_DevilSquareMaxEntryCount[2] = GetPrivateProfileInt(section,"DevilSquareMaxEntryCount_AL2",0,path);
	this->m_DevilSquareMaxEntryCount[3] = GetPrivateProfileInt(section,"DevilSquareMaxEntryCount_AL3",0,path);

	this->m_DevilSquareNPCWithoutEntrance = GetPrivateProfileInt(section,"DevilSquareNPCWithoutEntrance",0,path);

	this->m_DropEventSwitch = GetPrivateProfileInt(section,"DropEventSwitch",0,path);

	this->m_InvasionManagerSwitch = GetPrivateProfileInt(section,"InvasionManagerSwitch",0,path);
}

void CServerInfo::ReadHackInfo(char* section,char* path) // OK
{
	this->m_AntiFloodSwitch = GetPrivateProfileInt(section,"AntiFloodSwitch",0,path);
	this->m_AntiFloodBlockTime = GetPrivateProfileInt(section,"AntiFloodBlockTime",0,path);
	this->m_AntiFloodMaxCount = GetPrivateProfileInt(section,"AntiFloodMaxCount",0,path);
    this->m_AntiFloodMaxTime = GetPrivateProfileInt(section,"AntiFloodMaxTime",0,path);

	this->m_CheckSpeedHack = GetPrivateProfileInt(section,"CheckSpeedHack",0,path);
	this->m_CheckSpeedHackTolerance = GetPrivateProfileInt(section,"CheckSpeedHackTolerance",0,path);
	this->m_CheckSpeedHackAction = GetPrivateProfileInt(section,"CheckSpeedHackAction",0,path);

	this->m_CheckLatencyHack = GetPrivateProfileInt(section,"CheckLatencyHack",0,path);
	this->m_CheckLatencyHackTolerance = GetPrivateProfileInt(section,"CheckLatencyHackTolerance",0,path);
	this->m_CheckLatencyHackAction = GetPrivateProfileInt(section,"CheckLatencyHackAction",0,path);

	this->m_CheckAutoPotionHack = GetPrivateProfileInt(section,"CheckAutoPotionHack",0,path);
	this->m_CheckAutoPotionHackTolerance = GetPrivateProfileInt(section,"CheckAutoPotionHackTolerance",0,path);
	this->m_CheckAutoPotionHackAction = GetPrivateProfileInt(section,"CheckAutoPotionHackAction",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_CheckAutoComboHack = GetPrivateProfileInt(section,"CheckAutoComboHack",0,path);
	this->m_CheckAutoComboHackTolerance = GetPrivateProfileInt(section,"CheckAutoComboHackTolerance",0,path);
	this->m_CheckAutoComboHackAction = GetPrivateProfileInt(section,"CheckAutoComboHackAction",0,path);
	#endif

	this->m_CheckMoveHack = GetPrivateProfileInt(section,"CheckMoveHack",0,path);
	this->m_CheckMoveHackMaxDelay = GetPrivateProfileInt(section,"CheckMoveHackMaxDelay",0,path);
	this->m_CheckMoveHackMaxCount = GetPrivateProfileInt(section,"CheckMoveHackMaxCount",0,path);
	this->m_CheckMoveHackAction = GetPrivateProfileInt(section,"CheckMoveHackAction",0,path);
}

void CServerInfo::ReadItemInfo(char* section,char* path) // OK
{
	this->m_TransformationRing1 = GetPrivateProfileInt(section,"TransformationRing1",0,path);
	this->m_TransformationRing2 = GetPrivateProfileInt(section,"TransformationRing2",0,path);
	this->m_TransformationRing3 = GetPrivateProfileInt(section,"TransformationRing3",0,path);
	this->m_TransformationRing4 = GetPrivateProfileInt(section,"TransformationRing4",0,path);
	this->m_TransformationRing5 = GetPrivateProfileInt(section,"TransformationRing5",0,path);
	this->m_TransformationRing6 = GetPrivateProfileInt(section,"TransformationRing6",0,path);

	this->m_SatanIncDamageConstA = GetPrivateProfileInt(section,"SatanIncDamageConstA",0,path);

	this->m_DinorantIncDamageConstA = GetPrivateProfileInt(section,"DinorantIncDamageConstA",0,path);

	this->m_AngelDecDamageConstA = GetPrivateProfileInt(section,"AngelDecDamageConstA",0,path);

	this->m_DinorantDecDamageConstA = GetPrivateProfileInt(section,"DinorantDecDamageConstA",0,path);
	this->m_DinorantDecDamageConstB = GetPrivateProfileInt(section,"DinorantDecDamageConstB",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_DarkHorseDecDamageConstA = GetPrivateProfileInt(section,"DarkHorseDecDamageConstA",0,path);
	this->m_DarkHorseDecDamageConstB = GetPrivateProfileInt(section,"DarkHorseDecDamageConstB",0,path);
	#endif

	this->m_ApplePotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWApplePotionRate",0,path);
	this->m_ApplePotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKApplePotionRate",0,path);
	this->m_ApplePotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FEApplePotionRate",0,path);
	this->m_ApplePotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGApplePotionRate",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_ApplePotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLApplePotionRate",0,path);
	#endif
	
	this->m_SmallLifePotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWSmallLifePotionRate",0,path);
	this->m_SmallLifePotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKSmallLifePotionRate",0,path);
	this->m_SmallLifePotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FESmallLifePotionRate",0,path);
	this->m_SmallLifePotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGSmallLifePotionRate",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_SmallLifePotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLSmallLifePotionRate",0,path);
	#endif
	
	this->m_MidleLifePotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWMidleLifePotionRate",0,path);
	this->m_MidleLifePotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKMidleLifePotionRate",0,path);
	this->m_MidleLifePotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FEMidleLifePotionRate",0,path);
	this->m_MidleLifePotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGMidleLifePotionRate",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_MidleLifePotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLMidleLifePotionRate",0,path);
	#endif
	
	this->m_LargeLifePotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWLargeLifePotionRate",0,path);
	this->m_LargeLifePotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKLargeLifePotionRate",0,path);
	this->m_LargeLifePotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FELargeLifePotionRate",0,path);
	this->m_LargeLifePotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGLargeLifePotionRate",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_LargeLifePotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLLargeLifePotionRate",0,path);
	#endif
	
	this->m_SmallManaPotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWSmallManaPotionRate",0,path);
	this->m_SmallManaPotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKSmallManaPotionRate",0,path);
	this->m_SmallManaPotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FESmallManaPotionRate",0,path);
	this->m_SmallManaPotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGSmallManaPotionRate",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_SmallManaPotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLSmallManaPotionRate",0,path);
	#endif
	
	this->m_MidleManaPotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWMidleManaPotionRate",0,path);
	this->m_MidleManaPotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKMidleManaPotionRate",0,path);
	this->m_MidleManaPotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FEMidleManaPotionRate",0,path);
	this->m_MidleManaPotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGMidleManaPotionRate",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_MidleManaPotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLMidleManaPotionRate",0,path);
	#endif
	
	this->m_LargeManaPotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWLargeManaPotionRate",0,path);
	this->m_LargeManaPotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKLargeManaPotionRate",0,path);
	this->m_LargeManaPotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FELargeManaPotionRate",0,path);
	this->m_LargeManaPotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGLargeManaPotionRate",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_LargeManaPotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLLargeManaPotionRate",0,path);
	#endif

	this->m_AleIncSpeed = GetPrivateProfileInt(section,"AleIncSpeed",0,path);
	this->m_AleIncSpeedTime = GetPrivateProfileInt(section,"AleIncSpeedTime",0,path);

	this->m_OliveOfLoveIncSpeed = GetPrivateProfileInt(section,"OliveOfLoveIncSpeed",0,path);
	this->m_OliveOfLoveIncSpeedTime = GetPrivateProfileInt(section,"OliveOfLoveIncSpeedTime",0,path);

	this->m_RemedyOfLoveIncDamage = GetPrivateProfileInt(section,"RemedyOfLoveIncDamage",0,path);
	this->m_RemedyOfLoveIncDamageTime = GetPrivateProfileInt(section,"RemedyOfLoveIncDamageTime",0,path);
}

void CServerInfo::ReadSkillInfo(char* section,char* path) // OK
{
	this->m_PoisonEffectConstA = GetPrivateProfileInt(section,"PoisonEffectConstA",0,path);
	this->m_PoisonEffectConstB = GetPrivateProfileInt(section,"PoisonEffectConstB",0,path);
	this->m_PoisonEffectTime = GetPrivateProfileInt(section,"PoisonEffectTime",0,path);
	
	this->m_IceEffectTime = GetPrivateProfileInt(section,"IceEffectTime",0,path);
	
	this->m_DecayEffectConstA = GetPrivateProfileInt(section,"DecayEffectConstA",0,path);
	this->m_DecayEffectConstB = GetPrivateProfileInt(section,"DecayEffectConstB",0,path);
	this->m_DecayEffectTime = GetPrivateProfileInt(section,"DecayEffectTime",0,path);
	
	this->m_IceStormEffectTime = GetPrivateProfileInt(section,"IceStormEffectTime",0,path);

	this->m_IceArrowEffectTime = GetPrivateProfileInt(section,"IceArrowEffectTime",0,path);

	this->m_ManaShieldConstA = GetPrivateProfileInt(section,"ManaShieldConstA",0,path);
	this->m_ManaShieldConstB = GetPrivateProfileInt(section,"ManaShieldConstB",0,path);
	this->m_ManaShieldConstC = GetPrivateProfileInt(section,"ManaShieldConstC",0,path);

	this->m_ManaShieldRate[CLASS_DW] = GetPrivateProfileInt(section,"ManaShieldRateDW",0,path);
	this->m_ManaShieldRate[CLASS_DK] = GetPrivateProfileInt(section,"ManaShieldRateDK",0,path);
	this->m_ManaShieldRate[CLASS_FE] = GetPrivateProfileInt(section,"ManaShieldRateFE",0,path);
	this->m_ManaShieldRate[CLASS_MG] = GetPrivateProfileInt(section,"ManaShieldRateMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_ManaShieldRate[CLASS_DL] = GetPrivateProfileInt(section,"ManaShieldRateDL",0,path);
	#endif

	this->m_ManaShieldTimeConstA = GetPrivateProfileInt(section,"ManaShieldTimeConstA",0,path);
	this->m_ManaShieldTimeConstB = GetPrivateProfileInt(section,"ManaShieldTimeConstB",0,path);

	this->m_ManaShieldMaxRate = GetPrivateProfileInt(section,"ManaShieldMaxRate",0,path);

	this->m_DefenseConstA = GetPrivateProfileInt(section,"DefenseConstA",0,path);

	this->m_DefenseTimeConstA = GetPrivateProfileInt(section,"DefenseTimeConstA",0,path);

	this->m_HealConstA = GetPrivateProfileInt(section,"HealConstA",0,path);
	this->m_HealConstB = GetPrivateProfileInt(section,"HealConstB",0,path);

	this->m_GreaterDefenseConstA = GetPrivateProfileInt(section,"GreaterDefenseConstA",0,path);
	this->m_GreaterDefenseConstB = GetPrivateProfileInt(section,"GreaterDefenseConstB",0,path);

	this->m_GreaterDefenseRate[CLASS_DW] = GetPrivateProfileInt(section,"GreaterDefenseRateDW",0,path);
	this->m_GreaterDefenseRate[CLASS_DK] = GetPrivateProfileInt(section,"GreaterDefenseRateDK",0,path);
	this->m_GreaterDefenseRate[CLASS_FE] = GetPrivateProfileInt(section,"GreaterDefenseRateFE",0,path);
	this->m_GreaterDefenseRate[CLASS_MG] = GetPrivateProfileInt(section,"GreaterDefenseRateMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_GreaterDefenseRate[CLASS_DL] = GetPrivateProfileInt(section,"GreaterDefenseRateDL",0,path);
	#endif

	this->m_GreaterDefenseTimeConstA = GetPrivateProfileInt(section,"GreaterDefenseTimeConstA",0,path);

	this->m_GreaterDamageConstA = GetPrivateProfileInt(section,"GreaterDamageConstA",0,path);
	this->m_GreaterDamageConstB = GetPrivateProfileInt(section,"GreaterDamageConstB",0,path);

	this->m_GreaterDamageRate[CLASS_DW] = GetPrivateProfileInt(section,"GreaterDamageRateDW",0,path);
	this->m_GreaterDamageRate[CLASS_DK] = GetPrivateProfileInt(section,"GreaterDamageRateDK",0,path);
	this->m_GreaterDamageRate[CLASS_FE] = GetPrivateProfileInt(section,"GreaterDamageRateFE",0,path);
	this->m_GreaterDamageRate[CLASS_MG] = GetPrivateProfileInt(section,"GreaterDamageRateMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_GreaterDamageRate[CLASS_DL] = GetPrivateProfileInt(section,"GreaterDamageRateDL",0,path);
	#endif

	this->m_GreaterDamageTimeConstA = GetPrivateProfileInt(section,"GreaterDamageTimeConstA",0,path);

	this->m_SummonMonster1 = GetPrivateProfileInt(section,"SummonMonster1",0,path);
	this->m_SummonMonster2 = GetPrivateProfileInt(section,"SummonMonster2",0,path);
	this->m_SummonMonster3 = GetPrivateProfileInt(section,"SummonMonster3",0,path);
	this->m_SummonMonster4 = GetPrivateProfileInt(section,"SummonMonster4",0,path);
	this->m_SummonMonster5 = GetPrivateProfileInt(section,"SummonMonster5",0,path);
	this->m_SummonMonster6 = GetPrivateProfileInt(section,"SummonMonster6",0,path);
	this->m_SummonMonster7 = GetPrivateProfileInt(section,"SummonMonster7",0,path);

	this->m_GreaterLifeConstA = GetPrivateProfileInt(section,"GreaterLifeConstA",0,path);
	this->m_GreaterLifeConstB = GetPrivateProfileInt(section,"GreaterLifeConstB",0,path);
	this->m_GreaterLifeConstC = GetPrivateProfileInt(section,"GreaterLifeConstC",0,path);

	this->m_GreaterLifeRate[CLASS_DW] = GetPrivateProfileInt(section,"GreaterLifeRateDW",0,path);
	this->m_GreaterLifeRate[CLASS_DK] = GetPrivateProfileInt(section,"GreaterLifeRateDK",0,path);
	this->m_GreaterLifeRate[CLASS_FE] = GetPrivateProfileInt(section,"GreaterLifeRateFE",0,path);
	this->m_GreaterLifeRate[CLASS_MG] = GetPrivateProfileInt(section,"GreaterLifeRateMG",0,path);
	#if(GAMESERVER_UPDATE>=99)
	this->m_GreaterLifeRate[CLASS_DL] = GetPrivateProfileInt(section,"GreaterLifeRateDL",0,path);
	#endif

	this->m_GreaterLifeTimeConstA = GetPrivateProfileInt(section,"GreaterLifeTimeConstA",0,path);
	this->m_GreaterLifeTimeConstB = GetPrivateProfileInt(section,"GreaterLifeTimeConstB",0,path);

	this->m_GreaterLifeMaxRate = GetPrivateProfileInt(section,"GreaterLifeMaxRate",0,path);

	this->m_FireSlashConstA = GetPrivateProfileInt(section,"FireSlashConstA",0,path);
	this->m_FireSlashConstB = GetPrivateProfileInt(section,"FireSlashConstB",0,path);

	this->m_FireSlashTimeConstA = GetPrivateProfileInt(section,"FireSlashTimeConstA",0,path);

	this->m_FireSlashMaxRate = GetPrivateProfileInt(section,"FireSlashMaxRate",0,path);

	#if(GAMESERVER_UPDATE>=99)
	this->m_GreaterCriticalDamageConstA = GetPrivateProfileInt(section,"GreaterCriticalDamageConstA",0,path);
	this->m_GreaterCriticalDamageConstB = GetPrivateProfileInt(section,"GreaterCriticalDamageConstB",0,path);

	this->m_GreaterCriticalDamageTimeConstA = GetPrivateProfileInt(section,"GreaterCriticalDamageTimeConstA",0,path);
	this->m_GreaterCriticalDamageTimeConstB = GetPrivateProfileInt(section,"GreaterCriticalDamageTimeConstB",0,path);
	#endif

	this->m_InfinityArrowSwitch[0] = GetPrivateProfileInt(section,"InfinityArrowSwitch_AL0",0,path);
	this->m_InfinityArrowSwitch[1] = GetPrivateProfileInt(section,"InfinityArrowSwitch_AL1",0,path);
	this->m_InfinityArrowSwitch[2] = GetPrivateProfileInt(section,"InfinityArrowSwitch_AL2",0,path);
	this->m_InfinityArrowSwitch[3] = GetPrivateProfileInt(section,"InfinityArrowSwitch_AL3",0,path);

	this->m_MagicDamageImmunityTimeConstA = GetPrivateProfileInt(section,"MagicDamageImmunityTimeConstA",0,path);

	this->m_PhysiDamageImmunityTimeConstA = GetPrivateProfileInt(section,"PhysiDamageImmunityTimeConstA",0,path);
}

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 99 (2.5.8) - Sistema de multi-idioma (ServerLang)
void CServerInfo::ReadLangInfo()
{
	this->ReadLangInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat");

	switch (this->m_ServerLang)
	{
		case 1:
			gMessage.Load(gPath.GetFullPath("Lang\\Spn\\Message.txt"));
			gNotice.Load(gPath.GetFullPath("Lang\\Spn\\Notice.txt"));
			break;
		case 2:
			gMessage.Load(gPath.GetFullPath("Lang\\Por\\Message.txt"));
			gNotice.Load(gPath.GetFullPath("Lang\\Por\\Notice.txt"));
			break;
		default:
			gMessage.Load(gPath.GetFullPath("Lang\\Eng\\Message.txt"));
			gNotice.Load(gPath.GetFullPath("Lang\\Eng\\Notice.txt"));
			break;
	}
}

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 99 (2.5.8) - Leitura de ServerLang
void CServerInfo::ReadLangInfo(char* section,char* path)
{
	this->m_ServerLang = GetPrivateProfileInt(section,"ServerLang",0,path);
}