// ChaosBox.cpp: implementation of the CChaosBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaosBox.h"
#include "BonusManager.h"
#include "DSProtocol.h"
#include "ItemBagManager.h"
#include "ItemOptionRate.h"
#include "Log.h"
#include "Notice.h"
#include "RandomManager.h"
#include "ServerInfo.h"
#include "Shop.h"
#include "Util.h"

CChaosBox gChaosBox;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChaosBox::CChaosBox() // OK
{
	
}

CChaosBox::~CChaosBox() // OK
{

}

void CChaosBox::ChaosBoxInit(LPOBJ lpObj) // OK
{
	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		lpObj->ChaosBox[n].Clear();
		lpObj->ChaosBoxMap[n] = 0xFF;
	}
}

void CChaosBox::ChaosBoxItemDown(LPOBJ lpObj,int slot) // OK
{
	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(n != slot)
		{
			lpObj->ChaosBox[n].Clear();
			lpObj->ChaosBoxMap[n] = 0xFF;
			continue;
		}

		CItem* lpItem = &lpObj->ChaosBox[n];
		
		lpItem->m_Level = 0;

		float dur = (float)gItemManager.GetItemDurability(lpItem->m_Index,lpItem->m_Level,lpItem->IsExcItem(),lpItem->IsSetItem());

		lpItem->m_Durability = dur*(lpItem->m_Durability/lpItem->m_BaseDurability);

		lpItem->Convert(lpItem->m_Index,lpItem->m_Option1,lpItem->m_Option2,lpItem->m_Option3,lpItem->m_NewOption,lpItem->m_SetOption);
	}
}

void CChaosBox::ChaosBoxItemKeep(LPOBJ lpObj,int slot) // OK
{
	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(n != slot)
		{
			lpObj->ChaosBox[n].Clear();
			lpObj->ChaosBoxMap[n] = 0xFF;
			continue;
		}
	}
}

void CChaosBox::ChaosBoxItemSave(LPOBJ lpObj) // OK
{
	if(gObjInventoryCommit(lpObj->Index) != 0)
	{
		for(int n=0;n < CHAOS_BOX_SIZE;n++)
		{
			if(lpObj->ChaosBox[n].IsItem() != 0)
			{
				gItemManager.InventoryInsertItem(lpObj->Index,lpObj->ChaosBox[n]);
			}
		}
	}
}

