// Attack.cpp: implementation of the CAttack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Attack.h"
#include "BattleSoccer.h"
#include "CustomArena.h"
#include "CustomAttack.h"
#include "DamageTable.h"
#include "EffectManager.h"
#include "GuildManager.h"
#include "HackSkillSpeedCheck.h"
#include "Map.h"
#include "MapManager.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "Party.h"
#include "PetDamageTable.h"
#include "ServerInfo.h"
#include "SkillDamage.h"
#include "SkillEnabler.h"
#include "SkillManager.h"
#include "Util.h"

CAttack gAttack;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAttack::CAttack() // OK
{

}

CAttack::~CAttack() // OK
{

}

bool CAttack::Attack(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,bool send,BYTE flag,int damage,int count,bool combo) // OK
{
	#pragma region ATTACK_CHECK

	if(lpObj->Index == lpTarget->Index)
	{
		return 0;
	}

	if(lpObj->Type == OBJECT_USER && gObjIsConnectedGP(lpObj->Index) == 0)
	{
		return 0;
	}

	if(lpTarget->Type == OBJECT_USER && gObjIsConnectedGP(lpTarget->Index) == 0)
	{
		return 0;
	}

	if(lpObj->Permission[13] != 0)
	{
		return 0;
	}

	if(lpObj->Map != lpTarget->Map || lpObj->Teleport == 2)
	{
		return 0;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0 || gMap[lpTarget->Map].CheckAttr(lpTarget->X,lpTarget->Y,1) != 0)
	{
		return 0;
	}

	if(lpObj->Guild != 0 && lpObj->Guild->WarState != GUILD_WAR_STATE_NONE)
	{
		if(lpObj->Guild->WarType == GUILD_WAR_TYPE_NORMAL && lpTarget->Type == OBJECT_MONSTER)
		{
			return 0;
		}

		if(lpObj->Guild->WarType == GUILD_WAR_TYPE_SOCCER && gBattleSoccer.GetState() != BS_STATE_START)
		{
			return 1;
		}
	}

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 99 (2.5.8) - Correcao da checagem de ataque durante teletransporte
	if(gServerInfo.m_TeleportAttackCheck != 0 && lpObj->Teleport != 0)
	{
		return 0;
	}

	if(lpTarget->Type == OBJECT_NPC || lpTarget->Live == 0 || lpTarget->State != OBJECT_PLAYING || lpTarget->Teleport != 0)
	{
		return 0;
	}

	if(lpTarget->Type == OBJECT_MONSTER)
	{
		if(lpTarget->Attribute == ATTRIBUTE_KALIMA)
		{
			return 0;
		}

		if(lpTarget->Class >= 100 && lpTarget->Class <= 110) // Trap
		{
			return 0;
		}
	}

	if(lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_MONSTER)
	{
		if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
		{
			if(lpObj->SummonIndex == lpTarget->Index)
			{
				return 0;
			}
		}
	}

	int SummonIndex = lpObj->Index;

	if(lpObj->Type == OBJECT_MONSTER && OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		SummonIndex = lpObj->SummonIndex;
	}

	int SummonTargetIndex = lpTarget->Index;

	if(lpTarget->Type == OBJECT_MONSTER && OBJECT_RANGE(lpTarget->SummonIndex) != 0)
	{
		SummonTargetIndex = lpTarget->SummonIndex;
	}

	if(this->CheckPlayerTarget(&gObj[SummonIndex],&gObj[SummonTargetIndex]) == 0)
	{
		return 0;
	}

	#pragma endregion

	#pragma region ATTACK_RETURN

	int skill = ((lpSkill==0)?SKILL_NONE:lpSkill->m_index);

	if(gSkillEnabler.CheckUseSkill(lpObj,lpTarget,skill) == 0)
	{
		return 0;
	}

	if(damage == 0 && this->DecreaseArrow(lpObj) == 0)
	{
		return 0;
	}

	if(lpObj->Type == OBJECT_USER)
	{
		lpObj->HPAutoRecuperationTime = GetTickCount();
		lpObj->MPAutoRecuperationTime = GetTickCount();
		lpObj->BPAutoRecuperationTime = GetTickCount();

		if(lpObj->Authority == AUTHORITY_USER)
		{
			gEffectManager.DelEffect(lpObj,EFFECT_INVISIBILITY);
		}
	}

	if(lpTarget->Type == OBJECT_USER)
	{
		lpTarget->HPAutoRecuperationTime = GetTickCount();
		lpTarget->MPAutoRecuperationTime = GetTickCount();
		lpTarget->BPAutoRecuperationTime = GetTickCount();
	}

	if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonSetEnemy(lpObj,lpTarget->Index);
	}

	if(gEffectManager.CheckEffect(lpTarget,EFFECT_PHYSI_DAMAGE_IMMUNITY) != 0 && lpSkill == 0)
	{
		this->MissSend(lpObj,lpTarget,lpSkill,send,count);
		return 1;
	}

	if(gEffectManager.CheckEffect(lpTarget,EFFECT_MAGIC_DAMAGE_IMMUNITY) != 0 && lpSkill != 0)
	{
		this->MissSend(lpObj,lpTarget,lpSkill,send,count);
		return 1;
	}

	if(lpTarget->Type == OBJECT_MONSTER)
	{
		if(lpTarget->Class == 200 && lpSkill == 0) // Soccer Ball
		{
			gObjMonsterStateProc(lpTarget,6,lpObj->Index,0);
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 1;
		}

		if(lpTarget->Class == 200 && lpSkill != 0) // Soccer Ball
		{
			gObjMonsterStateProc(lpTarget,7,lpObj->Index,0);
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 1;
		}

		if(gEffectManager.CheckEffect(lpTarget,EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY) != 0 && skill == SKILL_NONE)
		{
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 1;
		}

		if(gEffectManager.CheckEffect(lpTarget,EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY) != 0 && skill != SKILL_NONE)
		{
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 1;
		}
	}

	if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].IsItem() != 0 && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Durability > 0 && damage == 0)
	{
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index == GET_ITEM(2,4) || lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index == GET_ITEM(2,5))
		{
			if(rand()%30 == 0)
			{
				static CSkill* lpSkill2 = new CSkill;

				skill = SKILL_ICE;

				lpSkill2->Set(skill);

				lpSkill = lpSkill2;

				send = 1;
			}
		}
	}

	#pragma endregion

	#pragma region DAMAGE_CALC

	flag = 0;

	BYTE miss = 0;

	WORD effect = 0;

	if(damage == 0)
	{
		if(this->MissCheck(lpObj,lpTarget,lpSkill,send,count,&miss) == 0)
		{
			return 1;
		}

		int defense = this->GetTargetDefense(lpObj,lpTarget,&effect);

		if((lpObj->Class == CLASS_DW || lpObj->Class == CLASS_MG) && skill != SKILL_NONE && skill != SKILL_FALLING_SLASH && skill != SKILL_LUNGE && skill != SKILL_UPPERCUT && skill != SKILL_CYCLONE && skill != SKILL_SLASH && skill != SKILL_TWISTING_SLASH && skill != SKILL_IMPALE && skill != SKILL_FIRE_SLASH && skill != SKILL_POWER_SLASH)
		{
			damage = this->GetAttackDamageWizard(lpObj,lpTarget,lpSkill,&effect,defense);
		}
		else
		{
			damage = this->GetAttackDamage(lpObj,lpTarget,lpSkill,&effect,defense);
		}

		if(damage > 0)
		{
			this->WeaponDurabilityDown(lpObj,lpTarget);
		}

		if(miss != 0)
		{
			damage = (damage*30)/100;
		}

		for(int n=0;n < MAX_DAMAGE_REDUCTION;n++)
		{
			damage -= (damage*lpTarget->DamageReduction[n])/100;
		}

		if(lpTarget->EffectOption.AddDamageReduction > 0)
		{
			damage -= (damage*lpTarget->EffectOption.AddDamageReduction)/100;
		}

		if((GetTickCount()-lpTarget->ShieldDamageReductionTime) < ((DWORD)(gServerInfo.m_DefenseTimeConstA*1000)))
		{
			damage -= (damage*lpTarget->ShieldDamageReduction)/100;
		}

		this->WingSprite(lpObj,lpTarget,&damage);

		this->HelperSprite(lpObj,lpTarget,&damage);

		int MinDamage = lpObj->Level/10;

		MinDamage = ((MinDamage<1)?1:MinDamage);

		damage = ((damage<MinDamage)?(MinDamage+(GetLargeRand()%MinDamage)):damage);

		this->DamageSprite(lpTarget,damage);

		if(skill == SKILL_FALLING_SLASH || skill == SKILL_LUNGE || skill == SKILL_UPPERCUT || skill == SKILL_CYCLONE || skill == SKILL_SLASH || skill == SKILL_TWISTING_SLASH || skill == SKILL_RAGEFUL_BLOW || skill == SKILL_DEATH_STAB || skill == SKILL_IMPALE || skill == SKILL_FIRE_BREATH || skill == SKILL_ICE_ARROW || skill == SKILL_PENETRATION || skill == SKILL_FIRE_SLASH || skill == SKILL_POWER_SLASH)
		{
			if(skill != SKILL_IMPALE || lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,2) || lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,3))
			{
				if(lpObj->Class == CLASS_DK)
				{
					damage = (damage*lpObj->DKDamageMultiplierRate)/100;
				}
				else
				{
					damage = (damage*200)/100;
				}
			}
		}
		else if(skill == SKILL_FORCE || skill == SKILL_FIRE_BURST || skill == SKILL_EARTHQUAKE || skill == SKILL_ELECTRIC_SPARK)
		{
			damage = (damage*lpObj->DLDamageMultiplierRate)/100;
		}

		// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 97 (2.5.5) - Isolate damage properties from damage types
		if((GetLargeRand()%100) < lpObj->DoubleDamageRate)
		{
			effect |= DAMAGE_PROP_DOUBLE;
			damage += damage;
		}

		if(combo != 0)
		{
			effect |= DAMAGE_PROP_COMBO;
			damage += damage;
			damage += (((lpObj->Strength+lpObj->AddStrength)+(lpObj->Dexterity+lpObj->AddDexterity)+(lpObj->Energy+lpObj->AddEnergy))/gServerInfo.m_ComboDamageConstA[lpObj->Class])*gServerInfo.m_ComboDamageConstB[lpObj->Class];
			damage = (damage*gServerInfo.m_ComboDamageConstC[lpObj->Class])/100;
			skill = SKILL_COMBO;
		}
	}
	else
	{
		effect = DAMAGE_TYPE_REFLECT;
		damage = (damage*((lpObj->Type==OBJECT_USER&&lpTarget->Type==OBJECT_USER)?gServerInfo.m_ReflectDamageRatePvP:gServerInfo.m_ReflectDamageRatePvM))/100;

		if (lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER)
		{
			damage = (damage*gServerInfo.m_ReflectDamageRateTo[lpObj->Class][lpTarget->Class])/100;
		}
	}

	#pragma endregion

	#pragma region DAMAGE_CONFIG

	if(lpObj->Type == OBJECT_USER)
	{
		if(lpTarget->Type == OBJECT_USER)
		{
			damage = (damage*gServerInfo.m_GeneralDamageRatePvP)/100;

			damage = (damage*gServerInfo.m_DamageRatePvP[lpObj->Class])/100;

			damage = (damage*gServerInfo.m_DamageRateTo[lpObj->Class][lpTarget->Class])/100;

			if(gCustomArena.CheckMap(lpObj->Map) != 0 && gCustomArena.CheckMap(lpTarget->Map) != 0)
			{
				damage = (damage*gServerInfo.m_CustomArenaDamageRate)/100;
			}
		}
		else
		{
			damage = (damage*gServerInfo.m_GeneralDamageRatePvM)/100;

			damage = (damage*gServerInfo.m_DamageRatePvM[lpObj->Class])/100;
		}
	}

	#pragma endregion

	#pragma region DAMAGE_FINISH

	damage = ((damage<0)?0:damage);

	if(lpObj->Type == OBJECT_MONSTER && lpTarget->Type == OBJECT_USER)
	{
		damage = (damage*gDamageTable.GetMonsterDamage(lpTarget))/100;
	}

	if(lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_MONSTER)
	{
		damage = (damage*gDamageTable.GetPlayerDamage(lpObj,lpTarget))/100;
	}

	#pragma endregion

	#pragma region APPLY_EFFECT

	if(lpSkill != 0 && count <= 1)
	{
		if(this->ApplySkillEffect(lpObj,lpTarget,lpSkill,damage) == 0)
		{
			if(send != 0)
			{
				gSkillManager.GCSkillAttackSend(lpObj,skill,lpTarget->Index,0);
			}
		}
		else
		{
			if(send != 0)
			{
				gSkillManager.GCSkillAttackSend(lpObj,skill,lpTarget->Index,1);
			}
		}
	}

	#pragma endregion

	#pragma region DAMAGE_APPLY

	if(lpTarget->Life < damage)
	{
		lpTarget->Life = 0;
	}
	else
	{
		lpTarget->Life -= damage;
	}

	if(lpTarget->Type == OBJECT_MONSTER)
	{
		lpTarget->LastAttackerID = lpObj->Index;

		gObjAddMsgSendDelay(lpTarget,0,lpObj->Index,100,0);
	}

	#pragma endregion

	#pragma region CHECK_SELF_DEFENSE

	if(damage > 0)
	{
		if(lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER && lpObj->Index != lpTarget->Index)
		{
			bool CheckSelfDefense = 1;

			if((effect & 0x0F) == DAMAGE_TYPE_REFLECT)
			{
				CheckSelfDefense = 0;
			}

			if(gCustomArena.CheckMap(lpObj->Map) != 0 && gCustomArena.CheckMap(lpTarget->Map) != 0)
			{
				CheckSelfDefense = 0;
			}

			if(gGuild.gObjTargetGuildWarCheck(lpObj,lpTarget) != 0)
			{
				CheckSelfDefense = 0;
			}

			if(gMapManager.GetMapNonOutlaw(lpObj->Map) != 0)
			{
				CheckSelfDefense = 0;
			}

			if(CheckSelfDefense != 0)
			{
				gObjCheckSelfDefense(lpObj,lpTarget->Index);
			}
		}

		this->ArmorDurabilityDown(lpObj,lpTarget);
	}

	lpObj->Rest = 0;

	#pragma endregion

	#pragma region ATTACK_FINISH

	if(damage > 0)
	{
		if(lpTarget->Type == OBJECT_USER)
		{
			if((effect & 0x0F) != DAMAGE_TYPE_REFLECT)
			{
				if(lpTarget->DamageReflect > 0)
				{
					gObjAddMsgSendDelay(lpTarget,10,lpObj->Index,10,((damage*lpTarget->DamageReflect)/100));
				}
			}
			
			bool stuck = true;

			if(lpTarget->Inventory[INVENTORY_SLOT_HELPER].IsItem() != 0 && lpTarget->Inventory[INVENTORY_SLOT_HELPER].m_Durability > 0)
			{
				if(lpTarget->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,2) && gServerInfo.m_DamageStuckOnPetUniria == 0) // Uniria
				{
					stuck = false;
				}
				else if(lpTarget->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,3) && gServerInfo.m_DamageStuckOnPetDinorant == 0) // Dinorant
				{
					stuck = false;
				}
			}

			if(stuck == true)
			{
				if((GetLargeRand()%100) < gServerInfo.m_DamageStuckRate[lpTarget->Class])
				{
					flag = 1;
				}
			}
		}

		gObjectManager.CharacterLifeCheck(lpObj,lpTarget,damage,0,flag,effect,skill,combo);
	}
	else
	{
		GCDamageSend(lpObj->Index,lpTarget->Index,0,0,effect);
	}

	if(lpObj->Type == OBJECT_USER && lpObj->Life <= 0 && lpObj->CheckLifeTime <= 0)
	{
		lpObj->AttackObj = lpTarget;
		lpObj->AttackerKilled = ((lpTarget->Type==OBJECT_USER)?1:0);
		lpObj->CheckLifeTime = 3;
	}

	#pragma endregion

	return 1;
}

