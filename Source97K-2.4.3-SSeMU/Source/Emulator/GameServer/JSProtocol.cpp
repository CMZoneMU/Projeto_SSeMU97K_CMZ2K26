#include "stdafx.h"
#include "JSProtocol.h"
#include "ConnectionManager.h"
#include "ConnectMember.h"
#include "CustomAttack.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Log.h"
#include "Message.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "Util.h"

void JoinServerProtocolCore(BYTE head,BYTE* lpMsg,int size) // OK
{
	switch(head)
	{
		case 0x01:
			JGConnectAccountRecv((SDHP_CONNECT_ACCOUNT_RECV*)lpMsg);
			break;
		case 0x02:
			JGDisconnectAccountRecv((SDHP_DISCONNECT_ACCOUNT_RECV*)lpMsg);
			break;
		case 0x05:
			JGAccountLevelRecv((SDHP_ACCOUNT_LEVEL_RECV*)lpMsg);
			break;
		case 0x30:
			JGAccountAlreadyConnectedRecv((SDHP_ACCOUNT_ALREADY_CONNECTED_RECV*)lpMsg);
			break;
	}
}

void JGConnectAccountRecv(SDHP_CONNECT_ACCOUNT_RECV* lpMsg) // OK
{
	if(OBJECT_RANGE(lpMsg->index) == 0)
	{
		return;
	}

	if((gObj[lpMsg->index].LoginMessageSend--) == 0)
	{
		CloseClient(lpMsg->index);
		return;
	}

	if(gObj[lpMsg->index].Connected != OBJECT_CONNECTED)
	{
		GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr,gObj[lpMsg->index].HardwareId);
		return;
	}

	if(gServerInfo.m_ConnectMemberCheck != 0 && gConnectMember.CheckAccount(lpMsg->account) == 0)
	{
		GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr,gObj[lpMsg->index].HardwareId);
		GCConnectAccountSend(lpMsg->index,2);
		return;
	}

	if(lpMsg->result != 1)
	{
		GCConnectAccountSend(lpMsg->index,((gObj[lpMsg->index].LoginMessageCount>=3)?8:lpMsg->result));
		return;
	}

	if(lpMsg->BlockCode != 0)
	{
		GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr,gObj[lpMsg->index].HardwareId);
		GCConnectAccountSend(lpMsg->index,5);
		return;
	}

	if(gServerInfo.m_ServerLock > lpMsg->AccountLevel)
	{
		GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr,gObj[lpMsg->index].HardwareId);
		GCConnectAccountSend(lpMsg->index,11);
		return;
	}

	if(gConnectionManager.CheckMaxConnection(lpMsg->index) == 0)
	{
		GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr,gObj[lpMsg->index].HardwareId);
		GCConnectAccountSend(lpMsg->index,7);
		return;
	}

	gObj[lpMsg->index].Connected = OBJECT_LOGGED;

	memcpy(gObj[lpMsg->index].Account,lpMsg->account,sizeof(gObj[lpMsg->index].Account));

	memcpy(gObj[lpMsg->index].PersonalCode,lpMsg->PersonalCode,sizeof(gObj[lpMsg->index].PersonalCode));

	gObj[lpMsg->index].AccountLevel = lpMsg->AccountLevel;

	memcpy(gObj[lpMsg->index].AccountExpireDate,lpMsg->AccountExpireDate,sizeof(gObj[lpMsg->index].AccountExpireDate));

	GCConnectAccountSend(lpMsg->index,1);

	gLog.Output(LOG_CONNECT,"[ObjectManager][%d] AddAccountInfo [%s] [%s][%s]",lpMsg->index,gObj[lpMsg->index].Account,gObj[lpMsg->index].IpAddr,gObj[lpMsg->index].HardwareId);
}

void JGDisconnectAccountRecv(SDHP_DISCONNECT_ACCOUNT_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) != 0)
	{
		CloseClient(lpMsg->index);
	}
}