void CChaosBox::ChaosItemMix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int ItemCount = 0;
	int ItemMoney = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
			lpObj->ChaosBox[n].OldValue();
			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13) || lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
		{
			lpObj->ChaosBox[n].OldValue();
			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if(lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_Option3 >= 1)
		{
			ItemCount++;
			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
	}

	if(ChaosCount == 0 || ItemCount == 0)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	if(gServerInfo.m_ChaosItemMixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = (ItemMoney/20000);
	}
	else
	{
		lpObj->ChaosSuccessRate = gServerInfo.m_ChaosItemMixRate[lpObj->AccountLevel];
	}

	lpObj->ChaosSuccessRate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,lpObj->ChaosSuccessRate);

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = lpObj->ChaosSuccessRate*10000;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		CItem item;

		if(gItemBagManager.GetItemBySpecialValue(ITEM_BAG_SPECIAL_CHAOS_BOX,0,-1,lpObj,&item) == 0)
		{
			this->GCChaosMixSend(lpObj->Index,0,0);
			return;
		}

		GDCreateItemSend(lpObj->Index,0xFF,0,0,item.m_Index,(BYTE)item.m_Level,(BYTE)item.m_Durability,item.m_Option1,item.m_Option2,item.m_Option3,-1,item.m_NewOption,item.m_SetOption,0);

		gLog.Output(LOG_CHAOS_MIX,"[ChaosItemMix][Success][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[ChaosItemMix][Failure][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::DevilSquareMix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int EyeCount = 0;
	int EyeLevel = 0;
	int KeyCount = 0;
	int KeyLevel = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,17))
		{
			EyeCount++;
			EyeLevel = lpObj->ChaosBox[n].m_Level;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,18))
		{
			KeyCount++;
			KeyLevel = lpObj->ChaosBox[n].m_Level;
		}
	}

	if(ChaosCount != 1 || EyeCount != 1 || KeyCount != 1 || EyeLevel != KeyLevel)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	if(EyeLevel > 4 || KeyLevel > 4)
	{
		this->GCChaosMixSend(lpObj->Index,8,0);
		return;
	}

	switch(EyeLevel)
	{
		case 1:
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate1[lpObj->AccountLevel];
			lpObj->ChaosMoney = 100000;
			break;
		case 2:
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate2[lpObj->AccountLevel];
			lpObj->ChaosMoney = 200000;
			break;
		case 3:
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate3[lpObj->AccountLevel];
			lpObj->ChaosMoney = 400000;
			break;
		case 4:
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate4[lpObj->AccountLevel];
			lpObj->ChaosMoney = 700000;
			break;
	}

	lpObj->ChaosSuccessRate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,lpObj->ChaosSuccessRate);

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		GDCreateItemSend(lpObj->Index,0xFF,0,0,GET_ITEM(14,19),EyeLevel,1,0,0,0,-1,0,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[DevilSquareMix][Success][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[DevilSquareMix][Failure][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::PlusItemLevelMix(LPOBJ lpObj,int type) // OK
{
	int ChaosCount = 0;
	int BlessCount = 0;
	int SoulCount = 0;
	int ItemCount = 0;
	int ItemSlot = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13))
		{
			BlessCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
		{
			SoulCount++;
		}
		else if(lpObj->ChaosBox[n].m_Level == (9+type))
		{
			ItemCount++;
			ItemSlot = n;
		}
	}

	if(ChaosCount != 1 || SoulCount < (type+1) || BlessCount < (type+1) || ItemCount != 1)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	if(lpObj->ChaosBox[ItemSlot].IsExcItem() != 0 || lpObj->ChaosBox[ItemSlot].IsSetItem() != 0)
	{
		lpObj->ChaosSuccessRate = gServerInfo.m_PlusExcSetItemLevelMixRate[type][lpObj->AccountLevel];
	}
	else
	{
		lpObj->ChaosSuccessRate = gServerInfo.m_PlusCommonItemLevelMixRate[type][lpObj->AccountLevel];
	}

	if(lpObj->ChaosBox[ItemSlot].m_Option2 != 0)
	{
		lpObj->ChaosSuccessRate += gServerInfo.m_AddLuckSuccessRate2[lpObj->AccountLevel];
	}

	lpObj->ChaosSuccessRate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,lpObj->ChaosSuccessRate);

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = 2000000*(type+1);

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		CItem item = lpObj->ChaosBox[ItemSlot];

		item.m_Level++;

		item.m_Durability = (float)gItemManager.GetItemDurability(item.m_Index,item.m_Level,item.IsExcItem(),item.IsSetItem());

		item.m_Durability = (item.m_Durability*lpObj->ChaosBox[ItemSlot].m_Durability)/item.m_BaseDurability;

		this->ChaosBoxInit(lpObj);

		gItemManager.ChaosBoxAddItem(lpObj->Index,item,0);

		this->GCChaosMixSend(lpObj->Index,1,&item);

		// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 101 (2.6.1) - Chamada com item.m_Index e item.m_Level
		gObjCustomLogPlusChaosMix(lpObj,item.m_Index,item.m_Level);

		gLog.Output(LOG_CHAOS_MIX,"[PlusItemLevelMix][Success][%s][%s] - (Type: %d,ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[PlusItemLevelMix][Failure][%s][%s] - (Type: %d,ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::DinorantMix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int UniriaCount = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,2) && lpObj->ChaosBox[n].m_Durability == 255)
		{
			UniriaCount++;
		}
	}

	if(ChaosCount != 1 || UniriaCount != 10)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	lpObj->ChaosSuccessRate = gServerInfo.m_DinorantMixRate[lpObj->AccountLevel];

	lpObj->ChaosSuccessRate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,lpObj->ChaosSuccessRate);

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = 500000;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		WORD ItemIndex = GET_ITEM(13,3);
		BYTE ItemNewOption = 0;

		gItemOptionRate.GetItemOption4(3,&ItemNewOption);

		gItemOptionRate.MakeNewOption(ItemIndex,1,ItemNewOption,&ItemNewOption);

		GDCreateItemSend(lpObj->Index,0xFF,0,0,ItemIndex,0,255,0,0,0,-1,ItemNewOption,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[DinorantMix][Success][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[DinorantMix][Failure][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::FruitMix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int CreationCount = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,22))
		{
			CreationCount++;
		}
	}

	if(ChaosCount != 1 || CreationCount != 1)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	lpObj->ChaosSuccessRate = gServerInfo.m_FruitMixRate[lpObj->AccountLevel];

	lpObj->ChaosSuccessRate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,lpObj->ChaosSuccessRate);

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = 3000000;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		BYTE ItemLevel = GetLargeRand()%4;

		GDCreateItemSend(lpObj->Index,0xFF,0,0,GET_ITEM(13,15),ItemLevel,0,0,0,0,-1,0,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[FruitMix][Success][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[FruitMix][Failure][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::Wing2Mix(LPOBJ lpObj,int type) // OK
{
	int ChaosCount = 0;
	int FeatherCount = 0;
	int SleeveCount = 0;
	int WingItemCount = 0;
	int WingItemMoney = 0;
	int ItemCount = 0;
	int ItemMoney = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,14) && lpObj->ChaosBox[n].m_Level == 0)
		{
			FeatherCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index >= GET_ITEM(12,0) && lpObj->ChaosBox[n].m_Index <= GET_ITEM(12,2))
		{
			WingItemCount++;
			WingItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
		else if(lpObj->ChaosBox[n].IsExcItem() != 0 && lpObj->ChaosBox[n].m_Level >= 4)
		{
			ItemCount++;
			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
	}

	if(ChaosCount != 1 || (type == 0 && FeatherCount != 1) || (type == 1 && SleeveCount != 1) || WingItemCount != 1)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	if(gServerInfo.m_Wing2MixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = (WingItemMoney/4000000)+(ItemMoney/40000);
	}
	else
	{
		lpObj->ChaosSuccessRate = gServerInfo.m_Wing2MixRate[lpObj->AccountLevel];
	}

	lpObj->ChaosSuccessRate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,lpObj->ChaosSuccessRate);

	if(gServerInfo.m_Wing2MixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>90)?90:lpObj->ChaosSuccessRate);
	}
	else
	{
		lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);
	}

	lpObj->ChaosMoney = 5000000;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		if(type == 0)
		{
			CItem item;

			if(gItemBagManager.GetItemBySpecialValue(ITEM_BAG_SPECIAL_CHAOS_BOX,2,-1,lpObj,&item) == 0)
			{
				this->GCChaosMixSend(lpObj->Index,0,0);
				return;
			}

			GDCreateItemSend(lpObj->Index,0xFF,0,0,item.m_Index,(BYTE)item.m_Level,(BYTE)item.m_Durability,item.m_Option1,item.m_Option2,item.m_Option3,-1,item.m_NewOption,item.m_SetOption,0);

			gLog.Output(LOG_CHAOS_MIX,"[Wing2Mix][Success][%s][%s] - (Type: %d,ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
		}
		else
		{
			CItem item;

			if(gItemBagManager.GetItemBySpecialValue(ITEM_BAG_SPECIAL_CHAOS_BOX,3,-1,lpObj,&item) == 0)
			{
				this->GCChaosMixSend(lpObj->Index,0,0);
				return;
			}

			GDCreateItemSend(lpObj->Index,0xFF,0,0,item.m_Index,(BYTE)item.m_Level,(BYTE)item.m_Durability,item.m_Option1,item.m_Option2,item.m_Option3,-1,item.m_NewOption,item.m_SetOption,0);

			gLog.Output(LOG_CHAOS_MIX,"[Wing2Mix][Success][%s][%s] - (Type: %d,ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
		}
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[Wing2Mix][Failure][%s][%s] - (Type: %d,ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,type,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::BloodCastleMix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int ScrollCount = 0;
	int ScrollLevel = 0;
	int BoneCount = 0;
	int BoneLevel = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,16))
		{
			ScrollCount++;
			ScrollLevel = lpObj->ChaosBox[n].m_Level;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,17))
		{
			BoneCount++;
			BoneLevel = lpObj->ChaosBox[n].m_Level;
		}
	}

	if(ChaosCount != 1 || ScrollCount != 1 || BoneCount != 1 || ScrollLevel != BoneLevel)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}
	
	if(ScrollLevel > 6 || BoneLevel > 6)
	{
		this->GCChaosMixSend(lpObj->Index,8,0);
		return;
	}

	switch(ScrollLevel)
	{
		case 1:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate1[lpObj->AccountLevel];
			lpObj->ChaosMoney = 50000;
			break;
		case 2:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate2[lpObj->AccountLevel];
			lpObj->ChaosMoney = 80000;
			break;
		case 3:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate3[lpObj->AccountLevel];
			lpObj->ChaosMoney = 150000;
			break;
		case 4:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate4[lpObj->AccountLevel];
			lpObj->ChaosMoney = 250000;
			break;
		case 5:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate5[lpObj->AccountLevel];
			lpObj->ChaosMoney = 400000;
			break;
		case 6:
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate6[lpObj->AccountLevel];
			lpObj->ChaosMoney = 600000;
			break;
	}

	lpObj->ChaosSuccessRate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,lpObj->ChaosSuccessRate);

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	if((GetLargeRand()%100) <= lpObj->ChaosSuccessRate)
	{
		GDCreateItemSend(lpObj->Index,0xFF,0,0,GET_ITEM(13,18),ScrollLevel,1,0,0,0,-1,0,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[BloodCastleMix][Success][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[BloodCastleMix][Failure][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::Wing1Mix(LPOBJ lpObj) // OK
{
	int ChaosCount = 0;
	int ChaosItem = 0;
	int ItemCount = 0;
	int ItemMoney = 0;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
		{
			ChaosCount++;
			lpObj->ChaosBox[n].OldValue();
			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13) || lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
		{
			ItemCount++;
			lpObj->ChaosBox[n].OldValue();
			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if((lpObj->ChaosBox[n].m_Index == GET_ITEM(2,6) || lpObj->ChaosBox[n].m_Index == GET_ITEM(4,6) || lpObj->ChaosBox[n].m_Index == GET_ITEM(5,7)) && lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_Option3 >= 1)
		{
			ChaosItem++;
			ItemCount++;
			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
		else if(lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_Option3 >= 1)
		{
			ItemCount++;
			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
	}

	if(ChaosCount == 0 || ChaosItem == 0)
	{
		this->GCChaosMixSend(lpObj->Index,7,0);
		return;
	}

	if(gServerInfo.m_Wing1MixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = (ItemMoney/20000);
	}
	else
	{
		lpObj->ChaosSuccessRate = gServerInfo.m_Wing1MixRate[lpObj->AccountLevel];
	}

	lpObj->ChaosSuccessRate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,lpObj->ChaosSuccessRate);

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate>100)?100:lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = lpObj->ChaosSuccessRate*10000;

	if(lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index,2,0);
		return;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index,lpObj->Money);

	if((GetLargeRand()%100) < lpObj->ChaosSuccessRate)
	{
		CItem item;

		if(gItemBagManager.GetItemBySpecialValue(ITEM_BAG_SPECIAL_CHAOS_BOX,1,-1,lpObj,&item) == 0)
		{
			this->GCChaosMixSend(lpObj->Index,0,0);
			return;
		}

		GDCreateItemSend(lpObj->Index,0xFF,0,0,item.m_Index,(BYTE)item.m_Level,(BYTE)item.m_Durability,item.m_Option1,item.m_Option2,item.m_Option3,-1,item.m_NewOption,item.m_SetOption,0);

		gLog.Output(LOG_CHAOS_MIX,"[Wing1Mix][Success][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj,0);

		this->GCChaosMixSend(lpObj->Index,0,0);

		gLog.Output(LOG_CHAOS_MIX,"[Wing1Mix][Failure][%s][%s] - (ChaosSuccessRate: %d,ChaosMoney: %d)",lpObj->Account,lpObj->Name,lpObj->ChaosSuccessRate,lpObj->ChaosMoney);
	}
}

void CChaosBox::PetMix(LPOBJ lpObj,int type) // OK
{
	
}

void CChaosBox::CGChaosMixRecv(PMSG_CHAOS_MIX_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->ChaosLock != 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || (lpObj->Interface.type != INTERFACE_CHAOS_BOX && lpObj->Interface.type != INTERFACE_TRAINER))
	{
		return;
	}

	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	if(lpObj->Permission[9] != 0)
	{
		return;
	}

	lpObj->ChaosLock = 1;

	lpObj->ChaosMoney = 0;

	lpObj->ChaosSuccessRate = 0;

	lpObj->IsChaosMixCompleted = 1;

	switch(lpMsg->type)
	{
		case CHAOS_MIX_CHAOS_ITEM:
			this->ChaosItemMix(lpObj);
			break;
		case CHAOS_MIX_DEVIL_SQUARE:
			this->DevilSquareMix(lpObj);
			break;
		case CHAOS_MIX_PLUS_ITEM_LEVEL1:
			this->PlusItemLevelMix(lpObj,0);
			break;
		case CHAOS_MIX_PLUS_ITEM_LEVEL2:
			this->PlusItemLevelMix(lpObj,1);
			break;
		case CHAOS_MIX_DINORANT:
			this->DinorantMix(lpObj);
			break;
		case CHAOS_MIX_FRUIT:
			this->FruitMix(lpObj);
			break;
		case CHAOS_MIX_WING1:
			this->Wing2Mix(lpObj,0);
			break;
		case CHAOS_MIX_BLOOD_CASTLE:
			this->BloodCastleMix(lpObj);
			break;
		case CHAOS_MIX_WING2:
			this->Wing1Mix(lpObj);
			break;
		case CHAOS_MIX_WING3:
			this->Wing2Mix(lpObj,1);
			break;
	}
}

void CChaosBox::CGChaosMixRateRecv(PMSG_CHAOS_MIX_RATE_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || (lpObj->Interface.type != INTERFACE_CHAOS_BOX && lpObj->Interface.type != INTERFACE_TRAINER))
	{
		return;
	}

	if(lpObj->Permission[9] != 0)
	{
		return;
	}

	PMSG_CHAOS_MIX_RATE_SEND pMsg;

	pMsg.header.set(0x88,sizeof(pMsg));

	pMsg.rate = 0;

	pMsg.money = 0;

	switch(lpMsg->type)
	{
		case CHAOS_MIX_CHAOS_ITEM:
		{
			int ChaosCount = 0;
			int ItemCount = 0;
			int ItemMoney = 0;

			for(int n=0;n < CHAOS_BOX_SIZE;n++)
			{
				if(lpObj->ChaosBox[n].IsItem() == 0)
				{
					continue;
				}

				if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
				{
					ChaosCount++;
					lpObj->ChaosBox[n].OldValue();
					ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
				}
				else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13) || lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
				{
					lpObj->ChaosBox[n].OldValue();
					ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
				}
				else if(lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_Option3 >= 1)
				{
					ItemCount++;
					ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
				}
			}

			if(ChaosCount == 0 || ItemCount == 0)
			{
				return;
			}

			if(gServerInfo.m_ChaosItemMixRate[lpObj->AccountLevel] == -1)
			{
				pMsg.rate = (ItemMoney/20000);
			}
			else
			{
				pMsg.rate = gServerInfo.m_ChaosItemMixRate[lpObj->AccountLevel];
			}

			pMsg.rate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,pMsg.rate);

			pMsg.rate = ((pMsg.rate>100)?100:pMsg.rate);

			pMsg.money = pMsg.rate*10000;
		} break;
		case CHAOS_MIX_DEVIL_SQUARE:
		{
			int ChaosCount = 0;
			int EyeCount = 0;
			int EyeLevel = 0;
			int KeyCount = 0;
			int KeyLevel = 0;

			for(int n=0;n < CHAOS_BOX_SIZE;n++)
			{
				if(lpObj->ChaosBox[n].IsItem() == 0)
				{
					continue;
				}

				if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
				{
					ChaosCount++;
				}
				else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,17))
				{
					EyeCount++;
					EyeLevel = lpObj->ChaosBox[n].m_Level;
				}
				else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,18))
				{
					KeyCount++;
					KeyLevel = lpObj->ChaosBox[n].m_Level;
				}
			}

			if(ChaosCount != 1 || EyeCount != 1 || KeyCount != 1 || EyeLevel != KeyLevel)
			{
				return;
			}

			if(EyeLevel > 4 || KeyLevel > 4)
			{
				return;
			}

			switch(EyeLevel)
			{
				case 1:
					pMsg.rate = gServerInfo.m_DevilSquareMixRate1[lpObj->AccountLevel];
					pMsg.money = 100000;
					break;
				case 2:
					pMsg.rate = gServerInfo.m_DevilSquareMixRate2[lpObj->AccountLevel];
					pMsg.money = 200000;
					break;
				case 3:
					pMsg.rate = gServerInfo.m_DevilSquareMixRate3[lpObj->AccountLevel];
					pMsg.money = 400000;
					break;
				case 4:
					pMsg.rate = gServerInfo.m_DevilSquareMixRate4[lpObj->AccountLevel];
					pMsg.money = 700000;
					break;
			}

			pMsg.rate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,pMsg.rate);
		} break;
		case CHAOS_MIX_PLUS_ITEM_LEVEL1:
		case CHAOS_MIX_PLUS_ITEM_LEVEL2:
		{
			int ChaosCount = 0;
			int BlessCount = 0;
			int SoulCount = 0;
			int ItemCount = 0;
			int ItemSlot = 0;
			int type = 0;

			switch(lpMsg->type)
			{
				case CHAOS_MIX_PLUS_ITEM_LEVEL1:
					type = 0;
					break;
				case CHAOS_MIX_PLUS_ITEM_LEVEL2:
					type = 1;
					break;
			}

			for(int n=0;n < CHAOS_BOX_SIZE;n++)
			{
				if(lpObj->ChaosBox[n].IsItem() == 0)
				{
					continue;
				}

				if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
				{
					ChaosCount++;
				}
				else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13))
				{
					BlessCount++;
				}
				else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
				{
					SoulCount++;
				}
				else if(lpObj->ChaosBox[n].m_Level == (9+type))
				{
					ItemCount++;
					ItemSlot = n;
				}
			}

			if(ChaosCount != 1 || SoulCount < (type+1) || BlessCount < (type+1) || ItemCount != 1)
			{
				return;
			}

			if(lpObj->ChaosBox[ItemSlot].IsExcItem() != 0 || lpObj->ChaosBox[ItemSlot].IsSetItem() != 0)
			{
				pMsg.rate = gServerInfo.m_PlusExcSetItemLevelMixRate[type][lpObj->AccountLevel];
			}
			else
			{
				pMsg.rate = gServerInfo.m_PlusCommonItemLevelMixRate[type][lpObj->AccountLevel];
			}

			if(lpObj->ChaosBox[ItemSlot].m_Option2 != 0)
			{
				pMsg.rate += gServerInfo.m_AddLuckSuccessRate2[lpObj->AccountLevel];
			}

			pMsg.rate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,pMsg.rate);

			pMsg.money = 2000000*(type+1);
		} break;
		case CHAOS_MIX_DINORANT:
		{
			int ChaosCount = 0;
			int UniriaCount = 0;

			for(int n=0;n < CHAOS_BOX_SIZE;n++)
			{
				if(lpObj->ChaosBox[n].IsItem() == 0)
				{
					continue;
				}

				if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
				{
					ChaosCount++;
				}
				else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,2) && lpObj->ChaosBox[n].m_Durability == 255)
				{
					UniriaCount++;
				}
			}

			if(ChaosCount != 1 || UniriaCount != 10)
			{
				return;
			}

			pMsg.rate = gServerInfo.m_DinorantMixRate[lpObj->AccountLevel];

			pMsg.rate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,pMsg.rate);

			pMsg.money = 500000;
		} break;
		case CHAOS_MIX_FRUIT:
		{
			int ChaosCount = 0;
			int CreationCount = 0;

			for(int n=0;n < CHAOS_BOX_SIZE;n++)
			{
				if(lpObj->ChaosBox[n].IsItem() == 0)
				{
					continue;
				}

				if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
				{
					ChaosCount++;
				}
				else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,22))
				{
					CreationCount++;
				}
			}

			if(ChaosCount != 1 || CreationCount != 1)
			{
				return;
			}

			pMsg.rate = gServerInfo.m_FruitMixRate[lpObj->AccountLevel];

			pMsg.rate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,pMsg.rate);

			pMsg.money = 3000000;
		} break;
		case CHAOS_MIX_WING1:
		case CHAOS_MIX_WING3:
		{
			int ChaosCount = 0;
			int FeatherCount = 0;
			int SleeveCount = 0;
			int WingItemCount = 0;
			int WingItemMoney = 0;
			int ItemCount = 0;
			int ItemMoney = 0;
			int type = 0;

			switch(lpMsg->type)
			{
				case CHAOS_MIX_WING1:
					type = 0;
					break;
				case CHAOS_MIX_WING3:
					type = 1;
					break;
			}

			for(int n=0;n < CHAOS_BOX_SIZE;n++)
			{
				if(lpObj->ChaosBox[n].IsItem() == 0)
				{
					continue;
				}

				if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
				{
					ChaosCount++;
				}
				else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,14) && lpObj->ChaosBox[n].m_Level == 0)
				{
					FeatherCount++;
				}
				else if(lpObj->ChaosBox[n].m_Index >= GET_ITEM(12,0) && lpObj->ChaosBox[n].m_Index <= GET_ITEM(12,2))
				{
					WingItemCount++;
					WingItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
				}
				else if(lpObj->ChaosBox[n].IsExcItem() != 0 && lpObj->ChaosBox[n].m_Level >= 4)
				{
					ItemCount++;
					ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
				}
			}

			if(ChaosCount != 1 || (type == 0 && FeatherCount != 1) || (type == 1 && SleeveCount != 1) || WingItemCount != 1)
			{
				return;
			}

			if(gServerInfo.m_Wing2MixRate[lpObj->AccountLevel] == -1)
			{
				pMsg.rate = (WingItemMoney/4000000)+(ItemMoney/40000);
			}
			else
			{
				pMsg.rate = gServerInfo.m_Wing2MixRate[lpObj->AccountLevel];
			}

			pMsg.rate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,pMsg.rate);

			if(gServerInfo.m_Wing2MixRate[lpObj->AccountLevel] == -1)
			{
				pMsg.rate = ((pMsg.rate>90)?90:pMsg.rate);
			}

			pMsg.money = 5000000;
		} break;
		case CHAOS_MIX_BLOOD_CASTLE:
		{
			int ChaosCount = 0;
			int ScrollCount = 0;
			int ScrollLevel = 0;
			int BoneCount = 0;
			int BoneLevel = 0;

			for(int n=0;n < CHAOS_BOX_SIZE;n++)
			{
				if(lpObj->ChaosBox[n].IsItem() == 0)
				{
					continue;
				}

				if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
				{
					ChaosCount++;
				}
				else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,16))
				{
					ScrollCount++;
					ScrollLevel = lpObj->ChaosBox[n].m_Level;
				}
				else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(13,17))
				{
					BoneCount++;
					BoneLevel = lpObj->ChaosBox[n].m_Level;
				}
			}

			if(ChaosCount != 1 || ScrollCount != 1 || BoneCount != 1 || ScrollLevel != BoneLevel)
			{
				return;
			}

			if(ScrollLevel > 6 || BoneLevel > 6)
			{
				return;
			}
			switch(ScrollLevel)
			{
				case 1:
					pMsg.rate = gServerInfo.m_BloodCastleMixRate1[lpObj->AccountLevel];
					pMsg.money = 50000;
					break;
				case 2:
					pMsg.rate = gServerInfo.m_BloodCastleMixRate2[lpObj->AccountLevel];
					pMsg.money = 80000;
					break;
				case 3:
					pMsg.rate = gServerInfo.m_BloodCastleMixRate3[lpObj->AccountLevel];
					pMsg.money = 150000;
					break;
				case 4:
					pMsg.rate = gServerInfo.m_BloodCastleMixRate4[lpObj->AccountLevel];
					pMsg.money = 250000;
					break;
				case 5:
					pMsg.rate = gServerInfo.m_BloodCastleMixRate5[lpObj->AccountLevel];
					pMsg.money = 400000;
					break;
				case 6:
					pMsg.rate = gServerInfo.m_BloodCastleMixRate6[lpObj->AccountLevel];
					pMsg.money = 600000;
					break;
			}

			pMsg.rate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,pMsg.rate);
		} break;
		case CHAOS_MIX_WING2:
		{
			int ChaosCount = 0;
			int ChaosItem = 0;
			int ItemCount = 0;
			int ItemMoney = 0;

			for(int n=0;n < CHAOS_BOX_SIZE;n++)
			{
				if(lpObj->ChaosBox[n].IsItem() == 0)
				{
					continue;
				}

				if(lpObj->ChaosBox[n].m_Index == GET_ITEM(12,15))
				{
					ChaosCount++;
					lpObj->ChaosBox[n].OldValue();
					ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
				}
				else if(lpObj->ChaosBox[n].m_Index == GET_ITEM(14,13) || lpObj->ChaosBox[n].m_Index == GET_ITEM(14,14))
				{
					ItemCount++;
					lpObj->ChaosBox[n].OldValue();
					ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
				}
				else if((lpObj->ChaosBox[n].m_Index == GET_ITEM(2,6) || lpObj->ChaosBox[n].m_Index == GET_ITEM(4,6) || lpObj->ChaosBox[n].m_Index == GET_ITEM(5,7)) && lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_Option3 >= 1)
				{
					ChaosItem++;
					ItemCount++;
					ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
				}
				else if(lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_Option3 >= 1)
				{
					ItemCount++;
					ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
				}
			}

			if(ChaosCount == 0 || ChaosItem == 0)
			{
				return;
			}

			if(gServerInfo.m_Wing1MixRate[lpObj->AccountLevel] == -1)
			{
				pMsg.rate = (ItemMoney/20000);
			}
			else
			{
				pMsg.rate = gServerInfo.m_Wing1MixRate[lpObj->AccountLevel];
			}

			pMsg.rate = gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_CHAOS_MIX_RATE,pMsg.rate);

			pMsg.rate = ((pMsg.rate>100)?100:pMsg.rate);

			pMsg.money = pMsg.rate*10000;
		} break;
	}

	pMsg.rate = ((pMsg.rate>100)?100:((pMsg.rate<0)?0:pMsg.rate));

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void CChaosBox::CGChaosMixCloseRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_CHAOS_BOX)
	{
		return;
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;

	this->ChaosBoxInit(lpObj);

	gObjInventoryCommit(aIndex);

	lpObj->ChaosLock = 0;

	lpObj->IsChaosMixCompleted = 0;

	PBMSG_HEAD pMsg;

	pMsg.set(0x87,sizeof(pMsg));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.size);
}

