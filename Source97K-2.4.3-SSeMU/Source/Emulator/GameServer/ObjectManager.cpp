// ObjectManager.cpp: implementation of the CObjectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectManager.h"
#include "Attack.h"
#include "BattleSoccer.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "ChaosBox.h"
#include "CommandManager.h"
#include "Console.h"
#include "CustomArena.h"
#include "CustomAttack.h"
#include "CustomMonster.h"
#include "CustomPick.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "ExperienceTable.h"
#include "GameMaster.h"
#include "Gate.h"
#include "GuildManager.h"
#include "HackMoveSpeedCheck.h"
#include "InvasionManager.h"
#include "ItemOption.h"
#include "JSProtocol.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "Message.h"
#include "Monster.h"
#include "Move.h"
#include "Notice.h"
#include "Party.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "Reconnect.h"
#include "SerialCheck.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "SkillUseArea.h"
#include "Trade.h"
#include "Util.h"
#include "ScriptLoader.h"

CObjectManager gObjectManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectManager::CObjectManager() // OK
{
	this->m_ObjectList.clear();

	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		this->m_ObjectList.push_back(n);
	}
}

CObjectManager::~CObjectManager() // OK
{

}

void CObjectManager::ObjectMsgProc(LPOBJ lpObj) // OK
{
	for(int n=0;n < MAX_MONSTER_SEND_MSG;n++)
	{
		if(gSMMsg[lpObj->Index][n].MsgCode != -1 && GetTickCount() > ((DWORD)gSMMsg[lpObj->Index][n].MsgTime))
		{
			if(lpObj->Type == OBJECT_MONSTER || lpObj->Type == OBJECT_NPC)
			{
				gObjMonsterStateProc(lpObj,gSMMsg[lpObj->Index][n].MsgCode,gSMMsg[lpObj->Index][n].SendUser,gSMMsg[lpObj->Index][n].SubCode);
				gSMMsg[lpObj->Index][n].Clear();
			}
			else
			{
				this->ObjectStateProc(lpObj,gSMMsg[lpObj->Index][n].MsgCode,gSMMsg[lpObj->Index][n].SendUser,gSMMsg[lpObj->Index][n].SubCode);
				gSMMsg[lpObj->Index][n].Clear();
			}
		}
	}
}

void CObjectManager::ObjectSetStateCreate(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if(lpObj->DieRegen == 1 && (GetTickCount()-lpObj->RegenTime) > (lpObj->MaxRegenTime+1000))
	{
		lpObj->DieRegen = 2;
		lpObj->State = OBJECT_DIECMD;
		this->CharacterCalcAttribute(aIndex);
	}

	if(lpObj->Type == OBJECT_USER && lpObj->Teleport == 2)
	{
		lpObj->Teleport = 3;

		lpObj->TX = lpObj->X;
		lpObj->TY = lpObj->Y;

		gObjClearViewport(lpObj);

		gMove.GCTeleportSend(aIndex,0,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,lpObj->Dir);

		gObjViewportListProtocolCreate(lpObj);

		lpObj->Teleport = 0;
		lpObj->PathCur = 0;
		lpObj->PathCount = 0;
		lpObj->PathStartEnd = 0;
	}

	if(lpObj->Type == OBJECT_MONSTER && lpObj->Teleport == 2)
	{
		lpObj->Teleport = 3;

		lpObj->TX = lpObj->X;
		lpObj->TY = lpObj->Y;

		gObjViewportListProtocolCreate(lpObj);

		lpObj->Teleport = 0;
		lpObj->PathCur = 0;
		lpObj->PathCount = 0;
		lpObj->PathStartEnd = 0;
	}

	if(lpObj->Type == OBJECT_NPC && lpObj->Teleport == 2)
	{
		lpObj->Teleport = 3;

		if(OBJECT_RANGE(lpObj->SummonIndex) == 0)
		{
			lpObj->X = lpObj->X+((GetLargeRand()%6)-3);
			lpObj->Y = lpObj->Y+((GetLargeRand()%6)-3);
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;
		}
		else
		{
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;
		}

		gObjViewportListProtocolCreate(lpObj);

		lpObj->Teleport = 0;
		lpObj->PathCur = 0;
		lpObj->PathCount = 0;
		lpObj->PathStartEnd = 0;
	}

	lpObj->Teleport = ((lpObj->Teleport==1)?((lpObj->DieRegen==0)?(((GetTickCount()-lpObj->TeleportTime)>100)?2:lpObj->Teleport):0):lpObj->Teleport);

	if(lpObj->Type == OBJECT_USER && lpObj->RegenOk == 2)
	{
		lpObj->RegenOk = 3;
		lpObj->State = OBJECT_CREATE;
		lpObj->X = lpObj->RegenMapX;
		lpObj->Y = lpObj->RegenMapY;
		lpObj->TX = lpObj->RegenMapX;
		lpObj->TY = lpObj->RegenMapY;
		lpObj->Map = lpObj->RegenMapNumber;
	}
}

void CObjectManager::ObjectSetStateProc() // OK
{
	for(int n=0;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		lpObj->Teleport = ((lpObj->Teleport==3)?0:lpObj->Teleport);

		if(lpObj->State == OBJECT_CREATE)
		{
			lpObj->State = OBJECT_PLAYING;
			lpObj->RegenOk = ((lpObj->RegenOk==3)?0:lpObj->RegenOk);
		}

		if(lpObj->Type == OBJECT_MONSTER)
		{
			if(lpObj->DieRegen != 2)
			{
				continue;
			}

			lpObj->AttackerKilled = 0;

			if(lpObj->RegenType != 0)
			{
				continue;
			}

			if(BC_MAP_RANGE(lpObj->Map) != 0 && lpObj->Class >= 131 && lpObj->Class <= 134)
			{
				gObjDel(lpObj->Index);
				continue;
			}

			if(lpObj->Attribute == ATTRIBUTE_SUMMON)
			{
				gObjDel(lpObj->Index);
				continue;
			}

			lpObj->Live = 1;
			lpObj->ViewState = VIEW_STATE_NONE;
			lpObj->Teleport = 0;
			lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
			lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;

			gEffectManager.ClearAllEffect(lpObj);

			if(gObjMonsterRegen(lpObj) == 0)
			{
				continue;
			}

			lpObj->DieRegen = 0;
			lpObj->State = OBJECT_CREATE;

			gObjViewportListProtocolCreate(lpObj);
		}

		if(lpObj->Type == OBJECT_USER)
		{
			if(lpObj->DieRegen != 2)
			{
				continue;
			}

			lpObj->Live = 1;
			lpObj->ViewState = VIEW_STATE_NONE;
			lpObj->Teleport = 0;
			lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
			lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;
			lpObj->BP = lpObj->MaxBP+lpObj->AddBP;

			lpObj->HPAutoRecuperationTime = GetTickCount();
			lpObj->MPAutoRecuperationTime = GetTickCount();
			lpObj->BPAutoRecuperationTime = GetTickCount();

			memset(lpObj->SelfDefenseTime,0,sizeof(lpObj->SelfDefenseTime));

			gObjTimeCheckSelfDefense(lpObj);

			gEffectManager.ClearAllEffect(lpObj);

			gObjClearViewport(lpObj);

			switch(lpObj->KillerType)
			{
				case 0:
					if(this->CharacterGetRespawnLocation(lpObj) == 0)
					{
						gMap[lpObj->Map].GetMapRandomPos(&lpObj->X,&lpObj->Y,5);
					}
					break;
				case 1:
					if(this->CharacterGetRespawnLocation(lpObj) == 0)
					{
						gMap[lpObj->Map].GetMapRandomPos(&lpObj->X,&lpObj->Y,5);
					}
					break;
				case 2:
					if(lpObj->Guild == 0 || lpObj->Guild->WarType == GUILD_WAR_TYPE_NORMAL)
					{
						gMap[lpObj->Map].GetMapRandomPos(&lpObj->X,&lpObj->Y,18);
					}
					else
					{
						gBattleSoccer.GetMapRandomPos(&lpObj->X,&lpObj->Y);
					}
					break;
			}

			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
			lpObj->PathStartEnd = 0;
			lpObj->DieRegen = 0;
			lpObj->State = OBJECT_CREATE;

			GCCharacterRegenSend(lpObj);

			gObjViewportListProtocolCreate(lpObj);
		}
	}

	for(int n=0;n < MAX_MAP;n++)
	{
		gMap[n].StateSetDestroy();
	}
}

void CObjectManager::ObjectStateProc(LPOBJ lpObj,int MessageCode,int aIndex,int SubCode) // OK
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	switch(MessageCode)
	{
		case 2:
			if(lpObj->Live != 0)
			{
				gObjBackSpring(lpObj,&gObj[aIndex]);
			}
			break;
		case 3:
			if(lpObj->Live != 0)
			{
				this->CharacterMonsterDieHunt(lpObj,&gObj[aIndex]);
			}
			break;
		case 4:
			if(lpObj->Guild != 0 && lpObj->Guild->WarEnemy != 0)
			{
				gGuild.gObjGuildWarEnd(lpObj->Guild,lpObj->Guild->WarEnemy);
			}
			break;
		case 10:
			if(lpObj->Live != 0 && SubCode > 0)
			{
				gAttack.Attack(lpObj,&gObj[aIndex],0,0,0,SubCode,0,0);
			}
			break;
		case 16:
			if((lpObj->Life+SubCode) > (lpObj->MaxLife+lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
			}
			else
			{
				lpObj->Life += SubCode;
			}

			GCLifeSend(lpObj->Index,0xFF,(int)lpObj->Life);
			break;
	}
}

void CObjectManager::ObjectStateAttackProc(LPOBJ lpObj,int MessageCode,int aIndex,int SubCode1,int SubCode2) // OK
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	switch(MessageCode)
	{
		case 50:
			if(lpObj->Live != 0 && (SubCode1 == 0 || gSkillManager.GetSkill(lpObj,SubCode1) != 0))
			{
				gAttack.Attack(lpObj,&gObj[aIndex],gSkillManager.GetSkill(lpObj,SubCode1),0,0,0,0,(bool)(SubCode2&1));
			}
			break;
		case 52:
			if(lpObj->Live != 0 && (SubCode1 == 0 || gSkillManager.GetSkill(lpObj,SubCode1) != 0))
			{
				gAttack.Attack(lpObj,&gObj[aIndex],gSkillManager.GetSkill(lpObj,SubCode1),0,0,0,SubCode2,0);
			}
			break;
		case 53:
			if(lpObj->Live != 0 && (SubCode1 == 0 || gSkillManager.GetSkill(lpObj,SubCode1) != 0))
			{
				gAttack.Attack(lpObj,&gObj[aIndex],gSkillManager.GetSkill(lpObj,SubCode1),1,0,0,0,(bool)(SubCode2&1));
			}
			break;
		case 54:
			if(lpObj->Live != 0 && (SubCode1 == 0 || gSkillManager.GetSkill(lpObj,SubCode1) != 0))
			{
				gAttack.Attack(lpObj,&gObj[aIndex],gSkillManager.GetSkill(lpObj,SubCode1),1,0,0,SubCode2,0);
			}
			break;
	}
}

void CObjectManager::ObjectMoveProc() // OK
{
	for(int n=0;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		if(lpObj->State != OBJECT_PLAYING)
		{
			continue;
		}

		if(lpObj->PathCount == 0 || gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
		{
			continue;
		}

		if(lpObj->Type == OBJECT_MONSTER && (lpObj->Attribute == ATTRIBUTE_KALIMA || lpObj->Class == 131 || lpObj->Class == 132 || lpObj->Class == 133 || lpObj->Class == 134 || lpObj->Class == 204 || lpObj->Class == 205 || lpObj->Class == 206 || lpObj->Class == 207 || lpObj->Class == 208 || lpObj->Class == 209 || lpObj->Class == 216 || lpObj->Class == 217 || lpObj->Class == 218 || lpObj->Class == 219))
		{
			continue;
		}

		DWORD MoveTime = 0;

		if((lpObj->PathDir[lpObj->PathCur]%2) == 0)
		{
			MoveTime = (DWORD)((lpObj->MoveSpeed+((lpObj->DelayLevel==0)?0:300))*(double)1.3);
		}
		else
		{
			MoveTime = (DWORD)((lpObj->MoveSpeed+((lpObj->DelayLevel==0)?0:300))*(double)1.0);
		}

		if((GetTickCount()-lpObj->PathTime) > MoveTime && lpObj->PathCur < (MAX_ROAD_PATH_TABLE-1))
		{
			if(gMap[lpObj->Map].CheckAttr(lpObj->PathX[lpObj->PathCur],lpObj->PathY[lpObj->PathCur],4) != 0 || gMap[lpObj->Map].CheckAttr(lpObj->PathX[lpObj->PathCur],lpObj->PathY[lpObj->PathCur],8) != 0)
			{
				lpObj->PathCur = 0;
				lpObj->PathCount = 0;
				lpObj->PathTime = GetTickCount();
				lpObj->PathStartEnd = ((lpObj->Type==OBJECT_USER)?lpObj->PathStartEnd:0);

				memset(lpObj->PathX,0,sizeof(lpObj->PathX));

				memset(lpObj->PathY,0,sizeof(lpObj->PathY));

				memset(lpObj->PathOri,0,sizeof(lpObj->PathOri));

				gObjSetPosition(lpObj->Index,lpObj->X,lpObj->Y);
			}
			else
			{
				lpObj->X = lpObj->PathX[lpObj->PathCur];
				lpObj->Y = lpObj->PathY[lpObj->PathCur];
				lpObj->Dir = lpObj->PathDir[lpObj->PathCur];
				lpObj->PathTime = GetTickCount();

				if((++lpObj->PathCur) >= lpObj->PathCount)
				{
					lpObj->PathCur = 0;
					lpObj->PathCount = 0;
					lpObj->PathStartEnd = ((lpObj->Type==OBJECT_USER)?lpObj->PathStartEnd:0);
				}
			}
		}
	}
}

void CObjectManager::ObjectMonsterAndMsgProc() // OK
{
	for(int n=0;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) != 0)
		{
			if(gObj[n].Type == OBJECT_MONSTER || gObj[n].Type == OBJECT_NPC)
			{
				gObjectManager.ObjectMsgProc(&gObj[n]);
				gObjMonsterUpdateProc(&gObj[n]);
			}
			else
			{
				gObjSkillNovaCheckTime(&gObj[n]);
				gObjectManager.ObjectMsgProc(&gObj[n]);
			}
		}
	}

	std::random_shuffle(this->m_ObjectList.begin(),this->m_ObjectList.end());

	for each(int n in this->m_ObjectList)
	{
		gCustomAttack.MainProc(&gObj[n]);
		gCustomPick.MainProc(&gObj[n]);
		// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 99 (2.5.8) - Processamento de MoveHack em usuarios conectados
		gHackMoveSpeedCheck.MainProc(&gObj[n]);
	}

	for(int n=0;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) != 0)
		{
			for(int i=0;i < MAX_MONSTER_SEND_ATTACK_MSG;i++)
			{
				if(gSMAttackProcMsg[n][i].MsgCode != -1 && GetTickCount() > ((DWORD)gSMAttackProcMsg[n][i].MsgTime))
				{
					gObjectManager.ObjectStateAttackProc(&gObj[n],gSMAttackProcMsg[n][i].MsgCode,gSMAttackProcMsg[n][i].SendUser,gSMAttackProcMsg[n][i].SubCode,gSMAttackProcMsg[n][i].SubCode2);
					gSMAttackProcMsg[n][i].Clear();
				}
			}
		}
	}
}

