// DiePenalty.h: interface for the CDiePenalty class.
// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 105 (2.6.5) - Sistema de penalidades ao morrer DiePenalty
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct DIE_PENALTY_INFO
{
	int Type; // 0: PvM, 1: PvP
	int Index; // PKLevel
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	int ExpDecRate[MAX_ACCOUNT_LEVEL];
	int MoneyDecRate[MAX_ACCOUNT_LEVEL];
	int DropRate[MAX_ACCOUNT_LEVEL];
	int DropItemMinLevel;
	int DropItemMaxLevel;
	int MaxNewOptionCount;
	int DropHelperSwitch;
	int DropWingSwitch;
	int DropSetSwitch;
};

class CDiePenalty
{
public:
	CDiePenalty();
	virtual ~CDiePenalty();
	void Load(char* path);
	void UserDieProc(LPOBJ lpObj,LPOBJ lpTarget);
private:
	std::vector<DIE_PENALTY_INFO> m_DiePenaltyInfo;
};

extern CDiePenalty gDiePenalty;
