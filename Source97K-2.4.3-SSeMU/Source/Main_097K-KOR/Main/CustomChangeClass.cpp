#include "stdafx.h"
#include "CustomChangeClass.h"
#include "Protect.h"
#include "Protocol.h"
#include "Util.h"
#include "LoadModels.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "Opengl32.lib")
#pragma comment(lib, "Glu32.lib")

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
	this->m_CurrentFaceModel = -1;
	this->m_LastConfirmTime = 0;
}

CCustomChangeClass::~CCustomChangeClass()
{
}

void CCustomChangeClass::Init()
{
	// Update CMZ 17 (3.1.7) 28-09-26 - Hook Windows mouse update loop
	SetCompleteHook(0xE8, 0x005254B2, &CCustomChangeClass::MyUpdateWindowsMouse);

	// Update CMZ 17 (3.1.7) 28-09-26 - Hook Windows render loop
	SetCompleteHook(0xE8, 0x00525CEC, &CCustomChangeClass::MyRenderWindows);
}

void CCustomChangeClass::LoadImages()
{
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.1.9) - Load background window texture
	OpenJPG("Interface\\back_middle.jpg", CUSTOM_CHANGE_CLASS_BG_TEXTURE, GL_NEAREST, GL_CLAMP, NULL, true);

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.2) - Load class avatar face textures (OZT with native alpha)
	if (!OpenTGA("Custom\\Interface\\CustomClass\\face_sm.tga", CUSTOM_CHANGE_CLASS_FACE_SM, GL_NEAREST, GL_CLAMP, NULL, false))
	{
		OpenTGA("Custom\\Interface\\face_sm.tga", CUSTOM_CHANGE_CLASS_FACE_SM, GL_NEAREST, GL_CLAMP, NULL, true);
	}

	if (!OpenTGA("Custom\\Interface\\CustomClass\\face_bk.tga", CUSTOM_CHANGE_CLASS_FACE_BK, GL_NEAREST, GL_CLAMP, NULL, false))
	{
		OpenTGA("Custom\\Interface\\face_bk.tga", CUSTOM_CHANGE_CLASS_FACE_BK, GL_NEAREST, GL_CLAMP, NULL, true);
	}

	if (!OpenTGA("Custom\\Interface\\CustomClass\\face_me.tga", CUSTOM_CHANGE_CLASS_FACE_ME, GL_NEAREST, GL_CLAMP, NULL, false))
	{
		OpenTGA("Custom\\Interface\\face_me.tga", CUSTOM_CHANGE_CLASS_FACE_ME, GL_NEAREST, GL_CLAMP, NULL, true);
	}

	if (!OpenTGA("Custom\\Interface\\CustomClass\\face_mg.tga", CUSTOM_CHANGE_CLASS_FACE_MG, GL_NEAREST, GL_CLAMP, NULL, false))
	{
		OpenTGA("Custom\\Interface\\face_mg.tga", CUSTOM_CHANGE_CLASS_FACE_MG, GL_NEAREST, GL_CLAMP, NULL, true);
	}

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.1) - Load CustomClass_Frame texture (OZT with native alpha or fallback)
	if (!OpenTGA("Custom\\Interface\\CustomClass_Frame.tga", CUSTOM_CHANGE_CLASS_FRAME, GL_NEAREST, GL_CLAMP, NULL, false))
	{
		if (!OpenTGA("Custom\\Interface\\CustomClass\\CustomClass_Frame.tga", CUSTOM_CHANGE_CLASS_FRAME, GL_NEAREST, GL_CLAMP, NULL, false))
		{
			if (!OpenJPG("Custom\\Interface\\CustomClass_Frame.jpg", CUSTOM_CHANGE_CLASS_FRAME, GL_NEAREST, GL_CLAMP, NULL, false))
			{
				OpenJPG("Custom\\Interface\\CustomClass\\CustomClass_Frame.jpg", CUSTOM_CHANGE_CLASS_FRAME, GL_NEAREST, GL_CLAMP, NULL, true);
			}
		}
	}

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.3) - Load TC button textures from Custom\Interface\CustomClass
	if (!OpenJPG("Custom\\Interface\\CustomClass\\Mix_button1.jpg", CUSTOM_CHANGE_CLASS_BTN_NORMAL, GL_NEAREST, GL_CLAMP, NULL, false))
	{
		OpenJPG("Interface\\Mix_button1.jpg", CUSTOM_CHANGE_CLASS_BTN_NORMAL, GL_NEAREST, GL_CLAMP, NULL, true);
	}

	if (!OpenJPG("Custom\\Interface\\CustomClass\\Mix_button2.jpg", CUSTOM_CHANGE_CLASS_BTN_CLICK, GL_NEAREST, GL_CLAMP, NULL, false))
	{
		OpenJPG("Interface\\Mix_button2.jpg", CUSTOM_CHANGE_CLASS_BTN_CLICK, GL_NEAREST, GL_CLAMP, NULL, true);
	}
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

	this->m_CurrentFaceModel = -1;
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
	// Close button [X] is at X=475, Y=395 (24x24). Mirrored button on bottom right is at X=578, Y=395.
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.5) - Align button vertically with native close button
	float btX = 578.0f;
	float btY = 395.0f;
	float btW = 24.0f;
	float btH = 24.0f;

	bool bHover = this->IsWorkZone(btX, btY, btW, btH);

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.3) - Render native Mix_button texture for TC button
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	EnableAlphaTest(true);

	int btnTexture = (bHover && (MouseLButton || MouseLButtonPush)) ? CUSTOM_CHANGE_CLASS_BTN_CLICK : CUSTOM_CHANGE_CLASS_BTN_NORMAL;
	RenderBitmap(btnTexture, btX, btY, btW, btH, (0.0f / 64.0f), (0.0f / 32.0f), (44.0f / 64.0f), (32.0f / 32.0f), true, true);

	// Show tooltip on hover
	if (bHover)
	{
		pRenderTipText((int)btX - 25, (int)btY - 13, "Trocar classe");
	}

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

	// 1. Render Window Background Texture (back_middle.jpg / back_middle.ozj)
	// Update Kayito 92 2.4.9 -> 97K SSeMU Update (3.1.8) - Render window background texture
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	EnableAlphaTest(true);

	RenderBitmap(CUSTOM_CHANGE_CLASS_BG_TEXTURE, JanelaX, JanelaY, Largura, Altura, 0.0f, 0.0f, (1024.0f / 1024.0f), (851.0f / 1024.0f), true, true);

	// Close button at top-right
	float closeX = JanelaX + Largura - 22.0f;
	float closeY = JanelaY + 4.0f;
	float closeW = 18.0f;
	float closeH = 18.0f;
	bool bCloseHover = this->IsWorkZone(closeX, closeY, closeW, closeH);

	// 2. CustomClass_Frame Coordinates (Proportionally scaled down and centered)
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.0) - Proportional scaling and centering
	float frameW = 96.0f;
	float frameH = 128.0f;
	float frameX = JanelaX + (Largura - frameW) / 2.0f;
	float frameY = JanelaY + 34.0f;

	// Navigation Arrow Buttons (Symmetrically centered in the side margins)
	float arrowW = 20.0f;
	float arrowH = 20.0f;
	float arrowLeftX = JanelaX + 23.5f;
	float arrowRightX = JanelaX + Largura - 23.5f - arrowW;
	float arrowY = frameY + 37.0f;

	bool bLeftHover = this->IsWorkZone(arrowLeftX, arrowY, arrowW, arrowH);
	bool bRightHover = this->IsWorkZone(arrowRightX, arrowY, arrowW, arrowH);

	// Action button at the bottom (CONFIRMAR compact and centered with native texture 0xF0)
	float confW = 96.0f;
	float confH = 22.0f;
	float confX = JanelaX + (Largura - confW) / 2.0f;
	float confY = JanelaY + 234.0f;
	bool bConfHover = this->IsWorkZone(confX, confY, confW, confH);

	// 3. Re-enable textures and reset OpenGL states before drawing textures and texts
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	EnableAlphaTest(true);

	// 1. Render native close button (0x118 = exit_01.jpg normal/hover, 0x119 = exit_02.jpg pressed/clicked)
	// Update Kayito 92 2.4.9 -> 97K SSeMU Update (3.1.8) - Render native Close button with pressed state on click
	int closeTexture = (bCloseHover && (MouseLButton || MouseLButtonPush)) ? 0x119 : 0x118;
	RenderBitmap(closeTexture, closeX, closeY, closeW, closeH, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);

	// 2. Render native confirm button texture (0xF0 = Interface\Message_box.jpg)
	// Update Kayito 92 2.4.9 -> 97K SSeMU Update (3.1.8) - Render native Message_box button
	RenderBitmap(0xF0, confX, confY, confW, confH, (0.0f / 256.0f), (0.0f / 64.0f), (213.0f / 256.0f), (64.0f / 64.0f), true, true);

	// Show close tooltip on hover
	if (bCloseHover)
	{
		pRenderTipText((int)closeX - 15, (int)closeY - 13, "Fechar");
	}

	// 3. Render native left arrow (0xFE = back1.jpg normal, 0xFF = back2.jpg hover, 0x100 = back3.jpg pressed)
	// Update Kayito 92 2.4.9 -> 97K SSeMU Update (3.1.8) - Render native left arrow button
	int leftArrowTexture = 0xFE;
	if (bLeftHover)
	{
		leftArrowTexture = (MouseLButton || MouseLButtonPush) ? 0x100 : 0xFF;
	}
	RenderBitmap(leftArrowTexture, arrowLeftX, arrowY, arrowW, arrowH, (0.0f / 32.0f), (0.0f / 32.0f), (32.0f / 32.0f), (32.0f / 32.0f), true, true);

	// 4. Render native right arrow (flipped horizontally: u = 32/32, uWidth = -32/32)
	// Update Kayito 92 2.4.9 -> 97K SSeMU Update (3.1.8) - Render native right arrow button
	int rightArrowTexture = 0xFE;
	if (bRightHover)
	{
		rightArrowTexture = (MouseLButton || MouseLButtonPush) ? 0x100 : 0xFF;
	}
	RenderBitmap(rightArrowTexture, arrowRightX, arrowY, arrowW, arrowH, (32.0f / 32.0f), (0.0f / 32.0f), (-32.0f / 32.0f), (32.0f / 32.0f), true, true);

	// 5. Render Class Avatar Face Texture (Inside upper frame box)
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.2) - Render selected class avatar inside frame
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	EnableAlphaTest(true);

	float avatarW = 80.0f;
	float avatarH = 80.0f;
	float avatarX = frameX + (frameW - avatarW) / 2.0f;
	float avatarY = frameY + 7.0f;
	int faceTexture = CUSTOM_CHANGE_CLASS_FACE_BK;
	switch (this->m_SelectedClass)
	{
		case 0: faceTexture = CUSTOM_CHANGE_CLASS_FACE_BK; break; // Blade Knight
		case 1: faceTexture = CUSTOM_CHANGE_CLASS_FACE_SM; break; // Soul Master
		case 2: faceTexture = CUSTOM_CHANGE_CLASS_FACE_ME; break; // Muse Elf
		case 3: faceTexture = CUSTOM_CHANGE_CLASS_FACE_MG; break; // Magic Gladiator
	}
	RenderBitmap(faceTexture, avatarX, avatarY, avatarW, avatarH, 0.0f, 0.0f, 1.0f, 1.0f, true, true);

	// 6. Render CustomClass_Frame texture (Overlaying the avatar as an elegant golden frame)
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.1) - Render CustomClass_Frame texture with native alpha channel
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	EnableAlphaTest(true);
	RenderBitmap(CUSTOM_CHANGE_CLASS_FRAME, frameX, frameY, frameW, frameH, (30.0f / 256.0f), (24.0f / 256.0f), (152.0f / 256.0f), (203.0f / 256.0f), true, true);

	// 7. Backup font and text colors
	HFONT hOldFont = (HFONT)SelectObject(m_hFontDC, g_hFont);
	DWORD dwOldColor = SetTextColor;
	DWORD dwOldBgColor = SetBackgroundTextColor;

	// Title text (Centered - Golden Archer style)
	this->DrawTextCenter((int)JanelaX, (int)JanelaY + 6, (int)Largura, g_hFontBold, RGBA_TEXT(225, 225, 225, 255), "Trocar de Classe");

	// Class Name inside the frame bottom box
	float nameY = frameY + 109.0f;
	this->DrawTextCenter((int)frameX, (int)nameY, (int)frameW, g_hFontBold, RGBA_TEXT(255, 215, 0, 255), s_Classes[this->m_SelectedClass].Name);

	// Informational Requirements Text (Centered horizontally in window)
	float infoY = frameY + frameH + 8.0f;
	this->DrawTextCenter((int)JanelaX, (int)infoY, (int)Largura, g_hFont, RGBA_TEXT(225, 225, 225, 255), "A troca de classe e apenas para VIP");
	this->DrawTextCenter((int)JanelaX, (int)(infoY + 14.0f), (int)Largura, g_hFont, RGBA_TEXT(190, 190, 190, 255), "VIP possui vantagens e mantem o servidor");
	this->DrawTextCenter((int)JanelaX, (int)(infoY + 28.0f), (int)Largura, g_hFont, RGBA_TEXT(255, 140, 100, 255), "Desequipe todos os itens antes de trocar");

	// Action button text (Centered inside native button)
	this->DrawTextCenter((int)confX, (int)(confY + 5.0f), (int)confW, g_hFontBold, bConfHover ? RGBA_TEXT(255, 255, 255, 255) : RGBA_TEXT(220, 220, 220, 255), "CONFIRMAR");

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

	// 1. Mouse check for button in Character Status window (X=578, Y=395, 24x24)
	if (CharacterOpened == 1)
	{
		// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.5) - Align click zone vertically with native close button
		float btX = 578.0f;
		float btY = 395.0f;
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

		// Navigation Arrows [<] and [>] (Symmetrically centered in the side margins)
		float frameH = 128.0f;
		float frameY = JanelaY + 34.0f;
		float arrowW = 20.0f;
		float arrowH = 20.0f;
		float arrowLeftX = JanelaX + 23.5f;
		float arrowRightX = JanelaX + Largura - 23.5f - arrowW;
		float arrowY = frameY + 37.0f;

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

		// Confirm button (Compact and centered)
		float confW = 96.0f;
		float confH = 22.0f;
		float confX = JanelaX + (Largura - confW) / 2.0f;
		float confY = JanelaY + 234.0f;

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
	}
}

void CCustomChangeClass::ConfirmChange()
{
	if (this->m_SelectedClass < 0 || this->m_SelectedClass >= s_TotalClasses)
	{
		return;
	}

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 93 (3.2.6) - Anti-spam rate limit on Confirm button (3 seconds)
	if ((GetTickCount() - this->m_LastConfirmTime) < 3000)
	{
		return;
	}
	this->m_LastConfirmTime = GetTickCount();

	// Update CMZ 17 (3.1.7) 28-09-26 - Send class change request packet
	PMSG_CUSTOM_CHANGE_CLASS_REQ pMsg;
	pMsg.h.set(0xF3, 0xE5, sizeof(pMsg));
	pMsg.TargetClass = (BYTE)s_Classes[this->m_SelectedClass].ClassCode;
	DataSend((BYTE*)&pMsg, pMsg.h.size);
}

// Update CMZ 17 (3.1.7) 28-09-26 - Receive class change response packet
void CCustomChangeClass::GCChangeClassRecv(PMSG_CUSTOM_CHANGE_CLASS_ANS* lpMsg)
{
	if (lpMsg->Result == 0)
	{
		this->Close();
	}
}

