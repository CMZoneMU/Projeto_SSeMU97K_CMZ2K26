#include "stdafx.h"
#include "ItemMove.h"
#include "Offset.h"
#include "Protect.h"
#include "Shop.h"
#include "Util.h"

void InitItemMove() // OK
{
	MemorySet(0x004D5C78,0x90,0x04); // Item Move Inventory -> Interface

	MemorySet(0x004D5CC5,0x90,0x04); // Item Move Interface -> Inventory

	SetCompleteHook(0xE8,0x004D5C98,&AutoMoveItem);
}

void RightClickMove() // OK
{
	if(gProtect.m_MainInfo.ItemMoveRightClicSwitch == 0)
	{
		return;
	}

	if(*(DWORD*)0x05826D1C > 0 || EquipmentItem || MouseLButtonPush || !MouseRButton || !*(BYTE*)0x07EAA117)
	{
		return;
	}

	// Do not intercept right-click if Trade, Warehouse (Bau), or Chaos Machine is open
	if(*(BYTE*)0x07D3EADE || *(BYTE*)0x07EAA11A || *(BYTE*)0x07EAA119)
	{
		return;
	}

	DWORD lpItem = 0;

	if(ItemTarget)
	{
		lpItem = ItemTarget;
	}
	else if(*(DWORD*)0x07EAA13C == 1)
	{
		lpItem = PickedItem;
	}

	if(lpItem == 0)
	{
		return;
	}

	BYTE SourceSlot = 0xFF;

	BYTE TargetSlot = 0xFF;

	for(int i = 0; i < 2; i++)
	{
		for(int n = 0; n < INVENTORY_WEAR_SIZE; ++n)
		{
			if(*(DWORD*)0x07EA840C == (slotx[n]-(190*i)) && *(DWORD*)0x07EA8408 == sloty[n])
			{
				SourceSlot = n;
				break;
			}
		}
	}

	if(SourceSlot != 0xFF)
	{
		STRUCT_DECRYPT;

		if(World == 10)
		{
			if(SourceSlot == 7 && GetItemEquipedIndex(8) != GET_ITEM(13,3))
			{
				return;
			}

			if(SourceSlot == 8 && GetItemEquipedIndex(8) == GET_ITEM(13,3) && GetItemEquipedIndex(7) == 0xFFFF)
			{
				return;
			}
		}

		DWORD ItemStruct = GetItemEquiped(SourceSlot);

		TargetSlot = GetInventoryEmptySlot(*(WORD*)ItemStruct);

		if(TargetSlot != 0xFF)
		{
			InventoryBackup = (DWORD)InventoryMap;

			memcpy(&PickedItem,(const void *)ItemStruct,0x44);

			*(WORD*)(ItemStruct) = -1;
			*(DWORD*)(ItemStruct + 4) = 0;
			*(BYTE*)(ItemStruct + 27) = 0;

			PointerSlot = SourceSlot;

			DeleteEquippingEffect(Hero);

			ItemTarget = 0;

			if(SourceSlot == 8)
			{
				DeleteBugOrPet(Hero);
			}

			SendRequestEquipmentItem(0,SourceSlot,0,TargetSlot);

			MouseRButton = false;
		}

		STRUCT_ENCRYPT;
	}
	else
	{
		SourceSlot = *(BYTE*)(lpItem + 62) + 8 * *(BYTE*)(lpItem + 63) + INVENTORY_WEAR_SIZE;

		TargetSlot = GetInventoryWearSlot(lpItem);

		if(TargetSlot != 0xFF)
		{
			memcpy(&PickedItem,&InventoryMap[34*(SourceSlot-INVENTORY_WEAR_SIZE)],0x44);		
			InventoryBackup = (DWORD)InventoryMap;

			PointerSlot = SourceSlot;
			PointerDeleteItem(SourceSlot,InventoryMap,8);

			ItemTarget = 0;

			SendRequestEquipmentItem(0,SourceSlot,0,TargetSlot);

			MouseRButton = false;
		}
	}
}

bool AutoMoveItem(int x,int y,DWORD inventory,int width,int height) // OK
{
	((UINT(__thiscall*)(DWORD,DWORD)) 0x0043D8A0)(0x055C9BC8,0x07EAA11B);

	int trade_var = *(BYTE*)0x07D3EADE;

	((UINT(__thiscall*)(DWORD,DWORD)) 0x00404040)(0x055C9BC8,0x07EAA11B);

	if(trade_var)
	{
		return ((bool(*)(int,int,DWORD,int,int))0x004D6020)(275,271,0x07EA7B88,8,4);
	}
	else if(*(BYTE*)0x07EAA11A)
	{
		return ((bool(*)(int,int,DWORD,int,int))0x004D6020)(275,110,0x07EA9848,8,4);
	}
	else if(*(BYTE*)0x07EAA119)
	{
		return ((bool(*)(int,int,DWORD,int,int))0x004D6020)(x,y,inventory,width,height);
	}

	return false;
}

