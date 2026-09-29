// Update CMZ 17 (3.1.7) 28-09-26 - Custom Change Class System
// CustomChangeClass.cpp: implementation of the CCustomChangeClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomChangeClass.h"
#include "CommandManager.h"
#include "DSProtocol.h"
#include "DefaultClassInfo.h"
#include "Item.h"
#include "Log.h"
#include "Message.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Util.h"

CCustomChangeClass gCustomChangeClass;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomChangeClass::CCustomChangeClass()
{
	this->m_CustomChangeClassSwitch = 1;
	this->m_CustomChangeClassRequireVip = 1;
	this->m_CustomChangeClassCheckItem = 1;
	this->m_CustomChangeClassResetPoints = 1;
	this->m_CustomChangeClassResetSkills = 1;
	this->m_CustomChangeClassMinLevel = 1;
	this->m_CustomChangeClassReqMoney = 0;
	this->m_CustomChangeClassGateMove = 0;
}

CCustomChangeClass::~CCustomChangeClass()
{
}

void CCustomChangeClass::ReadCustomChangeClassInfo(char* section, char* path)
{
	this->m_CustomChangeClassSwitch = GetPrivateProfileInt(section, "CustomChangeClassSwitch", 1, path);
	this->m_CustomChangeClassRequireVip = GetPrivateProfileInt(section, "CustomChangeClassRequireVip", 1, path);
	this->m_CustomChangeClassCheckItem = GetPrivateProfileInt(section, "CustomChangeClassCheckItem", 1, path);
	this->m_CustomChangeClassResetPoints = GetPrivateProfileInt(section, "CustomChangeClassResetPoints", 1, path);
	this->m_CustomChangeClassResetSkills = GetPrivateProfileInt(section, "CustomChangeClassResetSkills", 1, path);
	this->m_CustomChangeClassMinLevel = GetPrivateProfileInt(section, "CustomChangeClassMinLevel", 1, path);
	this->m_CustomChangeClassReqMoney = GetPrivateProfileInt(section, "CustomChangeClassReqMoney", 0, path);
	this->m_CustomChangeClassGateMove = GetPrivateProfileInt(section, "CustomChangeClassGateMove", 0, path);
}

bool CCustomChangeClass::ChangeClass(LPOBJ lpObj, int targetClass)
{
	if (this->m_CustomChangeClassSwitch == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(760));
		return false;
	}

	if (targetClass != DB_CLASS_BK && targetClass != DB_CLASS_SM && targetClass != DB_CLASS_ME && targetClass != DB_CLASS_MG)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(761));
		return false;
	}

	if (lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(762));
		return false;
	}

	if (this->m_CustomChangeClassRequireVip != 0 && lpObj->AccountLevel == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(763));
		return false;
	}

	if (this->m_CustomChangeClassMinLevel > 0 && lpObj->Level < this->m_CustomChangeClassMinLevel)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(764), this->m_CustomChangeClassMinLevel);
		return false;
	}

	if (this->m_CustomChangeClassReqMoney > 0 && lpObj->Money < (DWORD)this->m_CustomChangeClassReqMoney)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(765), this->m_CustomChangeClassReqMoney);
		return false;
	}

	if (lpObj->DBClass == targetClass)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(766));
		return false;
	}

	if (this->m_CustomChangeClassCheckItem != 0)
	{
		for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if (lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(767));
				return false;
			}
		}
	}

	if (this->m_CustomChangeClassReqMoney > 0)
	{
		lpObj->Money -= this->m_CustomChangeClassReqMoney;
		GCMoneySend(lpObj->Index, lpObj->Money);
	}

	// Update CMZ 17 (3.1.7) 28-09-26 - Calculate distributed points from previous class
	if (this->m_CustomChangeClassResetPoints != 0)
	{
		int oldStr = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 0);
		int oldAgi = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 1);
		int oldVit = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 2);
		int oldEne = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 3);

		int point = 0;
		point += (lpObj->Strength - oldStr);
		point += (lpObj->Dexterity - oldAgi);
		point += (lpObj->Vitality - oldVit);
		point += (lpObj->Energy - oldEne);

		if (point > 0)
		{
			lpObj->LevelUpPoint += point;
		}
	}

	// Update CMZ 17 (3.1.7) 28-09-26 - Set new class, category and changeup
	int newClass = targetClass / 16;
	int newChangeUp = targetClass % 16;

	lpObj->DBClass = targetClass;
	lpObj->Class = newClass;
	lpObj->ChangeUp = newChangeUp;

	// Update CMZ 17 (3.1.7) 28-09-26 - Apply new default stats
	if (this->m_CustomChangeClassResetPoints != 0)
	{
		lpObj->Strength = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 0);
		lpObj->Dexterity = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 1);
		lpObj->Vitality = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 2);
		lpObj->Energy = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 3);
	}

	// Update CMZ 17 (3.1.7) 28-09-26 - Remove incompatible learned skills
	if (this->m_CustomChangeClassResetSkills != 0)
	{
		for (int n = 0; n < MAX_SKILL_LIST; n++)
		{
			if (lpObj->Skill[n].IsSkill() != 0)
			{
				if (gSkillManager.CheckSkillRequireClass(lpObj, lpObj->Skill[n].m_index) == 0)
				{
					lpObj->Skill[n].Clear();
				}
			}
		}

		gSkillManager.GCSkillListSend(lpObj);
	}

	// Update CMZ 17 (3.1.7) 28-09-26 - Set quest level 2 for evolved classes
	if (targetClass == DB_CLASS_BK || targetClass == DB_CLASS_SM || targetClass == DB_CLASS_ME)
	{
		gQuest.AddQuestList(lpObj, 0, QUEST_FINISH);
		gQuest.AddQuestList(lpObj, 1, QUEST_FINISH);
		gQuest.AddQuestList(lpObj, 2, QUEST_FINISH);
	}

	lpObj->SendQuestInfo = 0;
	gQuest.GCQuestInfoSend(lpObj->Index);

	// Update CMZ 17 (3.1.7) 28-09-26 - Send evolution reward packet (golden aura effect)
	BYTE ClassReward = (lpObj->ChangeUp * 16);
	ClassReward -= (ClassReward / 32);
	ClassReward += (lpObj->Class * 32);

	gQuest.GCQuestRewardSend(lpObj->Index, 201, ClassReward);

	// Update CMZ 17 (3.1.7) 28-09-26 - Recalculate attributes and broadcast charset
	gObjectManager.CharacterCalcAttribute(lpObj->Index);
	gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);
	GDCharacterInfoSaveSend(lpObj->Index);
	gObjViewportListProtocolCreate(lpObj);

	const char* className = "Desconhecida";
	switch (targetClass)
	{
	case DB_CLASS_BK:
		className = "Blade Knight";
		break;
	case DB_CLASS_SM:
		className = "Soul Master";
		break;
	case DB_CLASS_ME:
		className = "Muse Elf";
		break;
	case DB_CLASS_MG:
		className = "Magic Gladiator";
		break;
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(768), className);
	gLog.Output(LOG_COMMAND, "[CustomChangeClass][%s][%s] - Class changed to %s (%d)", lpObj->Account, lpObj->Name, className, targetClass);

	// Update CMZ 17 (3.1.7) 28-09-26 - Move to town gate to refresh client view
	if (this->m_CustomChangeClassGateMove != 0)
	{
		switch (lpObj->Class)
		{
		case CLASS_DW:
			gObjMoveGate(lpObj->Index, 17);
			break;
		case CLASS_DK:
			gObjMoveGate(lpObj->Index, 17);
			break;
		case CLASS_FE:
			gObjMoveGate(lpObj->Index, 27);
			break;
		case CLASS_MG:
			gObjMoveGate(lpObj->Index, 17);
			break;
		default:
			gObjMoveGate(lpObj->Index, 17);
			break;
		}
	}

	return true;
}