bool CAttack::DecreaseArrow(LPOBJ lpObj) // OK
{
	if(lpObj->Type != OBJECT_USER || lpObj->Class != CLASS_FE)
	{
		return 1;
	}

	if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index != GET_ITEM(4,15) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Slot == 0)
	{
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index != GET_ITEM(4,7) || lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Durability < 1)
		{
			return 0;
		}
		else
		{
			if(gServerInfo.m_InfinityArrowSwitch[lpObj->AccountLevel] == 0)
			{
				gItemManager.DecreaseItemDur(lpObj,1,1);
			}
		}
	}

	if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index != GET_ITEM(4,7) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Slot == 1)
	{
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index != GET_ITEM(4,15) || lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Durability < 1)
		{
			return 0;
		}
		else
		{
			if(gServerInfo.m_InfinityArrowSwitch[lpObj->AccountLevel] == 0)
			{
				gItemManager.DecreaseItemDur(lpObj,0,1);
			}
		}
	}

	return 1;
}

void CAttack::WingSprite(LPOBJ lpObj,LPOBJ lpTarget,int* damage) // OK
{
	if(lpObj != 0 && lpObj->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpObj->Inventory[INVENTORY_SLOT_WING];

		if(lpItem->IsItem() != 0 && lpItem->m_Durability > 0)
		{
			if(lpObj->Class == CLASS_DW || lpObj->Class == CLASS_FE)
			{
				lpObj->Life -= 1;
			}
			else
			{
				lpObj->Life -= 3;
			}

			GCLifeSend(lpObj->Index,0xFF,(int)lpObj->Life);
			
			if(lpItem->m_Index >= GET_ITEM(12,0) && lpItem->m_Index <= GET_ITEM(12,2)) // 1st wing
			{
				(*damage) = ((*damage)*(112+(lpItem->m_Level*2)))/100;
			}
			else if(lpItem->m_Index >= GET_ITEM(12,3) && lpItem->m_Index <= GET_ITEM(12,6)) // 2sd wing
			{
				(*damage) = ((*damage)*(132+(lpItem->m_Level*1)))/100;
			}
			else if(lpItem->m_Index == GET_ITEM(13,30)) // Cloak of Lord
			{
				(*damage) = ((*damage)*(120+(lpItem->m_Level*2)))/100;
			}
		}
	}

	if(lpTarget != 0 && lpTarget->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpTarget->Inventory[INVENTORY_SLOT_WING];

		if(lpItem->IsItem() != 0 && lpItem->m_Durability > 0)
		{
			if(lpItem->m_Index >= GET_ITEM(12,0) && lpItem->m_Index <= GET_ITEM(12,2)) // 1st wing
			{
				(*damage) = ((*damage)*(88-(lpItem->m_Level*2)))/100;
			}
			else if(lpItem->m_Index >= GET_ITEM(12,3) && lpItem->m_Index <= GET_ITEM(12,6)) // 2sd wing
			{
				(*damage) = ((*damage)*(75-(lpItem->m_Level*2)))/100;
			}
			else if(lpItem->m_Index == GET_ITEM(13,30)) // Cloak of Lord
			{
				(*damage) = ((*damage)*(90-(lpItem->m_Level*1)))/100;
			}
		}
	}
}