bool CObjectManager::CharacterGameClose(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_ONLINE)
	{
		return 0;
	}

	if(lpObj->Type != OBJECT_USER)
	{
		return 0;
	}

	gScriptLoader.OnCharacterClose(aIndex);

	if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonKill(lpObj->Index);
	}

	if(lpObj->Interface.use != 0 && (lpObj->Interface.type == INTERFACE_CHAOS_BOX || lpObj->Interface.type == INTERFACE_TRAINER))
	{
		if(lpObj->IsChaosMixCompleted != 0)
		{
			gChaosBox.ChaosBoxItemSave(lpObj);
		}
		else
		{
			gObjInventoryRollback(aIndex);
		}
	}

	if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
	{
		gTrade.ResetTrade(lpObj->TargetNumber);
		gTrade.GCTradeResultSend(lpObj->TargetNumber,0);
		gTrade.ResetTrade(aIndex);
	}

	gReconnect.SetReconnectInfo(lpObj);

	if(OBJECT_RANGE(lpObj->PartyNumber) != 0)
	{
		// Update 89 2.4.7 -> 97K - Reconnect Time Unificado
		if(gParty.GetMemberCount(lpObj->PartyNumber) <= ((gServerInfo.m_ReconnectTime == 0) ? 2 : 1))
		{
			gParty.Destroy(lpObj->PartyNumber);
		}
		else
		{
			gParty.DelMember(lpObj->PartyNumber,lpObj->Index);
		}
	}

	if(lpObj->Guild != 0 && lpObj->Guild->WarState == 1)
	{
		gGuild.gObjGuildWarMasterClose(lpObj);
	}

	if(BC_MAP_RANGE(lpObj->Map) != 0)
	{
		if(gBloodCastle.GetState(GET_BC_LEVEL(lpObj->Map)) == BC_STATE_START)
		{
			gBloodCastle.SearchUserDropEventItem(aIndex);
		}
		else
		{
			gBloodCastle.SearchUserDeleteEventItem(aIndex);
		}
	}

	gObjFixInventoryPointer(aIndex);

	GDCharacterInfoSaveSend(aIndex);

	GDDisconnectCharacterSend(aIndex);


	gObjClearViewport(lpObj);

	// Update 91 2.4.9 -> 97K - Reset de rastreamento de movimento ao deslogar personagem
	gHackMoveSpeedCheck.Reset(lpObj);

	gLog.Output(LOG_CONNECT,"[ObjectManager][%d] DelCharacterInfo [%s] [%s][%s]",lpObj->Index,lpObj->Name,lpObj->IpAddr,lpObj->HardwareId);

	memset(lpObj->Name,0,sizeof(lpObj->Name));

	lpObj->Connected = OBJECT_LOGGED;

	return 1;
}

void CObjectManager::CharacterGameCloseSet(int aIndex,int type) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->CloseCount > 0)
	{
		return;
	}

	if(lpObj->DieRegen != 0)
	{
		return;
	}

	if(lpObj->Connected == OBJECT_ONLINE)
	{
		if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_TRADE)
		{
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(261));
			return;
		}

		if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_WAREHOUSE)
		{
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(262));
			return;
		}

		if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_CHAOS_BOX)
		{
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(263));
			return;
		}

		if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_TRAINER)
		{
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(264));
			return;
		}

		for(int n=0;n < MAX_SELF_DEFENSE;n++)
		{
			if(lpObj->SelfDefense[n] >= 0)
			{
				if(GetTickCount() < lpObj->SelfDefenseTime[n])
				{
					gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(265));
					return;
				}
			}
		}
	}

	lpObj->CloseCount = 6;

	lpObj->CloseType = ((type>2)?0:type);

	lpObj->EnableDelCharacter = 1;
}

bool CObjectManager::CharacterGetRespawnLocation(LPOBJ lpObj) // OK
{
	bool result = 0;

	int gate,map,x,y,dir,level,target;

	if((target=gMapManager.GetRespawnGate(lpObj->Map)) != -1)
	{
		if(target == 0)
		{
			return 1;
		}

		result = gGate.GetGate(target,&gate,&map,&x,&y,&dir,&level);
	}
	else if(gCustomArena.CheckMap(lpObj->Map) != 0)
	{
		result = gCustomArena.GetUserRespawnLocation(lpObj,&gate,&map,&x,&y,&dir,&level);
	}
	else if(lpObj->Map == MAP_LORENCIA)
	{
		result = gGate.GetGate(17,&gate,&map,&x,&y,&dir,&level);
	}
	else if(lpObj->Map == MAP_DUNGEON)
	{
		result = gGate.GetGate(17,&gate,&map,&x,&y,&dir,&level);
	}
	else if(lpObj->Map == MAP_DEVIAS)
	{
		result = gGate.GetGate(22,&gate,&map,&x,&y,&dir,&level);
	}
	else if(lpObj->Map == MAP_NORIA)
	{
		result = gGate.GetGate(27,&gate,&map,&x,&y,&dir,&level);
	}
	else if(lpObj->Map == MAP_LOST_TOWER)
	{
		result = gGate.GetGate(42,&gate,&map,&x,&y,&dir,&level);
	}
	else if(lpObj->Map == MAP_ARENA)
	{
		result = gGate.GetGate(50,&gate,&map,&x,&y,&dir,&level);
	}
	else if(lpObj->Map == MAP_ATLANS)
	{
		result = gGate.GetGate(49,&gate,&map,&x,&y,&dir,&level);
	}
	else if(lpObj->Map == MAP_TARKAN)
	{
		result = gGate.GetGate(57,&gate,&map,&x,&y,&dir,&level);
	}
	else if(DS_MAP_RANGE(lpObj->Map) != 0)
	{
		result = gGate.GetGate(27,&gate,&map,&x,&y,&dir,&level);
	}
	else if(lpObj->Map == MAP_ICARUS)
	{
		result = gGate.GetGate(22,&gate,&map,&x,&y,&dir,&level);
	}
	else if(BC_MAP_RANGE(lpObj->Map) != 0)
	{
		result = gGate.GetGate(22,&gate,&map,&x,&y,&dir,&level);
	}
	
	if(result != 0)
	{
		lpObj->Map = map;
		lpObj->X = x;
		lpObj->Y = y;
		lpObj->Dir = dir;
	}

	return result;
}

void CObjectManager::CharacterCalcExperienceSplit(LPOBJ lpObj,LPOBJ lpMonster,int damage,int flag) // OK
{
	if(lpMonster->Type != OBJECT_MONSTER)
	{
		return;
	}

	gObjMonsterDelHitDamageUser(lpMonster);

	lpMonster->Money = 0;

	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		if(OBJECT_RANGE(lpMonster->HitDamage[n].index) == 0)
		{
			continue;
		}

		if(lpMonster->HitDamage[n].damage == 0)
		{
			continue;
		}

		if(lpMonster->HitDamage[n].index == lpObj->Index)
		{
			this->CharacterCalcExperienceAlone(&gObj[lpMonster->HitDamage[n].index],lpMonster,lpMonster->HitDamage[n].damage,flag,0,damage);
		}
		else
		{
			this->CharacterCalcExperienceAlone(&gObj[lpMonster->HitDamage[n].index],lpMonster,lpMonster->HitDamage[n].damage,flag,1,damage);
		}
	}
}

void CObjectManager::CharacterCalcExperienceAlone(LPOBJ lpObj,LPOBJ lpMonster,int damage,int flag,bool type,int AttackDamage) // OK
{
	if(lpMonster->Type != OBJECT_MONSTER)
	{
		return;
	}

	int level = ((lpMonster->Level+25)*lpMonster->Level)/3;

	level = (((lpMonster->Level+10)<lpObj->Level)?((level*(lpMonster->Level+10))/lpObj->Level):level);

	if(lpMonster->Level >= 65)
	{
		level += (lpMonster->Level-64)*(lpMonster->Level/4);
	}

	level = ((level<0)?0:level);

	damage = ((damage>lpMonster->MaxLife)?(int)lpMonster->MaxLife:damage);

	QWORD experience = level+(level/4);

	experience = ((damage*experience)/(int)lpMonster->MaxLife)*gServerInfo.m_AddExperienceRate[lpObj->AccountLevel];

	experience = (experience*gMapManager.GetMapExperienceRate(lpObj->Map))/100;

	experience = (experience*gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_EXPERIENCE_RATE,100))/100;

	experience = (experience*gCustomMonster.GetCustomMonsterExperienceRate(lpMonster->Class,lpMonster->Map))/100;

	experience = (experience*gExperienceTable.GetExperienceRate(lpObj))/100;

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 97 (2.5.5) - Custom attack experience rate
	experience = (experience*gCustomAttack.GetExperienceRate(lpObj))/100;

	lpMonster->Money += (DWORD)experience;

	this->CharacterPetLevelUp(lpObj,(DWORD)experience);

	if(this->CharacterLevelUp(lpObj,(DWORD)experience,gServerInfo.m_MaxLevelUp,EXPERIENCE_COMMON) == 0)
	{
		GCMonsterDieSend(lpObj->Index,lpMonster->Index,(DWORD)experience,((type==0)?AttackDamage:0),flag);
	}
	else
	{
		GCMonsterDieSend(lpObj->Index,lpMonster->Index,0,((type==0)?AttackDamage:0),flag);
	}
}

