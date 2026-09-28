// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ (2.5.9) - Custom Change Class Interface
#include "stdafx.h"
#include "CustomChangeClass.h"
#include "Protect.h"
#include "Protocol.h"
#include "Util.h"
#include <gl/GL.h>

#ifndef RGBA_TEXT
#define RGBA_TEXT(r, g, b, a) (((DWORD)(a) << 24) | ((DWORD)(b) << 16) | ((DWORD)(g) << 8) | ((DWORD)(r)))
#endif

CCustomChangeClass gCustomChangeClass;

static const CHANGE_CLASS_INFO s_Classes[] =
{
	{ 17, "Blade Knight", "BK" },
	{ 1,  "Soul Master", "SM" },
	{ 33, "Muse Elf", "ME" },
	{ 48, "Magic Gladiator", "MG" }
};

static const int s_TotalClasses = 4;

CCustomChangeClass::CCustomChangeClass()
{
	this->m_Active = false;
	this->m_SelectedClass = 0; // Default to first class (Blade Knight)
}

CCustomChangeClass::~CCustomChangeClass()
{
}

void CCustomChangeClass::Init()
{
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ (2.5.9) - Hook Windows mouse update loop
	SetCompleteHook(0xE8, 0x005254B2, &CCustomChangeClass::MyUpdateWindowsMouse);

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ (2.5.9) - Hook Windows render loop
	SetCompleteHook(0xE8, 0x00525CEC, &CCustomChangeClass::MyRenderWindows);
}

void CCustomChangeClass::Toggle()
{
	if (SceneFlag != 5)
	{
		this->m_Active = false;
		return;
	}

	this->m_Active = !this->m_Active;
}

void CCustomChangeClass::Open()
{
	if (SceneFlag != 5)
	{
		return;
	}

	this->m_Active = true;
}

void CCustomChangeClass::Close()
{
	this->m_Active = false;
}

void CCustomChangeClass::MyUpdateWindowsMouse()
{
	// Call original UpdateWindowsMouse
	((void(__cdecl*)()) 0x004ECB00)();

	gCustomChangeClass.UpdateMouse();
}

void CCustomChangeClass::MyRenderWindows()
{
	// Call original RenderWindows
	((void(__cdecl*)()) 0x004C3530)();

	gCustomChangeClass.Render();
}

bool CCustomChangeClass::IsWorkZone(float x, float y, float w, float h)
{
	return (MouseX >= (int)x && MouseX <= (int)(x + w) && MouseY >= (int)y && MouseY <= (int)(y + h));
}

void CCustomChangeClass::DrawTextCenter(int x, int y, int w, HFONT font, DWORD color, const char* text)
{
	SelectObject(m_hFontDC, font);
	SetBackgroundTextColor = 0;
	SetTextColor = color;

	int centerX = x + (w / 2);
	DrawInterfaceText(centerX, y, (char*)text);
}

void CCustomChangeClass::Render()
{
	if (SceneFlag != 5)
	{
		return;
	}

	// 1. Render button in Character Status window if opened
	this->RenderCharacterButton();

	// 2. Render Change Class popup window if active
	if (this->m_Active)
	{
		this->RenderChangeClassWindow();
	}
}

