// CustomAttack.h: interface for the CCustomAttack class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DefaultClassInfo.h"
#include "Map.h"
#include "Protocol.h"
#include "User.h"

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_CUSTOM_ATTACK_TOGGLE_RECV
{
	PSBMSG_HEAD header;// C1:F3:0A
	int index;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_CUSTOM_ATTACK_STATUS_SEND
{
	PSBMSG_HEAD header; // C1:F3:EB
	int Started;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct CUSTOM_ATTACK_DELAY_INFO
{
	int Index;
	int MinDexterity;
	int MaxDexterity;
	int MinCharSpeed;
	int MaxCharSpeed;
	int AttackDelay;
	int BaseSpeed;
	int MainSpeed;
};

struct CUSTOM_ATTACK_SKILL_INFO
{
	int Index;
	int Group;
	int Value;
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 105 (2.6.5) - MaxRange individual por skill
	int Range;
	int RequireClass[MAX_CLASS];
};

class CCustomAttack
{
public:
	CCustomAttack();
	virtual ~CCustomAttack();
	void MainProc(LPOBJ lpObj);
	void SecondProc(LPOBJ lpObj);
	void AttackOfflineClose(LPOBJ lpObj);
	void ReadCustomAttackInfo(char* section,char* path);
	void Load(char* path);
	void SetInfo(CUSTOM_ATTACK_SKILL_INFO info);
	DWORD GetAttackDelay(LPOBJ lpObj,int skill);
	CUSTOM_ATTACK_SKILL_INFO* GetInfo(int Class,int skill);
	int GetSkilLAuto(LPOBJ lpObj);
	void CustomAttackSend(int aIndex);
	void CustomAttackClose(int aIndex);
	void CommandCustomAttack(LPOBJ lpObj,char* arg);
	void CommandCustomAttackOffline(LPOBJ lpObj);
	void CustomAttackAutoPotion(LPOBJ lpObj);
	void CustomAttackAutoRepair(LPOBJ lpObj);
	void CustomAttackAutoReload(LPOBJ lpObj);
	void CustomAttackAutoBuff(LPOBJ lpObj);
	void CustomAttackAutoAttack(LPOBJ lpObj);
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 105 (2.6.5) - MaxRange individual por skill
	int CustomAttackFindTarget(LPOBJ lpObj,int skill,int range);
	CSkill* CustomAttackFindSkill(LPOBJ lpObj,int skill);
	void CustomAttackUseSkill(LPOBJ lpObj,LPOBJ lpTarget,CUSTOM_ATTACK_SKILL_INFO* lpInfo,CSkill* lpSkill);
	void CustomAttackSkillAttack(LPOBJ lpObj,int bIndex,int skill);
	void CustomAttackMultilAttack(LPOBJ lpObj,int bIndex,int skill);
	void CustomAttackDurationlAttack(LPOBJ lpObj,int bIndex,int skill);
	bool CustomAttackCheckRequeriment(LPOBJ lpObj);
	void CGCustomAttackToggleRecv(PMSG_CUSTOM_ATTACK_TOGGLE_RECV* lpMsg,int aIndex);
	int GetExperienceRate(LPOBJ lpObj);
public:
	std::vector<CUSTOM_ATTACK_DELAY_INFO> m_CustomAttackDelayInfo;
	std::map<int,CUSTOM_ATTACK_SKILL_INFO> m_CustomAttackSkillInfo;
	int m_CustomAttackSwitch;
	int m_CustomAttackMapZone;
	int m_CustomAttackMapList[MAX_MAP];
	int m_CustomAttackBuffEnable[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackMaxTimeLimit[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackPotionEnable[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackPotionDelay[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackPotionRate1;
	int m_CustomAttackPotionRate2;
	int m_CustomAttackRepair[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackExperienceRate[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackOfflineSwitch;
	int m_CustomAttackOfflineCoinGain[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackOfflineBuffEnable[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackOfflineKeepEnable[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackOfflineMaxTimeLimit[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackOfflineExperienceRate[MAX_ACCOUNT_LEVEL];
};

extern CCustomAttack gCustomAttack;