void CObjectManager::CharacterCalcExperienceParty(LPOBJ lpObj,LPOBJ lpMonster,int damage,int flag) // OK
{
	if(OBJECT_RANGE(lpObj->PartyNumber) == 0)
	{
		return;
	}

	if(lpMonster->Type != OBJECT_MONSTER)
	{
		return;
	}

	PARTY_INFO* lpParty = &gParty.m_PartyInfo[lpObj->PartyNumber];

	int PartyTopLevel = 0;

	for(int n=0;n < MAX_PARTY_USER;n++)
	{
		if(OBJECT_RANGE(lpParty->Index[n]) != 0)
		{
			if(lpMonster->Map == gObj[lpParty->Index[n]].Map && gObjCalcDistance(lpMonster,&gObj[lpParty->Index[n]]) < MAX_PARTY_DISTANCE)
			{
				PartyTopLevel = ((gObj[lpParty->Index[n]].Level>PartyTopLevel)?gObj[lpParty->Index[n]].Level:PartyTopLevel);
			}
		}
	}

	int TotalLevel = 0;
	int PartyTable = 0;
	int PartyClass = 0;
	int PartyCount = 0;

	for(int n=0;n < MAX_PARTY_USER;n++)
	{
		if(OBJECT_RANGE(lpParty->Index[n]) != 0)
		{
			if(gObj[lpParty->Index[n]].Map == lpMonster->Map && gObjCalcDistance(lpMonster,&gObj[lpParty->Index[n]]) < MAX_PARTY_DISTANCE)
			{
				TotalLevel += (((gObj[lpParty->Index[n]].Level+200)<PartyTopLevel)?(gObj[lpParty->Index[n]].Level+200):gObj[lpParty->Index[n]].Level);
				
				if((PartyTable & (1 << gObj[lpParty->Index[n]].Class)) == 0)
				{
					PartyTable |= (1 << gObj[lpParty->Index[n]].Class);
					PartyClass++;
				}

				PartyCount++;
			}
		}
	}

	if(PartyCount == 0){return;}

	int ExperienceRate = (((PartyClass>=3)?gServerInfo.m_PartySpecialExperience[(PartyCount-1)]:gServerInfo.m_PartyGeneralExperience[(PartyCount-1)])*PartyCount);

	int PartyLevel = TotalLevel/PartyCount;

	int level = ((lpMonster->Level+25)*lpMonster->Level)/3;

	if((lpMonster->Level+10) < PartyLevel)
	{
		level = (level*(lpMonster->Level+10))/PartyLevel;
	}

	if(lpMonster->Level >= 65)
	{
		if(PartyCount == 1)
		{
			level += (lpMonster->Level-64)*(lpMonster->Level/4);
		}
		else
		{
			level += (200-(lpObj->Level/5));
		}
	}

	level = ((level<0)?0:level);

	DWORD TotalExperience = level+(level/4);

	lpMonster->Money = 0;

	for(int n=0;n < MAX_PARTY_USER;n++)
	{
		if(OBJECT_RANGE(lpParty->Index[n]) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpParty->Index[n]];

		if(lpTarget->Map != lpMonster->Map || gObjCalcDistance(lpMonster,lpTarget) >= MAX_PARTY_DISTANCE)
		{
			continue;
		}

		QWORD experience = 0;

		experience = ((((TotalExperience*ExperienceRate)*lpTarget->Level)/TotalLevel)/100)*gServerInfo.m_AddExperienceRate[lpTarget->AccountLevel];

		experience = (experience*gMapManager.GetMapExperienceRate(lpTarget->Map))/100;

		experience = (experience*gBonusManager.GetBonusValue(lpTarget,BONUS_INDEX_EXPERIENCE_RATE,100))/100;

		experience = (experience*gCustomMonster.GetCustomMonsterExperienceRate(lpMonster->Class,lpMonster->Map))/100;

		experience = (experience*gExperienceTable.GetExperienceRate(lpTarget))/100;

		// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 97 (2.5.5) - Custom attack experience rate
		experience = (experience*gCustomAttack.GetExperienceRate(lpTarget))/100;

		lpMonster->Money += (DWORD)(experience/PartyCount);

		this->CharacterPetLevelUp(lpTarget,(DWORD)experience);

		if(this->CharacterLevelUp(lpTarget,(DWORD)experience,gServerInfo.m_MaxLevelUp,EXPERIENCE_PARTY) == 0)
		{
			GCMonsterDieSend(lpTarget->Index,lpMonster->Index,(DWORD)experience,damage,flag);
		}
		else
		{
			GCMonsterDieSend(lpTarget->Index,lpMonster->Index,0,damage,flag);
		}
	}
}

bool CObjectManager::CharacterLevelUp(LPOBJ lpObj,DWORD AddExperience,int MaxLevelUp,int ExperienceType) // OK
{
	if(lpObj->Level >= gServerInfo.m_MaxLevel)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(266));
		return 1;
	}

	if((lpObj->Experience+AddExperience) < lpObj->NextExperience)
	{
		lpObj->Experience += AddExperience;
		return 0;
	}

	while(true)
	{
		lpObj->Level++;

		lpObj->LevelUpPoint += gServerInfo.m_LevelUpPoint[lpObj->Class][lpObj->AccountLevel];

		lpObj->LevelUpPoint += ((lpObj->Level>gServerInfo.m_PlusStatMinLevel)?((gQuest.CheckQuestListState(lpObj,2,QUEST_FINISH)==0)?0:gServerInfo.m_PlusStatPoint):0);

		AddExperience -= (((--MaxLevelUp)==0)?AddExperience:(lpObj->NextExperience-lpObj->Experience));

		lpObj->Experience = lpObj->NextExperience;

		gObjCalcExperience(lpObj);

		if(lpObj->Level >= gServerInfo.m_MaxLevel)
		{
			AddExperience = 0;
			break;
		}

		if((lpObj->Experience+AddExperience) < lpObj->NextExperience)
		{
			lpObj->Experience += AddExperience;
			break;
		}
	}

	this->CharacterCalcAttribute(lpObj->Index);

	lpObj->Life = lpObj->MaxLife+lpObj->AddLife;

	lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;

	lpObj->BP = lpObj->MaxBP+lpObj->AddBP;

	GCLevelUpSend(lpObj);

	if((GetTickCount()-lpObj->CharSaveTime) > 60000)
	{
		lpObj->CharSaveTime = GetTickCount();
		GDCharacterInfoSaveSend(lpObj->Index);
	}

	return 1;
}

bool CObjectManager::CharacterLevelUpPointAdd(LPOBJ lpObj,int type,int amount) // OK
{
	if(lpObj->Type != OBJECT_USER)
	{
		return 0;
	}

	if(amount <= 0 || lpObj->LevelUpPoint < amount)
	{
		return 0;
	}

	int* stat;

	switch(type)
	{
		case 0:
			stat = &lpObj->Strength;
			break;
		case 1:
			stat = &lpObj->Dexterity;
			break;
		case 2:
			stat = &lpObj->Vitality;
			break;
		case 3:
			stat = &lpObj->Energy;
			break;
		case 4:
			stat = &lpObj->Leadership;
			break;
		default:
			return 0;
	}

	if(((*stat)+amount) > gServerInfo.m_MaxStatPoint[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(268),gServerInfo.m_MaxStatPoint[lpObj->AccountLevel]);
		return 0;
	}

	(*stat) += amount;

	lpObj->LevelUpPoint -= amount;

	this->CharacterCalcAttribute(lpObj->Index);
	return 1;
}

void CObjectManager::CharacterPetLevelUp(LPOBJ lpObj,DWORD AddExperience) // OK
{
	if(lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,4)) // Dark Horse
	{
		if(lpObj->Inventory[INVENTORY_SLOT_HELPER].AddPetItemExp(AddExperience) == 0)
		{
			GCPetItemInfoSend(lpObj->Index,1,0,8,lpObj->Inventory[INVENTORY_SLOT_HELPER].m_PetItemLevel,lpObj->Inventory[INVENTORY_SLOT_HELPER].m_PetItemExp,(BYTE)lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Durability);
		}
		else
		{
			if(gItemManager.IsValidItem(lpObj,&lpObj->Inventory[INVENTORY_SLOT_HELPER]) == 0)
			{
				if(lpObj->Inventory[INVENTORY_SLOT_HELPER].DecPetItemExp(AddExperience) != 0)
				{
					GCPetItemInfoSend(lpObj->Index,1,0,8,lpObj->Inventory[INVENTORY_SLOT_HELPER].m_PetItemLevel,lpObj->Inventory[INVENTORY_SLOT_HELPER].m_PetItemExp,(BYTE)lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Durability);
				}
			}
			else
			{
				GCPetItemInfoSend(lpObj->Index,1,0xFE,8,lpObj->Inventory[INVENTORY_SLOT_HELPER].m_PetItemLevel,lpObj->Inventory[INVENTORY_SLOT_HELPER].m_PetItemExp,(BYTE)lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Durability);
			}
		}
	}

	if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index == GET_ITEM(13,5)) // Dark Reaven
	{
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].AddPetItemExp(AddExperience) == 0)
		{
			GCPetItemInfoSend(lpObj->Index,0,0,1,lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_PetItemLevel,lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_PetItemExp,(BYTE)lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Durability);
		}
		else
		{
			if(gItemManager.IsValidItem(lpObj,&lpObj->Inventory[INVENTORY_SLOT_WEAPON2]) == 0)
			{
				if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].DecPetItemExp(AddExperience) != 0)
				{
					GCPetItemInfoSend(lpObj->Index,0,0,1,lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_PetItemLevel,lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_PetItemExp,(BYTE)lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Durability);
				}
			}
			else
			{
				GCPetItemInfoSend(lpObj->Index,0,0xFE,1,lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_PetItemLevel,lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_PetItemExp,(BYTE)lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Durability);
			}
		}
	}
}

void CObjectManager::CharacterMakePreviewCharSet(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	memset(lpObj->CharSet,0,sizeof(lpObj->CharSet));

	lpObj->CharSet[0] = (lpObj->ChangeUp*16);
	lpObj->CharSet[0] -= (lpObj->CharSet[0]/32);
	lpObj->CharSet[0] += (lpObj->Class*32);

	if(lpObj->ActionNumber == ACTION_SIT1)
	{
		lpObj->CharSet[0] |= 2;
	}
	
	if(lpObj->ActionNumber == ACTION_POSE1)
	{
		lpObj->CharSet[0] |= 3;
	}

	WORD TempInventory[INVENTORY_WEAR_SIZE];

	for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
	{
		if(n == 0 || n == 1)
		{
			if(lpObj->Inventory[n].IsItem() == 0)
			{
				TempInventory[n] = 0xFFFF;
			}
			else
			{
				TempInventory[n] = lpObj->Inventory[n].m_Index;
			}
		}
		else
		{
			if(lpObj->Inventory[n].IsItem() == 0)
			{
				TempInventory[n] = (MAX_ITEM_TYPE-1);
			}
			else
			{
				TempInventory[n] = lpObj->Inventory[n].m_Index%MAX_ITEM_TYPE;
			}
		}
	}

	lpObj->CharSet[1] = TempInventory[0]%256;

	lpObj->CharSet[2] = TempInventory[1]%256;

	lpObj->CharSet[3] |= (TempInventory[2] & 0x0F) << 4;
	lpObj->CharSet[9] |= (TempInventory[2] & 0x10) << 3;

	lpObj->CharSet[3] |= (TempInventory[3] & 0x0F);
	lpObj->CharSet[9] |= (TempInventory[3] & 0x10) << 2;

	lpObj->CharSet[4] |= (TempInventory[4] & 0x0F) << 4;
	lpObj->CharSet[9] |= (TempInventory[4] & 0x10) << 1;

	lpObj->CharSet[4] |= (TempInventory[5] & 0x0F);
	lpObj->CharSet[9] |= (TempInventory[5] & 0x10);

	lpObj->CharSet[5] |= (TempInventory[6] & 0x0F) << 4;
	lpObj->CharSet[9] |= (TempInventory[6] & 0x10) >> 1;

	int level = 0;

	BYTE table[7] = {1,0,6,5,4,3,2};

	for(int n=0;n < 7;n++)
	{
		if(TempInventory[n] != (MAX_ITEM_TYPE-1) && TempInventory[n] != 0xFFFF)
		{
			level |= LevelSmallConvert(lpObj->Inventory[n].m_Level) << (n*3);

			lpObj->CharSet[10] |= ((lpObj->Inventory[n].m_NewOption & 0x3F)?2:0) << table[n];
		}
	}

	this->CharacterCalcAttribute(aIndex);

	lpObj->CharSet[6] = level >> 16;
	lpObj->CharSet[7] = level >> 8;
	lpObj->CharSet[8] = level;

	if(TempInventory[INVENTORY_SLOT_WING] == (MAX_ITEM_TYPE-1))
	{
		lpObj->CharSet[5] |= 12;
	}
	else if(TempInventory[INVENTORY_SLOT_WING] >= 0 && TempInventory[INVENTORY_SLOT_WING] <= 2)
	{
		lpObj->CharSet[5] |= TempInventory[INVENTORY_SLOT_WING] << 2;
	}
	else if(TempInventory[INVENTORY_SLOT_WING] >= 3 && TempInventory[INVENTORY_SLOT_WING] <= 6)
	{
		lpObj->CharSet[5] |= 12;
		lpObj->CharSet[9] |= TempInventory[INVENTORY_SLOT_WING]-2;
	}
	else if(TempInventory[INVENTORY_SLOT_WING] == 30)
	{
		lpObj->CharSet[5] |= 12;
		lpObj->CharSet[9] |= 5;
	}

	if(TempInventory[INVENTORY_SLOT_HELPER] == (MAX_ITEM_TYPE-1))
	{
		lpObj->CharSet[5] |= 3;
	}
	else if(TempInventory[INVENTORY_SLOT_HELPER] >= 0 && TempInventory[INVENTORY_SLOT_HELPER] <= 2)
	{
		lpObj->CharSet[5] |= TempInventory[INVENTORY_SLOT_HELPER];
	}
	else if(TempInventory[INVENTORY_SLOT_HELPER] == 3)
	{
		lpObj->CharSet[5] |= 3;
		lpObj->CharSet[10] |= 1;
	}
}

bool CObjectManager::CharacterUseScroll(LPOBJ lpObj,CItem* lpItem) // OK
{
	int skill,slot;

	if((lpObj->Strength+lpObj->AddStrength) < lpItem->m_RequireStrength)
	{
		return 0;
	}

	if((lpObj->Dexterity+lpObj->AddDexterity) < lpItem->m_RequireDexterity)
	{
		return 0;
	}

	if(lpItem->m_Index >= GET_ITEM(12,8) && lpItem->m_Index <= GET_ITEM(12,24))
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return 0;
		}
	}

	if(lpItem->m_Index == GET_ITEM(15,18)) // Scroll of Nova
	{
		if(gQuest.CheckQuestListState(lpObj,2,QUEST_FINISH) == 0)
		{
			return 0;
		}
	}

	if(gItemManager.CheckItemRequireClass(lpObj,lpItem->m_Index) == 0)
	{
		return 0;
	}

	if((skill=gSkillManager.GetSkillNumber(lpItem->m_Index,lpItem->m_Level)) == -1)
	{
		return 0;
	}

	if((slot=gSkillManager.AddSkill(lpObj,skill)) == -1)
	{
		return 0;
	}

	gSkillManager.GCSkillAddSend(lpObj->Index,slot,skill,(BYTE)lpItem->m_Level);
	return 1;
}