void CCustomChangeClass::RenderCharacterButton()
{
	if (CharacterOpened != 1)
	{
		return;
	}

	// In 97K 640x480 coordinate space, Character window is at X=450, Y=0 (width 190, height 433)
	// Close button [X] is at X=468, Y=390. Mirrored slot on bottom right is at X=578, Y=390.
	float btX = 578.0f;
	float btY = 390.0f;
	float btW = 24.0f;
	float btH = 24.0f;

	bool bHover = this->IsWorkZone(btX, btY, btW, btH);

	// Disable textures for solid OpenGL color drawing
	glDisable(GL_TEXTURE_2D);
	EnableAlphaTest(true);

	if (bHover)
	{
		// Golden glowing border
		glColor4f(0.85f, 0.70f, 0.15f, 0.90f);
		RenderColor(btX - 1.0f, btY - 1.0f, btW + 2.0f, btH + 2.0f);

		// Button background
		glColor4f(0.20f, 0.20f, 0.25f, 0.95f);
		RenderColor(btX, btY, btW, btH);
	}
	else
	{
		// Subtle dark golden border
		glColor4f(0.50f, 0.40f, 0.15f, 0.75f);
		RenderColor(btX - 1.0f, btY - 1.0f, btW + 2.0f, btH + 2.0f);

		// Button background
		glColor4f(0.10f, 0.10f, 0.12f, 0.90f);
		RenderColor(btX, btY, btW, btH);
	}

	// Reset OpenGL color and re-enable textures for text/tooltips
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	DisableAlphaBlend();

	// Show tooltip on hover
	if (bHover)
	{
		pRenderTipText((int)btX - 25, (int)btY - 13, "Trocar classe");
	}

	// Draw button text transparently without background box
	DWORD dwOldColor = SetTextColor;
	DWORD dwOldBgColor = SetBackgroundTextColor;
	HFONT hOldFont = (HFONT)SelectObject(m_hFontDC, g_hFontBold);

	this->DrawTextCenter((int)btX, (int)btY + 6, (int)btW, g_hFontBold, bHover ? RGBA_TEXT(255, 255, 255, 255) : RGBA_TEXT(255, 204, 0, 255), "TC");

	SelectObject(m_hFontDC, hOldFont);
	SetTextColor = dwOldColor;
	SetBackgroundTextColor = dwOldBgColor;

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	DisableAlphaBlend();
}