void CAttack::HelperSprite(LPOBJ lpObj,LPOBJ lpTarget,int* damage) // OK
{
	if(lpObj != 0 && lpObj->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpObj->Inventory[INVENTORY_SLOT_HELPER];

		if(lpItem->IsItem() != 0 && lpItem->m_Durability > 0)
		{
			if(lpItem->m_Index == GET_ITEM(13,1)) // Satan
			{
				lpObj->Life -= 3;

				GCLifeSend(lpObj->Index,0xFF,(int)lpObj->Life);

				(*damage) = ((*damage)*(100+gServerInfo.m_SatanIncDamageConstA))/100;
			}
			else if(lpItem->m_Index == GET_ITEM(13,3)) // Dinorant
			{
				lpObj->Life -= 1;

				GCLifeSend(lpObj->Index,0xFF,(int)lpObj->Life);

				(*damage) = ((*damage)*(100+gServerInfo.m_DinorantIncDamageConstA))/100;
			}
		}
	}

	if(lpTarget != 0 && lpTarget->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpTarget->Inventory[INVENTORY_SLOT_HELPER];

		if(lpItem->IsItem() != 0 && lpItem->m_Durability > 0)
		{
			if(lpItem->m_Index == GET_ITEM(13,0)) // Angel
			{
				(*damage) = ((*damage)*(100-gServerInfo.m_AngelDecDamageConstA))/100;
			}
			else if(lpItem->m_Index == GET_ITEM(13,3)) // Dinorant
			{
				(*damage) = ((*damage)*(100-gServerInfo.m_DinorantDecDamageConstA-(((lpItem->m_NewOption & 1)==0)?0:gServerInfo.m_DinorantDecDamageConstB)))/100;
			}
		}
	}
}