bool CObjectManager::CharacterUsePotion(LPOBJ lpObj,CItem* lpItem) // OK
{
	if(gServerInfo.m_CheckAutoPotionHack != 0)
	{
		DWORD PotionTime = (GetTickCount()-lpObj->PotionTime);

		if(PotionTime < (DWORD)gServerInfo.m_CheckAutoPotionHackTolerance)
		{
			Console(1,"[HackAutoPotionCheck][%s] Time [%d][%d]",lpObj->Name,PotionTime,gServerInfo.m_CheckAutoPotionHackTolerance);

			if(gServerInfo.m_CheckAutoPotionHackAction == 1)
			{
				gObjUserKill(lpObj->Index);
			}
			else if(gServerInfo.m_CheckAutoPotionHackAction == 2)
			{
				GCCloseClientSend(lpObj->Index,0);
			}

			return 0;
		}
	}

	lpObj->PotionTime = GetTickCount();

	int HPValue = 0;
	int MPValue = 0;
	int BPValue = 0;

	switch(lpItem->m_Index)
	{
		case GET_ITEM(14,0):
			HPValue = (int)(((lpObj->MaxLife+lpObj->AddLife)*gServerInfo.m_ApplePotionRate[lpObj->Class])/100);
			break;
		case GET_ITEM(14,1):
			HPValue = (int)(((lpObj->MaxLife+lpObj->AddLife)*gServerInfo.m_SmallLifePotionRate[lpObj->Class])/100);
			break;
		case GET_ITEM(14,2):
			HPValue = (int)(((lpObj->MaxLife+lpObj->AddLife)*gServerInfo.m_MidleLifePotionRate[lpObj->Class])/100);
			break;
		case GET_ITEM(14,3):
			HPValue = (int)(((lpObj->MaxLife+lpObj->AddLife)*gServerInfo.m_LargeLifePotionRate[lpObj->Class])/100);
			break;
		case GET_ITEM(14,4):
			MPValue = (int)(((lpObj->MaxMana+lpObj->AddMana)*gServerInfo.m_SmallManaPotionRate[lpObj->Class])/100);
			break;
		case GET_ITEM(14,5):
			MPValue = (int)(((lpObj->MaxMana+lpObj->AddMana)*gServerInfo.m_MidleManaPotionRate[lpObj->Class])/100);
			break;
		case GET_ITEM(14,6):
			MPValue = (int)(((lpObj->MaxMana+lpObj->AddMana)*gServerInfo.m_LargeManaPotionRate[lpObj->Class])/100);
			break;
	}

	if((lpObj->Life+HPValue) > (lpObj->MaxLife+lpObj->AddLife))
	{
		lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
	}
	else
	{
		lpObj->Life += HPValue;
	}

	GCLifeSend(lpObj->Index,0xFF,(int)lpObj->Life);

	if((lpObj->Mana+MPValue) > (lpObj->MaxMana+lpObj->AddMana))
	{
		lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;
	}
	else
	{
		lpObj->Mana += MPValue;
	}

	if((lpObj->BP+BPValue) > (lpObj->MaxBP+lpObj->AddBP))
	{
		lpObj->BP = lpObj->MaxBP+lpObj->AddBP;
	}
	else
	{
		lpObj->BP += BPValue;
	}

	GCManaSend(lpObj->Index,0xFF,(int)lpObj->Mana,lpObj->BP);
	return 1;
}

bool CObjectManager::CharacterUsePortal(LPOBJ lpObj,CItem* lpItem) // OK
{
	if(lpObj->Interface.use != 0)
	{
		return 0;
	}

	if(lpObj->Map == MAP_LORENCIA)
	{
		gObjMoveGate(lpObj->Index,17);
	}
	else if(lpObj->Map == MAP_DUNGEON)
	{
		gObjMoveGate(lpObj->Index,17);
	}
	else if(lpObj->Map == MAP_DEVIAS)
	{
		gObjMoveGate(lpObj->Index,22);
	}
	else if(lpObj->Map == MAP_NORIA)
	{
		gObjMoveGate(lpObj->Index,27);
	}
	else if(lpObj->Map == MAP_LOST_TOWER)
	{
		gObjMoveGate(lpObj->Index,42);
	}
	else if(lpObj->Map == MAP_ARENA)
	{
		gObjMoveGate(lpObj->Index,50);
	}
	else if(lpObj->Map == MAP_ATLANS)
	{
		gObjMoveGate(lpObj->Index,49);
	}
	else if(lpObj->Map == MAP_TARKAN)
	{
		gObjMoveGate(lpObj->Index,57);
	}
	else if(DS_MAP_RANGE(lpObj->Map) != 0)
	{
		gObjMoveGate(lpObj->Index,27);
	}
	else if(lpObj->Map == MAP_ICARUS)
	{
		gObjMoveGate(lpObj->Index,42);
	}
	else if(BC_MAP_RANGE(lpObj->Map) != 0)
	{
		gObjMoveGate(lpObj->Index,22);
	}
	else
	{
		gObjMoveGate(lpObj->Index,17);
	}

	return 1;
}

bool CObjectManager::CharacterUseJewelOfBles(LPOBJ lpObj,int SourceSlot,int TargetSlot) // OK
{
	if(INVENTORY_FULL_RANGE(SourceSlot) == 0)
	{
		return 0;
	}

	if(INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0;
	}

	if(lpObj->Inventory[SourceSlot].IsItem() == 0)
	{
		return 0;
	}

	if(lpObj->Inventory[TargetSlot].IsItem() == 0)
	{
		return 0;
	}

	if(lpObj->Inventory[TargetSlot].m_IsPeriodicItem != 0)
	{
		return 0;
	}

	CItem* lpItem = &lpObj->Inventory[TargetSlot];

	int slot = gItemManager.GetItemSlot(lpItem->m_Index);

	if(slot != INVENTORY_SLOT_WEAPON1 && slot != INVENTORY_SLOT_WEAPON2 && slot != INVENTORY_SLOT_HELM && slot != INVENTORY_SLOT_ARMOR && slot != INVENTORY_SLOT_PANTS && slot != INVENTORY_SLOT_GLOVES && slot != INVENTORY_SLOT_BOOTS && slot != INVENTORY_SLOT_WING)
	{
		return 0;
	}

	if(lpItem->m_Level >= 6)
	{
		return 0;
	}

	lpItem->m_Level++;

	float dur = (float)gItemManager.GetItemDurability(lpItem->m_Index,lpItem->m_Level,lpItem->IsExcItem(),lpItem->IsSetItem());

	lpItem->m_Durability = (dur*lpItem->m_Durability)/lpItem->m_BaseDurability;

	lpItem->Convert(lpItem->m_Index,lpItem->m_Option1,lpItem->m_Option2,lpItem->m_Option3,lpItem->m_NewOption,lpItem->m_SetOption);

	this->CharacterMakePreviewCharSet(lpObj->Index);
	return 1;
}

bool CObjectManager::CharacterUseJewelOfSoul(LPOBJ lpObj,int SourceSlot,int TargetSlot) // OK
{
	if(INVENTORY_FULL_RANGE(SourceSlot) == 0)
	{
		return 0;
	}

	if(INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0;
	}

	if(lpObj->Inventory[SourceSlot].IsItem() == 0)
	{
		return 0;
	}

	if(lpObj->Inventory[TargetSlot].IsItem() == 0)
	{
		return 0;
	}

	if(lpObj->Inventory[TargetSlot].m_IsPeriodicItem != 0)
	{
		return 0;
	}

	CItem* lpItem = &lpObj->Inventory[TargetSlot];

	int slot = gItemManager.GetItemSlot(lpItem->m_Index);

	if(slot != INVENTORY_SLOT_WEAPON1 && slot != INVENTORY_SLOT_WEAPON2 && slot != INVENTORY_SLOT_HELM && slot != INVENTORY_SLOT_ARMOR && slot != INVENTORY_SLOT_PANTS && slot != INVENTORY_SLOT_GLOVES && slot != INVENTORY_SLOT_BOOTS && slot != INVENTORY_SLOT_WING)
	{
		return 0;
	}

	if(lpItem->m_Level >= 9)
	{
		return 0;
	}

	int rate = gServerInfo.m_SoulSuccessRate[lpObj->AccountLevel];

	if(lpItem->m_Option2 != 0)
	{
		rate += gServerInfo.m_AddLuckSuccessRate1[lpObj->AccountLevel];
	}

	if((GetLargeRand()%100) < rate)
	{
		lpItem->m_Level++;
	}
	else
	{
		if(lpItem->m_Level >= 7)
		{
			lpItem->m_Level = 0;
		}
		else if(lpItem->m_Level >= 1)
		{
			lpItem->m_Level--;
		}
	}

	float dur = (float)gItemManager.GetItemDurability(lpItem->m_Index,lpItem->m_Level,lpItem->IsExcItem(),lpItem->IsSetItem());

	lpItem->m_Durability = (dur*lpItem->m_Durability)/lpItem->m_BaseDurability;

	lpItem->Convert(lpItem->m_Index,lpItem->m_Option1,lpItem->m_Option2,lpItem->m_Option3,lpItem->m_NewOption,lpItem->m_SetOption);

	this->CharacterMakePreviewCharSet(lpObj->Index);
	return 1;
}

bool CObjectManager::CharacterUseJewelOfLife(LPOBJ lpObj,int SourceSlot,int TargetSlot) // OK
{
	if(INVENTORY_FULL_RANGE(SourceSlot) == 0)
	{
		return 0;
	}

	if(INVENTORY_FULL_RANGE(TargetSlot) == 0)
	{
		return 0;
	}

	if(lpObj->Inventory[SourceSlot].IsItem() == 0)
	{
		return 0;
	}

	if(lpObj->Inventory[TargetSlot].IsItem() == 0)
	{
		return 0;
	}

	if(lpObj->Inventory[TargetSlot].m_IsPeriodicItem != 0)
	{
		return 0;
	}

	CItem* lpItem = &lpObj->Inventory[TargetSlot];

	int slot = gItemManager.GetItemSlot(lpItem->m_Index);

	if(slot != INVENTORY_SLOT_WEAPON1 && slot != INVENTORY_SLOT_WEAPON2 && slot != INVENTORY_SLOT_HELM && slot != INVENTORY_SLOT_ARMOR && slot != INVENTORY_SLOT_PANTS && slot != INVENTORY_SLOT_GLOVES && slot != INVENTORY_SLOT_BOOTS && slot != INVENTORY_SLOT_WING)
	{
		return 0;
	}

	if(lpItem->m_Option3 >= gServerInfo.m_MaxItemOption)
	{
		return 0;
	}

	if((GetLargeRand()%100) < gServerInfo.m_LifeSuccessRate[lpObj->AccountLevel])
	{
		if(lpItem->m_Option3 == 0 && lpItem->m_Index >= GET_ITEM(12,3) && lpItem->m_Index <= GET_ITEM(12,6))
		{
			lpItem->m_NewOption &= 0xDF;
			lpItem->m_NewOption |= (32*(GetLargeRand()%2));
		}

		lpItem->m_Option3++;
	}
	else
	{
		lpItem->m_Option3 = 0;
	}

	float dur = (float)gItemManager.GetItemDurability(lpItem->m_Index,lpItem->m_Level,lpItem->IsExcItem(),lpItem->IsSetItem());

	lpItem->m_Durability = (dur*lpItem->m_Durability)/lpItem->m_BaseDurability;

	lpItem->Convert(lpItem->m_Index,lpItem->m_Option1,lpItem->m_Option2,lpItem->m_Option3,lpItem->m_NewOption,lpItem->m_SetOption);

	this->CharacterMakePreviewCharSet(lpObj->Index);
	return 1;
}

void CObjectManager::CharacterMonsterDieHunt(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	int HPValue = (int)(((lpObj->MaxLife+lpObj->AddLife)*lpObj->HuntHP)/100)+lpTarget->Level;
	int MPValue = (int)(((lpObj->MaxMana+lpObj->AddMana)*lpObj->HuntMP)/100);
	int BPValue = (int)((lpObj->MaxBP+lpObj->AddBP)/100);

	if(HPValue != 0)
	{
		if((lpObj->Life+HPValue) > (lpObj->MaxLife+lpObj->AddLife))
		{
			lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
		}
		else
		{
			lpObj->Life += HPValue;
		}

		GCLifeSend(lpObj->Index,0xFF,(int)lpObj->Life);
	}

	if(MPValue != 0 || BPValue != 0)
	{
		if((lpObj->Mana+MPValue) > (lpObj->MaxMana+lpObj->AddMana))
		{
			lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;
		}
		else
		{
			lpObj->Mana += MPValue;
		}

		if((lpObj->BP+BPValue) > (lpObj->MaxBP+lpObj->AddBP))
		{
			lpObj->BP = lpObj->MaxBP+lpObj->AddBP;
		}
		else
		{
			lpObj->BP += BPValue;
		}

		GCManaSend(lpObj->Index,0xFF,(int)lpObj->Mana,lpObj->BP);
	}
}

