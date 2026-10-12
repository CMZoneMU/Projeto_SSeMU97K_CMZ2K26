#pragma once

// Window
#define m_Resolution *(int*)0x055C9E38
#define WindowWidth *(int*)0x0056156C
#define WindowHeight *(int*)0x00561570
#define g_fScreenRate_x *(float*)0x55C9B70
#define g_fScreenRate_y *(float*)0x55C9B74
#define m_hFontDC *(HDC*)0x055C9FEC
#define g_hWnd *(HWND*)0x055C9FFC
#define g_hDC *(HDC*)0x055CA004
#define g_hRC *(HGLRC*)0x055CA008
#define WndProc (WNDPROC)(0x4149D0) 

// State
#define SceneFlag *(int*)0x005615C0
#define	World *(int*)0x0055A7AC
#define MouseX *(int*)0x083A427C
#define MouseY *(int*)0x083A4278
#define MouseLButton *(bool*)0x083A42C4
#define MouseLButtonPush *(bool*)0x083A4124
#define MouseLButtonPop *(bool*)0x083A413C
#define MouseUpdateTime *(DWORD*)0x07E11D28
#define MouseUpdateTimeMax *(DWORD*)0x00559BEC
#define MouseRButton *(bool*)0x083A42AC
#define MouseRButtonPush *(bool*)0x083A42D0
#define MouseRButtonPop *(bool*)0x083A4290
#define ItemKey *(int*)0x07E109C8
#define KeyState 0x07E118EC

#define pGetTextLine(x)			((char*)(0x007D29D24+(0x12C*x)))
#define pRenderTipText			((void(*)(int,int,char*))0x0047F7F0)
#define pMouseOnZone			((int(__cdecl*)(int,int,int,int,int))0x0040C490)

// Interface
#define CreateGuildMark ((void(__cdecl*)(int nMarkIndex, bool blend)) 0x004F0100)
#define RenderBoolean ((void(__cdecl*)(int x, int y, DWORD c)) 0x00480E00)
#define LoadImageJPG ((int(__cdecl*)(char *Folder, int ModelID, GLint a3, GLint a4, int a5, int a6)) 0x00529740)
#define LoadImageTGA ((int(__cdecl*)(char *Folder, int ModelID, GLint a3, GLint a4, int a5, int a6)) 0x00529BD0)
#define MouseOnWindow *(bool*)0x07D78094
#define InventoryOpened *(BYTE*)0x07EAA117
#define GuildOpened *(BYTE*)0x07EAA114
#define CharacterOpened *(BYTE*)0x07EAA116
#define PartyOpened *(BYTE*)0x07EAA115
#define ShopOpened *(BYTE*)0x07EAA118
#define WarehouseOpened *(BYTE*)0x07EAA119
#define ChaosMixOpened *(BYTE*)0x07EAA11A
#define TradeOpened *(BYTE*)0x07EAA11B
#define EventWindowOpened *(BYTE*)0x07EAA11C
#define GuildCreatorOpened *(BYTE*)0x07EAA124
#define ErrorMessage *(BYTE*)0x083A7C24

// Drawing
#define RenderBitmap ((void(__cdecl*)(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)) 0x005125A0)
#define RenderNumber2D ((void(*)(float x, float y, int Num, float Width, float Height))0x005122F0)
#define RenderColor ((void(__cdecl*)(float x, float y, float width, float height)) 0x005124C0)
#define CreatePoint ((void(_cdecl*)(float Position[3], int Value, float Color[3], float scale, bool bMove))0x004792C0)

// Font
#define FontHeight *(int*)0x07D78080
#define g_hFont *(HFONT*)0x055CA00C
#define g_hFontBold *(HFONT*)0x055CA010
#define g_hFontBig *(HFONT*)0x055CA014
#define OpenFont ((void(_cdecl*)())0x0050F690)

// Text
#define SetTextColor *(DWORD*)0x00559C78
#define SetBackgroundTextColor *(DWORD*)0x00559C80
#define RenderText ((int(__cdecl*)(int iPos_x, int iPos_y, char *pszText, int iBoxWidth, int iSort, SIZE *lpTextSize)) 0x0047F650)
#define GetTextLine(x) (char*)(0x07D29D24 + (300 * x))
#define CreateNotice ((void(__cdecl*)(char *Text, int Color)) 0x0047FAE0)
#define DrawInterfaceText ((void(*)(int,int,char*))0x00514270)