void CAttack::DamageSprite(LPOBJ lpObj,int damage) // OK
{
	if(lpObj->Type != OBJECT_USER)
	{
		return;
	}

	CItem* lpItem = &lpObj->Inventory[INVENTORY_SLOT_HELPER];

	if(lpItem->IsItem() == 0 || lpItem->m_IsPeriodicItem != 0)
	{
		return;
	}

	float DurabilityValue = (1.0f/gServerInfo.m_GuardianDurabilityRate)*100;

	if(lpItem->m_Index == GET_ITEM(13,0)) // Angel
	{
		gPetDamageTable.GetPetDamage(lpObj,0,&damage);
		lpItem->m_Durability -= (damage*(3.0f*DurabilityValue))/100;
	}
	else if(lpItem->m_Index == GET_ITEM(13,1)) // Satan
	{
		gPetDamageTable.GetPetDamage(lpObj,1,&damage);
		lpItem->m_Durability -= (damage*(2.0f*DurabilityValue))/100;
	}
	else if(lpItem->m_Index == GET_ITEM(13,2)) // Uniria
	{
		gPetDamageTable.GetPetDamage(lpObj,2,&damage);
		lpItem->m_Durability -= (damage*(1.0f*DurabilityValue))/100;
	}
	else if(lpItem->m_Index == GET_ITEM(13,3)) // Dinorant
	{
		gPetDamageTable.GetPetDamage(lpObj,3,&damage);
		lpItem->m_Durability -= (damage*(1.0f*DurabilityValue))/100;
	}
	else
	{
		return;
	}

	gItemManager.GCItemDurSend(lpObj->Index,8,(BYTE)lpItem->m_Durability,0);

	if(lpItem->m_Durability < 1)
	{
		short ItemIndex = lpItem->m_Index;

		gItemManager.InventoryDelItem(lpObj->Index,8);

		gItemManager.GCItemDeleteSend(lpObj->Index,8,0);

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		gItemManager.GCItemChangeSend(lpObj->Index,8);

		if(lpObj->Map == MAP_ICARUS && lpObj->Inventory[INVENTORY_SLOT_WING].IsItem() == 0 && ItemIndex == GET_ITEM(13,3))
		{
			gObjMoveGate(lpObj->Index,22);
		}
	}
}