void CObjectManager::CharacterAutoRecuperation(LPOBJ lpObj) // OK
{
	int HPChange = 0;
	int MPChange = 0;
	int BPChange = 0;

	if(((lpObj->HPRecoveryCount++)%5) == 0)
	{
		if(lpObj->Life != (lpObj->MaxLife+lpObj->AddLife))
		{
			int rate = gServerInfo.m_HPRecoveryRate[lpObj->Class];

			rate += (((GetTickCount()-lpObj->HPAutoRecuperationTime)>5000)?5:0);

			rate += lpObj->HPRecoveryRate;

			int value = (int)(((lpObj->MaxLife+lpObj->AddLife)*rate)/100);

			value += lpObj->HPRecovery;

			if((lpObj->Life+value) > (lpObj->MaxLife+lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
			}
			else
			{
				lpObj->Life += value;
			}

			HPChange = 1;
		}
	}

	if(((lpObj->MPRecoveryCount++)%3) == 0)
	{
		if(lpObj->Mana != (lpObj->MaxMana+lpObj->AddMana))
		{
			int rate = gServerInfo.m_MPRecoveryRate[lpObj->Class];

			rate += (((GetTickCount()-lpObj->MPAutoRecuperationTime)>5000)?3:0);

			rate += lpObj->MPRecoveryRate;

			int value = (int)(((lpObj->MaxMana+lpObj->AddMana)*rate)/100);

			value += lpObj->MPRecovery;

			if((lpObj->Mana+value) > (lpObj->MaxMana+lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;
			}
			else
			{
				lpObj->Mana += value;
			}

			MPChange = 1;
		}
	}

	if(((lpObj->BPRecoveryCount++)%3) == 0)
	{
		if(lpObj->BP != (lpObj->MaxBP+lpObj->AddBP))
		{
			int rate = gServerInfo.m_BPRecoveryRate[lpObj->Class];

			rate += (((GetTickCount()-lpObj->BPAutoRecuperationTime)>5000)?3:0);

			rate += lpObj->BPRecoveryRate;

			int value = (int)(((lpObj->MaxBP+lpObj->AddBP)*rate)/100);

			value += lpObj->BPRecovery;

			if((lpObj->BP+value) > (lpObj->MaxBP+lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP+lpObj->AddBP;
			}
			else
			{
				lpObj->BP += value;
			}

			BPChange = 1;
		}
	}

	if(HPChange != 0)
	{
		GCLifeSend(lpObj->Index,0xFF,(int)lpObj->Life);
	}

	if(MPChange != 0 || BPChange != 0)
	{
		GCManaSend(lpObj->Index,0xFF,(int)lpObj->Mana,lpObj->BP);
	}
}

void CObjectManager::CharacterItemDurationDown(LPOBJ lpObj) // OK
{
	if(((lpObj->TimeCount++)%10) != 0)
	{
		return;
	}

	for(int n=2;n < 10;n++)
	{
		if(lpObj->Inventory[n].IsItem() != 0)
		{
			bool result = 0;

			switch(n)
			{
				case 7:
					result = lpObj->Inventory[n].WingDurabilityDown(lpObj->Index,((gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1)==0)?1:0));
					break;
				case 9:
					result = lpObj->Inventory[n].PendantDurabilityDown(lpObj->Index,((gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1)==0)?1:0));
					break;
			}

			if(result != 0)
			{
				gItemManager.GCItemDurSend(lpObj->Index,n,(BYTE)lpObj->Inventory[n].m_Durability,0);
			}
		}
	}

	for(int n=10;n < 12;n++)
	{
		if(lpObj->Inventory[n].IsItem() != 0)
		{
			bool result = 0;

			switch(lpObj->Inventory[n].m_Index)
			{
				case GET_ITEM(13,10):
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index,113);
					break;
				case GET_ITEM(13,20):
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index,((gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1)==0)?70:0));
					break;
				default:
					result = lpObj->Inventory[n].RingDurabilityDown(lpObj->Index,((gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1)==0)?1:0));
					break;
			}

			if(result != 0)
			{
				gItemManager.GCItemDurSend(lpObj->Index,n,(BYTE)lpObj->Inventory[n].m_Durability,0);

				if(gSkillManager.SkillChangeUse(lpObj->Index) != 0)
				{
					gObjViewportListProtocolCreate(lpObj);
				}
			}
		}
	}
}

void CObjectManager::CharacterCalcBP(LPOBJ lpObj) // OK
{
	switch(lpObj->Class)
	{
		case CLASS_DW:
			lpObj->MaxBP = (int)(((lpObj->Strength+lpObj->AddStrength)*0.20)+((lpObj->Dexterity+lpObj->AddDexterity)*0.40)+((lpObj->Vitality+lpObj->AddVitality)*0.30)+((lpObj->Energy+lpObj->AddEnergy)*0.20));
			break;
		case CLASS_DK:
			lpObj->MaxBP = (int)(((lpObj->Strength+lpObj->AddStrength)*0.15)+((lpObj->Dexterity+lpObj->AddDexterity)*0.20)+((lpObj->Vitality+lpObj->AddVitality)*0.30)+((lpObj->Energy+lpObj->AddEnergy)*1.00));
			break;
		case CLASS_FE:
			lpObj->MaxBP = (int)(((lpObj->Strength+lpObj->AddStrength)*0.30)+((lpObj->Dexterity+lpObj->AddDexterity)*0.20)+((lpObj->Vitality+lpObj->AddVitality)*0.30)+((lpObj->Energy+lpObj->AddEnergy)*0.20));
			break;
		case CLASS_MG:
			lpObj->MaxBP = (int)(((lpObj->Strength+lpObj->AddStrength)*0.20)+((lpObj->Dexterity+lpObj->AddDexterity)*0.25)+((lpObj->Vitality+lpObj->AddVitality)*0.30)+((lpObj->Energy+lpObj->AddEnergy)*0.15));
			break;
	}
}

