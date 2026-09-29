// Update CMZ 17 (3.1.7) 28-09-26 - Custom Change Class System
// CustomChangeClass.h: interface for the CCustomChangeClass class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DefaultClassInfo.h"
#include "Protocol.h"
#include "User.h"

#pragma pack(push, 1)
struct PMSG_CUSTOM_CHANGE_CLASS_REQ
{
	PSBMSG_HEAD h;
	BYTE TargetClass;
};

struct PMSG_CUSTOM_CHANGE_CLASS_ANS
{
	PSBMSG_HEAD h;
	BYTE Result; // 0 = Success, 1 = Error/Not VIP, 2 = Items equipped, 3 = Not enough zen, 4 = Same class
};
#pragma pack(pop)

class CCustomChangeClass
{
public:
	CCustomChangeClass();
	virtual ~CCustomChangeClass();
	void ReadCustomChangeClassInfo(char* section, char* path);
	bool ChangeClass(LPOBJ lpObj, int targetClass);
	void CommandChangeClass(LPOBJ lpObj, char* arg);
	void CommandChangeClassBK(LPOBJ lpObj, char* arg);
	void CommandChangeClassSM(LPOBJ lpObj, char* arg);
	void CommandChangeClassME(LPOBJ lpObj, char* arg);
	void CommandChangeClassMG(LPOBJ lpObj, char* arg);
	void CGChangeClassRecv(PMSG_CUSTOM_CHANGE_CLASS_REQ* lpMsg, int aIndex);
private:
	int m_CustomChangeClassSwitch;
	int m_CustomChangeClassRequireVip;
	int m_CustomChangeClassCheckItem;
	int m_CustomChangeClassResetPoints;
	int m_CustomChangeClassResetSkills;
	int m_CustomChangeClassMinLevel;
	int m_CustomChangeClassReqMoney;
	int m_CustomChangeClassGateMove;
};

extern CCustomChangeClass gCustomChangeClass;