void CCustomChangeClass::RenderChangeClassWindow()
{
	float Largura = 230.0f;
	float Altura = 270.0f;
	float JanelaY = 85.0f;
	float JanelaX;

	// In 640x480 space:
	// If Character window is open (X=450), position Change Class window to its left (X=210) with 10px margin
	// Otherwise, center horizontally on 640 screen space: (640 - 230) / 2 = 205
	if (CharacterOpened == 1)
	{
		JanelaX = 450.0f - Largura - 10.0f;
	}
	else
	{
		JanelaX = (640.0f - Largura) / 2.0f;
	}

	// 1. Disable textures so quads are clean solid colored shapes
	glDisable(GL_TEXTURE_2D);
	EnableAlphaTest(true);

	// Outer gold border
	glColor4f(0.85f, 0.70f, 0.15f, 0.70f);
	RenderColor(JanelaX - 1.0f, JanelaY - 1.0f, Largura + 2.0f, Altura + 2.0f);

	// Main window background (dark semitransparent stone)
	glColor4f(0.08f, 0.08f, 0.10f, 0.94f);
	RenderColor(JanelaX, JanelaY, Largura, Altura);

	// Header title bar
	glColor4f(0.15f, 0.15f, 0.20f, 0.95f);
	RenderColor(JanelaX, JanelaY, Largura, 26.0f);

	glColor4f(0.85f, 0.70f, 0.15f, 0.90f);
	RenderColor(JanelaX, JanelaY + 25.0f, Largura, 1.0f);

	// Close [X] button at top-right
	float closeX = JanelaX + Largura - 22.0f;
	float closeY = JanelaY + 4.0f;
	float closeW = 18.0f;
	float closeH = 18.0f;
	bool bCloseHover = this->IsWorkZone(closeX, closeY, closeW, closeH);

	glColor4f(bCloseHover ? 0.80f : 0.35f, 0.15f, 0.15f, 0.90f);
	RenderColor(closeX, closeY, closeW, closeH);

	// 2. Center Character Head Preview Box (Simulates NEW CHARACTER screen avatar box)
	float boxW = 84.0f;
	float boxH = 84.0f;
	float boxX = JanelaX + (Largura - boxW) / 2.0f;
	float boxY = JanelaY + 36.0f;

	// Gold outer frame for avatar
	glColor4f(0.85f, 0.70f, 0.15f, 0.95f);
	RenderColor(boxX - 2.0f, boxY - 2.0f, boxW + 4.0f, boxH + 4.0f);

	// Pure deep black background for character head
	glColor4f(0.02f, 0.02f, 0.03f, 0.98f);
	RenderColor(boxX, boxY, boxW, boxH);

	// Subtle inner border
	glColor4f(0.30f, 0.30f, 0.38f, 0.50f);
	RenderColor(boxX + 2.0f, boxY + 2.0f, boxW - 4.0f, boxH - 4.0f);

	// Re-darken center
	glColor4f(0.02f, 0.02f, 0.03f, 0.98f);
	RenderColor(boxX + 3.0f, boxY + 3.0f, boxW - 6.0f, boxH - 6.0f);

	// 3. Navigation Arrow Buttons [<] and [>] on sides of the avatar box
	float arrowW = 26.0f;
	float arrowH = 34.0f;
	float arrowLeftX = JanelaX + 22.0f;
	float arrowY = boxY + (boxH - arrowH) / 2.0f;
	float arrowRightX = JanelaX + Largura - 22.0f - arrowW;

	bool bLeftHover = this->IsWorkZone(arrowLeftX, arrowY, arrowW, arrowH);
	bool bRightHover = this->IsWorkZone(arrowRightX, arrowY, arrowW, arrowH);

	// Left Arrow Button [<]
	glColor4f(bLeftHover ? 0.85f : 0.40f, bLeftHover ? 0.70f : 0.40f, bLeftHover ? 0.15f : 0.45f, 0.90f);
	RenderColor(arrowLeftX - 1.0f, arrowY - 1.0f, arrowW + 2.0f, arrowH + 2.0f);

	glColor4f(bLeftHover ? 0.18f : 0.12f, bLeftHover ? 0.40f : 0.14f, bLeftHover ? 0.22f : 0.18f, 0.90f);
	RenderColor(arrowLeftX, arrowY, arrowW, arrowH);

	// Right Arrow Button [>]
	glColor4f(bRightHover ? 0.85f : 0.40f, bRightHover ? 0.70f : 0.40f, bRightHover ? 0.15f : 0.45f, 0.90f);
	RenderColor(arrowRightX - 1.0f, arrowY - 1.0f, arrowW + 2.0f, arrowH + 2.0f);

	glColor4f(bRightHover ? 0.18f : 0.12f, bRightHover ? 0.40f : 0.14f, bRightHover ? 0.22f : 0.18f, 0.90f);
	RenderColor(arrowRightX, arrowY, arrowW, arrowH);

	// 4. Class Name Display Bar below the avatar box
	float nameW = Largura - 44.0f;
	float nameH = 22.0f;
	float nameX = JanelaX + 22.0f;
	float nameY = boxY + boxH + 8.0f;

	// Gold frame for class name bar
	glColor4f(0.85f, 0.70f, 0.15f, 0.85f);
	RenderColor(nameX - 1.0f, nameY - 1.0f, nameW + 2.0f, nameH + 2.0f);

	// Dark fill
	glColor4f(0.10f, 0.12f, 0.16f, 0.95f);
	RenderColor(nameX, nameY, nameW, nameH);

	// 5. Action buttons at the bottom (CONFIRMAR and CANCELAR)
	float confX = JanelaX + 15.0f;
	float confY = JanelaY + 230.0f;
	float confW = 95.0f;
	float confH = 26.0f;
	bool bConfHover = this->IsWorkZone(confX, confY, confW, confH);

	float cancX = JanelaX + Largura - 110.0f;
	float cancY = JanelaY + 230.0f;
	float cancW = 95.0f;
	float cancH = 26.0f;
	bool bCancHover = this->IsWorkZone(cancX, cancY, cancW, cancH);

	// Confirm button quad
	glColor4f(bConfHover ? 0.35f : 0.20f, bConfHover ? 0.95f : 0.70f, bConfHover ? 0.45f : 0.30f, 0.95f);
	RenderColor(confX - 1.0f, confY - 1.0f, confW + 2.0f, confH + 2.0f);

	glColor4f(bConfHover ? 0.18f : 0.10f, bConfHover ? 0.55f : 0.35f, bConfHover ? 0.22f : 0.15f, 0.92f);
	RenderColor(confX, confY, confW, confH);

	// Cancel button quad
	glColor4f(bCancHover ? 0.95f : 0.70f, bCancHover ? 0.35f : 0.20f, bCancHover ? 0.35f : 0.20f, 0.95f);
	RenderColor(cancX - 1.0f, cancY - 1.0f, cancW + 2.0f, cancH + 2.0f);

	glColor4f(bCancHover ? 0.55f : 0.35f, bCancHover ? 0.15f : 0.10f, bCancHover ? 0.15f : 0.10f, 0.92f);
	RenderColor(cancX, cancY, cancW, cancH);

	// 6. Re-enable textures and reset OpenGL states before drawing texts
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	EnableAlphaTest(false);
	EnableAlphaTest(true);

	// Backup font and text colors
	HFONT hOldFont = (HFONT)SelectObject(m_hFontDC, g_hFont);
	DWORD dwOldColor = SetTextColor;
	DWORD dwOldBgColor = SetBackgroundTextColor;

	// Title text (Centered)
	this->DrawTextCenter((int)JanelaX, (int)JanelaY + 6, (int)Largura, g_hFontBig, RGBA_TEXT(255, 204, 0, 255), "TROCAR DE CLASSE");

	// Close [X] text (Centered)
	this->DrawTextCenter((int)closeX, (int)closeY + 3, (int)closeW, g_hFontBold, RGBA_TEXT(255, 255, 255, 255), "X");

	// Arrow symbols [<] and [>]
	this->DrawTextCenter((int)arrowLeftX, (int)arrowY + 10, (int)arrowW, g_hFontBig, bLeftHover ? RGBA_TEXT(255, 255, 255, 255) : RGBA_TEXT(255, 204, 0, 255), "<");
	this->DrawTextCenter((int)arrowRightX, (int)arrowY + 10, (int)arrowW, g_hFontBig, bRightHover ? RGBA_TEXT(255, 255, 255, 255) : RGBA_TEXT(255, 204, 0, 255), ">");

	// Inside avatar box: draw class tag and preview indicator
	char szTag[16];
	wsprintf(szTag, "[ %s ]", s_Classes[this->m_SelectedClass].Tag);
	this->DrawTextCenter((int)boxX, (int)boxY + 22, (int)boxW, g_hFontBig, RGBA_TEXT(255, 204, 0, 255), szTag);
	this->DrawTextCenter((int)boxX, (int)boxY + 50, (int)boxW, g_hFont, RGBA_TEXT(150, 150, 170, 255), "Avatar Face");

	// Class Name inside the display bar
	this->DrawTextCenter((int)nameX, (int)nameY + 5, (int)nameW, g_hFontBold, RGBA_TEXT(255, 215, 0, 255), s_Classes[this->m_SelectedClass].Name);

	// Informational Requirements Text (Centered horizontally in window)
	float infoY = nameY + nameH + 12.0f;
	this->DrawTextCenter((int)JanelaX, (int)infoY, (int)Largura, g_hFont, RGBA_TEXT(225, 225, 225, 255), "A troca de classe e apenas para VIP");
	this->DrawTextCenter((int)JanelaX, (int)infoY + 16, (int)Largura, g_hFont, RGBA_TEXT(190, 190, 190, 255), "VIP possui vantagens e mantem o servidor");
	this->DrawTextCenter((int)JanelaX, (int)infoY + 32, (int)Largura, g_hFont, RGBA_TEXT(255, 140, 100, 255), "Desequipe todos os itens antes de trocar");

	// Action button texts (Centered inside button borders)
	this->DrawTextCenter((int)confX, (int)confY + 7, (int)confW, g_hFontBold, bConfHover ? RGBA_TEXT(255, 255, 255, 255) : RGBA_TEXT(220, 255, 220, 255), "CONFIRMAR");
	this->DrawTextCenter((int)cancX, (int)cancY + 7, (int)cancW, g_hFontBold, bCancHover ? RGBA_TEXT(255, 255, 255, 255) : RGBA_TEXT(200, 255, 200, 255), "CANCELAR");

	// Restore font and text colors
	SelectObject(m_hFontDC, hOldFont);
	SetTextColor = dwOldColor;
	SetBackgroundTextColor = dwOldBgColor;

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	DisableAlphaBlend();
}

