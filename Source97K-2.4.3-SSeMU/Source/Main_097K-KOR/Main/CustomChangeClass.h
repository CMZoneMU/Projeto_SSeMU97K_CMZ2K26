// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ (2.5.9) - Custom Change Class Interface
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

class CCustomChangeClass
{
public:
	CCustomChangeClass();
	virtual ~CCustomChangeClass();

	void Init();
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
};

extern CCustomChangeClass gCustomChangeClass;
