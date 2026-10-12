#pragma once

#define MAX_MAIN_VIEWPORT 400

/* SOUND */
#define MAX_SOUND_LEVEL 9
#define MAX_MUSIC_LEVEL 9

#define Color3f(r, b, g)	((g << 16) + (b << 8) + r)
#define Color4f(r, b, g, o)	((o << 24) + (g << 16) + (b << 8) + r)

#define MAX_ITEM_SECTION 16
#define MAX_ITEM_TYPE 32
#define MAX_ITEM (MAX_ITEM_SECTION*MAX_ITEM_TYPE)
#define ITEM_BASE_MODEL 400
#define GET_ITEM(x,y) (((x)*MAX_ITEM_TYPE)+(y))
#define GET_ITEM_MODEL(x,y) ((((x)*MAX_ITEM_TYPE)+(y))+ITEM_BASE_MODEL)

enum eModelNumbers
{
	MODEL_SWORD = ITEM_BASE_MODEL,
	MODEL_AXE = MODEL_SWORD + MAX_ITEM_TYPE,
	MODEL_MACE = MODEL_AXE + MAX_ITEM_TYPE,
	MODEL_SPEAR = MODEL_MACE + MAX_ITEM_TYPE,
	MODEL_BOW = MODEL_SPEAR + MAX_ITEM_TYPE,
	MODEL_STAFF = MODEL_BOW + MAX_ITEM_TYPE,
	MODEL_SHIELD = MODEL_STAFF + MAX_ITEM_TYPE,
	MODEL_HELM = MODEL_SHIELD + MAX_ITEM_TYPE,
	MODEL_ARMOR = MODEL_HELM + MAX_ITEM_TYPE,
	MODEL_PANTS = MODEL_ARMOR + MAX_ITEM_TYPE,
	MODEL_GLOVES = MODEL_PANTS + MAX_ITEM_TYPE,
	MODEL_BOOTS = MODEL_GLOVES + MAX_ITEM_TYPE,
	MODEL_WING = MODEL_BOOTS + MAX_ITEM_TYPE,
	MODEL_HELPER = MODEL_WING + MAX_ITEM_TYPE,
	MODEL_POTION = MODEL_HELPER + MAX_ITEM_TYPE,
	MODEL_ETC = MODEL_POTION + MAX_ITEM_TYPE,
	MODEL_EVENT = 947,
};

// Update 21 Kayito -> 97K - Expansao de Texturas e Modelos (Zeus)
#define MAX_TEXTURE 5500
#define PLUS_TEXTURE 5500
#define TOTAL_TEXTURE (MAX_TEXTURE + PLUS_TEXTURE)

#define MAX_MODELS 963
#define PLUS_MODELS 5500
#define TOTAL_MODELS (MAX_MODELS + PLUS_MODELS)

#define MAX_BITMAP_FILE_NAME 32

struct BITMAP_t
{
	char FileName[MAX_BITMAP_FILE_NAME];
	float Width;
	float Height;
	char Components;
	GLuint TextureNumber;
	BYTE Ref;
	BYTE* Buffer;
};

struct BMD
{
	BYTE Data[188];
};
// Fim Update 21 Kayito -> 97K

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 107 (2.6.6) - Expand MonsterIndex up to 512
#define MAX_MONSTER 512

enum eMapNumber
{
	MAP_LORENCIA = 0,
	MAP_DUNGEON = 1,
	MAP_DEVIAS = 2,
	MAP_NORIA = 3,
	MAP_LOST_TOWER = 4,
	MAP_ARENA = 6,
	MAP_ATLANS = 7,
	MAP_TARKAN = 8,
	MAP_DEVIL_SQUARE1 = 9,
	MAP_ICARUS = 10,
	MAP_BLOOD_CASTLE1 = 11,
	MAP_BLOOD_CASTLE2 = 12,
	MAP_BLOOD_CASTLE3 = 13,
	MAP_BLOOD_CASTLE4 = 14,
	MAP_BLOOD_CASTLE5 = 15,
	MAP_BLOOD_CASTLE6 = 16,
	MAP_BLOOD_CASTLE7 = 17,
	MAP_CHAOS_CASTLE1 = 18,
	MAP_CHAOS_CASTLE2 = 19,
	MAP_CHAOS_CASTLE3 = 20,
	MAP_CHAOS_CASTLE4 = 21,
	MAP_CHAOS_CASTLE5 = 22,
};