void CCustomChangeClass::UpdateMouse()
{
	if (SceneFlag != 5)
	{
		return;
	}

	// 1. Mouse check for button in Character Status window (X=578, Y=390, 24x24)
	if (CharacterOpened == 1)
	{
		float btX = 578.0f;
		float btY = 390.0f;
		float btW = 24.0f;
		float btH = 24.0f;

		if (this->IsWorkZone(btX, btY, btW, btH))
		{
			MouseOnWindow = true;

			if (MouseLButtonPush)
			{
				MouseLButtonPush = false;
				PlayBuffer(25, 0, 0);
				this->Toggle();
				return;
			}
		}
	}

	// 2. Mouse check for main Change Class window
	if (!this->m_Active)
	{
		return;
	}

	float Largura = 230.0f;
	float Altura = 270.0f;
	float JanelaY = 85.0f;
	float JanelaX;

	if (CharacterOpened == 1)
	{
		JanelaX = 450.0f - Largura - 10.0f;
	}
	else
	{
		JanelaX = (640.0f - Largura) / 2.0f;
	}

	if (this->IsWorkZone(JanelaX, JanelaY, Largura, Altura))
	{
		MouseOnWindow = true;

		// Close button [X]
		float closeX = JanelaX + Largura - 22.0f;
		float closeY = JanelaY + 4.0f;
		float closeW = 18.0f;
		float closeH = 18.0f;

		if (this->IsWorkZone(closeX, closeY, closeW, closeH))
		{
			if (MouseLButtonPush)
			{
				MouseLButtonPush = false;
				PlayBuffer(25, 0, 0);
				this->Close();
				return;
			}
		}

		// Navigation Arrows [<] and [>]
		float boxH = 84.0f;
		float boxY = JanelaY + 36.0f;
		float arrowW = 26.0f;
		float arrowH = 34.0f;
		float arrowLeftX = JanelaX + 22.0f;
		float arrowY = boxY + (boxH - arrowH) / 2.0f;
		float arrowRightX = JanelaX + Largura - 22.0f - arrowW;

		// Left Arrow [<] click
		if (this->IsWorkZone(arrowLeftX, arrowY, arrowW, arrowH))
		{
			if (MouseLButtonPush)
			{
				MouseLButtonPush = false;
				PlayBuffer(25, 0, 0);
				this->m_SelectedClass = (this->m_SelectedClass - 1 + s_TotalClasses) % s_TotalClasses;
				return;
			}
		}

		// Right Arrow [>] click
		if (this->IsWorkZone(arrowRightX, arrowY, arrowW, arrowH))
		{
			if (MouseLButtonPush)
			{
				MouseLButtonPush = false;
				PlayBuffer(25, 0, 0);
				this->m_SelectedClass = (this->m_SelectedClass + 1) % s_TotalClasses;
				return;
			}
		}

		// Confirm button
		float confX = JanelaX + 15.0f;
		float confY = JanelaY + 230.0f;
		float confW = 95.0f;
		float confH = 26.0f;

		if (this->IsWorkZone(confX, confY, confW, confH))
		{
			if (MouseLButtonPush)
			{
				MouseLButtonPush = false;
				PlayBuffer(25, 0, 0);
				this->ConfirmChange();
				return;
			}
		}

		// Cancel button
		float cancX = JanelaX + Largura - 110.0f;
		float cancY = JanelaY + 230.0f;
		float cancW = 95.0f;
		float cancH = 26.0f;

		if (this->IsWorkZone(cancX, cancY, cancW, cancH))
		{
			if (MouseLButtonPush)
			{
				MouseLButtonPush = false;
				PlayBuffer(25, 0, 0);
				this->Close();
				return;
			}
		}
	}
}

void CCustomChangeClass::ConfirmChange()
{
	if (this->m_SelectedClass < 0 || this->m_SelectedClass >= s_TotalClasses)
	{
		return;
	}

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ (2.5.9) - Send class change request packet
	PMSG_CUSTOM_CHANGE_CLASS_REQ pMsg;
	pMsg.h.set(0xF3, 0xE5, sizeof(pMsg));
	pMsg.TargetClass = (BYTE)s_Classes[this->m_SelectedClass].ClassCode;
	DataSend((BYTE*)&pMsg, pMsg.h.size);

	this->Close();
}

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update CMZ (2.5.9) - Receive class change response packet
void CCustomChangeClass::GCChangeClassRecv(PMSG_CUSTOM_CHANGE_CLASS_ANS* lpMsg)
{
	if (lpMsg->Result == 0)
	{
		this->Close();
	}
}