void CObjectManager::CharacterCalcAttribute(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJECT_USER)
	{
		return;
	}

	CItem* Right = &lpObj->Inventory[INVENTORY_SLOT_WEAPON1];
	CItem* Left = &lpObj->Inventory[INVENTORY_SLOT_WEAPON2];
	CItem* Gloves = &lpObj->Inventory[INVENTORY_SLOT_GLOVES];
	CItem* Wings = &lpObj->Inventory[INVENTORY_SLOT_WING];
	CItem* Helper = &lpObj->Inventory[INVENTORY_SLOT_HELPER];
	CItem* Amulet = &lpObj->Inventory[INVENTORY_SLOT_AMULET];
	CItem* Ring1 = &lpObj->Inventory[INVENTORY_SLOT_RING1];
	CItem* Ring2 = &lpObj->Inventory[INVENTORY_SLOT_RING2];

	float TotalHP = 100;
	float TotalMP = 100;
	float TotalBP = 100;

	if((lpObj->MaxLife+lpObj->AddLife) != 0)
	{
		TotalHP = (float)(lpObj->Life*100)/(float)(lpObj->MaxLife+lpObj->AddLife);
	}

	if((lpObj->MaxMana+lpObj->AddMana) != 0)
	{
		TotalMP = (float)(lpObj->Mana*100)/(lpObj->MaxMana+lpObj->AddMana);
	}

	if((lpObj->MaxBP+lpObj->AddBP) != 0)
	{
		TotalBP = (float)(lpObj->BP*100)/(float)(lpObj->MaxBP+lpObj->AddBP);
	}

	for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
	{
		if(lpObj->Inventory[n].IsItem() != 0)
		{
			lpObj->Inventory[n].m_IsValidItem = 1;
		}
	}

	lpObj->AddLife = 0;
	lpObj->AddMana = 0;
	lpObj->AddBP = 0;

	lpObj->SkillLongSpearChange = 0;

	gObjClearSpecialOption(lpObj);

	bool change = 1;

	while(change != 0)
	{
		change = 0;
		lpObj->AddStrength = 0;
		lpObj->AddDexterity = 0;
		lpObj->AddVitality = 0;
		lpObj->AddEnergy = 0;
		lpObj->AddLeadership = 0;

		gItemOption.CalcItemCommonOption(lpObj,1);

		for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
		{
			if(lpObj->Inventory[n].m_IsValidItem != 0)
			{
				if(gItemManager.IsValidItem(lpObj,&lpObj->Inventory[n]) == 0)
				{
					change = 1;
					lpObj->Inventory[n].m_IsValidItem = 0;
				}
			}
		}
	}

	int Strength = lpObj->Strength+lpObj->AddStrength;
	int Dexterity = lpObj->Dexterity+lpObj->AddDexterity;
	int Vitality = lpObj->Vitality+lpObj->AddVitality;
	int Energy = lpObj->Energy+lpObj->AddEnergy;
	int Leadership = lpObj->Leadership+lpObj->AddLeadership;

	if(lpObj->Class == CLASS_DW)
	{
		if(gServerInfo.m_CalcLuaScriptSwitch != 0)
		{
			gServerInfo.m_CalcCharacter.Execute("DWPhysiDamageCalculate","i>iiii",aIndex,&lpObj->PhysiDamageMinRight,&lpObj->PhysiDamageMinLeft,&lpObj->PhysiDamageMaxRight,&lpObj->PhysiDamageMaxLeft);
			gServerInfo.m_CalcCharacter.Execute("DWMagicDamageCalculate","i>ii",aIndex,&lpObj->MagicDamageMin,&lpObj->MagicDamageMax);
		}
		else
		{
			lpObj->PhysiDamageMinRight = Strength/gServerInfo.m_DWPhysiDamageMinConstA;
			lpObj->PhysiDamageMaxRight = Strength/gServerInfo.m_DWPhysiDamageMaxConstA;
			lpObj->PhysiDamageMinLeft = Strength/gServerInfo.m_DWPhysiDamageMinConstA;
			lpObj->PhysiDamageMaxLeft = Strength/gServerInfo.m_DWPhysiDamageMaxConstA;
			lpObj->MagicDamageMin = Energy/gServerInfo.m_DWMagicDamageMinConstA;
			lpObj->MagicDamageMax = Energy/gServerInfo.m_DWMagicDamageMaxConstA;
		}
	}
	else if(lpObj->Class == CLASS_DK)
	{
		if(gServerInfo.m_CalcLuaScriptSwitch != 0)
		{
			gServerInfo.m_CalcCharacter.Execute("DKPhysiDamageCalculate","i>iiii",aIndex,&lpObj->PhysiDamageMinRight,&lpObj->PhysiDamageMinLeft,&lpObj->PhysiDamageMaxRight,&lpObj->PhysiDamageMaxLeft);
			gServerInfo.m_CalcCharacter.Execute("DKMagicDamageCalculate","i>ii",aIndex,&lpObj->MagicDamageMin,&lpObj->MagicDamageMax);
		}
		else
		{
			lpObj->PhysiDamageMinRight = Strength / gServerInfo.m_DKPhysiDamageMinConstA;
			lpObj->PhysiDamageMaxRight = Strength/gServerInfo.m_DKPhysiDamageMaxConstA;
			lpObj->PhysiDamageMinLeft = Strength/gServerInfo.m_DKPhysiDamageMinConstA;
			lpObj->PhysiDamageMaxLeft = Strength/gServerInfo.m_DKPhysiDamageMaxConstA;
			lpObj->MagicDamageMin = Energy/gServerInfo.m_DKMagicDamageMinConstA;
			lpObj->MagicDamageMax = Energy/gServerInfo.m_DKMagicDamageMaxConstA;
		}

		lpObj->DKDamageMultiplierRate = 200+(Energy/gServerInfo.m_DKDamageMultiplierConstA);
		lpObj->DKDamageMultiplierRate = ((lpObj->DKDamageMultiplierRate>gServerInfo.m_DKDamageMultiplierMaxRate)?gServerInfo.m_DKDamageMultiplierMaxRate:lpObj->DKDamageMultiplierRate);
	}
	else if(lpObj->Class == CLASS_FE)
	{
		if(Right->IsItem() != 0 && Right->m_Index >= GET_ITEM(4,0) && Right->m_Index < GET_ITEM(5,0) && Right->m_Index != GET_ITEM(4,15))
		{
			if(gServerInfo.m_CalcLuaScriptSwitch != 0)
			{
				gServerInfo.m_CalcCharacter.Execute("FEPhysiDamageCalculate","ii>iiii",aIndex,1,&lpObj->PhysiDamageMinRight,&lpObj->PhysiDamageMinLeft,&lpObj->PhysiDamageMaxRight,&lpObj->PhysiDamageMaxLeft);
			}
			else
			{
				lpObj->PhysiDamageMinRight = (Strength/gServerInfo.m_FEPhysiDamageMinBowConstA)+(Dexterity/gServerInfo.m_FEPhysiDamageMinBowConstB);
				lpObj->PhysiDamageMaxRight = (Strength/gServerInfo.m_FEPhysiDamageMaxBowConstA)+(Dexterity/gServerInfo.m_FEPhysiDamageMaxBowConstB);
				lpObj->PhysiDamageMinLeft = (Strength/gServerInfo.m_FEPhysiDamageMinBowConstA)+(Dexterity/gServerInfo.m_FEPhysiDamageMinBowConstB);
				lpObj->PhysiDamageMaxLeft = (Strength/gServerInfo.m_FEPhysiDamageMaxBowConstA)+(Dexterity/gServerInfo.m_FEPhysiDamageMaxBowConstB);
			}
		}
		else if(Left->IsItem() != 0 && Left->m_Index >= GET_ITEM(4,0) && Left->m_Index < GET_ITEM(5,0) && Left->m_Index != GET_ITEM(4,7))
		{
			if(gServerInfo.m_CalcLuaScriptSwitch != 0)
			{
				gServerInfo.m_CalcCharacter.Execute("FEPhysiDamageCalculate","ii>iiii",aIndex,2,&lpObj->PhysiDamageMinRight,&lpObj->PhysiDamageMinLeft,&lpObj->PhysiDamageMaxRight,&lpObj->PhysiDamageMaxLeft);
			}
			else
			{
				lpObj->PhysiDamageMinRight = (Strength/gServerInfo.m_FEPhysiDamageMinBowConstA)+(Dexterity/gServerInfo.m_FEPhysiDamageMinBowConstB);
				lpObj->PhysiDamageMaxRight = (Strength/gServerInfo.m_FEPhysiDamageMaxBowConstA)+(Dexterity/gServerInfo.m_FEPhysiDamageMaxBowConstB);
				lpObj->PhysiDamageMinLeft = (Strength/gServerInfo.m_FEPhysiDamageMinBowConstA)+(Dexterity/gServerInfo.m_FEPhysiDamageMinBowConstB);
				lpObj->PhysiDamageMaxLeft = (Strength/gServerInfo.m_FEPhysiDamageMaxBowConstA)+(Dexterity/gServerInfo.m_FEPhysiDamageMaxBowConstB);
			}
		}
		else
		{
			if(gServerInfo.m_CalcLuaScriptSwitch != 0)
			{
				gServerInfo.m_CalcCharacter.Execute("FEPhysiDamageCalculate","ii>iiii",aIndex,0,&lpObj->PhysiDamageMinRight,&lpObj->PhysiDamageMinLeft,&lpObj->PhysiDamageMaxRight,&lpObj->PhysiDamageMaxLeft);
			}
			else
			{
				lpObj->PhysiDamageMinRight = (Strength+Dexterity)/gServerInfo.m_FEPhysiDamageMinConstA;
				lpObj->PhysiDamageMaxRight = (Strength+Dexterity)/gServerInfo.m_FEPhysiDamageMaxConstA;
				lpObj->PhysiDamageMinLeft = (Strength+Dexterity)/gServerInfo.m_FEPhysiDamageMinConstA;
				lpObj->PhysiDamageMaxLeft = (Strength+Dexterity)/gServerInfo.m_FEPhysiDamageMaxConstA;
			}
		}

		if(gServerInfo.m_CalcLuaScriptSwitch != 0)
		{
			gServerInfo.m_CalcCharacter.Execute("FEMagicDamageCalculate","i>ii",aIndex,&lpObj->MagicDamageMin,&lpObj->MagicDamageMax);
		}
		else
		{
			lpObj->MagicDamageMin = Energy/gServerInfo.m_FEMagicDamageMinConstA;
			lpObj->MagicDamageMax = Energy/gServerInfo.m_FEMagicDamageMaxConstA;
		}
	}
	else if(lpObj->Class == CLASS_MG)
	{
		if(gServerInfo.m_CalcLuaScriptSwitch != 0)
		{
			gServerInfo.m_CalcCharacter.Execute("MGPhysiDamageCalculate","i>iiii",aIndex,&lpObj->PhysiDamageMinRight,&lpObj->PhysiDamageMinLeft,&lpObj->PhysiDamageMaxRight,&lpObj->PhysiDamageMaxLeft);
			gServerInfo.m_CalcCharacter.Execute("MGMagicDamageCalculate","i>ii",aIndex,&lpObj->MagicDamageMin,&lpObj->MagicDamageMax);
		}
		else
		{
			lpObj->PhysiDamageMinRight = (Strength/gServerInfo.m_MGPhysiDamageMinConstA)+(Energy/gServerInfo.m_MGPhysiDamageMinConstB);
			lpObj->PhysiDamageMaxRight = (Strength/gServerInfo.m_MGPhysiDamageMaxConstA)+(Energy/gServerInfo.m_MGPhysiDamageMaxConstB);
			lpObj->PhysiDamageMinLeft = (Strength/gServerInfo.m_MGPhysiDamageMinConstA)+(Energy/gServerInfo.m_MGPhysiDamageMinConstB);
			lpObj->PhysiDamageMaxLeft = (Strength/gServerInfo.m_MGPhysiDamageMaxConstA)+(Energy/gServerInfo.m_MGPhysiDamageMaxConstB);
			lpObj->MagicDamageMin = Energy/gServerInfo.m_MGMagicDamageMinConstA;
			lpObj->MagicDamageMax = Energy/gServerInfo.m_MGMagicDamageMaxConstA;
		}
	}

	if(Right->IsItem() != 0)
	{
		if(gServerInfo.m_CalcLuaScriptSwitch != 0)
		{
			int DamageMinRight = 0;
			int DamageMaxRight = 0;
			gServerInfo.m_CalcCharacter.Execute("PhysiDamageRight","iii>ii",Right->m_Index,Right->GetDamageMin(),Right->GetDamageMax(),&DamageMinRight,&DamageMaxRight);
			lpObj->PhysiDamageMinRight += DamageMinRight;
			lpObj->PhysiDamageMaxRight += DamageMaxRight;
			lpObj->SkillLongSpearChange = ((Right->m_SkillChange==0)?lpObj->SkillLongSpearChange:1);
		}
		else
		{
			if(Right->m_Index >= GET_ITEM(5,0) && Right->m_Index < GET_ITEM(6,0))
			{
				lpObj->PhysiDamageMinRight += Right->GetDamageMin()/2;
				lpObj->PhysiDamageMaxRight += Right->GetDamageMax()/2;
				lpObj->SkillLongSpearChange = ((Right->m_SkillChange==0)?lpObj->SkillLongSpearChange:1);
			}
			else
			{
				lpObj->PhysiDamageMinRight += Right->GetDamageMin()/1;
				lpObj->PhysiDamageMaxRight += Right->GetDamageMax()/1;
				lpObj->SkillLongSpearChange = ((Right->m_SkillChange==0)?lpObj->SkillLongSpearChange:1);
			}
		}
	}

	if(Left->IsItem() != 0)
	{
		if(gServerInfo.m_CalcLuaScriptSwitch != 0)
		{
			int DamageMinLeft = 0;
			int DamageMaxLeft = 0;
			gServerInfo.m_CalcCharacter.Execute("PhysiDamageLeft","iii>ii",Left->m_Index,Left->GetDamageMin(),Left->GetDamageMax(),&DamageMinLeft,&DamageMaxLeft);
			lpObj->PhysiDamageMinLeft += DamageMinLeft;
			lpObj->PhysiDamageMaxLeft += DamageMaxLeft;
			lpObj->SkillLongSpearChange = ((Left->m_SkillChange==0)?lpObj->SkillLongSpearChange:1);
		}
		else
		{
			if(Left->m_Index >= GET_ITEM(5,0) && Left->m_Index < GET_ITEM(6,0))
			{
				lpObj->PhysiDamageMinLeft += Left->GetDamageMin()/2;
				lpObj->PhysiDamageMaxLeft += Left->GetDamageMax()/2;
				lpObj->SkillLongSpearChange = ((Left->m_SkillChange==0)?lpObj->SkillLongSpearChange:1);
			}
			else
			{
				lpObj->PhysiDamageMinLeft += Left->GetDamageMin()/1;
				lpObj->PhysiDamageMaxLeft += Left->GetDamageMax()/1;
				lpObj->SkillLongSpearChange = ((Left->m_SkillChange==0)?lpObj->SkillLongSpearChange:1);
			}
		}
	}

	lpObj->PhysiDamageMinRight += lpObj->DrinkDamage;
	lpObj->PhysiDamageMaxRight += lpObj->DrinkDamage;
	lpObj->PhysiDamageMinLeft += lpObj->DrinkDamage;
	lpObj->PhysiDamageMaxLeft += lpObj->DrinkDamage;
	lpObj->MagicDamageMin += lpObj->DrinkDamage;
	lpObj->MagicDamageMax += lpObj->DrinkDamage;

	if(gServerInfo.m_CalcLuaScriptSwitch != 0)
	{
		gServerInfo.m_CalcCharacter.Execute("CalcAttackSuccessRate","i>i",aIndex,&lpObj->AttackSuccessRate);
	}
	else
	{
		if(lpObj->Class == CLASS_DW)
		{
			lpObj->AttackSuccessRate = (lpObj->Level*gServerInfo.m_DWAttackSuccessRateConstA)+((Dexterity*gServerInfo.m_DWAttackSuccessRateConstB)/gServerInfo.m_DWAttackSuccessRateConstC)+(Strength/gServerInfo.m_DWAttackSuccessRateConstD);
		}
		else if(lpObj->Class == CLASS_DK)
		{
			lpObj->AttackSuccessRate = (lpObj->Level*gServerInfo.m_DKAttackSuccessRateConstA)+((Dexterity*gServerInfo.m_DKAttackSuccessRateConstB)/gServerInfo.m_DKAttackSuccessRateConstC)+(Strength/gServerInfo.m_DKAttackSuccessRateConstD);
		}
		else if(lpObj->Class == CLASS_FE)
		{
			lpObj->AttackSuccessRate = (lpObj->Level*gServerInfo.m_FEAttackSuccessRateConstA)+((Dexterity*gServerInfo.m_FEAttackSuccessRateConstB)/gServerInfo.m_FEAttackSuccessRateConstC)+(Strength/gServerInfo.m_FEAttackSuccessRateConstD);
		}
		else if(lpObj->Class == CLASS_MG)
		{
			lpObj->AttackSuccessRate = (lpObj->Level*gServerInfo.m_MGAttackSuccessRateConstA)+((Dexterity*gServerInfo.m_MGAttackSuccessRateConstB)/gServerInfo.m_MGAttackSuccessRateConstC)+(Strength/gServerInfo.m_MGAttackSuccessRateConstD);
		}
	}

	if(gServerInfo.m_CalcLuaScriptSwitch != 0)
	{
		gServerInfo.m_CalcCharacter.Execute("CalcAttackSpeed","i>ii",aIndex,&lpObj->PhysiSpeed,&lpObj->MagicSpeed);
	}
	else
	{
		if(lpObj->Class == CLASS_DW)
		{
			lpObj->PhysiSpeed = Dexterity/gServerInfo.m_DWPhysiSpeedConstA;
			lpObj->MagicSpeed = Dexterity/gServerInfo.m_DWMagicSpeedConstA;
		}
		else if(lpObj->Class == CLASS_DK)
		{
			lpObj->PhysiSpeed = Dexterity/gServerInfo.m_DKPhysiSpeedConstA;
			lpObj->MagicSpeed = Dexterity/gServerInfo.m_DKMagicSpeedConstA;
		}
		else if(lpObj->Class == CLASS_FE)
		{
			lpObj->PhysiSpeed = Dexterity/gServerInfo.m_FEPhysiSpeedConstA;
			lpObj->MagicSpeed = Dexterity/gServerInfo.m_FEMagicSpeedConstA;
		}
		else if(lpObj->Class == CLASS_MG)
		{
			lpObj->PhysiSpeed = Dexterity/gServerInfo.m_MGPhysiSpeedConstA;
			lpObj->MagicSpeed = Dexterity/gServerInfo.m_MGMagicSpeedConstA;
		}
	}

	lpObj->PhysiSpeed += lpObj->DrinkSpeed;
	lpObj->MagicSpeed += lpObj->DrinkSpeed;

	bool RightItem = 0;

	if(Right->m_Index >= GET_ITEM(0,0) && Right->m_Index < GET_ITEM(6,0) && Right->m_Index != GET_ITEM(4,7) && Right->m_Index != GET_ITEM(4,15))
	{
		if(Right->m_IsValidItem != 0)
		{
			RightItem = 1;
		}
	}

	bool LeftItem = 0;

	if(Left->m_Index >= GET_ITEM(0,0) && Left->m_Index < GET_ITEM(6,0) && Left->m_Index != GET_ITEM(4,7) && Left->m_Index != GET_ITEM(4,15))
	{
		if(Left->m_IsValidItem != 0)
		{
			LeftItem = 1;
		}
	}

	if(RightItem != 0 && LeftItem != 0)
	{
		lpObj->PhysiSpeed += (Right->m_AttackSpeed+Left->m_AttackSpeed)/2;
		lpObj->MagicSpeed += (Right->m_AttackSpeed+Left->m_AttackSpeed)/2;
	}
	else if(RightItem != 0)
	{
		lpObj->PhysiSpeed += Right->m_AttackSpeed;
		lpObj->MagicSpeed += Right->m_AttackSpeed;
	}
	else if(LeftItem != 0)
	{
		lpObj->PhysiSpeed += Left->m_AttackSpeed;
		lpObj->MagicSpeed += Left->m_AttackSpeed;
	}

	if(Gloves->IsItem() != 0 && Gloves->m_IsValidItem != 0)
	{
		lpObj->PhysiSpeed += Gloves->m_AttackSpeed;
		lpObj->MagicSpeed += Gloves->m_AttackSpeed;
	}

	if(Helper->IsItem() != 0 && Helper->m_IsValidItem != 0)
	{
		lpObj->PhysiSpeed += Helper->m_AttackSpeed;
		lpObj->MagicSpeed += Helper->m_AttackSpeed;
	}

	if(Amulet->IsItem() != 0 && Amulet->m_IsValidItem != 0)
	{
		lpObj->PhysiSpeed += Amulet->m_AttackSpeed;
		lpObj->MagicSpeed += Amulet->m_AttackSpeed;
	}

	if(gServerInfo.m_CalcLuaScriptSwitch != 0)
	{
		gServerInfo.m_CalcCharacter.Execute("CalcDefenseSuccessRate","i>i",aIndex,&lpObj->DefenseSuccessRate);
	}
	else
	{
		if(lpObj->Class == CLASS_DW)
		{
			lpObj->DefenseSuccessRate = Dexterity/gServerInfo.m_DWDefenseSuccessRateConstA;
		}
		else if(lpObj->Class == CLASS_DK)
		{
			lpObj->DefenseSuccessRate = Dexterity/gServerInfo.m_DKDefenseSuccessRateConstA;
		}
		else if(lpObj->Class == CLASS_FE)
		{
			lpObj->DefenseSuccessRate = Dexterity/gServerInfo.m_FEDefenseSuccessRateConstA;
		}
		else if(lpObj->Class == CLASS_MG)
		{
			lpObj->DefenseSuccessRate = Dexterity/gServerInfo.m_MGDefenseSuccessRateConstA;
		}
	}

	lpObj->DefenseSuccessRate += lpObj->Inventory[INVENTORY_SLOT_WEAPON2].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[INVENTORY_SLOT_HELM].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[INVENTORY_SLOT_ARMOR].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[INVENTORY_SLOT_PANTS].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[INVENTORY_SLOT_GLOVES].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[INVENTORY_SLOT_BOOTS].GetDefenseSuccessRate();

	lpObj->DefenseSuccessRate += lpObj->Inventory[INVENTORY_SLOT_WING].GetDefenseSuccessRate();

	int LastItemIndex = -1;

	for(int n=2;n <= 6;n++)
	{
		if(n == 2 && lpObj->Class == CLASS_MG)
		{
			continue;
		}

		if(lpObj->Inventory[n].IsItem() == 0 || lpObj->Inventory[n].m_IsValidItem == 0 || (LastItemIndex != -1 && (lpObj->Inventory[n].m_Index%MAX_ITEM_TYPE) != LastItemIndex))
		{
			lpObj->ArmorSetBonus = 0;
			break;
		}
		else
		{
			lpObj->ArmorSetBonus = 1;
			LastItemIndex = lpObj->Inventory[n].m_Index%MAX_ITEM_TYPE; 
		}
	}

	int Level11Count = 0;
	int Level10Count = 0;
	int Level12Count = 0;
	int Level13Count = 0;
	int Level14Count = 0;
	int Level15Count = 0;

	if(lpObj->ArmorSetBonus != 0)
	{
		for(int n=2;n <= 6;n++)
		{
			if(n == 2 && lpObj->Class == CLASS_MG)
			{
				Level15Count++;
				continue;
			}

			if(lpObj->Inventory[n].m_Level == 10)
			{
				Level10Count++;
			}
			else if(lpObj->Inventory[n].m_Level == 11)
			{
				Level11Count++;
			}
			else if(lpObj->Inventory[n].m_Level == 12)
			{
				Level12Count++;
			}
			else if(lpObj->Inventory[n].m_Level == 13)
			{
				Level13Count++;
			}
			else if(lpObj->Inventory[n].m_Level == 14)
			{
				Level14Count++;
			}
			else if(lpObj->Inventory[n].m_Level == 15)
			{
				Level15Count++;
			}
		}

		lpObj->DefenseSuccessRate += (lpObj->DefenseSuccessRate*10)/100;
	}

	if(gServerInfo.m_CalcLuaScriptSwitch != 0)
	{
		gServerInfo.m_CalcCharacter.Execute("CalcDefense","i>i",aIndex,&lpObj->Defense);
	}
	else
	{
		if(lpObj->Class == CLASS_DW)
		{
			lpObj->Defense = Dexterity/gServerInfo.m_DWDefenseConstA;
		}
		else if(lpObj->Class == CLASS_DK)
		{
			lpObj->Defense = Dexterity/gServerInfo.m_DKDefenseConstA;
		}
		else if(lpObj->Class == CLASS_FE)
		{
			lpObj->Defense = Dexterity/gServerInfo.m_FEDefenseConstA;
		}
		else if(lpObj->Class == CLASS_MG)
		{
			lpObj->Defense = Dexterity/gServerInfo.m_MGDefenseConstA;
		}
	}

	lpObj->Defense += lpObj->Inventory[INVENTORY_SLOT_WEAPON2].GetDefense();

	lpObj->Defense += lpObj->Inventory[INVENTORY_SLOT_HELM].GetDefense();

	lpObj->Defense += lpObj->Inventory[INVENTORY_SLOT_ARMOR].GetDefense();

	lpObj->Defense += lpObj->Inventory[INVENTORY_SLOT_PANTS].GetDefense();

	lpObj->Defense += lpObj->Inventory[INVENTORY_SLOT_GLOVES].GetDefense();

	lpObj->Defense += lpObj->Inventory[INVENTORY_SLOT_BOOTS].GetDefense();

	lpObj->Defense += lpObj->Inventory[INVENTORY_SLOT_WING].GetDefense();

	if(Helper->IsItem() != 0 && Helper->m_IsValidItem != 0 && Helper->m_Durability != 0)
	{
		if(Helper->m_Index == GET_ITEM(13,4))
		{
			lpObj->Defense += (Dexterity/20)+(Helper->m_PetItemLevel*2)+5;
		}
	}

	if(lpObj->ArmorSetBonus != 0 && (Level15Count+Level14Count+Level13Count+Level12Count+Level11Count+Level10Count) >= 5)
	{
		if(Level15Count == 5)
		{
			if(gServerInfo.m_CalcLuaScriptSwitch != 0)
			{
				gServerInfo.m_CalcCharacter.Execute("CalcBonusDefense","ii>i",aIndex,5,&lpObj->Defense);
			}
			else
			{
				lpObj->Defense += (lpObj->Defense*30)/100;
			}
		}
		else if(Level14Count == 5 || (Level14Count+Level15Count) == 5)
		{
			if(gServerInfo.m_CalcLuaScriptSwitch != 0)
			{
				gServerInfo.m_CalcCharacter.Execute("CalcBonusDefense","ii>i",aIndex,4,&lpObj->Defense);
			}
			else
			{
				lpObj->Defense += (lpObj->Defense*25)/100;
			}
		}
		else if(Level13Count == 5 || (Level13Count+Level14Count+Level15Count) == 5)
		{
			if(gServerInfo.m_CalcLuaScriptSwitch != 0)
			{
				gServerInfo.m_CalcCharacter.Execute("CalcBonusDefense","ii>i",aIndex,3,&lpObj->Defense);
			}
			else
			{
				lpObj->Defense += (lpObj->Defense*20)/100;
			}
		}
		else if(Level12Count == 5 || (Level12Count+Level13Count+Level14Count+Level15Count) == 5)
		{
			if(gServerInfo.m_CalcLuaScriptSwitch != 0)
			{
				gServerInfo.m_CalcCharacter.Execute("CalcBonusDefense","ii>i",aIndex,2,&lpObj->Defense);
			}
			else
			{
				lpObj->Defense += (lpObj->Defense*15)/100;
			}
		}
		else if(Level11Count == 5 || (Level11Count+Level12Count+Level13Count+Level14Count+Level15Count) == 5)
		{
			if(gServerInfo.m_CalcLuaScriptSwitch != 0)
			{
				gServerInfo.m_CalcCharacter.Execute("CalcBonusDefense","ii>i",aIndex,1,&lpObj->Defense);
			}
			else
			{
				lpObj->Defense += (lpObj->Defense*10)/100;
			}
		}
		else if(Level10Count == 5 || (Level10Count+Level11Count+Level12Count+Level13Count+Level14Count+Level15Count) == 5)
		{
			if(gServerInfo.m_CalcLuaScriptSwitch != 0)
			{
				gServerInfo.m_CalcCharacter.Execute("CalcBonusDefense","ii>i",aIndex,0,&lpObj->Defense);
			}
			else
			{
				lpObj->Defense += (lpObj->Defense*5)/100;
			}
		}
	}

	lpObj->Resistance[0] = GET_MAX_RESISTANCE(Amulet->m_Resistance[0],Ring1->m_Resistance[0],Ring2->m_Resistance[0]);

	lpObj->Resistance[1] = GET_MAX_RESISTANCE(Amulet->m_Resistance[1],Ring1->m_Resistance[1],Ring2->m_Resistance[1]);

	lpObj->Resistance[2] = GET_MAX_RESISTANCE(Amulet->m_Resistance[2],Ring1->m_Resistance[2],Ring2->m_Resistance[2]);

	lpObj->Resistance[3] = GET_MAX_RESISTANCE(Amulet->m_Resistance[3],Ring1->m_Resistance[3],Ring2->m_Resistance[3]);

	gObjCalcExperience(lpObj);
	gItemOption.CalcItemCommonOption(lpObj,0);

	if(Right->m_Index >= GET_ITEM(4,0) && Right->m_Index < GET_ITEM(5,0) && Right->m_Index != GET_ITEM(4,15) && Right->m_Slot == 0)
	{
		if(Left->m_Index == GET_ITEM(4,7) && Left->m_Level > 0)
		{
			lpObj->PhysiDamageMinRight += ((lpObj->PhysiDamageMinRight*((Left->m_Level*2)+1))/100)+1;
			lpObj->PhysiDamageMaxRight += ((lpObj->PhysiDamageMinRight*((Left->m_Level*2)+1))/100)+1;
		}
	}
	else if(Left->m_Index >= GET_ITEM(4,0) && Left->m_Index < GET_ITEM(5,0) && Left->m_Index != GET_ITEM(4,7) && Left->m_Slot == 1)
	{
		if(Right->m_Index == GET_ITEM(4,15) && Right->m_Level > 0)
		{
			lpObj->PhysiDamageMinLeft += ((lpObj->PhysiDamageMinLeft*((Right->m_Level*2)+1))/100)+1;
			lpObj->PhysiDamageMaxLeft += ((lpObj->PhysiDamageMinLeft*((Right->m_Level*2)+1))/100)+1;
		}
	}

	if(lpObj->Class == CLASS_DK || lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL)
	{
		if(Right->IsItem() != 0 && Left->IsItem() != 0)
		{
			if(Right->m_Index >= GET_ITEM(0,0) && Right->m_Index < GET_ITEM(4,0) && Left->m_Index >= GET_ITEM(0,0) && Left->m_Index < GET_ITEM(4,0))
			{
				lpObj->PhysiDamageMinRight = (lpObj->PhysiDamageMinRight*55)/100;
				lpObj->PhysiDamageMaxRight = (lpObj->PhysiDamageMaxRight*55)/100;
				lpObj->PhysiDamageMinLeft = (lpObj->PhysiDamageMinLeft*55)/100;
				lpObj->PhysiDamageMaxLeft = (lpObj->PhysiDamageMaxLeft*55)/100;
			}
		}
	}

	lpObj->MaxLife = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].MaxLife;
	lpObj->MaxMana = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].MaxMana;

	lpObj->MaxLife += gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].LevelLife*(lpObj->Level-1);
	lpObj->MaxMana += gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].LevelMana*(lpObj->Level-1);

	lpObj->MaxLife += ((lpObj->Vitality-gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality)+lpObj->AddVitality)*lpObj->VitalityToLife;
	lpObj->MaxMana += ((lpObj->Energy-gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy)+lpObj->AddEnergy)*lpObj->EnergyToMana;

	this->CharacterCalcBP(lpObj);

	lpObj->AddLife += (int)(lpObj->MaxLife*lpObj->EffectOption.MulMaxHP)/100;

	lpObj->Life = ((lpObj->MaxLife+lpObj->AddLife)*TotalHP)/100;

	lpObj->Life = ((lpObj->Life>(lpObj->MaxLife+lpObj->AddLife))?(lpObj->MaxLife+lpObj->AddLife):lpObj->Life);

	lpObj->Mana = ((lpObj->MaxMana+lpObj->AddMana)*TotalMP)/100;

	lpObj->Mana = ((lpObj->Mana>(lpObj->MaxMana+lpObj->AddMana))?(lpObj->MaxMana+lpObj->AddMana):lpObj->Mana);

	lpObj->BP = (int)(((lpObj->MaxBP+lpObj->AddBP)*TotalBP)/100);

	lpObj->BP = (int)(((lpObj->BP>(lpObj->MaxBP+lpObj->AddBP))?(lpObj->MaxBP+lpObj->AddBP):lpObj->BP));

	#if(GAMESERVER_EXTRA==0)

	GCLifeSend(aIndex,0xFE,(int)(lpObj->MaxLife+lpObj->AddLife));
	GCLifeSend(aIndex,0xFF,(int)lpObj->Life);

	GCManaSend(aIndex,0xFE,(int)(lpObj->MaxMana+lpObj->AddMana),(lpObj->MaxBP+lpObj->AddBP));
	GCManaSend(aIndex,0xFF,(int)lpObj->Mana,lpObj->BP);

	#else

	GCNewCharacterCalcSend(lpObj);

	#endif
}

