// DiePenalty.cpp: implementation of the CDiePenalty class.
// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 105 (2.6.5) - Sistema de penalidades ao morrer DiePenalty
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DiePenalty.h"
#include "Guild.h"
#include "ItemManager.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "MemScript.h"
#include "RandomManager.h"
#include "ServerInfo.h"
#include "Util.h"

CDiePenalty gDiePenalty;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDiePenalty::CDiePenalty()
{
	this->m_DiePenaltyInfo.clear();
}

CDiePenalty::~CDiePenalty()
{

}

void CDiePenalty::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_DiePenaltyInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0 || section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					DIE_PENALTY_INFO info;

					info.Type = section; // 0: PvM, 1: PvP

					info.Index = lpMemScript->GetNumber();

					info.MinLevel = lpMemScript->GetAsNumber();

					info.MaxLevel = lpMemScript->GetAsNumber();

					info.MinReset = lpMemScript->GetAsNumber();

					info.MaxReset = lpMemScript->GetAsNumber();

					for(int n=0;n < MAX_ACCOUNT_LEVEL;n++)
					{
						info.ExpDecRate[n] = lpMemScript->GetAsNumber();
					}

					for(int n=0;n < MAX_ACCOUNT_LEVEL;n++)
					{
						info.MoneyDecRate[n] = lpMemScript->GetAsNumber();
					}

					for(int n=0;n < MAX_ACCOUNT_LEVEL;n++)
					{
						info.DropRate[n] = lpMemScript->GetAsNumber();
					}

					info.DropItemMinLevel = lpMemScript->GetAsNumber();

					info.DropItemMaxLevel = lpMemScript->GetAsNumber();

					info.MaxNewOptionCount = lpMemScript->GetAsNumber();

					info.DropHelperSwitch = lpMemScript->GetAsNumber();

					info.DropWingSwitch = lpMemScript->GetAsNumber();

					info.DropSetSwitch = lpMemScript->GetAsNumber();

					this->m_DiePenaltyInfo.push_back(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CDiePenalty::UserDieProc(LPOBJ lpObj,LPOBJ lpTarget)
{
	if(lpObj->Type != OBJECT_USER)
	{
		return;
	}

	if(OBJECT_RANGE(lpObj->DuelUser) != 0 && lpObj->DuelUser == lpTarget->Index)
	{
		return;
	}

	if(gGuild.gObjTargetGuildWarCheck(lpObj,lpTarget) != 0)
	{
		return;
	}

	if(gMapManager.GetMapPKItemDrop(lpObj->Map) == 0)
	{
		return;
	}

	if(this->m_DiePenaltyInfo.empty())
	{
		return;
	}

	int deathType = ((lpTarget->Type == OBJECT_USER) ? 1 : 0);

	for(std::vector<DIE_PENALTY_INFO>::iterator it = this->m_DiePenaltyInfo.begin(); it != this->m_DiePenaltyInfo.end(); it++)
	{
		if(it->Type != deathType)
		{
			continue;
		}

		if(it->Index != lpObj->PKLevel)
		{
			continue;
		}

		if(it->MinLevel != -1 && it->MinLevel > lpObj->Level)
		{
			continue;
		}

		if(it->MaxLevel != -1 && it->MaxLevel < lpObj->Level)
		{
			continue;
		}

		if(it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if(it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		int exprate = it->ExpDecRate[lpObj->AccountLevel];

		if(exprate > 0 && lpObj->Level > 0)
		{
			DWORD experience = 0;

			experience = (gLevelExperience[lpObj->Level] - gLevelExperience[lpObj->Level-1]);

			experience = (experience * exprate) / 100;

			if(experience > 0)
			{
				lpObj->Experience -= experience;

				lpObj->Experience = (lpObj->Experience < gLevelExperience[lpObj->Level-1]) ? gLevelExperience[lpObj->Level-1] : lpObj->Experience;

				if(lpObj->Experience < 0)
				{
					lpObj->Experience = 0;
				}

				gLog.Output(LOG_PENALTY,"[PenaltyExpDecrease][%s][%s] - (Rate:%d, PKLevel:%d, Experience: %d)",lpObj->Account,lpObj->Name,exprate,lpObj->PKLevel,experience);

				GCNewCharacterInfoSend(lpObj);
			}
		}

		int moneydecrate = it->MoneyDecRate[lpObj->AccountLevel];

		if(moneydecrate > 0 && lpObj->Money > 0)
		{
			DWORD money = (DWORD)(((QWORD)lpObj->Money * moneydecrate) / 100);

			if(money > 0)
			{
				if(lpObj->Money <= money)
				{
					lpObj->Money = 0;
				}
				else
				{
					lpObj->Money -= money;
				}

				gLog.Output(LOG_PENALTY,"[PenaltyMoneyDecrease][%s][%s] - (Rate:%d, PKLevel:%d, Money: %d)",lpObj->Account,lpObj->Name,moneydecrate,lpObj->PKLevel,money);

				GCMoneySend(lpObj->Index,lpObj->Money);
			}
		}

		int droprate = it->DropRate[lpObj->AccountLevel];

		if(droprate > 0)
		{
			if((GetLargeRand() % 100) < droprate)
			{
				int count = 0;

				CRandomManager RandomManager;

				for(int n = 0; n < INVENTORY_MAIN_SIZE; n++)
				{
					if(n == INVENTORY_WEAR_SIZE && count != 0)
					{
						break;
					}

					CItem* lpItem = &lpObj->Inventory[n];

					if(lpItem->IsItem() == 0)
					{
						continue;
					}

					if(lpItem->m_Index == GET_ITEM(4,7) || lpItem->m_Index == GET_ITEM(4,15)) // Bolt, Arrow
					{
						continue;
					}

					if(lpItem->m_Index == GET_ITEM(13,20) && (lpItem->m_Level >= 1 && lpItem->m_Level <= 2)) // Wizard's Ring, Starter Ring
					{
						continue;
					}

					if(it->DropItemMinLevel != -1 && it->DropItemMinLevel > lpItem->m_Level)
					{
						continue;
					}

					if(it->DropItemMaxLevel != -1 && it->DropItemMaxLevel < lpItem->m_Level)
					{
						continue;
					}

					if((lpItem->m_Slot == INVENTORY_SLOT_HELPER || lpItem->m_Index == GET_ITEM(13,5)) && it->DropHelperSwitch == 0)
					{
						continue;
					}

					if(lpItem->m_Slot == INVENTORY_SLOT_WING && it->DropWingSwitch == 0)
					{
						continue;
					}

					if(lpItem->m_Slot != INVENTORY_SLOT_WING && lpItem->IsExcItem() != 0 && GetNewOptionCount(lpItem->m_NewOption) > it->MaxNewOptionCount)
					{
						continue;
					}

					if(lpItem->IsSetItem() != 0 && it->DropSetSwitch == 0)
					{
						continue;
					}

					count++;

					RandomManager.AddElement(n,1);
				}

				int slot = -1;

				if(RandomManager.GetRandomElement(&slot) != 0)
				{
					PMSG_ITEM_DROP_SEND pMsg;

					pMsg.header.set(0x23,sizeof(pMsg));

					pMsg.result = 0;

					pMsg.slot = slot;

					CItem* lpItem = &lpObj->Inventory[slot];

					if(gMap[lpObj->Map].ItemDrop(lpItem->m_Index,lpItem->m_Level,lpItem->m_Durability,lpObj->X,lpObj->Y,lpItem->m_Option1,lpItem->m_Option2,lpItem->m_Option3,lpItem->m_NewOption,lpItem->m_SetOption,lpItem->m_Serial,lpObj->Index,lpItem->m_PetItemLevel,lpItem->m_PetItemExp,lpItem->m_PeriodicItemTime) != 0)
					{
						gLog.Output(LOG_PENALTY,"[PenaltyDropItem][%s][%s] - (Rate:%d, PKLevel:%d, Map:%d, X:%d, Y:%d) (Index: %04d, Level: %02d, Serial: %08X, Option1: %01d, Option2: %01d, Option3: %01d, NewOption: %03d)",lpObj->Account,lpObj->Name,droprate,lpObj->PKLevel,lpObj->Map,lpObj->X,lpObj->Y,lpItem->m_Index,lpItem->m_Level,lpItem->m_Serial,lpItem->m_Option1,lpItem->m_Option2,lpItem->m_Option3,lpItem->m_NewOption);
						pMsg.result = 1;
						gItemManager.InventoryDelItem(lpObj->Index,slot);
						gItemManager.UpdateInventoryViewport(lpObj->Index,slot);
					}

					DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
					return;
				}
			}
		}

		break;
	}
}