bool CAttack::DarkHorseSprite(LPOBJ lpObj,int damage) // OK
{
	CItem* lpItem = &lpObj->Inventory[INVENTORY_SLOT_HELPER];

	if(lpItem->m_Durability < 1)
	{
		return 0;
	}

	gPetDamageTable.GetPetDamage(lpObj,4,&damage);

	lpItem->m_DurabilitySmall += ((damage*2)/100)+1;

	int MaxSmallDur = (1500*gServerInfo.m_PetDurabilityRate)/100;

	if(lpItem->m_DurabilitySmall > MaxSmallDur)
	{
		lpItem->m_Durability = (((--lpItem->m_Durability)<1)?0:lpItem->m_Durability);
		lpItem->m_DurabilitySmall = 0;

		if(lpItem->CheckDurabilityState() != 0)
		{
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}

		gItemManager.GCItemDurSend(lpObj->Index,8,(BYTE)lpItem->m_Durability,0);

		GCPetItemInfoSend(lpObj->Index,1,0,8,lpItem->m_PetItemLevel,lpItem->m_PetItemExp,(BYTE)lpItem->m_Durability);
	}

	return 0;
}

void CAttack::WeaponDurabilityDown(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Type != OBJECT_USER)
	{
		return;
	}

	for(int n=0;n < 2;n++)
	{
		if(lpObj->Inventory[n].IsItem() != 0)
		{
			bool result = 0;

			switch((lpObj->Inventory[n].m_Index/MAX_ITEM_TYPE))
			{
				case 0:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,0);
					break;
				case 1:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,0);
					break;
				case 2:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,0);
					break;
				case 3:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,0);
					break;
				case 4:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,1);
					break;
				case 5:
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index,lpTarget->Defense,((lpObj->Inventory[n].m_Slot==0)?2:3));
					break;
			}

			if(result != 0)
			{
				gItemManager.GCItemDurSend(lpObj->Index,n,(BYTE)lpObj->Inventory[n].m_Durability,0);
			}
		}
	}
}

