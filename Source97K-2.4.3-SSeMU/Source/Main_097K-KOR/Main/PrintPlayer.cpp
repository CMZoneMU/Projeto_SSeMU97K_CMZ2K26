#include "stdafx.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "SkillAttackSpeed.h"

DWORD ViewIndex = 0;
DWORD ViewLevel = 0;
DWORD ViewReset = 0;
// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Initialize ViewAccountLevel
DWORD ViewAccountLevel = 0;
DWORD ViewPoint = 0;
DWORD ViewCurHP = 0;
DWORD ViewMaxHP = 0;
DWORD ViewCurMP = 0;
DWORD ViewMaxMP = 0;
DWORD ViewCurBP = 0;
DWORD ViewMaxBP = 0;
DWORD ViewDamageHP = 0;
DWORD ViewExperience = 0;
DWORD ViewNextExperience = 0;
DWORD ViewStrength = 0;
DWORD ViewDexterity = 0;
DWORD ViewVitality = 0;
DWORD ViewEnergy = 0;
DWORD ViewLeadership = 0;
DWORD ViewAddStrength = 0;
DWORD ViewAddDexterity = 0;
DWORD ViewAddVitality = 0;
DWORD ViewAddEnergy = 0;
DWORD ViewPhysiSpeed = 0;
DWORD ViewMagicSpeed = 0;
DWORD ViewFruitPoint = 0;
DWORD ViewMaxFruitPoint = 0;

void InitPrintPlayer()
{
	SetCompleteHook(0xE8, 0x004BD98F, &PrintDrawCircleHPMP);

	SetCompleteHook(0xE8, 0x004BDA56, &PrintDrawCircleHPMP);

	SetCompleteHook(0xE8, 0x004BCFF2, &PrintDrawCircleAG);

	SetCompleteHook(0xE8, 0x004BD9E7, &PrintPlayerViewHP1); //case 358: "Life: %d/%d"

	SetCompleteHook(0xE8, 0x004EE8E9, &PrintPlayerViewHP1); //case 211: "HP: %d / %d"

	SetCompleteHook(0xE8, 0x004BE2C5, &PrintPlayerViewHP2);

	SetCompleteHook(0xE8, 0x004BDAAE, &PrintPlayerViewMP1); //case 359: "Mana: %d/%d"

	SetCompleteHook(0xE8, 0x004EE9DC, &PrintPlayerViewMP1); //case 213: "Mana: %d / %d"

	SetCompleteHook(0xE8, 0x004BE2EC, &PrintPlayerViewMP2);

	SetCompleteHook(0xE8, 0x004BD055, &PrintPlayerViewBP1); //case 214: "A G: %d / %d"

	SetCompleteHook(0xE8, 0x004BD00C, &PrintPlayerViewBP2);

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Hook Character Status Window Level Text
	SetCompleteHook(0xE8, 0x004ED3A5, &PrintPlayerRenderLevelText);

	SetCompleteHook(0xE8, 0x004ED48F, &PrintPlayerViewExperience); //case 201: "Exp: %u/%u"

	SetCompleteHook(0xE8, 0x004BFCD3, &PrintPlayerViewExperience); //case 357: "Exp: %u/%u"

	SetCompleteHook(0xE8, 0x004EEE67, &PrintPlayerViewLevelUpPoint); //case 217: "Point: %d"

	SetCompleteHook(0xE8, 0x004ED7E3, &PrintPlayerViewStrength); //case 202: "Strength: %d"

	SetCompleteHook(0xE8, 0x004EE452, &PrintPlayerViewDexterity); //case 205: "Agility: %d"

	SetCompleteHook(0xE8, 0x004EE85B, &PrintPlayerViewVitality); //case 210: "Vitality: %d"

	SetCompleteHook(0xE8, 0x004EE95E, &PrintPlayerViewEnergy); //case 212: "Energy: %d"

	SetCompleteHook(0xE8, 0x004EE6BB, &PrintPlayerViewAttackSpeed);

	SetCompleteHook(0xE8, 0x00428C4B, &PrintMapName); //case 404: "Welcome to"

	SetCompleteHook(0xE8, 0x0042AD92, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042B38E, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042B402, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042DA3D, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042E2CA, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042E2FE, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042ED2C, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042ED60, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x004BFC26, &PrintBarExperience1);

	SetCompleteHook(0xE8, 0x004BFC5B, &PrintBarExperience2);

	SetCompleteHook(0xE9, 0x0047DD80, &CalculateAttackSpeed);
}

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Render Reset and VIP Text in Character Status Window
int PrintPlayerRenderLevelText(int iPos_x, int iPos_y, char *pszText, int iBoxWidth, int iSort, SIZE *lpTextSize)
{
	int result = RenderText(iPos_x, iPos_y, pszText, iBoxWidth, iSort, lpTextSize);

	DWORD dwOldColor = SetTextColor;
	DWORD dwOldBgColor = SetBackgroundTextColor;
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Select bold font (g_hFontBold) to match Spare Points style
	HFONT hOldFont = (HFONT)SelectObject(m_hFontDC, g_hFontBold);

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Soft black background opacity from WebZen DecIDA (0x80000000)
	SetBackgroundTextColor = 0x80000000;
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Match text color with Spare Points (Color4f: 100, 150, 255, 255)
	SetTextColor = Color4f(100, 150, 255, 255);

	char szReset[32];
	wsprintf(szReset, "Resets: %d", ViewReset);

	const char* pszAccountLevelName = "Free";
	if (ViewAccountLevel == 1)
	{
		pszAccountLevelName = "Vip";
	}
	else if (ViewAccountLevel == 2)
	{
		pszAccountLevelName = "Vip Premium";
	}
	else if (ViewAccountLevel == 3)
	{
		pszAccountLevelName = "Vip Events";
	}

	char szVip[64];
	wsprintf(szVip, "Tipo de Conta: %s", pszAccountLevelName);

	SIZE sz1, sz2;
	GetTextExtentPoint32A(m_hFontDC, szReset, lstrlenA(szReset), &sz1);
	GetTextExtentPoint32A(m_hFontDC, szVip, lstrlenA(szVip), &sz2);

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ 14 (3.1.4) - Align center with Point text from WebZen DecIDA (Line 170751)
	int iWindowX = iPos_x - 14;
	int iPointBoxWidth = (80 * WindowWidth) / 640;
	int iPointBoxX = iWindowX + 95;
	int iPointCenterX = iPointBoxX + (iPointBoxWidth / 2);

	int iMaxWidth = (sz1.cx > sz2.cx) ? sz1.cx : sz2.cx;
	int iBoxWidthFinal = (iMaxWidth < iPointBoxWidth) ? iPointBoxWidth : (iMaxWidth + 4);

	int iBoxX = iPointCenterX - (iBoxWidthFinal / 2);

	RenderText(iBoxX, iPos_y - 1, szReset, iBoxWidthFinal, 1, NULL);
	RenderText(iBoxX, iPos_y + 8, szVip, iBoxWidthFinal, 1, NULL);

	SelectObject(m_hFontDC, hOldFont);
	SetTextColor = dwOldColor;
	SetBackgroundTextColor = dwOldBgColor;

	return result;
}

