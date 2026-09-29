#include "stdafx.h"
#include "DSProtocol.h"
#include "BloodCastle.h"
#include "ChaosBox.h"
#include "CommandManager.h"
#include "CustomDailyReward.h"
#include "CustomGift.h"
#include "CustomMonsterKillCount.h"
#include "CustomPick.h"
#include "DefaultClassFreebies.h"
#include "Filter.h"
#include "Fruit.h"
#include "GameMain.h"
#include "GameMaster.h"
#include "Guild.h"
#include "GuildManager.h"
#include "GoldenArcher.h"
#include "HackMoveSpeedCheck.h"
#include "InvasionManager.h"
#include "ItemBagManager.h"
#include "ItemManager.h"
#include "ItemStack.h"
#include "ItemValue.h"
#include "JSProtocol.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "Message.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "ObjectManager.h"
#include "Party.h"
#include "PeriodicItem.h"
#include "Quest.h"
#include "Reconnect.h"
#include "ServerInfo.h"
#include "Util.h"
#include "Viewport.h"
#include "Warehouse.h"
#include "ScriptLoader.h"

void DataServerProtocolCore(BYTE head,BYTE* lpMsg,int size) // OK
{
	switch(head)
	{
		case 0x01:
			DGCharacterListRecv((SDHP_CHARACTER_LIST_RECV*)lpMsg);
			break;
		case 0x02:
			DGCharacterCreateRecv((SDHP_CHARACTER_CREATE_RECV*)lpMsg);
			break;
		case 0x03:
			DGCharacterDeleteRecv((SDHP_CHARACTER_DELETE_RECV*)lpMsg);
			break;
		case 0x04:
			DGCharacterInfoRecv((SDHP_CHARACTER_INFO_RECV*)lpMsg);
			break;
		case 0x05:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gWarehouse.DGWarehouseItemRecv((SDHP_WAREHOUSE_ITEM_RECV*)lpMsg);
					break;
				case 0x01:
					gWarehouse.DGWarehouseFreeRecv((SDHP_WAREHOUSE_FREE_RECV*)lpMsg);
					break;
			}
			break;
		case 0x06:
			gItemManager.DGGetItemSerialRecv((SDHP_GET_ITEM_SERIAL_RECV*)lpMsg);
			break;
		case 0x07:
			DGCreateItemRecv((SDHP_CREATE_ITEM_RECV*)lpMsg);
			break;
		case 0x08:
			DGOptionDataRecv((SDHP_OPTION_DATA_RECV*)lpMsg);
			break;
		case 0x09:
			DGPetItemInfoRecv((SDHP_PET_ITEM_INFO_RECV*)lpMsg);
			break;
		case 0x0F:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gCommandManager.DGCommandResetRecv((SDHP_COMMAND_RESET_RECV*)lpMsg);
					break;
				case 0x01:
					gCommandManager.DGCommandMasterResetRecv((SDHP_COMMAND_MASTER_RESET_RECV*)lpMsg);
					break;
				case 0x03:
					gCustomGift.DGCommandGiftRecv((SDHP_COMMAND_GIFT_RECV*)lpMsg);
					break;
				case 0x04:
					gCommandManager.DGCommandRenameRecv((SDHP_COMMAND_RENAME_RECV*)lpMsg);
					break;
				case 0x05:
					gCommandManager.DGCommandBanAccountRecv((SDHP_COMMAND_BAN_ACCOUNT_RECV*)lpMsg);
					break;
				case 0x06:
					gCommandManager.DGCommandBanCharacterRecv((SDHP_COMMAND_BAN_CHARACTER_RECV*)lpMsg);
					break;
			}
			break;
		case 0x13:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gCustomPick.DGCustomPickRecv((SDHP_CUSTOM_PICK_RECV*)lpMsg);
					break;
			}
			break;
		// Update 88 2.4.6 -> 97K - Sistema de recompensas diarias
		case 0x16:
			gCustomDailyReward.DGDailyRewardCheckRecv((SDHP_DAILY_REWARD_INFO_RECV*)lpMsg);
			break;
		case 0x50:
			gCustomMonsterKillCount.DGMonsterKillCountRecv((SDHP_MONSTER_KILL_COUNT_RECV*)lpMsg);
			break;
		case 0x20:
			DGGlobalPostRecv((SDHP_GLOBAL_POST_RECV*)lpMsg);
			break;
		case 0x21:
			DGGlobalNoticeRecv((SDHP_GLOBAL_NOTICE_RECV*)lpMsg);
			break;
		// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 96 (2.5.4) - Processamento de recebimento de mensagem global
		case 0x2B:
			DGGlobalMessageRecv((SDHP_GLOBAL_MESSAGE_RECV*)lpMsg);
			break;
		case 0x2A:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gPeriodicItem.DGPeriodicItemRecv((SDHP_PERIODIC_ITEM_RECV*)lpMsg);
					break;
			}
			break;
		case 0x72:
			DGGlobalWhisperRecv((SDHP_GLOBAL_WHISPER_RECV*)lpMsg);
			break;
		case 0x73:
			DGGlobalWhisperEchoRecv((SDHP_GLOBAL_WHISPER_ECHO_RECV*)lpMsg);
			break;
		case 0x94:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gGoldenArcher.DGGoldenArcherCountRecv((SDHP_GOLDEN_ARCHER_COUNT_RECV*)lpMsg);
					break;
				case 0x01:
					gGoldenArcher.DGGoldenArcherRewardRecv((SDHP_GOLDEN_ARCHER_REWARD_RECV*)lpMsg);
					break;
			}
			break;
		case 0xA0:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gGuild.DGGuildCreateRecv((SDHP_GUILD_CREATE_RECV*)lpMsg);
					break;
				case 0x01:
					gGuild.DGGuildDeleteRecv((SDHP_GUILD_DELETE_RECV*)lpMsg);
					break;
				case 0x02:
					gGuild.DGGuildMemberAddRecv((SDHP_GUILD_MEMBER_ADD_RECV*)lpMsg);
					break;
				case 0x03:
					gGuild.DGGuildMemberDelRecv((SDHP_GUILD_MEMBER_DEL_RECV*)lpMsg);
					break;
				case 0x04:
					gGuild.DGGuildNoticeRecv((SDHP_GUILD_NOTICE_RECV*)lpMsg);
					break;
				case 0x05:
					gGuild.DGGuildPostRecv((SDHP_GUILD_POST_RECV*)lpMsg);
					break;
				case 0x06:
					gGuild.DGGuildScoreRecv((SDHP_GUILD_SCORE_RECV*)lpMsg);
					break;
				case 0x07:
					gGuild.DGGuildInfoRecv((SDHP_GUILD_INFO_RECV*)lpMsg);
					break;
				case 0x08:
					gGuild.DGGuildMemberUpdateRecv((SDHP_GUILD_MEMBER_UPDATE_RECV*)lpMsg);
					break;
				}
			break;
		// Update 89 2.4.7 -> 97K - Sistema de Reconexão (Fase 1: Persistência no DataServer)
		case 0xC0:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gReconnect.DGReconnectInfoInsertRecv((SDHP_RECONNECT_INFO_INSERT_RECV*)lpMsg);
					break;
			}
			break;
	}
}

