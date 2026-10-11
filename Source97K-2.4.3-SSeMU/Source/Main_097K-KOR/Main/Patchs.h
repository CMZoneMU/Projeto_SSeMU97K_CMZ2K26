#pragma once

void InitPatchs();

void CalcFPS();

DWORD WINAPI FrameWait(DWORD dwStartTime);
void HookFrameWait();

void ReduceRam(LPVOID lpThreadParameter);

void FixShopNpcClose();

void IgnoreRandomStuck(DWORD c, DWORD Damage);

void FixChasingAttackMovement();

void DecBMD();

int OpenTerrainMapping(char* FileName);

int OpenTerrainAttribute(char* FileName);

int OpenObjectsEnc(char* FileName);

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.4) - Render right dragon (Menu04)
void __cdecl RenderRightDragon(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

extern char WindowName[128];
extern int g_IncreaseFPSSwitch;