void CCustomChangeClass::CommandChangeClass(LPOBJ lpObj, char* arg)
{
	char target[32] = { 0 };
	gCommandManager.GetString(arg, target, sizeof(target), 0);

	if (target[0] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(769));
		return;
	}

	int targetClass = -1;

	if (_stricmp(target, "bk") == 0 || _stricmp(target, "bladeknight") == 0 || _stricmp(target, "dk") == 0 || _stricmp(target, "darkknight") == 0)
	{
		targetClass = DB_CLASS_BK;
	}
	else if (_stricmp(target, "sm") == 0 || _stricmp(target, "soulmaster") == 0 || _stricmp(target, "dw") == 0 || _stricmp(target, "darkwizard") == 0)
	{
		targetClass = DB_CLASS_SM;
	}
	else if (_stricmp(target, "me") == 0 || _stricmp(target, "museelf") == 0 || _stricmp(target, "elf") == 0 || _stricmp(target, "fe") == 0)
	{
		targetClass = DB_CLASS_ME;
	}
	else if (_stricmp(target, "mg") == 0 || _stricmp(target, "magicgladiator") == 0)
	{
		targetClass = DB_CLASS_MG;
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(770), target);
		return;
	}

	this->ChangeClass(lpObj, targetClass);
}

void CCustomChangeClass::CommandChangeClassBK(LPOBJ lpObj, char* arg)
{
	this->ChangeClass(lpObj, DB_CLASS_BK);
}

void CCustomChangeClass::CommandChangeClassSM(LPOBJ lpObj, char* arg)
{
	this->ChangeClass(lpObj, DB_CLASS_SM);
}

void CCustomChangeClass::CommandChangeClassME(LPOBJ lpObj, char* arg)
{
	this->ChangeClass(lpObj, DB_CLASS_ME);
}

void CCustomChangeClass::CommandChangeClassMG(LPOBJ lpObj, char* arg)
{
	this->ChangeClass(lpObj, DB_CLASS_MG);
}

void CCustomChangeClass::CGChangeClassRecv(PMSG_CUSTOM_CHANGE_CLASS_REQ* lpMsg, int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	bool result = this->ChangeClass(lpObj, lpMsg->TargetClass);

	PMSG_CUSTOM_CHANGE_CLASS_ANS pMsg;
	pMsg.h.set(0xF3, 0xE5, sizeof(pMsg));
	pMsg.Result = (result ? 0 : 1);
	DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
}

