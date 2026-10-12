#include "stdafx.h"
#include "ItemToolTip.h"
#include "CustomTooltip.h"
#include "PeriodicItem.h"

char TipTextList[30][100];
int TipTextColor[30];
int TipTextBold[30];
DWORD lpItem = 0;

void InitItemToolTip() // OK
{
	SetCompleteHook(0xE8,0x004F66CA,&RenderItemInfo);

	SetCompleteHook(0xE8,0x004F67DB,&RenderItemInfo);

	SetCompleteHook(0xE8,0x004C8C26,&RenderTipTextList);
}

void RenderItemInfo(int sx,int sy,DWORD ip,bool sell,int Inventype) // OK
{
	if(*(WORD*)ip == -1)
	{
		return;
	}

	lpItem = ip;

	((void(*)(int,int,DWORD,bool,int))0x004C4650)(sx,sy,ip,sell,Inventype);
}

void RenderTipTextList(int x,int y,int count,int tab,int sort,int point) // OK
{
	if(*(WORD*)lpItem == -1)
	{
		return;
	}

	RenderPeriodicTipText();

	memcpy(TipTextList,TextList(0),sizeof(TipTextList));
	memcpy(TipTextColor,TextListColor,sizeof(TipTextColor));
	memcpy(TipTextBold,TextBold,sizeof(TipTextBold));

	int m_ItemIndex = *(WORD*)lpItem;

	int m_ItemLevel = (*(DWORD*)(lpItem + 4) >> 3) & 15;

	for(int n = 0; n < MAX_CUSTOM_TOOLTIP; n++)
	{
		CUSTOM_TOOLTIP_INFO ToolTip = gCustomTooltip.m_CustomTooltipInfo[n];

		if(ToolTip.Index != -1 && ToolTip.ItemIndex == m_ItemIndex)
		{
			if(ToolTip.ItemLevel == -1 || ToolTip.ItemLevel == m_ItemLevel)
			{
				AddTipText(ToolTip.Text,ToolTip.FontColor,ToolTip.FontBold,ToolTip.Position);
			}
		}
	}

	memcpy(TextList(0),TipTextList,sizeof(TipTextList));
	memcpy(TextListColor,TipTextColor,sizeof(TipTextColor));
	memcpy(TextBold,TipTextBold,sizeof(TipTextBold));

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 106 (2.6.5-1) - Fix tooltip visual overflow when item has too many lines
	int BoxHeight = TextNum * 11;

	if((y + BoxHeight) > 425)
	{
		y = 425 - BoxHeight;
	}

	if(y < 5)
	{
		y = 5;
	}

	((void(*)(int,int,int,int,int,int))0x004C2420)(x,y,TextNum,tab,sort,point);
}

void AddTipText(char* text,int color,int bold) // OK
{
	if(TextNum >= 30)
	{
		return;
	}
	
	wsprintf(TextList(TextNum),text);
	TextListColor[TextNum] = color;
	TextBold[TextNum] = bold;
	
	TextNum += 1;
}

void AddTipText(char* text,int color,int bold,int slot) // OK
{
	if(TextNum >= 30)
	{
		return;
	}

	if(slot >= 0 && slot <= TextNum) 
	{
		for(int n = TextNum-1; n >= slot; n--) 
		{
			strncpy_s(TipTextList[n+1],TipTextList[n],100);
			TipTextColor[n+1] = TipTextColor[n];
			TipTextBold[n+1] = TipTextBold[n];
		}

		strncpy_s(TipTextList[slot],text,100);
		TipTextColor[slot] = color;
		TipTextBold[slot] = bold;
	}
	else
	{
		strncpy_s(TipTextList[TextNum],text,100);
		TipTextColor[TextNum] = color;
		TipTextBold[TextNum] = bold;
	}

	TextNum += 1;
}