void CAttack::ArmorDurabilityDown(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	int slot = 1+(GetLargeRand()%6);

	if(lpTarget->Inventory[slot].IsItem() != 0)
	{
		if(slot != 1 || (lpTarget->Inventory[slot].m_Index >= GET_ITEM(6,0) && lpTarget->Inventory[slot].m_Index < GET_ITEM(7,0)))
		{
			if(lpTarget->Inventory[slot].ArmorDurabilityDown(lpTarget->Index,lpObj->PhysiDamageMin) != 0)
			{
				gItemManager.GCItemDurSend(lpTarget->Index,slot,(BYTE)lpTarget->Inventory[slot].m_Durability,0);
			}
		}
	}
}

bool CAttack::CheckPlayerTarget(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Type != OBJECT_USER || lpTarget->Type != OBJECT_USER)
	{
		return 1;
	}
	
	if(lpTarget->Authority == AUTHORITY_ADMINISTRATOR)
	{
		return 0;
	}

	if(gServerInfo.m_PartyDisableKillBetweenMembers != 0 && gParty.CheckSameParty(lpObj,lpTarget) != 0)
	{
		return 0;
	}

	if(lpObj->Guild != 0 && lpTarget->Guild != 0)
	{
		if(lpObj->Guild->WarState != 0 && lpTarget->Guild->WarState != 0)
		{
			if(lpObj->Guild->Index == lpTarget->Guild->Index)
			{
				return 0;
			}
		}
	}

	if(gGuild.gObjTargetGuildWarCheck(lpObj,lpTarget) == 0)
	{
		if(lpTarget->Guild != 0 && lpTarget->Guild->WarState != 0)
		{
			if(lpTarget->Guild->WarType == GUILD_WAR_TYPE_SOCCER && lpTarget->Map != MAP_ARENA && gMapManager.GetMapNonPK(lpTarget->Map) == 0)
			{
				return 1;
			}

			if (gCustomArena.CheckMap(lpTarget->Map) == 0)
			{
				if(gServerInfo.m_GuildWarAttackEnable == 0)
				{
					return 0;
				}
			}
		}
	}

	if(gCustomArena.CheckMap(lpObj->Map) != 0 && gCustomArena.CheckMap(lpTarget->Map) != 0)
	{
		return ((gCustomArena.CheckPlayerTarget(lpObj,lpTarget)==0)?0:1);
	}

	if(DS_MAP_RANGE(lpObj->Map) != 0 && DS_MAP_RANGE(lpTarget->Map) != 0)
	{
		return 0;
	}

	if(BC_MAP_RANGE(lpObj->Map) != 0 && BC_MAP_RANGE(lpTarget->Map) != 0)
	{
		return 0;
	}

	if(lpObj->Level <= 5 || lpTarget->Level <= 5)
	{
		return 0;
	}

	if(gMapManager.GetMapNonPK(lpTarget->Map) != 0)
	{
		return 0;
	}

	return 1;
}

void CAttack::MissSend(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count) // OK
{
	GCDamageSend(lpObj->Index,lpTarget->Index,0,0,0);

	if(send != 0 && lpSkill != 0)
	{
		gSkillManager.GCSkillAttackSend(lpObj,lpSkill->m_index,lpTarget->Index,0);
	}
}

bool CAttack::MissCheck(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int send,int count,BYTE* miss) // OK
{
	int AttackSuccessRate = lpObj->AttackSuccessRate;

	AttackSuccessRate = ((AttackSuccessRate<0)?0:AttackSuccessRate);

	int DefenseSuccessRate = lpTarget->DefenseSuccessRate;

	DefenseSuccessRate = ((DefenseSuccessRate<0)?0:DefenseSuccessRate);

	if(AttackSuccessRate < DefenseSuccessRate)
	{
		(*miss) = 1;

		if((GetLargeRand()%100) >= 5)
		{
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 0;
		}
	}
	else
	{
		(*miss) = 0;

		if((GetLargeRand()%((AttackSuccessRate==0)?1:AttackSuccessRate)) < DefenseSuccessRate)
		{
			this->MissSend(lpObj,lpTarget,lpSkill,send,count);
			return 0;
		}
	}

	return 1;
}

bool CAttack::ApplySkillEffect(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,int damage) // OK
{
	if(gSkillManager.GetSkillType(lpSkill->m_index) != -1 && gObjCheckResistance(lpTarget,gSkillManager.GetSkillType(lpSkill->m_index)) != 0)
	{
		return 0;
	}

	if(gSkillDamage.GetEffectRate(lpSkill) != 0)
	{
		return 0;
	}

	switch(lpSkill->m_index)
	{
		case SKILL_POISON:
			gEffectManager.AddEffect(lpTarget,0,gSkillManager.GetSkillEffect(lpSkill->m_index),gServerInfo.m_PoisonEffectTime,lpObj->Index,gServerInfo.m_PoisonEffectConstA,gServerInfo.m_PoisonEffectConstB,0);
			break;
		case SKILL_LIGHTNING:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_ICE:
			gEffectManager.AddEffect(lpTarget,0,gSkillManager.GetSkillEffect(lpSkill->m_index),gServerInfo.m_IceEffectTime,lpObj->Index,0,0,0);
			break;
		case SKILL_FALLING_SLASH:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_LUNGE:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_UPPERCUT:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_CYCLONE:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_SLASH:
			gObjAddMsgSendDelay(lpTarget,2,lpObj->Index,150,0);
			break;
		case SKILL_DECAY:
			gEffectManager.AddEffect(lpTarget,0,gSkillManager.GetSkillEffect(lpSkill->m_index),gServerInfo.m_DecayEffectTime,lpObj->Index,gServerInfo.m_DecayEffectConstA,gServerInfo.m_DecayEffectConstB,0);
			break;
		case SKILL_ICE_STORM:
			gEffectManager.AddEffect(lpTarget,0,gSkillManager.GetSkillEffect(lpSkill->m_index),gServerInfo.m_IceStormEffectTime,0,0,0,0);
			break;
		case SKILL_ICE_ARROW:
			gEffectManager.AddEffect(lpTarget,0,gSkillManager.GetSkillEffect(lpSkill->m_index),gServerInfo.m_IceArrowEffectTime,0,0,0,0);
			break;
		case SKILL_FIRE_SLASH:
			gSkillManager.ApplyFireSlashEffect(lpObj,lpTarget,lpSkill,damage);
			break;
		case SKILL_EARTHQUAKE:
			gObjBackSpring2(lpTarget,lpObj,3);
			break;
	}

	return 1;
}