// Input
#define InputEnable *(bool*)0x00559C84

// Character
#define MAIN_CHARACTER_STRUCT 0x07CF1FF4
#define Hero *(DWORD*)0x07ABF5D8
#define PartyNumber *(int*)0x07EAA0E0
#define CharactersClient *(DWORD*)0x07ABF5D0
#define Projection ((void(__cdecl*)(float *Position, int *sx, int *sy)) 0x005113F0)
#define Action ((void(__cdecl*)(DWORD c, DWORD o)) 0x0048D640)

// Connection
#define MAIN_PACKET_SERIAL 0x05826CEB
#define MAIN_STRUCT_SERIAL 0x07CF1FFC
#define MAIN_HASH_CLASS 0x055C9BC8
#define MAIN_CONNECTION_STATUS 0x05826CF0
#define PACKET_DECRYPT ((void(__thiscall*)(void*,int))0x00422DF0)((void*)0x055C9BC8,MAIN_PACKET_SERIAL)
#define PACKET_ENCRYPT ((void(__thiscall*)(void*,int))0x00404040)((void*)0x055C9BC8,MAIN_PACKET_SERIAL)
#define STRUCT_DECRYPT ((void(__thiscall*)(void*,void*))0x00423040)((void*)MAIN_HASH_CLASS,*(void**)MAIN_STRUCT_SERIAL)
#define STRUCT_ENCRYPT ((void(__thiscall*)(void*,void*))0x0043D1D0)((void*)MAIN_HASH_CLASS,*(void**)MAIN_STRUCT_SERIAL)
#define pSocket	*(SOCKET*)(0x055CA168)
#define ProtocolCore ((BOOL(*)(BYTE*))0x004389A0)
#define SendChat ((void(__cdecl*)(const char* Text)) 0x004C1B90)

// OpenGL
#define EnableAlphaTest ((void(__cdecl*)(BYTE)) 0x00511680)
#define EnableAlphaBlend ((void(__cdecl*)()) 0x00511710)
#define DisableAlphaBlend ((void(__cdecl*)()) 0x00511600)
#define DisableTexture ((void(__cdecl*)(bool AlphaTest)) 0x00511590)
#define ConvertX ((float(__cdecl*)(float x)) 0x00511950)
#define ConvertY ((float(__cdecl*)(float y)) 0x00511980)
#define RenderColor ((void(__cdecl*)(float x, float y, float width, float height)) 0x005124C0)

// Sound
#define PlayBuffer ((int(__cdecl*)(int Buffer, DWORD Object, BOOL bLooped))0x00404BC0)
#define CreateStaticBuffer ((HRESULT(__cdecl*)(int Buffer, const char* strFileName, int MaxChannel, bool Enable))0x00404650)
#define FillBuffer ((HRESULT(__cdecl*)(int Buffer, int MaxChannel, bool Enable))0x00404A00)
#define LoadWaveFile ((HRESULT(__cdecl*)(int Buffer, TCHAR* strFileName, int MaxChannel, bool Enable))0x00404A10)
#define g_EnableSound *(bool*)0x00590AC8
#define g_Enable3DSound *(bool*)0x00590AC9
#define SoundLoadCount *(int*)0x00590ACC
#define BufferChannel (&*(int*)0x00583DAC)// int BufferChannel[420];
#define MaxBufferChannel (&*(int*)0x0058E1C4) // int MaxBufferChannel[420];
#define Enable3DSound (&*(bool*)0x00590924) // bool Enable3DSound[420];
#define BufferName 0x00585E7C // char BufferName[420][64];
#define g_lpDSBuffer ((LPDIRECTSOUNDBUFFER(*)[4])0x0058C780) // LPDIRECTSOUNDBUFFER g_lpDSBuffer[420][4]
#define m_SoundOnOff *(int*)0x055C9FE8
#define m_MusicOnOff *(int*)0x055C9E3C
#define PlayMp3 ((void(__cdecl*)(char* Name, BOOL bEnforce))0x00412890)
#define StopMp3 ((void(__cdecl*)(char* Name, BOOL bEnforce))0x004127F0)
#define Mp3FileName (char*)0x055C9D04
#define m_bWhisperSound *(bool*)0x07E11D80
#define Destroy *(bool*)0x055CA018
#define m_bAutoAttack *(bool*)0x00559C5C

