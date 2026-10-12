// ShopManager.h: interface for the CShopManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Shop.h"

#define MAX_SHOP 100

#define SHOP_RANGE(x) (((x)<0)?0:((x)>=MAX_SHOP)?0:1)

struct SHOP_MANAGER_INFO
{
	int Index;
	int MonsterClass;
	int MapNumber;
	int PosX;
	int PosY;
	int Dir;
	int Enable[MAX_ACCOUNT_LEVEL];
	int GameMasterLevel;
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 109 (2.6.7) - Support CoinShopType
	int CoinShopType;
	CShop Shop;
};

class CShopManager
{
public:
	CShopManager();
	virtual ~CShopManager();
	void Load(char* path);
	void ReloadShop();
	void ReloadShopInterface();
	short GetShopNumber(int MonsterClass,int MapNumber,int PosX,int PosY);
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 109 (2.6.7) - Support CoinShopType output
	bool GetItemByIndex(int index,CItem* lpItem,int slot,int* type = 0);
	long GetItemCountByIndex(int index);
	bool CheckShopAccountLevel(LPOBJ lpNpc,LPOBJ lpObj);
	bool CheckShopGameMasterLevel(LPOBJ lpNpc,LPOBJ lpObj);
	bool GCShopItemListSendByIndex(int index,int aIndex);
public:
	std::vector<SHOP_MANAGER_INFO> m_ShopManagerInfo;
};

extern CShopManager gShopManager;