void PrintDrawCircleHPMP(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	float HP = (float)(ViewMaxHP - ViewCurHP) / (float)ViewMaxHP;

	float MP = (float)(ViewMaxMP - ViewCurMP) / (float)ViewMaxMP;

	y = ((Texture == 0xEB) ? MP : HP) * 48.0f + 432.0f;

	Height = 48.0f - ((Texture == 0xEB) ? MP : HP) * 48.0f;

	v = ((Texture == 0xEB) ? MP : HP) * 48.0f / 64.0f;

	vHeight = (1.0f - ((Texture == 0xEB) ? MP : HP)) * 48.0f / 64.0f;

	return RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
}

void PrintDrawCircleAG(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	float BP = (float)(ViewMaxBP - ViewCurBP) / (float)ViewMaxBP;

	y = BP * 36.0f + 438.0f;

	Height = 36.0f - BP * 36.0f;

	v = BP * 36.0f / 64.0f;

	vHeight = (1.0f - BP) * 36.0f / 64.0f;

	return RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
}

void PrintPlayerViewHP1(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewCurHP, ViewMaxHP);
}

void PrintPlayerViewHP2(float x, float y, int Num, float Width, float Height)
{
	RenderNumber2D(x, y, ViewCurHP, 9.0f, 10.0f);
}

void PrintPlayerViewMP1(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewCurMP, ViewMaxMP);
}

void PrintPlayerViewMP2(float x, float y, int Num, float Width, float Height)
{
	RenderNumber2D(x - 20, y, ViewCurMP, 9.0f, 10.0f);
}

void PrintPlayerViewBP1(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewCurBP, ViewMaxBP);
}

void PrintPlayerViewBP2(float x, float y, int Num, float Width, float Height)
{
	RenderNumber2D(x, y, ViewCurBP, 9.0f, 10.0f);
}

void PrintPlayerViewFruitPoint(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewFruitPoint, ViewMaxFruitPoint);
}

void PrintPlayerViewExperience(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewExperience, ViewNextExperience);
}

void PrintPlayerViewLevelUpPoint(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewPoint);
}

void PrintPlayerViewStrength(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewStrength + ViewAddStrength);
}

void PrintPlayerViewDexterity(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewDexterity + ViewAddDexterity);
}

void PrintPlayerViewVitality(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewVitality + ViewAddVitality);
}

void PrintPlayerViewEnergy(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewEnergy + ViewAddEnergy);
}

void PrintPlayerViewAttackSpeed(char* Dest,char* Format) // OK
{
	CHARACTER_ATTRIBUTE* CharacterAttribute = (CHARACTER_ATTRIBUTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT));

	if((CharacterAttribute->Class & 7) == 0)
	{
		wsprintf(Dest, Format, ViewMagicSpeed);
	}
	else
	{
		wsprintf(Dest, Format, ViewPhysiSpeed);
	}
}