void CChaosBox::GCChaosBoxSend(LPOBJ lpObj,BYTE type) // OK
{
	if((type == 0 && (lpObj->Interface.type != INTERFACE_CHAOS_BOX || lpObj->Interface.state == 1)) || (type != 0 && (lpObj->Interface.type != INTERFACE_TRAINER || lpObj->Interface.state == 1)))
	{
		return;
	}

	BYTE send[2048];

	PMSG_SHOP_ITEM_LIST_SEND pMsg;

	pMsg.header.set(0x31,0);

	int size = sizeof(pMsg);

	pMsg.type = ((type==0)?3:5);

	pMsg.count = 0;

	PMSG_SHOP_ITEM_LIST info;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		info.slot = n;

		gItemManager.ItemByteConvert(info.ItemInfo,lpObj->ChaosBox[n]);

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpObj->Index,send,size);
}

void CChaosBox::GCChaosMixSend(int aIndex,BYTE result,CItem* lpItem) // OK
{
	PMSG_CHAOS_MIX_SEND pMsg;

	pMsg.header.set(0x86,sizeof(pMsg));

	pMsg.result = result;

	if(lpItem == 0)
	{
		memset(pMsg.ItemInfo,0xFF,sizeof(pMsg.ItemInfo));
	}
	else
	{
		gItemManager.ItemByteConvert(pMsg.ItemInfo,(*lpItem));
	}

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	gObj[aIndex].ChaosLock = 0;
}