void DGCharacterListRecv(SDHP_CHARACTER_LIST_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCharacterListRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	BYTE send[256];

	PMSG_CHARACTER_LIST_SEND pMsg;

	pMsg.header.setE(0xF3,0x00,0);

	int size = sizeof(pMsg);
	
	pMsg.count = 0;

	PMSG_CHARACTER_LIST info;

	WORD CharacterCreationLevel = 0;

	for(int n=0;n < lpMsg->count;n++)
	{
		SDHP_CHARACTER_LIST* lpInfo = (SDHP_CHARACTER_LIST*)(((BYTE*)lpMsg)+sizeof(SDHP_CHARACTER_LIST_RECV)+(sizeof(SDHP_CHARACTER_LIST)*n));

		info.slot = lpInfo->slot;

		memcpy(info.Name,lpInfo->name,sizeof(info.Name));

		info.Level = lpInfo->level;

		CharacterCreationLevel = ((info.Level>CharacterCreationLevel)?info.Level:CharacterCreationLevel);

		info.CtlCode = lpInfo->CtlCode;

		#pragma region CHAR_SET_CALC

		memset(info.CharSet,0,sizeof(info.CharSet));

		info.CharSet[0] = (lpInfo->Class%16)*16;
		info.CharSet[0] -= (info.CharSet[0]/32);
		info.CharSet[0] += (lpInfo->Class/16)*32;

		WORD Inventory[9];

		for(int i=0;i < 9;i++)
		{
			if(i == 0 || i == 1)
			{
				if(lpInfo->Inventory[0+(i*5)] == 0xFF && (lpInfo->Inventory[2+(i*5)] & 0x80) == 0x80 && (lpInfo->Inventory[4+(i*5)] & 0xF0) == 0xF0)
				{
					Inventory[i] = 0xFFFF;
				}
				else
				{
					Inventory[i] = (lpInfo->Inventory[0+(i*5)]+((lpInfo->Inventory[2+(i*5)] & 0x80)*2)+((lpInfo->Inventory[4+(i*5)] & 0xF0)*32));
				}
			}
			else
			{
				if(lpInfo->Inventory[0+(i*5)] == 0xFF && (lpInfo->Inventory[2+(i*5)] & 0x80) == 0x80 && (lpInfo->Inventory[4+(i*5)] & 0xF0) == 0xF0)
				{
					Inventory[i] = (MAX_ITEM_TYPE-1);
				}
				else
				{
					Inventory[i] = (lpInfo->Inventory[0+(i*5)]+((lpInfo->Inventory[2+(i*5)] & 0x80)*2)+((lpInfo->Inventory[4+(i*5)] & 0xF0)*32))%MAX_ITEM_TYPE;
				}
			}
		}
		
		info.CharSet[1] = Inventory[INVENTORY_SLOT_WEAPON1]%256;

		info.CharSet[2] = Inventory[INVENTORY_SLOT_WEAPON2]%256;

		info.CharSet[3] |= (Inventory[INVENTORY_SLOT_HELM] & 0x0F) << 4;
		info.CharSet[9] |= (Inventory[INVENTORY_SLOT_HELM] & 0x10) << 3;

		info.CharSet[3] |= (Inventory[INVENTORY_SLOT_ARMOR] & 0x0F);
		info.CharSet[9] |= (Inventory[INVENTORY_SLOT_ARMOR] & 0x10) << 2;

		info.CharSet[4] |= (Inventory[INVENTORY_SLOT_PANTS] & 0x0F) << 4;
		info.CharSet[9] |= (Inventory[INVENTORY_SLOT_PANTS] & 0x10) << 1;

		info.CharSet[4] |= (Inventory[INVENTORY_SLOT_GLOVES] & 0x0F);
		info.CharSet[9] |= (Inventory[INVENTORY_SLOT_GLOVES] & 0x10);

		info.CharSet[5] |= (Inventory[INVENTORY_SLOT_BOOTS] & 0x0F) << 4;
		info.CharSet[9] |= (Inventory[INVENTORY_SLOT_BOOTS] & 0x10) >> 1;

		int level = 0;

		BYTE table[7] = {1,0,6,5,4,3,2};

		for(int i=0;i < 7;i++)
		{
			if(Inventory[i] != (MAX_ITEM_TYPE-1) && Inventory[i] != 0xFFFF)
			{
				level |= LevelSmallConvert(((lpInfo->Inventory[1+(i*5)]/8) & 0x0F)) << (i * 3);

				info.CharSet[10] |= ((lpInfo->Inventory[2+(i*5)] & 0x3F)?2:0) << table[i];
			}
		}

		info.CharSet[6] = level >> 16;
		info.CharSet[7] = level >> 8;
		info.CharSet[8] = level;

		if(Inventory[INVENTORY_SLOT_WING] == (MAX_ITEM_TYPE-1))
		{
			info.CharSet[5] |= 12;
		}
		else if(Inventory[INVENTORY_SLOT_WING] >= 0 && Inventory[INVENTORY_SLOT_WING] <= 2)
		{
			info.CharSet[5] |= Inventory[INVENTORY_SLOT_WING] << 2;
		}
		else if(Inventory[INVENTORY_SLOT_WING] >= 3 && Inventory[INVENTORY_SLOT_WING] <= 6)
		{
			info.CharSet[5] |= 12;
			info.CharSet[9] |= Inventory[INVENTORY_SLOT_WING]-2;
		}
		else if(Inventory[INVENTORY_SLOT_WING] == 30)
		{
			info.CharSet[5] |= 12;
			info.CharSet[9] |= 5;
		}

		if(Inventory[INVENTORY_SLOT_HELPER] == (MAX_ITEM_TYPE-1))
		{
			info.CharSet[5] |= 3;
		}
		else if(Inventory[INVENTORY_SLOT_HELPER] >= 0 && Inventory[INVENTORY_SLOT_HELPER] <= 2)
		{
			info.CharSet[5] |= Inventory[INVENTORY_SLOT_HELPER];
		}
		else if(Inventory[INVENTORY_SLOT_HELPER] == 3)
		{
			info.CharSet[5] |= 3;
			info.CharSet[10] |= 1;
		}

		#pragma endregion

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	int ClassCode = 2;

	gObj[lpMsg->index].ClassCode = 0;

	if(CharacterCreationLevel >= gServerInfo.m_MGCreateLevel[gObj[lpMsg->index].AccountLevel])
	{
		ClassCode = 3;

		gObj[lpMsg->index].ClassCode |= CARD_CODE_MG;
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpMsg->index,send,size);

	GCCharacterCreationEnableSend(lpMsg->index,0,ClassCode);
}

void DGCharacterCreateRecv(SDHP_CHARACTER_CREATE_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCharacterCreateRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	PMSG_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0xF3,0x01,sizeof(pMsg));

	pMsg.result = lpMsg->result;

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	pMsg.slot = lpMsg->slot;

	memcpy(pMsg.equipment,lpMsg->equipment,sizeof(pMsg.equipment));

	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);
}