int CAttack::GetTargetDefense(LPOBJ lpObj,LPOBJ lpTarget,WORD* effect) // OK
{
	int defense = lpTarget->Defense;

	defense += lpTarget->EffectOption.AddDefense;

	if(lpTarget->EffectOption.DivDefense > 0)
	{
		defense -= (defense*lpTarget->EffectOption.DivDefense)/100;
	}

	if(lpTarget->Type == OBJECT_USER)
	{
		defense = (defense*50)/100;
	}

	if((GetLargeRand()%100) < lpObj->IgnoreDefenseRate)
	{
		(*effect) = 1;

		defense = 0;
	}

	defense = ((defense<0)?0:defense);

	return defense;
}

int CAttack::GetAttackDamage(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense) // OK
{
	CItem* Right = &lpObj->Inventory[INVENTORY_SLOT_WEAPON1];
	CItem* Left = &lpObj->Inventory[INVENTORY_SLOT_WEAPON2];

	int damage = 0;
	int DamageMin = 0;
	int DamageMax = 0;
	int SkillDamageMin = 0;
	int SkillDamageMax = 0;
	bool DualHandWeapon = 0;

	if(lpObj->Type == OBJECT_MONSTER || lpObj->Type == OBJECT_NPC)
	{
		DamageMin = lpObj->PhysiDamageMin;
		DamageMax = lpObj->PhysiDamageMax;

		DamageMin += lpObj->EffectOption.AddPhysiDamage;
		DamageMax += lpObj->EffectOption.AddPhysiDamage;

		int range = (DamageMax-DamageMin);

		range = ((range<1)?1:range);

		damage = DamageMin+(GetLargeRand()%range);
	}
	else
	{
		if(lpObj->Class == CLASS_DK || lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL)
		{
			if(Right->m_Index >= GET_ITEM(0,0) && Right->m_Index < GET_ITEM(4,0) && Left->m_Index >= GET_ITEM(0,0) && Left->m_Index < GET_ITEM(4,0))
			{
				if(Right->m_IsValidItem != 0 && Left->m_IsValidItem != 0)
				{
					DualHandWeapon = 1;
				}
			}
		}

		if(lpSkill != 0)
		{
			SkillDamageMin = lpSkill->m_DamageMin;
			SkillDamageMax = lpSkill->m_DamageMax;

			SkillDamageMin += lpObj->SkillDamageBonus;
			SkillDamageMax += lpObj->SkillDamageBonus;

			int type = gSkillManager.GetSkillType(lpSkill->m_index);
		}

		if(DualHandWeapon != 0)
		{
			DamageMin = lpObj->PhysiDamageMinRight+lpObj->PhysiDamageMinLeft+SkillDamageMin;
			DamageMax = lpObj->PhysiDamageMaxRight+lpObj->PhysiDamageMaxLeft+SkillDamageMax;
		}
		else if((Right->m_Index >= GET_ITEM(0,0) && Right->m_Index < GET_ITEM(4,0)) || (Right->m_Index >= GET_ITEM(5,0) && Right->m_Index < GET_ITEM(6,0)))
		{
			DamageMin = lpObj->PhysiDamageMinRight+SkillDamageMin;
			DamageMax = lpObj->PhysiDamageMaxRight+SkillDamageMax;
		}
		else if(Right->m_Index >= GET_ITEM(4,0) && Right->m_Index < GET_ITEM(5,0) && Right->m_Index != GET_ITEM(4,15) && Right->m_Slot == 0)
		{
			DamageMin = lpObj->PhysiDamageMinRight+SkillDamageMin;
			DamageMax = lpObj->PhysiDamageMaxRight+SkillDamageMax;
		}
		else if(Left->m_Index >= GET_ITEM(4,0) && Left->m_Index < GET_ITEM(5,0) && Left->m_Index != GET_ITEM(4,7) && Left->m_Slot == 1)
		{
			DamageMin = lpObj->PhysiDamageMinLeft+SkillDamageMin;
			DamageMax = lpObj->PhysiDamageMaxLeft+SkillDamageMax;
		}
		else
		{
			DamageMin = lpObj->PhysiDamageMinLeft+SkillDamageMin;
			DamageMax = lpObj->PhysiDamageMaxLeft+SkillDamageMax;
		}

		DamageMin += lpObj->EffectOption.AddPhysiDamage;
		DamageMax += lpObj->EffectOption.AddPhysiDamage;

		int range = (DamageMax-DamageMin);

		range = ((range<1)?1:range);

		damage = DamageMin+(GetLargeRand()%range);

		if((GetLargeRand()%100) < lpObj->CriticalDamageRate)
		{
			(*effect) = 3;

			damage = DamageMax;
			damage += lpObj->CriticalDamage;
			damage += lpObj->EffectOption.AddCriticalDamage;
		}

		if((GetLargeRand()%100) < lpObj->ExcellentDamageRate)
		{
			(*effect) = 2;

			damage = (DamageMax*120)/100;
			damage += lpObj->ExcellentDamage;
		}
	}

	damage -= TargetDefense;

	damage = ((damage<0)?0:damage);

	return gSkillDamage.GetDamageRate(lpSkill,lpTarget->Type,damage);
}