bool CObjectManager::CharacterInfoSet(BYTE* aRecv,int aIndex) // OK
{
	SDHP_CHARACTER_INFO_RECV* lpMsg = (SDHP_CHARACTER_INFO_RECV*)aRecv;

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected == OBJECT_OFFLINE || lpObj->Connected == OBJECT_CONNECTED)
	{
		return 0;
	}

	if(lpMsg->Class != DB_CLASS_DW && lpMsg->Class != DB_CLASS_SM && lpMsg->Class != DB_CLASS_DK && lpMsg->Class != DB_CLASS_BK && lpMsg->Class != DB_CLASS_FE && lpMsg->Class != DB_CLASS_ME && lpMsg->Class != DB_CLASS_MG && lpMsg->Class != DB_CLASS_DL)
	{
		return 0;
	}

	gObjCharZeroSet(lpObj->Index);

	gChaosBox.ChaosBoxInit(lpObj);

	lpObj->PosNum = -1;
	lpObj->Level = lpMsg->Level;
	lpObj->LevelUpPoint = lpMsg->LevelUpPoint;
	lpObj->DBClass = lpMsg->Class;
	lpObj->Class = lpMsg->Class/16;
	lpObj->ChangeUp = lpMsg->Class%16;
	lpObj->Reset = lpMsg->Reset;
	lpObj->MasterReset = lpMsg->MasterReset;

	memcpy(lpObj->Name,lpMsg->name,sizeof(lpObj->Name));

	lpObj->BCCount = lpMsg->BCCount;
	lpObj->CCCount = lpMsg->CCCount;
	lpObj->DSCount = lpMsg->DSCount;

	lpObj->Map = lpMsg->Map;
	lpObj->X = lpMsg->X;
	lpObj->Y = lpMsg->Y;
	lpObj->StartX = lpMsg->X;
	lpObj->StartY = lpMsg->Y;

	bool result = 0;

	int gate,map,x,y,dir,level,target;

	if(gMapManager.IsValidMap(lpObj->Map) == 0)
	{
		lpObj->Map = MAP_LORENCIA;
		gMap[lpObj->Map].GetMapRandomPos(&lpObj->X,&lpObj->Y,18);
	}

	if((target=gMapManager.GetSpawnGate(lpObj->Map)) != -1)
	{
		if(target != 0)
		{
			result = gGate.GetGate(target,&gate,&map,&x,&y,&dir,&level);
		}
	}
	else if(lpObj->Level < 6 && gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) == 0)
	{
		if(lpObj->Class == CLASS_DW || lpObj->Class == CLASS_DK || lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL)
		{
			result = gGate.GetGate(17,&gate,&map,&x,&y,&dir,&level);
		}
		else if(lpObj->Class == CLASS_FE)
		{
			result = gGate.GetGate(27,&gate,&map,&x,&y,&dir,&level);
		}
	}
	else if(gCustomArena.CheckMap(lpObj->Map) != 0)
	{
		result = gGate.GetGate(17,&gate,&map,&x,&y,&dir,&level);
	}
	else if(DS_MAP_RANGE(lpObj->Map) != 0)
	{
		result = gGate.GetGate(27,&gate,&map,&x,&y,&dir,&level);
	}
	else if(BC_MAP_RANGE(lpObj->Map) != 0)
	{
		result = gGate.GetGate(22,&gate,&map,&x,&y,&dir,&level);
	}

	if(result != 0)
	{
		lpObj->Map = map;
		lpObj->X = x;
		lpObj->Y = y;
		lpObj->Dir = dir;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,4) != 0 || gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,8) != 0)
	{
		gMap[lpObj->Map].GetMapRandomPos(&lpObj->X,&lpObj->Y,18);
	}

	gMap[lpObj->Map].SetStandAttr(lpObj->X,lpObj->Y);

	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->OldX = lpObj->X;
	lpObj->OldY = lpObj->Y;
	lpObj->Experience = lpMsg->Experience;
	lpObj->Strength = lpMsg->Strength;
	lpObj->Dexterity = lpMsg->Dexterity;
	lpObj->Vitality = lpMsg->Vitality;
	lpObj->Energy = lpMsg->Energy;
	lpObj->Life = (float)lpMsg->Life;
	lpObj->MaxLife = (float)lpMsg->MaxLife;
	lpObj->Mana = (float)lpMsg->Mana;
	lpObj->MaxMana = (float)lpMsg->MaxMana;
	lpObj->BP = lpMsg->BP;
	lpObj->MaxBP = lpMsg->MaxBP;
	lpObj->Money = lpMsg->Money;
	lpObj->FruitPoint = lpMsg->FruitPoint;
	lpObj->VitalityToLife = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].VitalityToLife;
	lpObj->EnergyToMana = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].EnergyToMana;
	lpObj->PKLevel = (BYTE)lpMsg->PKLevel;
	lpObj->PKCount = lpMsg->PKCount;
	lpObj->PKTime = lpMsg->PKTime;
	lpObj->MaxRegenTime = 4000;
	lpObj->MoveSpeed = 400;

	if(lpObj->PKLevel == 3)
	{
		lpObj->PKCount = 0;
		lpObj->PKTime = 0;
	}

	memcpy(lpObj->Quest,lpMsg->Quest,sizeof(lpObj->Quest));

	lpObj->ChatLimitTime = ((int)lpMsg->ChatLimitTime <= 0) ? 0 : lpMsg->ChatLimitTime;

	gObjSetInventory1Pointer(lpObj);

	lpObj->Guild = 0;

	for(int n=0;n < MAX_SKILL_LIST;n++)
	{
		CSkill skill;

		lpObj->Skill[n].Clear();

		if(gSkillManager.ConvertSkillByte(&skill,lpMsg->Skill[n]) == 0)
		{
			continue;
		}

		gSkillManager.AddSkill(lpObj,skill.m_index);
	}

	if(lpObj->Class == CLASS_DW)
	{
		gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL);
	}

	for(int n=0;n < INVENTORY_SIZE;n++)
	{
		CItem item;

		lpObj->Inventory[n].Clear();

		if(gItemManager.ConvertItemByte(&item,lpMsg->Inventory[n]) == 0)
		{
			continue;
		}

		gItemManager.InventoryAddItem(aIndex,item,n);
	}

	this->CharacterMakePreviewCharSet(aIndex);

	for(int n=0;n < MAX_EFFECT_LIST;n++)
	{
		CEffect effect;

		lpObj->Effect[n].Clear();

		if(gEffectManager.ConvertEffectByte(&effect,lpMsg->Effect[n]) == 0)
		{
			continue;
		}

		gEffectManager.AddEffect(lpObj,1,effect.m_index,effect.m_count,effect.m_value[0],effect.m_value[1],effect.m_value[2],effect.m_value[3]);
	}

	for(int n=0;n < MAX_SKILL_LIST;n++)
	{
		if(lpObj->Skill[n].IsSkill() != 0)
		{
			if(gSkillManager.CheckSkillRequireWeapon(lpObj,lpObj->Skill[n].m_index) == 0)
			{
				gSkillManager.DelSkill(lpObj,lpObj->Skill[n].m_index);
			}
		}
	}

	lpObj->Live = 1;
	lpObj->Type = OBJECT_USER;
	lpObj->State = OBJECT_CREATE;
	lpObj->Connected = OBJECT_ONLINE;
	lpObj->TargetNumber = -1;

	if(lpObj->Life == 0)
	{
		lpObj->Live = 1;
		lpObj->State = OBJECT_DYING;
		lpObj->DieRegen = 1;
		lpObj->RegenTime = GetTickCount();
	}

	lpObj->Authority = AUTHORITY_USER;

	if((lpMsg->CtlCode & 2) == 2)
	{
		lpObj->Penalty |= 1;
	}

	if((lpMsg->CtlCode & 4) == 4)
	{
		lpObj->Authority = AUTHORITY_INVISIBLE;
	}

	if((lpMsg->CtlCode & 8) == 8)
	{
		lpObj->Authority = AUTHORITY_GAME_MASTER;
	}

	if((lpMsg->CtlCode & 32) == 32)
	{
		lpObj->Authority = AUTHORITY_ADMINISTRATOR;
	}

	GCWeatherSend(lpObj->Index,(gMap[lpObj->Map].GetWeather()/16));

	GCMainCheckSend(aIndex);

	lpObj->HPAutoRecuperationTime = GetTickCount();
	lpObj->MPAutoRecuperationTime = GetTickCount();
	lpObj->BPAutoRecuperationTime = GetTickCount();
	lpObj->CharSaveTime = GetTickCount();
	return 1;
}

