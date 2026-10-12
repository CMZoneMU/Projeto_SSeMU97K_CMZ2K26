// ItemDrop.h: interface for the CItemDrop class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct ITEM_DROP_INFO
{
	int ItemIndex;
	int Level;
	int Durability;
	int Option0;
	int Option1;
	int Option2;
	int Option3;
	int Option4;
	int Option5;
	int Duration;
	int MapNumber;
	int MonsterClass;
	int MonsterLevelMin;
	int MonsterLevelMax;
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 101 (2.6.1) - DropRate por nivel VIP (MAX_ACCOUNT_LEVEL)
	int DropRate[MAX_ACCOUNT_LEVEL];
};

class CItemDrop
{
public:
	CItemDrop();
	virtual ~CItemDrop();
	void Load(char* path);
	int DropItem(LPOBJ lpObj,LPOBJ lpTarget);
private:
	std::vector<ITEM_DROP_INFO> m_ItemDropInfo;
};

extern CItemDrop gItemDrop;
