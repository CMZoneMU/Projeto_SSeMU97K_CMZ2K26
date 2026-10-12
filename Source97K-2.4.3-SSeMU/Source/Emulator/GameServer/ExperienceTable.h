// ExperienceTable.h: interface for the CExperienceTable class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct EXPERIENCE_TABLE_INFO
{
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	int MinMasterReset;
	int MaxMasterReset;
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 101 (2.6.1) - ExperienceRate por nivel VIP (MAX_ACCOUNT_LEVEL)
	int ExperienceRate[MAX_ACCOUNT_LEVEL];
};

class CExperienceTable
{
public:
	CExperienceTable();
	virtual ~CExperienceTable();
	void Load(char* path);
	int GetExperienceRate(LPOBJ lpObj);
private:
	std::vector<EXPERIENCE_TABLE_INFO> m_ExperienceTableInfo;
};

extern CExperienceTable gExperienceTable;