void DGCharacterDeleteRecv(SDHP_CHARACTER_DELETE_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCharacterDeleteRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	PMSG_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0xF3,0x02,sizeof(pMsg));

	pMsg.result = lpMsg->result;
	
	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);
}

void DGCharacterInfoRecv(SDHP_CHARACTER_INFO_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCharacterInfoRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	if(lpMsg->result == 0)
	{
		CloseClient(lpMsg->index);
		return;
	}

	if((lpMsg->CtlCode & 1) != 0)
	{
		CloseClient(lpMsg->index);
		return;
	}

	if(gObjectManager.CharacterInfoSet((BYTE*)lpMsg,lpMsg->index) == 0)
	{
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	PMSG_CHARACTER_INFO_SEND pMsg;

	pMsg.header.setE(0xF3,0x03,sizeof(pMsg));

	pMsg.X = (BYTE)lpObj->X;
	pMsg.Y = (BYTE)lpObj->Y;
	pMsg.Map = lpObj->Map;
	pMsg.Dir = lpObj->Dir;
	pMsg.Experience = lpObj->Experience;
	pMsg.NextExperience = lpObj->NextExperience;
	pMsg.LevelUpPoint = lpObj->LevelUpPoint;
	pMsg.Strength = GET_MAX_WORD_VALUE(lpObj->Strength);
	pMsg.Dexterity = GET_MAX_WORD_VALUE(lpObj->Dexterity);
	pMsg.Vitality = GET_MAX_WORD_VALUE(lpObj->Vitality);
	pMsg.Energy = GET_MAX_WORD_VALUE(lpObj->Energy);
	pMsg.Life = GET_MAX_WORD_VALUE(lpObj->Life);
	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife+lpObj->AddLife));
	pMsg.Mana = GET_MAX_WORD_VALUE(lpObj->Mana);
	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana+lpObj->AddMana));
	pMsg.BP = GET_MAX_WORD_VALUE(lpObj->BP);
	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP+lpObj->AddBP));
	pMsg.Money = lpObj->Money;
	pMsg.PKLevel = lpObj->PKLevel;
	pMsg.CtlCode = lpMsg->CtlCode;
	pMsg.FruitPoint = lpObj->FruitPoint;
	pMsg.MaxFruitPoint = gFruit.GetMaxFruitPoint(lpObj);
	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewReset = (DWORD)(lpObj->Reset);
	pMsg.ViewPoint = (DWORD)(lpObj->LevelUpPoint);
	pMsg.ViewCurHP = (DWORD)(lpObj->Life);
	pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife+lpObj->AddLife);
	pMsg.ViewCurMP = (DWORD)(lpObj->Mana);
	pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana+lpObj->AddMana);
	pMsg.ViewCurBP = (DWORD)(lpObj->BP);
	pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP+lpObj->AddBP);
	pMsg.ViewStrength = lpObj->Strength;
	pMsg.ViewDexterity = lpObj->Dexterity;
	pMsg.ViewVitality = lpObj->Vitality;
	pMsg.ViewEnergy = lpObj->Energy;
	// Update CMZ 15 (3.1.5) 26-09-26 - Set AccountLevel in Character Info
	pMsg.ViewAccountLevel = (DWORD)(lpObj->AccountLevel);
	#endif

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GDConnectCharacterSend(lpObj->Index);

	GDOptionDataSend(lpObj->Index);

	gItemManager.GCItemListSend(lpObj->Index);

	gSkillManager.GCSkillListSend(lpObj);

	gQuest.GCQuestInfoSend(lpObj->Index);

	gPeriodicItem.GCPeriodicItemInit(lpObj->Index);

	gPeriodicItem.GDPeriodicItemListSend(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	gReconnect.GetReconnectInfo(lpObj);

	gSkillManager.SkillChangeUse(lpObj->Index);

	gObjViewportListProtocolCreate(lpObj);
	
	gCustomPick.GDCustomPickSend(lpObj->Index);

	gItemStack.GCItemListStackSend(lpObj->Index);

	gItemValue.GCItemListValueSend(lpObj->Index);

	gScriptLoader.OnCharacterEntry(lpObj->Index);

	// Update 93 2.5.1 -> 97K - Presentes iniciais para novos personagens
	gDefaultClassFreebies.GetCharacterFreebies(lpObj,lpMsg->IsNewChar);

	// Update 91 2.4.9 -> 97K - Inicializacao de rastreamento de movimento no login
	gHackMoveSpeedCheck.Reset(lpObj);

	// Update 88 2.4.6 -> 97K - Sistema de recompensas diarias
	gCustomDailyReward.GDDailyRewardCheckSend(lpObj->Index);

	gLog.Output(LOG_CONNECT,"[ObjectManager][%d] AddCharacterInfo [%s] [%s][%s]",lpObj->Index,lpObj->Name,lpObj->IpAddr,lpObj->HardwareId);
}