void PrintMapName(char* Dest, char* Format,char* name,char* text)
{
	wsprintf(Dest, Format, text, name);
}

void PrintDamageOnScreenHP(float Position[3], int Value, float Color[3], float scale, bool bMove)
{
	if (Value > 0)
	{
		Value = ViewDamageHP;
	}

	CreatePoint(Position, Value, Color, scale, bMove);
}

void PrintBarExperience1(float a,float b,float c,float d) // OK
{
	float Experience = (float)ViewExperience-(float)gLevelExperience[ViewLevel-1];
	float NextExperience = (float)ViewNextExperience-(float)gLevelExperience[ViewLevel-1];
	
	Experience = ((ViewExperience < gLevelExperience[ViewLevel-1]) ? 0.0f : Experience);
	
	c = (Experience > 0.0f && NextExperience > 0.0f) ? ((Experience/NextExperience)*10.0f) : 0.0f;
	c -= (int)c;
	c *= 198.0f;

	RenderColor(a,b,c,d);
}

void PrintBarExperience2(float a,float b,DWORD c,float d,float e) // OK
{
	float Experience = (float)ViewExperience-(float)gLevelExperience[ViewLevel-1];
	float NextExperience = (float)ViewNextExperience-(float)gLevelExperience[ViewLevel-1];

	Experience = ((ViewExperience < gLevelExperience[ViewLevel-1])?0.0f:Experience);
	
	c = (Experience > 0.0f && NextExperience > 0.0f)?(int)((Experience/NextExperience)*10.0f):0;
	c = ((c<0)?0:((c>9)?9:c));

	RenderNumber2D(a,b,c,d,e);
}

void CalculateAttackSpeed(DWORD This)
{
	if(SceneFlag != 5)
	{
		return;
	}

	CHARACTER_ATTRIBUTE* CharacterAttribute = (CHARACTER_ATTRIBUTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT));

	CharacterAttribute->Level = GET_MAX_WORD_VALUE(ViewLevel);
	CharacterAttribute->LevelUpPoint = GET_MAX_WORD_VALUE(ViewPoint);

	CharacterAttribute->Experience = ((ViewExperience>ViewNextExperience)?ViewNextExperience:ViewExperience);
	CharacterAttribute->NextExperince = ViewNextExperience;

	CharacterAttribute->Strength = GET_MAX_WORD_VALUE(ViewStrength);
	CharacterAttribute->Dexterity = GET_MAX_WORD_VALUE(ViewDexterity);
	CharacterAttribute->Vitality = GET_MAX_WORD_VALUE(ViewVitality);
	CharacterAttribute->Energy = GET_MAX_WORD_VALUE(ViewEnergy);

	CharacterAttribute->Life = GET_MAX_WORD_VALUE(ViewCurHP);
	CharacterAttribute->LifeMax = GET_MAX_WORD_VALUE(ViewMaxHP);

	CharacterAttribute->Mana = GET_MAX_WORD_VALUE(ViewCurMP);
	CharacterAttribute->ManaMax = GET_MAX_WORD_VALUE(ViewMaxMP);

	CharacterAttribute->SkillMana = GET_MAX_WORD_VALUE(ViewCurBP);
	CharacterAttribute->SkillManaMax = GET_MAX_WORD_VALUE(ViewMaxBP);

	CharacterAttribute->AddPoint = GET_MAX_WORD_VALUE(ViewFruitPoint);
	CharacterAttribute->MaxAddPoint = GET_MAX_WORD_VALUE(ViewMaxFruitPoint);

	SKILL_ATTACK_SPEED_INFO* lpInfo = gSkillAttackSpeed.GetInfo(((BYTE(__thiscall*)(DWORD,DWORD))0x0045FAE0)(MAIN_HASH_CLASS,(Hero+770)));

	BYTE AnimationValue = gProtect.m_MainInfo.AttackAnimationValue[(CharacterAttribute->Class & 7)];
	DWORD PhysiSpeed = ViewPhysiSpeed;
	DWORD MagicSpeed = ViewMagicSpeed;

	if(lpInfo != 0)
	{
		if(lpInfo->AnimationValue != -1)
		{
			AnimationValue = lpInfo->AnimationValue;
		}

		if(lpInfo->MaxPhysiSpeed != -1 && ViewPhysiSpeed >= (DWORD)lpInfo->MaxPhysiSpeed)
		{
			PhysiSpeed = lpInfo->MaxPhysiSpeed;
		}

		if(lpInfo->MaxMagicSpeed != -1 && ViewMagicSpeed >= (DWORD)lpInfo->MaxMagicSpeed)
		{
			MagicSpeed = lpInfo->MaxMagicSpeed;
		}
	}

	SetByte(0x0044895B,AnimationValue);

	CharacterAttribute->AttackSpeed = GET_MAX_WORD_VALUE(PhysiSpeed);
	CharacterAttribute->MagicSpeed = GET_MAX_WORD_VALUE(MagicSpeed);
}