// Update CMZ 17 (3.1.7) 28-09-26 - Custom Change Class Interface
#pragma once

#include "Offset.h"
#include "ProtocolDefines.h"

#pragma pack(push, 1)
struct PMSG_CUSTOM_CHANGE_CLASS_REQ
{
	PSBMSG_HEAD h;
	BYTE TargetClass;
};

struct PMSG_CUSTOM_CHANGE_CLASS_ANS
{
	PSBMSG_HEAD h;
	BYTE Result; // 0 = Success, 1 = Error
};
#pragma pack(pop)

struct CHANGE_CLASS_INFO
{
	int ClassCode;
	char Name[32];
	char Tag[8];
};

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.1.9) - Texture definitions
#define CUSTOM_CHANGE_CLASS_BG_TEXTURE   5000
#define CUSTOM_CHANGE_CLASS_FACE_SM      5001
#define CUSTOM_CHANGE_CLASS_FACE_BK      5002
#define CUSTOM_CHANGE_CLASS_FACE_ME      5003
#define CUSTOM_CHANGE_CLASS_FACE_MG      5004
#define CUSTOM_CHANGE_CLASS_FRAME        5005
#define CUSTOM_CHANGE_CLASS_BTN_NORMAL   5006
#define CUSTOM_CHANGE_CLASS_BTN_CLICK    5007

class CCustomChangeClass
{
public:
	CCustomChangeClass();
	virtual ~CCustomChangeClass();

	void Init();
	void LoadImages();
	void Toggle();
	void Open();
	void Close();
	bool IsActive() const { return this->m_Active; }

	void Render();
	void UpdateMouse();
	void GCChangeClassRecv(PMSG_CUSTOM_CHANGE_CLASS_ANS* lpMsg);

private:
	static void MyRenderWindows();
	static void MyUpdateWindowsMouse();

	bool IsWorkZone(float x, float y, float w, float h);
	void DrawTextCenter(int x, int y, int w, HFONT font, DWORD color, const char* text);
	void RenderCharacterButton();
	void RenderChangeClassWindow();
	void ConfirmChange();

private:
	bool m_Active;
	int m_SelectedClass;
	int m_CurrentFaceModel;
	DWORD m_LastConfirmTime;
};

extern CCustomChangeClass gCustomChangeClass;