void DGCreateItemRecv(SDHP_CREATE_ITEM_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCreateItemRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpMsg->Map != 0xFF && lpMsg->Map != 0xFE && lpMsg->Map != 0xEB && gMapManager.IsValidMap(lpMsg->Map) == 0)
	{
		return;
	}

	if(lpMsg->Map == 0xFE || lpMsg->Map == 0xFF)
	{
		if((lpMsg->Map == 0xFE && lpObj->Interface.type != INTERFACE_TRAINER) || (lpMsg->Map == 0xFF && lpObj->Interface.type != INTERFACE_CHAOS_BOX))
		{
			return;
		}

		CItem item;

		item.m_Level = lpMsg->Level;

		item.m_Serial = lpMsg->Serial;

		if(lpMsg->ItemIndex == GET_ITEM(13,18) || lpMsg->ItemIndex == GET_ITEM(14,7) || lpMsg->ItemIndex == GET_ITEM(14,19))
		{
			item.m_Durability = (float)lpMsg->Dur;
		}
		else
		{
			item.m_Durability = (float)gItemManager.GetItemDurability(lpMsg->ItemIndex,lpMsg->Level,lpMsg->NewOption,lpMsg->SetOption);
		}

		item.Convert(lpMsg->ItemIndex,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption);

		if(lpMsg->ItemIndex == GET_ITEM(13,4) || lpMsg->ItemIndex == GET_ITEM(13,5))
		{
			item.SetPetItemInfo(1,0);
		}

		item.m_IsPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_LoadPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_PeriodicItemTime = ((lpMsg->Duration>0)?lpMsg->Duration:0);

		gChaosBox.ChaosBoxInit(lpObj);

		gItemManager.ChaosBoxAddItem(lpObj->Index,item,0);

		gChaosBox.GCChaosMixSend(lpObj->Index,((lpMsg->Map==0xFF)?1:100),&item);
	}
	else if(lpMsg->Map == 0xEB)
	{
		CItem item;

		item.m_Level = lpMsg->Level;

		item.m_Serial = lpMsg->Serial;

		item.m_Durability = lpMsg->Dur;

		item.Convert(lpMsg->ItemIndex,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption);

		if(lpMsg->ItemIndex == GET_ITEM(13,4) || lpMsg->ItemIndex == GET_ITEM(13,5))
		{
			item.SetPetItemInfo(1,0);
		}

		item.m_IsPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_LoadPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_PeriodicItemTime = ((lpMsg->Duration>0)?lpMsg->Duration:0);

		BYTE slot = gItemManager.InventoryInsertItem(lpObj->Index,item);

		if(slot != 0xFF)
		{
			gItemManager.GCItemModifySend(lpObj->Index,slot);

			if(lpMsg->Duration > 0)
			{
				gPeriodicItem.GCPeriodicItemSend(lpObj->Index,-1,slot,item.m_PeriodicItemTime);
			}
		}
	}
	else
	{
		if(gMap[lpMsg->Map].MonsterItemDrop(lpMsg->ItemIndex,lpMsg->Level,lpMsg->Dur,lpMsg->X,lpMsg->Y,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption,lpMsg->LootIndex,lpMsg->Serial,lpMsg->Duration) != 0)
		{
			if(BC_MAP_RANGE(lpMsg->Map) != 0)
			{
				gBloodCastle.SetEventItemSerial(lpMsg->Map,lpMsg->ItemIndex,lpMsg->Serial);
			}
		}
	}
}