int CAttack::GetAttackDamageWizard(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,WORD* effect,int TargetDefense) // OK
{
	CItem* Right = &lpObj->Inventory[INVENTORY_SLOT_WEAPON1];
	CItem* Left = &lpObj->Inventory[INVENTORY_SLOT_WEAPON2];

	int DamageMin = lpObj->MagicDamageMin;
	int DamageMax = lpObj->MagicDamageMax;

	if(lpSkill->m_index == SKILL_NOVA && lpObj->SkillNovaCount >= 0)
	{
		DamageMin += (gServerInfo.m_NovaDamageConstA*(lpObj->SkillNovaCount*gServerInfo.m_NovaDamageConstB))+((lpObj->Strength+lpObj->AddStrength)/gServerInfo.m_NovaDamageConstC);
		DamageMax += (gServerInfo.m_NovaDamageConstA*(lpObj->SkillNovaCount*gServerInfo.m_NovaDamageConstB))+((lpObj->Strength+lpObj->AddStrength)/gServerInfo.m_NovaDamageConstC);
	}
	else
	{
		DamageMin += lpSkill->m_DamageMin;
		DamageMax += lpSkill->m_DamageMax;
	}

	DamageMin += lpObj->SkillDamageBonus;
	DamageMax += lpObj->SkillDamageBonus;

	DamageMin += lpObj->EffectOption.AddMagicDamage;
	DamageMax += lpObj->EffectOption.AddMagicDamage;

	if(Right->IsItem() != 0 && Right->m_IsValidItem != 0 && ((Right->m_Index >= GET_ITEM(0,0) && Right->m_Index < GET_ITEM(1,0)) || (Right->m_Index >= GET_ITEM(5,0) && Right->m_Index < GET_ITEM(6,0))))
	{
		int rise = (int)(((Right->m_MagicDamageRate/2)+(Right->m_Level*2))*Right->m_CurrentDurabilityState);
		DamageMin += (DamageMin*rise)/100;
		DamageMax += (DamageMax*rise)/100;
	}

	int range = (DamageMax-DamageMin);

	range = ((range<1)?1:range);

	int damage = DamageMin+(GetLargeRand()%range);

	if((GetLargeRand()%100) < lpObj->CriticalDamageRate)
	{
		(*effect) = 3;

		damage = DamageMax;
		damage += lpObj->CriticalDamage;
		damage += lpObj->EffectOption.AddCriticalDamage;
	}

	if((GetLargeRand()%100) < lpObj->ExcellentDamageRate)
	{
		(*effect) = 2;

		damage = (DamageMax*120)/100;
		damage += lpObj->ExcellentDamage;
	}

	damage -= TargetDefense;

	damage = ((damage<0)?0:damage);

	return gSkillDamage.GetDamageRate(lpSkill,lpTarget->Type,damage);
}

void CAttack::GetPreviewDefense(LPOBJ lpObj,DWORD* defense) // OK
{
	(*defense) = lpObj->Defense;

	(*defense) += lpObj->EffectOption.AddDefense;

	(*defense) -= ((*defense)*lpObj->EffectOption.DivDefense)/100;
}

void CAttack::CGAttackRecv(PMSG_ATTACK_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(OBJECT_RANGE(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if(lpTarget->Live == 0)
	{
		return;
	}

	if(lpObj->Map != lpTarget->Map)
	{
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0 || gMap[lpTarget->Map].CheckAttr(lpTarget->X,lpTarget->Y,1) != 0)
	{
		return;
	}

	if(lpObj->Type == OBJECT_USER && sqrt(pow(((float)lpObj->X-(float)lpTarget->X),2)+pow(((float)lpObj->Y-(float)lpTarget->Y),2)) > ((lpObj->Class==CLASS_FE)?6:3))
	{
		return;
	}

	lpObj->Dir = lpMsg->dir;

	lpObj->MultiSkillIndex = 0;

	lpObj->MultiSkillCount = 0;

	GCActionSend(lpObj,lpMsg->action,aIndex,bIndex);

	this->Attack(lpObj,lpTarget,0,0,0,0,0,0);
}