void CObjectManager::CharacterLifeCheck(LPOBJ lpObj,LPOBJ lpTarget,int damage,int DamageType,int flag,int type,int skill,bool combo) // OK
{
	if(lpObj->Connected != OBJECT_ONLINE)
	{
		return;
	}

	if(lpObj->Attribute == ATTRIBUTE_KALIMA || lpTarget->Attribute == ATTRIBUTE_KALIMA)
	{
		return;
	}

	int SummonIndex = lpObj->Index;

	if(lpObj->Type == OBJECT_MONSTER && OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		SummonIndex = lpObj->SummonIndex;
	}

	if(gObj[SummonIndex].Type == OBJECT_USER && lpTarget->Type == OBJECT_MONSTER && damage > 0)
	{
		gObjMonsterSetHitDamage(lpTarget,SummonIndex,damage);
	}

	if(lpTarget->Type == OBJECT_MONSTER && OBJECT_RANGE(lpTarget->SummonIndex) != 0 && damage > 0)
	{
		GCSummonLifeSend(lpTarget->SummonIndex,(int)lpTarget->Life,(int)lpTarget->MaxLife);
	}

	if(lpTarget->Life <= 0 && lpTarget->Live != 0)
	{
		if(gObj[SummonIndex].Type == OBJECT_MONSTER)
		{
			lpTarget->KillerType = 1;
			gObjAddMsgSend(&gObj[SummonIndex],3,lpTarget->Index,0);
		}

		if(gObj[SummonIndex].Type == OBJECT_USER && lpTarget->Type == OBJECT_USER)
		{
			gObjSetKillCount(SummonIndex,1);
		}

		if(lpTarget->Type == OBJECT_MONSTER)
		{
			gObjAddMsgSendDelay(lpTarget,1,SummonIndex,500,0);

			gCustomMonster.MonsterDieProc(lpTarget,&gObj[SummonIndex]);

			gInvasionManager.MonsterDieProc(lpTarget,&gObj[SummonIndex]);

			if(BC_MAP_RANGE(lpTarget->Map) != 0)
			{
				gBloodCastle.MonsterDieProc(lpTarget,&gObj[SummonIndex]);
			}

			if(DS_MAP_RANGE(lpTarget->Map) != 0)
			{
				gDevilSquare.MonsterDieProc(lpTarget,&gObj[SummonIndex]);
			}

			if(gObj[SummonIndex].Type == OBJECT_USER)
			{
				gObjPKDownCheckTime(&gObj[SummonIndex],(lpTarget->Level/gServerInfo.m_PKDownPlusKillPoint),1);
				gObjAddMsgSendDelay(&gObj[SummonIndex],3,lpTarget->Index,2000,0);
			}
		}

		if(lpTarget->Type == OBJECT_USER)
		{
			gObjUserDie(lpTarget,&gObj[SummonIndex]);

			if(lpTarget->AttackerKilled == 0)
			{
				gObjPlayerKiller(&gObj[SummonIndex],lpTarget);
			}

			lpTarget->AttackerKilled = 0;

			if(gGuild.gObjGuildWarCheck(&gObj[SummonIndex],lpTarget) == 0)
			{
				lpTarget->KillerType = 0;
			}
			else
			{
				lpTarget->KillerType = 2;
			}

			if(OBJECT_RANGE(lpTarget->SummonIndex) != 0)
			{
				gObjSummonKill(lpTarget->Index);
			}
		}

		lpTarget->DeathMap = lpObj->Map;
		lpTarget->DeathX = lpObj->X;
		lpTarget->DeathY = lpObj->Y;
		lpTarget->Live = 0;
		lpTarget->State = OBJECT_DYING;
		lpTarget->RegenTime = GetTickCount();
		lpTarget->DieRegen = 1;
		lpTarget->PathCount = 0;
		lpTarget->Teleport = ((lpTarget->Teleport==1)?0:lpTarget->Teleport);

		GCUserDieSend(lpTarget,lpTarget->Index,skill,lpObj->Index);

		if(lpTarget->Type == OBJECT_MONSTER)
		{
			if(OBJECT_RANGE(lpTarget->SummonIndex) != 0)
			{
				return;
			}

			if(lpTarget->Class == 275) // Kundum
			{
				return;
			}
		}

		if(gObj[SummonIndex].Type == OBJECT_USER && lpTarget->Type == OBJECT_MONSTER)
		{
			if(gParty.IsParty(gObj[SummonIndex].PartyNumber) == 0)
			{
				this->CharacterCalcExperienceSplit(&gObj[SummonIndex],lpTarget,damage,flag);
			}
			else
			{
				this->CharacterCalcExperienceParty(&gObj[SummonIndex],lpTarget,damage,flag);
			}
		}
	}
	else
	{
		if(DamageType == 0)
		{
			GCDamageSend(SummonIndex,lpTarget->Index,flag,damage,type);
		}
		else if(DamageType == 1)
		{
			GCMonsterDamageSend(lpTarget->Index,damage);
		}
		else if(DamageType == 2)
		{
			GCDamageSend(SummonIndex,lpTarget->Index,flag,damage,5);
		}
	}
}