void DGPetItemInfoRecv(SDHP_PET_ITEM_INFO_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGPetItemInfoRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpMsg->type == 0)
	{
		for(int n=0;n < lpMsg->count;n++)
		{
			SDHP_PET_ITEM_INFO1* lpInfo = (SDHP_PET_ITEM_INFO1*)(((BYTE*)lpMsg)+sizeof(SDHP_PET_ITEM_INFO_RECV)+(sizeof(SDHP_PET_ITEM_INFO1)*n));

			if(INVENTORY_RANGE(lpInfo->slot) == 0)
			{
				continue;
			}

			if(lpObj->Inventory[lpInfo->slot].IsItem() == 0)
			{
				continue;
			}

			if(lpObj->Inventory[lpInfo->slot].m_Serial == lpInfo->serial)
			{
				lpObj->Inventory[lpInfo->slot].SetPetItemInfo(lpInfo->level,lpInfo->experience);

				if(INVENTORY_WEAR_RANGE(lpInfo->slot) != 0)
				{
					gObjectManager.CharacterCalcAttribute(lpObj->Index);
				}
			}
		}
	}
	
	if(lpMsg->type == 1)
	{
		for(int n=0;n < lpMsg->count;n++)
		{
			SDHP_PET_ITEM_INFO1* lpInfo = (SDHP_PET_ITEM_INFO1*)(((BYTE*)lpMsg)+sizeof(SDHP_PET_ITEM_INFO_RECV)+(sizeof(SDHP_PET_ITEM_INFO1)*n));

			if(WAREHOUSE_RANGE(lpInfo->slot) == 0)
			{
				continue;
			}

			if(lpObj->Warehouse[lpInfo->slot].IsItem() == 0)
			{
				continue;
			}

			if(lpObj->Warehouse[lpInfo->slot].m_Serial == lpInfo->serial)
			{
				lpObj->Warehouse[lpInfo->slot].SetPetItemInfo(lpInfo->level,lpInfo->experience);
			}
		}
	}
}