#define AccessModel ((void(*)(int,char*,char*,int))0x005060B0)
#define OpenTexture ((void(*)(int,char*,int,bool))0x00505C80)

#define EventChipDialogEnable	*(int*)0x07EAA128
#define GetInventoryItemCount	((int(__cdecl*)(int,int))0x00482FF0)

#define pSetPlayerStop			((void(*)(DWORD))0x004430C0)

#define Attacking				*(int*)0x0559C58
#define SelectedCharacter		*(DWORD*)(0x00559C50)
#define CheckAttack				((bool(_cdecl*)()) 0x00483160)
#define TargetX					*(int*)0x07E016C0
#define TargetY					*(int*)0x07E016C4
#define CurrentSkill			*(int*)0x05826D10
#define MovementSkillTarget		*(int*)0x07D780A0
#define FindCharacterIndex		((int(_cdecl*)(int Key)) 0x0045AC80)

#define g_iLimitAttackTime *(DWORD*)0x00559858
#define GlobalText ((char(*)[64])0x05626F78)

#define OpenTextData ((void(_cdecl*)()) 0x00510FE0)
#define OpenDialogFile ((void(_cdecl*)(char* FileName)) 0x0047B020)
#define Models *(DWORD*)0x05828D58
#define TransformPosition ((void(__thiscall*)(DWORD This, float* Matrix, float Position[3], float WorldPosition[3], bool Translate)) 0x004409A0)

struct CHARACTER_ATTRIBUTE
{
	char Name[11];
	BYTE Class;
	BYTE Skin;
	WORD Level;
	DWORD Experience;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	WORD Life;
	WORD Mana;
	WORD LifeMax;
	WORD ManaMax;
	WORD SkillMana;
	WORD SkillManaMax;
	BYTE Ability;
	WORD AbilityTime[2];
	short AddPoint;
	short MaxAddPoint;
	DWORD NextExperince;
	WORD AttackSpeed;
	WORD AttackRating;
	WORD AttackDamageMinRight;
	WORD AttackDamageMaxRight;
	WORD AttackDamageMinLeft;
	WORD AttackDamageMaxLeft;
	WORD MagicSpeed;
	WORD MagicDamageMin;
	WORD MagicDamageMax;
	WORD CriticalDamage;
	WORD SuccessfulBlocking;
	WORD Defense;
	WORD MagicDefense;
	WORD WalkSpeed;
	WORD LevelUpPoint;;
};

// Update 21 Kayito -> 97K - Fix Texturas (Zeus)
#define OpenJPG ((bool(_cdecl*)(const char *szFileName, GLuint uiTextureIndex, GLuint uiFilter, GLuint uiWrapMode, char *bFullPath, bool bCheck)) 0x00529740)
#define OpenTGA ((bool(_cdecl*)(const char *szFileName, GLuint uiTextureIndex, GLuint uiFilter, GLuint uiWrapMode, char *bFullPath, bool bCheck)) 0x00529BD0)
#define UnloadImage ((void(_cdecl*)(GLuint uiTextureIndex)) 0x0052A050)
#define TextureBegin *(int*)0x083A4104
#define TextureCurrent *(int*)0x083A4108
#define m_dwUsedTextureMemory *(DWORD*)0x083BB9D0
#define OpenPlayerTextures ((void(_cdecl*)())0x00507610)
#define OpenItems ((void(_cdecl*)()) 0x005079D0)
#define OpenItemTextures ((void(_cdecl*)())0x00508D10)
#define FindTextureByName ((short(_cdecl*)(char* Name, BITMAP_t** Texture)) 0x00505BF0)
#define PartObjectColor ((void(_cdecl*)(int Type, float Alpha, float Bright, float Light[3], bool ExtraMon))0x00503CF0)
// Fim Update 21 Kayito -> 97K

// FPS Monitor (97.11 WebZen)
#define FPS *(float*)0x05826DF8