struct ITEM
{
	/* +00 */	short Type;
	/* +02 */	//BYTEFILL
	/* +03 */	//BYTEFILL
	/* +04 */	int Level;
	/* +08 */	BYTE Part;
	/* +09 */	BYTE Class;
	/* +0A */	bool TwoHand;
	/* +0B */	//BYTEFILL
	/* +0C */	WORD DamageMin;
	/* +0E */	WORD DamageMax;
	/* +10 */	BYTE SuccessfulBlocking;
	/* +11 */	//BYTEFILL
	/* +12 */	WORD Defense;
	/* +14 */	BYTE MagicDefense;
	/* +15 */	BYTE MagicPower;
	/* +16 */	BYTE WeaponSpeed;
	/* +17 */	//BYTEFILL
	/* +18 */	WORD WalkSpeed;
	/* +1A */	signed char Durability;
	/* +1B */	BYTE Option1;
	/* +1C */	WORD RequireStrenght;
	/* +1E */	WORD RequireDexterity;
	/* +20 */	WORD RequireEnergy;
	/* +22 */	WORD RequireLevel;
	/* +24 */	BYTE SpecialNum;
	/* +25 */	//BYTEFILL
	/* +26 */	//BYTEFILL
	/* +27 */	//BYTEFILL
	/* +28 */	BYTE Special[8];
	/* +29 //	BYTE Special[1];
	/* +2A //	BYTE Special[2];
	/* +2B //	BYTE Special[3];
	/* +2C //	BYTE Special[4];
	/* +2D //	BYTE Special[5];
	/* +2E //	BYTE Special[6];
	/* +2F //	BYTE Special[7];
	/* +30 */	BYTE SpecialValue[8];
	/* +31 //	BYTE SpecialValue[1];
	/* +32 //	BYTE SpecialValue[2];
	/* +33 //	BYTE SpecialValue[3];
	/* +34 //	BYTE SpecialValue[4];
	/* +35 //	BYTE SpecialValue[5];
	/* +36 //	BYTE SpecialValue[6];
	/* +37 //	BYTE SpecialValue[7];
	/* +38 */	DWORD Key;
	/* +3C */	BYTE FILL1;
	/* +3D */	BYTE FILL2;
	/* +3E */	BYTE x;
	/* +3F */	BYTE y;
	/* +40 */	int Number;
	/* +44 */	BYTE Color;
	/* +45 */	BYTE byColorState;
	/* +46 */	BYTE FILL3;
	/* +47 */	BYTE FILL4;
	/* +48 */	int RefCount;
};

struct ITEM_ATTRIBUTE
{
	/* +00 */ char Name[30];
	/* +30 */ BYTE TwoHand;
	/* +31 */ BYTE Level;
	/* +32 */ BYTE Width;
	/* +33 */ BYTE Height;
	/* +34 */ BYTE DamageMin;
	/* +35 */ BYTE DamageMax;
	/* +36 */ BYTE DefenseRate;
	/* +37 */ BYTE Defense;
	/* +38 */ BYTE MagicDefense;
	/* +39 */ BYTE AttackSpeed;
	/* +40 */ BYTE WalkSpeed;
	/* +41 */ BYTE Durability;
	/* +42 */ BYTE MagicDurability;
	/* +43 */ BYTE MagicPower;
	/* +44 */ WORD RequireStrength;
	/* +46 */ WORD RequireAgility;
	/* +48 */ BYTE RequireEnergy;
	/* +49 */ BYTE RequireLevel;
	/* +50 */ BYTE Value;
	/* +50 */ //BYTEFILL
	/* +52 */ DWORD Money;
	/* +56 */ BYTE RequireDW;
	/* +57 */ BYTE RequireDK;
	/* +58 */ BYTE RequireELF;
	/* +59 */ BYTE RequireMG;
	/* +60 */ BYTE ResistanceIce;
	/* +61 */ BYTE ResistancePoison;
	/* +62 */ BYTE ResistanceLightning;
	/* +63 */ BYTE ResistanceFire;
};