void DGOptionDataRecv(SDHP_OPTION_DATA_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGOptionDataRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	PMSG_OPTION_DATA_SEND pMsg;

	pMsg.header.set(0xF3,0x30,sizeof(pMsg));

	memcpy(pMsg.SkillKey,lpMsg->SkillKey,sizeof(pMsg.SkillKey));

	pMsg.GameOption = lpMsg->GameOption;

	pMsg.QKey = lpMsg->QKey;

	pMsg.WKey = lpMsg->WKey;

	pMsg.EKey = lpMsg->EKey;

	pMsg.ChatWindow = lpMsg->ChatWindow;

	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);
}

void DGGlobalPostRecv(SDHP_GLOBAL_POST_RECV* lpMsg) // OK
{
	char buff[256] = {0};

	wsprintf(buff,gMessage.GetMessage(323),lpMsg->message);

	switch(lpMsg->type)
	{
		case 0:
			PostMessage1(lpMsg->name,buff,"");
			break;
		case 1:
			PostMessage2(lpMsg->name,buff,"");
			break;
		case 2:
			PostMessage3(lpMsg->name,buff,"");
			break;
		case 3:
			GCNewMessageSendToAll(2,0,"%s: %s",lpMsg->name,buff);
			break;
		case 4:
			GCNewMessageSendToAll(2,1,"%s: %s",lpMsg->name,buff);
			break;
		case 5:
			GCNewMessageSendToAll(2,2,"%s: %s",lpMsg->name,buff);
			break;
		case 6:
			GCNewMessageSendToAll(2,3,"%s: %s",lpMsg->name,buff);
			break;
		case 7:
			GCNewMessageSendToAll(2,4,"%s: %s",lpMsg->name,buff);
			break;
		case 8:
			GCNewMessageSendToAll(2,5,"%s: %s",lpMsg->name,buff);
			break;
		default:
			GCNewMessageSendToAll(2,6,"%s: %s",lpMsg->name,buff);
			break;
	}
}

void DGGlobalNoticeRecv(SDHP_GLOBAL_NOTICE_RECV* lpMsg) // OK
{
	gNotice.GCNoticeSendToAll(lpMsg->type,lpMsg->count,lpMsg->opacity,lpMsg->delay,lpMsg->color,lpMsg->speed,"%s",lpMsg->message);
}

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 96 (2.5.4) - Processamento de recebimento de mensagem global
void DGGlobalMessageRecv(SDHP_GLOBAL_MESSAGE_RECV* lpMsg) // OK
{
	GCNewMessageSendToAll(lpMsg->type,lpMsg->color,"%s",lpMsg->message);
}

void DGGlobalWhisperRecv(SDHP_GLOBAL_WHISPER_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGGlobalWhisperRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	if(lpMsg->result == 0)
	{
		GCServerMsgSend(lpMsg->index,0);
		return;
	}

	lpMsg->message[(sizeof(lpMsg->message)-1)] = ((strlen(lpMsg->message)>(sizeof(lpMsg->message)-1))?0:lpMsg->message[(sizeof(lpMsg->message)-1)]);

	gLog.Output(LOG_CHAT,"[Whisper][%s][%s] - (Name: %s,Message: %s)",lpMsg->account,lpMsg->name,lpMsg->TargetName,lpMsg->message);
}

void DGGlobalWhisperEchoRecv(SDHP_GLOBAL_WHISPER_ECHO_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGGlobalWhisperEchoRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	lpMsg->message[(sizeof(lpMsg->message)-1)] = ((strlen(lpMsg->message)>(sizeof(lpMsg->message)-1))?0:lpMsg->message[(sizeof(lpMsg->message)-1)]);

	// Update 89 2.4.7 -> 97K - Mascaramento de palavras censuradas com asteriscos
	gFilter.CheckSyntax(lpMsg->message);

	GCChatWhisperSend(lpMsg->index,lpMsg->SourceName,lpMsg->message);
}