BYTE InventoryRectCheck(int x,int y,int width,int height) // OK
{
	int slot = ((y*8)+x)+INVENTORY_WEAR_SIZE;

	if(((x+width) > 8 || (y+height) > 8))
	{
		return 0xFF;
	}

	for(int sy=0;sy < height;sy++)
	{
		for(int sx=0;sx < width;sx++)
		{
			if(InventoryMap[34*(((sy+y)*8)+(sx+x))] != 0xFFFF)
			{
				return 0xFF;
			}
		}
	}

	return slot;
}

BYTE GetInventoryEmptySlot(int ItemIndex) // OK
{
	ITEM_INFO* lpItemInfo = GetItemInfo(ItemIndex);

	for(int y=0;y < 8;y++)
	{
		for(int x=0;x < 8;x++)
		{
			if(InventoryMap[34*((y*8)+x)] == 0xFFFF)
			{
				if(InventoryRectCheck(x,y,lpItemInfo->Width,lpItemInfo->Height) != 0xFF)
				{
					return ((y*8)+x)+INVENTORY_WEAR_SIZE;
				}
			}
		}
	}

	return 0xFF;
}

BYTE GetInventoryWearSlot(DWORD Item) // OK
{
	if(Item == 0)
	{
		return 0xFF;
	}

	WORD itemIndex = *(WORD*)Item;

	if(itemIndex == 0xFFFF)
	{
		return 0xFF;
	}

	// Read item wear slot attribute from client item struct
	BYTE defaultSlot = *(BYTE*)(Item + 8);

	// If slot attribute is invalid or >= 12 (Orbs, Scrolls, Potions, Jewels, Quest items), it is not equipable!
	if(defaultSlot >= INVENTORY_WEAR_SIZE)
	{
		return 0xFF;
	}

	STRUCT_DECRYPT;

	ITEM_INFO* lpItemInfo = GetItemInfo(itemIndex);

	BYTE Class = ((*(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0B)) & 7);

	BYTE ChangeUp = GetChangeUp(*(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0B));

	WORD ItemR = GetItemEquipedIndex(0);
	WORD ItemL = GetItemEquipedIndex(1);
	WORD RingR = GetItemEquipedIndex(10);
	WORD RingL = GetItemEquipedIndex(11);

	BYTE slot = defaultSlot;

	// Handle 2nd Weapon/Shield slot for 1-hand weapons
	if(defaultSlot == 0 && lpItemInfo->TwoHand == 0)
	{
		if(ItemR != 0xFFFF && ItemL == 0xFFFF)
		{
			ITEM_INFO* lpItemRight = GetItemInfo(ItemR);

			if(lpItemRight->TwoHand == 0)
			{
				slot = 1;
			}
		}
	}
	// Handle 2nd Ring slot
	else if(defaultSlot == 10)
	{
		if(RingR != 0xFFFF && RingL == 0xFFFF)
		{
			slot = 11;
		}
	}

	STRUCT_ENCRYPT;

	bool bEquipable = false;

	if(lpItemInfo->RequireClass[Class])
	{
		bEquipable = true;
	}
	else if(Class == 3 && lpItemInfo->RequireClass[0] && lpItemInfo->RequireClass[1])
	{
		bEquipable = true;
	}

	if(lpItemInfo->RequireClass[Class] > ChangeUp)
	{
		bEquipable = false;
	}

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 101 (2.6.1) - Sincronizacao de requisitos de atributos ao equipar com clique direito
	CHARACTER_ATTRIBUTE* lpChar = (CHARACTER_ATTRIBUTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT));

	if(lpChar != 0)
	{
		if(lpChar->Level < lpItemInfo->RequireLevel)
		{
			bEquipable = false;
		}

		if(lpChar->Strength < lpItemInfo->RequireStrength)
		{
			bEquipable = false;
		}

		if(lpChar->Dexterity < lpItemInfo->RequireDexterity)
		{
			bEquipable = false;
		}

		if(lpChar->Energy < lpItemInfo->RequireEnergy)
		{
			bEquipable = false;
		}
	}

	return ((bEquipable) ? slot : 0xFF);
}