void JGAccountLevelRecv(SDHP_ACCOUNT_LEVEL_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[JGAccountLevelRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	if(gServerInfo.m_ServerLock > lpMsg->AccountLevel)
	{
		gObjUserKill(lpMsg->index);
		return;
	}

	if(gObj[lpMsg->index].AccountLevel == lpMsg->AccountLevel && strcmp(gObj[lpMsg->index].AccountExpireDate,lpMsg->AccountExpireDate) == 0)
	{
		gObj[lpMsg->index].AccountLevel = lpMsg->AccountLevel;
		memcpy(gObj[lpMsg->index].AccountExpireDate,lpMsg->AccountExpireDate,sizeof(gObj[lpMsg->index].AccountExpireDate));
	}
	else
	{
		gObj[lpMsg->index].AccountLevel = lpMsg->AccountLevel;
		memcpy(gObj[lpMsg->index].AccountExpireDate,lpMsg->AccountExpireDate,sizeof(gObj[lpMsg->index].AccountExpireDate));
		gNotice.GCNoticeSend(lpMsg->index,1,0,0,0,0,0,gMessage.GetMessage((155+gObj[lpMsg->index].AccountLevel)),gObj[lpMsg->index].AccountExpireDate);
	}

	// Update CMZ 15 (3.1.5) 26-09-26 - Synchronize AccountLevel with Client
	if(gObj[lpMsg->index].Connected == OBJECT_ONLINE)
	{
		GCNewCharacterInfoSend(&gObj[lpMsg->index]);
	}
}

void JGAccountAlreadyConnectedRecv(SDHP_ACCOUNT_ALREADY_CONNECTED_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[JGAccountAlreadyConnectedRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 98 (2.5.7) - Fix reconnect disconnect for offline and online accounts
	gNotice.GCNoticeSend(lpMsg->index,1,0,0,0,0,0,gMessage.GetMessage(153));

	if(gObj[lpMsg->index].Attack.Offline != 0)
	{
		gCustomAttack.AttackOfflineClose(&gObj[lpMsg->index]);
	}
	else
	{
		CloseClient(lpMsg->index);
	}
}

void GJServerInfoSend() // OK
{
	SDHP_JOIN_SERVER_INFO_SEND pMsg;

	pMsg.header.set(0x00,sizeof(pMsg));

	pMsg.type = 1;

	pMsg.ServerPort = (WORD)gServerInfo.m_ServerPort;

	strcpy_s(pMsg.ServerName,gServerInfo.m_ServerName);

	pMsg.ServerCode = (WORD)gServerInfo.m_ServerCode;

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJConnectAccountSend(int aIndex,char* account,char* password,char* IpAddress,char* HardwareId) // OK
{
	SDHP_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x01,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,account,sizeof(pMsg.account));

	memcpy(pMsg.password,password,sizeof(pMsg.password));

	memcpy(pMsg.IpAddress,IpAddress,sizeof(pMsg.IpAddress));

	memcpy(pMsg.HardwareId,HardwareId,sizeof(pMsg.HardwareId));

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJDisconnectAccountSend(int aIndex,char* account,char* IpAddress,char* HardwareId) // OK
{
	SDHP_DISCONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x02,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,account,sizeof(pMsg.account));

	memcpy(pMsg.IpAddress,IpAddress,sizeof(pMsg.IpAddress));

	memcpy(pMsg.HardwareId,HardwareId,sizeof(pMsg.HardwareId));

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJAccountLevelSend(int aIndex) // OK
{
	SDHP_ACCOUNT_LEVEL_SEND pMsg;

	pMsg.header.set(0x05,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJAccountLevelSaveSend(int aIndex,int AccountLevel,int AccountExpireTime) // OK
{
	SDHP_ACCOUNT_LEVEL_SAVE_SEND pMsg;

	pMsg.header.set(0x11,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.AccountLevel = AccountLevel;

	pMsg.AccountExpireTime = AccountExpireTime;

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJServerUserInfoSend() // OK
{
	SDHP_SERVER_USER_INFO_SEND pMsg;

	pMsg.header.set(0x20,sizeof(pMsg));

	pMsg.CurUserCount = (WORD)gObjTotalUser;

	pMsg.MaxUserCount = (WORD)gServerInfo.m_ServerMaxUserNumber;

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}