void GDServerInfoSend() // OK
{
	SDHP_DATA_SERVER_INFO_SEND pMsg;

	pMsg.header.set(0x00,sizeof(pMsg));

	memcpy(pMsg.ServerName,gServerInfo.m_ServerName,sizeof(pMsg.ServerName));

	pMsg.ServerPort = (WORD)gServerInfo.m_ServerPort;

	pMsg.ServerCode = (WORD)gServerInfo.m_ServerCode;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDCharacterListSend(int aIndex) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_LIST_SEND pMsg;

	pMsg.header.set(0x01,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDCharacterCreateSend(int aIndex,char* name,BYTE Class) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0x02,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	pMsg.Class = Class;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDCharacterDeleteSend(int aIndex,char* name) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0x03,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	pMsg.guild = 0;

	if(gObj[aIndex].Guild != 0)
	{
		if(strcmp(gObj[aIndex].Name,gObj[aIndex].Guild->Master) == 0)
		{
			pMsg.guild = 1; // Guild Master
		}
		else
		{
			pMsg.guild = 2; // Guild Member
		}

		memcpy(pMsg.GuildName,gObj[aIndex].Guild->Name,sizeof(pMsg.GuildName));
	}

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDCharacterInfoSend(int aIndex,char* name) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_INFO_SEND pMsg;

	pMsg.header.set(0x04,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDCreateItemSend(int aIndex,BYTE map,BYTE x,BYTE y,int index,BYTE level,BYTE dur,BYTE Option1,BYTE Option2,BYTE Option3,int LootIndex,BYTE NewOption,BYTE SetOption,DWORD duration) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	if(gItemManager.CheckItemIndex(index) == 0)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if(gItemManager.GetInfo(index,&ItemInfo) == 0)
	{
		return;
	}

	dur = ((dur==0)?gItemManager.GetItemDurability(index,level,NewOption,SetOption):dur);

	Option1 = ((gItemManager.GetItemSkill(index) == 0) ? 0 : Option1);

	if(ItemInfo.HaveSerial == 0 && map < MAX_MAP)
	{
		gMap[map].MonsterItemDrop(index,level,dur,x,y,Option1,Option2,Option3,NewOption,SetOption,LootIndex,0,duration);
		return;
	}

	if(map == 0xEB || map == 0xEC || map == 0xED || map == 0xFE || map == 0xFF || index == GET_ITEM(13,19))
	{
		SDHP_CREATE_ITEM_SEND pMsg;

		pMsg.header.set(0x07,sizeof(pMsg));

		pMsg.index = aIndex;

		memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

		pMsg.X = x;
		pMsg.Y = y;
		pMsg.Map = map;
		pMsg.ItemIndex = index;
		pMsg.Level = level;
		pMsg.Dur = dur;
		pMsg.Option1 = Option1;
		pMsg.Option2 = Option2;
		pMsg.Option3 = Option3;
		pMsg.NewOption = NewOption;
		pMsg.LootIndex = LootIndex;
		pMsg.SetOption = SetOption;
		pMsg.Duration = duration;

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
	}
	else
	{
		gMap[map].MonsterItemDrop(index,level,dur,x,y,Option1,Option2,Option3,NewOption,SetOption,LootIndex,-1,duration);
	}
}

void GDOptionDataSend(int aIndex) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_OPTION_DATA_SEND pMsg;

	pMsg.header.set(0x08,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDPetItemInfoSend(int aIndex,int type) // OK
{
	
}

void GDGlobalPostSend(BYTE type,char* name,char* message) // OK
{
	SDHP_GLOBAL_POST_SEND pMsg;

	pMsg.header.set(0x20,sizeof(pMsg));

	pMsg.type = type;

	pMsg.code = gServerInfo.m_ServerCode;

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,message,sizeof(pMsg.message));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}

void GDGlobalNoticeSend(BYTE type,BYTE count,BYTE opacity,WORD delay,DWORD color,BYTE speed,char* message,...) // OK
{
	char buff[256] = {0};

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	SDHP_GLOBAL_NOTICE_SEND pMsg;

	pMsg.header.set(0x21,sizeof(pMsg));

	pMsg.type = type;

	pMsg.count = count;

	pMsg.opacity = opacity;

	pMsg.delay = delay;

	pMsg.color = color;

	pMsg.speed = speed;

	memcpy(pMsg.message,buff,sizeof(pMsg.message));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 96 (2.5.4) - Envio de mensagem global para o DataServer
void GDGlobalMessageSend(int MapServerGroup,BYTE type,BYTE color,char* message,...) // OK
{
	char buff[256] = {0};

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	SDHP_GLOBAL_MESSAGE_SEND pMsg;

	pMsg.header.set(0x2B,sizeof(pMsg));

	pMsg.MapServerGroup = MapServerGroup;

	pMsg.type = type;

	pMsg.color = color;

	memcpy(pMsg.message,buff,sizeof(pMsg.message));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}

void GDCharacterInfoSaveSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_CHARACTER_INFO_SAVE_SEND pMsg;

	pMsg.header.set(0x30,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.Level = lpObj->Level;
	pMsg.Class = lpObj->DBClass;
	pMsg.LevelUpPoint = lpObj->LevelUpPoint;
	pMsg.Experience = lpObj->Experience;
	pMsg.Money = lpObj->Money;
	pMsg.Strength = lpObj->Strength;
	pMsg.Dexterity = lpObj->Dexterity;
	pMsg.Vitality = lpObj->Vitality;
	pMsg.Energy = lpObj->Energy;
	pMsg.Life = (DWORD)lpObj->Life;
	pMsg.MaxLife = (DWORD)lpObj->MaxLife;
	pMsg.Mana = (DWORD)lpObj->Mana;
	pMsg.MaxMana = (DWORD)lpObj->MaxMana;
	pMsg.BP = (DWORD)lpObj->BP;
	pMsg.MaxBP = (DWORD)lpObj->MaxBP;

	for(int n=0;n < INVENTORY_SIZE;n++){gItemManager.DBItemByteConvert(pMsg.Inventory[n],&lpObj->Inventory[n]);}

	for(int n=0;n < MAX_SKILL_LIST;n++){gSkillManager.SkillByteConvert(pMsg.Skill[n],&lpObj->Skill[n]);}

	pMsg.Map = lpObj->Map;
	pMsg.X = (BYTE)lpObj->X;
	pMsg.Y = (BYTE)lpObj->Y;
	pMsg.Dir = lpObj->Dir;
	pMsg.PKCount = lpObj->PKCount;
	pMsg.PKLevel = lpObj->PKLevel;
	pMsg.PKTime = lpObj->PKTime;

	memcpy(pMsg.Quest,lpObj->Quest,sizeof(pMsg.Quest));

	pMsg.ChatLimitTime = lpObj->ChatLimitTime;

	pMsg.FruitPoint = lpObj->FruitPoint;

	for(int n=0;n < MAX_EFFECT_LIST;n++){gEffectManager.EffectByteConvert(pMsg.Effect[n],&lpObj->Effect[n]);}

	pMsg.BCCount = lpObj->BCCount;
	pMsg.CCCount = lpObj->CCCount;
	pMsg.DSCount = lpObj->DSCount;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));

	gWarehouse.GDWarehouseItemSaveSend(aIndex);

	gPeriodicItem.GDPeriodicItemSaveSend(aIndex);

	gCustomPick.GDCustomPickSaveSend(aIndex);
}

void GDInventoryItemSaveSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_INVENTORY_ITEM_SAVE_SEND pMsg;

	pMsg.header.set(0x31,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	for(int n=0;n < INVENTORY_SIZE;n++){gItemManager.DBItemByteConvert(pMsg.Inventory[n],&lpObj->Inventory[n]);}

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}

void GDOptionDataSaveSend(int aIndex,BYTE* SkillKey,BYTE GameOption,BYTE QKey,BYTE WKey,BYTE EKey,BYTE ChatWindow) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_OPTION_DATA_SAVE_SEND pMsg;

	pMsg.header.set(0x33,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	memcpy(pMsg.SkillKey,SkillKey,sizeof(pMsg.SkillKey));

	pMsg.GameOption = GameOption;

	pMsg.QKey = QKey;

	pMsg.WKey = WKey;

	pMsg.EKey = EKey;

	pMsg.ChatWindow = ChatWindow;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDPetItemInfoSaveSend(int aIndex,int type) // OK
{
}

void GDResetInfoSaveSend(int aIndex,int ResetDay,int ResetWek,int ResetMon) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RESET_INFO_SAVE_SEND pMsg;

	pMsg.header.set(0x39,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.Reset = lpObj->Reset;

	pMsg.ResetDay = ResetDay;

	pMsg.ResetWek = ResetWek;

	pMsg.ResetMon = ResetMon;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDMasterResetInfoSaveSend(int aIndex,int MasterResetDay,int MasterResetWek,int MasterResetMon) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_MASTER_RESET_INFO_SAVE_SEND pMsg;

	pMsg.header.set(0x3A,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.Reset = lpObj->Reset;

	pMsg.MasterReset = lpObj->MasterReset;

	pMsg.MasterResetDay = MasterResetDay;

	pMsg.MasterResetWek = MasterResetWek;

	pMsg.MasterResetMon = MasterResetMon;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDRankingDuelSaveSend(int aIndex,DWORD WinScore,DWORD LoseScore) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RANKING_DUEL_SAVE_SEND pMsg;

	pMsg.header.set(0x3C,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.WinScore = WinScore;

	pMsg.LoseScore = LoseScore;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDRankingBloodCastleSaveSend(int aIndex,DWORD Score) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RANKING_BLOOD_CASTLE_SAVE_SEND pMsg;

	pMsg.header.set(0x3D,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.score = Score;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDRankingChaosCastleSaveSend(int aIndex,DWORD Score) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RANKING_CHAOS_CASTLE_SAVE_SEND pMsg;

	pMsg.header.set(0x3E,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.score = Score;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDRankingDevilSquareSaveSend(int aIndex,DWORD Score) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RANKING_DEVIL_SQUARE_SAVE_SEND pMsg;

	pMsg.header.set(0x3F,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.score = Score;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GDConnectCharacterSend(int aIndex) // OK
{
	SDHP_CONNECT_CHARACTER_SEND pMsg;

	pMsg.header.set(0x70,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}

void GDDisconnectCharacterSend(int aIndex) // OK
{
	SDHP_DISCONNECT_CHARACTER_SEND pMsg;

	pMsg.header.set(0x71,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}

void GDGlobalWhisperSend(int aIndex,char* TargetName,char* message) // OK
{
	SDHP_GLOBAL_WHISPER_SEND pMsg;

	pMsg.header.set(0x72,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	memcpy(pMsg.TargetName,TargetName,sizeof(pMsg.TargetName));

	memcpy(pMsg.message,message,sizeof(pMsg.message));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}