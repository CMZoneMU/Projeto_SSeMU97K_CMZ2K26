// Guild.cpp: implementation of the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guild.h"
#include "BattleSoccer.h"
#include "CustomArena.h"
#include "DefaultClassInfo.h"
#include "GameMain.h"
#include "GuildManager.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ServerInfo.h"
#include "Util.h"
#include "Viewport.h"

CGuild gGuild;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuild::CGuild() // OK
{

}

CGuild::~CGuild() // OK
{

}

void CGuild::DGGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg) // OK
{
	if(lpMsg->result == 1)
	{
		gGuildManager.AddGuild(lpMsg->Index,lpMsg->Name,lpMsg->Mark,lpMsg->Master,"", 0);
	}

	LPOBJ lpObj = gObjFind(lpMsg->Master);

	if(lpObj != 0)
	{
		this->GCGuildCreateResultSend(lpObj->Index,lpMsg->result);

		if(lpMsg->result == 1)
		{
			if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
			{
				lpObj->Interface.use = 0;
			}

			GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

			if(lpGuild != 0)
			{
				lpObj->Guild = lpGuild;

				lpObj->GuildIndex = lpGuild->Index;

				gViewport.GCViewportSimpleGuildSend(lpObj);
			}
		}
	}
}

void CGuild::DGGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg) // OK
{
	if(lpMsg->result == 1)
	{
		GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

		if(lpGuild != 0)
		{
			for(int n=0;n < MAX_GUILD_MEMBER;n++)
			{
				if(lpGuild->Member[n].IsValid() != 0)
				{
					gObj[lpGuild->Member[n].Index].Guild = 0;

					gObj[lpGuild->Member[n].Index].GuildIndex = 0;

					gViewport.DestroyViewportGuild(lpGuild->Member[n].Index);

					this->GCGuildDeleteSend(lpGuild->Member[n].Index,lpMsg->result);
				}
			}

			gGuildManager.DelGuild(lpGuild->Index);
		}
	}
	else
	{
		LPOBJ lpObj = gObjFind(lpMsg->Member);

		if(lpObj != 0)
		{
			this->GCGuildDeleteSend(lpObj->Index,lpMsg->result);
		}
	}
}

void CGuild::DGGuildMemberAddRecv(SDHP_GUILD_MEMBER_ADD_RECV* lpMsg) // OK
{
	LPOBJ lpObj = gObjFind(lpMsg->Member);

	if(lpObj != 0)
	{
		this->GCGuildResultSend(lpObj->Index,lpMsg->result);

		if(lpMsg->result == 1)
		{
			gGuildManager.AddMember(lpMsg->Name,lpMsg->Member,lpObj->Index,lpMsg->Server);

			GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

			if(lpGuild != 0)
			{
				lpObj->Guild = lpGuild;

				lpObj->GuildIndex = lpGuild->Index;

				gViewport.GCViewportSimpleGuildSend(lpObj);
			}
		}
	}
	else
	{
		if(lpMsg->result == 1)
		{
			gGuildManager.AddMember(lpMsg->Name,lpMsg->Member,-1,lpMsg->Server);
		}
	}
}

void CGuild::DGGuildMemberDelRecv(SDHP_GUILD_MEMBER_DEL_RECV* lpMsg) // OK
{
	if(lpMsg->result == 1)
	{
		gGuildManager.DelMember(lpMsg->Member);

		// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 99 (2.5.8) - Atualizacao da lista do Master ao remover membro
		this->GuildMasterUpdate(lpMsg->Name);
	}

	LPOBJ lpObj = gObjFind(lpMsg->Member);

	if(lpObj != 0)
	{
		this->GCGuildDeleteSend(lpObj->Index,lpMsg->result);

		if(lpMsg->result == 1)
		{
			lpObj->Guild = 0;

			lpObj->GuildIndex = 0;

			gViewport.DestroyViewportGuild(lpObj->Index);
		}
	}
}

void CGuild::DGGuildNoticeRecv(SDHP_GUILD_NOTICE_RECV* lpMsg) // OK
{
	GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(lpGuild != 0)
	{
		memcpy(lpGuild->Notice,lpMsg->Notice,sizeof(lpGuild->Notice));

		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(lpGuild->Member[n].IsValid() != 0)
			{
				gNotice.GCNoticeSend(lpGuild->Member[n].Index,2,0,0,0,0,0,"%s",lpGuild->Notice);
			}
		}
	}
}

void CGuild::DGGuildPostRecv(SDHP_GUILD_POST_RECV* lpMsg) // OK
{
	GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(lpGuild != 0)
	{
		PMSG_CHAT_SEND pMsg;

		pMsg.header.set(0x00,sizeof(pMsg));

		memcpy(pMsg.name,lpMsg->Member,sizeof(pMsg.name));

		memcpy(pMsg.message,lpMsg->Message,sizeof(pMsg.message));

		this->DataSendToAll(lpGuild,(BYTE*)&pMsg,pMsg.header.size);
	}
}

void CGuild::DGGuildScoreRecv(SDHP_GUILD_SCORE_RECV* lpMsg) // OK
{
	GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(lpGuild != 0)
	{
		lpGuild->Score = lpMsg->Score;
	}
}

void CGuild::DGGuildInfoRecv(SDHP_GUILD_INFO_RECV* lpMsg) // OK
{
	gGuildManager.AddGuild(lpMsg->Index,lpMsg->Name,lpMsg->Mark,lpMsg->Master,lpMsg->Notice,lpMsg->Score);

	for(int n=0;n < lpMsg->count;n++)
	{
		SDHP_GUILD_INFO* lpInfo = (SDHP_GUILD_INFO*)(((BYTE*)lpMsg)+sizeof(SDHP_GUILD_INFO_RECV)+(sizeof(SDHP_GUILD_INFO)*n));

		if(gGuildManager.AddMember(lpMsg->Name,lpInfo->Name,-1,lpInfo->Server) == 0)
		{
			continue;
		}
	}
}

void CGuild::DGGuildMemberUpdateRecv(SDHP_GUILD_MEMBER_UPDATE_RECV* lpMsg) // OK
{
	LPOBJ lpObj = gObjFind(lpMsg->Member);

	if(lpObj != 0)
	{
		GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

		if(lpGuild != 0)
		{
			lpObj->Guild = lpGuild;

			lpObj->GuildIndex = lpGuild->Index;

			gGuildManager.ConnectUser(lpMsg->Name,lpObj->Name,lpObj->Index,lpMsg->Server);

			if(strlen(lpObj->Guild->Notice) > 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,2,0,0,0,0,0,"%s",lpObj->Guild->Notice);
			}

			gViewport.GCViewportSimpleGuildSend(lpObj);

			if(lpObj->Guild->WarState != GUILD_WAR_STATE_NONE)
			{
				this->GCGuildWarScoreSend(lpObj->Index,lpObj->Guild->WarType);
				
				this->GCGuildWarDeclareSend(lpObj->Index,lpObj->Guild->WarEnemy->Name,lpObj->Guild->WarType,lpObj->Guild->WarTeam);
			}
		}
	}

	gGuildManager.SetServer(lpMsg->Member,lpMsg->Server);
}

void CGuild::GDGuildMemberAddSend(int aIndex,char* Name,char* Member) // OK
{
	SDHP_GUILD_MEMBER_ADD_SEND pMsg;

	pMsg.header.set(0xA0,0x02,sizeof(pMsg));

	memcpy(pMsg.Name,Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,Member,sizeof(pMsg.Member));

	pMsg.Server = gServerInfo.m_ServerCode;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GDGuildMemberDelSend(int aIndex,char* Name,char* Member) // OK
{
	SDHP_GUILD_MEMBER_DEL_SEND pMsg;

	pMsg.header.set(0xA0,0x03,sizeof(pMsg));

	memcpy(pMsg.Name,Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,Member,sizeof(pMsg.Member));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GDGuildNoticeSend(char* Name,char* Notice) // OK
{
	SDHP_GUILD_NOTICE_SEND pMsg;

	pMsg.header.set(0xA0,0x04,sizeof(pMsg));

	memcpy(pMsg.Name,Name,sizeof(pMsg.Name));

	memcpy(pMsg.Notice,Notice,sizeof(pMsg.Notice));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GDGuildPostSend(char* Name,char* Member,char* Message) // OK
{
	SDHP_GUILD_POST_SEND pMsg;

	pMsg.header.set(0xA0,0x05,sizeof(pMsg));

	memcpy(pMsg.Name,Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,Member,sizeof(pMsg.Member));

	memcpy(pMsg.Message,Message,sizeof(pMsg.Message));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GDGuildScoreSend(char* Name,int Score) // OK
{
	SDHP_GUILD_SCORE_SEND pMsg;

	pMsg.header.set(0xA0,0x06,sizeof(pMsg));

	memcpy(pMsg.Name,Name,sizeof(pMsg.Name));

	pMsg.Score = Score;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::CGGuildRequestRecv(PMSG_GUILD_REQUEST_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(gObjIsConnectedGP(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if((lpTarget->Option & 1) == 0)
	{
		this->GCGuildResultSend(aIndex,0);
		return;
	}

	if(lpTarget->Guild == 0)
	{
		this->GCGuildResultSend(aIndex,4);
		return;
	}

	if(strcmp(lpTarget->Guild->Master,lpTarget->Name) != 0)
	{
		this->GCGuildResultSend(aIndex,4);
		return;
	}

	if(lpObj->Guild != 0)
	{
		this->GCGuildResultSend(aIndex,5);
		return;
	}

	if(lpObj->Interface.use != 0 || lpTarget->Interface.use != 0)
	{
		this->GCGuildResultSend(aIndex,6);
		return;
	}

	if(lpTarget->Guild->WarState != 0)
	{
		this->GCGuildResultSend(aIndex,6);
		return;
	}

	if(lpObj->Level < 6)
	{
		this->GCGuildResultSend(aIndex,7);
		return;
	}

	if(lpTarget->Guild->GetMemberCount() >= this->GetMaxGuildUser(lpTarget))
	{
		this->GCGuildResultSend(aIndex,2);
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_GUILD;
	lpObj->Interface.state = 0;
	lpObj->InterfaceTime = GetTickCount();
	lpObj->TargetNumber = bIndex;

	lpTarget->Interface.use = 1;
	lpTarget->Interface.type = INTERFACE_GUILD;
	lpTarget->Interface.state = 0;
	lpTarget->InterfaceTime = GetTickCount();
	lpTarget->TargetNumber = aIndex;

	PMSG_GUILD_REQUEST_SEND pMsg;

	pMsg.header.set(0x50,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	DataSend(bIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::CGGuildResultRecv(PMSG_GUILD_RESULT_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(gObjIsConnectedGP(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_GUILD)
	{
		return;
	}

	if(lpTarget->Interface.use == 0 || lpTarget->Interface.type != INTERFACE_GUILD)
	{
		return;
	}

	if(lpMsg->result == 0)
	{
		this->GCGuildResultSend(bIndex,0);
		goto CLEAR_JUMP;
	}

	if((lpObj->Option & 1) == 0)
	{
		this->GCGuildResultSend(bIndex,0);
		goto CLEAR_JUMP;
	}

	if(lpObj->Guild == 0)
	{
		this->GCGuildResultSend(bIndex,4);
		goto CLEAR_JUMP;
	}

	if(strcmp(lpObj->Guild->Master,lpObj->Name) != 0)
	{
		this->GCGuildResultSend(bIndex,4);
		goto CLEAR_JUMP;
	}

	if(lpTarget->Guild != 0)
	{
		this->GCGuildResultSend(bIndex,5);
		goto CLEAR_JUMP;
	}

	if(lpObj->Guild->WarState != 0)
	{
		this->GCGuildResultSend(bIndex,6);
		goto CLEAR_JUMP;
	}

	if(lpTarget->Level < 6)
	{
		this->GCGuildResultSend(bIndex,7);
		goto CLEAR_JUMP;
	}

	if(lpObj->Guild->GetMemberCount() >= this->GetMaxGuildUser(lpObj))
	{
		this->GCGuildResultSend(bIndex,2);
		goto CLEAR_JUMP;
	}

	this->GDGuildMemberAddSend(bIndex,lpObj->Guild->Name,lpTarget->Name);

	CLEAR_JUMP:

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
	lpObj->InterfaceTime = 0;
	lpObj->TargetNumber = -1;

	lpTarget->Interface.use = 0;
	lpTarget->Interface.type = INTERFACE_NONE;
	lpTarget->Interface.state = 0;
	lpTarget->InterfaceTime = 0;
	lpTarget->TargetNumber = -1;
}

void CGuild::CGGuildListRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Guild == 0)
	{
		return;
	}

	BYTE send[2048];

	PMSG_GUILD_LIST_SEND pMsg;

	pMsg.header.set(0x52,0);

	int size = sizeof(pMsg);

	pMsg.result = 1;

	pMsg.count = 0;

	pMsg.TotalScore = lpObj->Guild->Score;

	pMsg.score = lpObj->Guild->WarScore;

	PMSG_GUILD_LIST info;

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpObj->Guild->Member[n].IsEmpty() != 0)
		{
			continue;
		}

		memcpy(info.name,lpObj->Guild->Member[n].Name,sizeof(info.name));

		info.number = (BYTE)(lpObj->Guild->Member[n].Server);

		info.connected = (lpObj->Guild->Member[n].Server & 0x7F) | ((lpObj->Guild->Member[n].Server!=-1)?0x80:0x00);

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	if(pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);
		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send,&pMsg,sizeof(pMsg));

		DataSend(aIndex,send,size);
	}
}

void CGuild::CGGuildDeleteRecv(PMSG_GUILD_DELETE_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(gServerInfo.m_GuildDeleteSwitch == 0)
	{
		this->GCGuildDeleteSend(aIndex,0);
		return;
	}

	if(lpObj->Guild == 0)
	{
		this->GCGuildDeleteSend(aIndex,0);
		return;
	}

	char name[11] = {0};

	char PersonalCode[14] = {0};

	memcpy(name,lpMsg->name,sizeof(lpMsg->name));

	memcpy(PersonalCode,lpMsg->PersonalCode,sizeof(lpMsg->PersonalCode));

	if(strcmp(lpObj->Guild->Master,lpObj->Name) == 0)
	{
		if(strcmp(lpObj->Name,name) != 0)
		{
			if(gObjCheckPersonalCode(aIndex,PersonalCode) == 0)
			{
				this->GCGuildDeleteSend(aIndex,0);
				return;
			}
			else
			{
				this->GDGuildMemberDelSend(aIndex,lpObj->Guild->Name,name);
				return;
			}
		}

		if(gObjCheckPersonalCode(aIndex,PersonalCode) == 0)
		{
			this->GCGuildDeleteSend(aIndex,0);
			return;
		}

		gObjGuildWarMasterClose(lpObj);

		SDHP_GUILD_DELETE_SEND pMsg;

		pMsg.header.set(0xA0,0x01,sizeof(pMsg));

		memcpy(pMsg.Name,lpObj->Guild->Name,sizeof(pMsg.Name));

		memcpy(pMsg.Member,name,sizeof(pMsg.Member));

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
	}
	else
	{
		if(strcmp(lpObj->Name,name) != 0)
		{
			this->GCGuildDeleteSend(aIndex,0);
			return;
		}

		if(gObjCheckPersonalCode(aIndex,PersonalCode) == 0)
		{
			this->GCGuildDeleteSend(aIndex,0);
			return;
		}

		this->GDGuildMemberDelSend(aIndex,lpObj->Guild->Name,name);
	}
}

void CGuild::CGGuildMasterOpenRecv(PMSG_GUILD_MASTER_OPEN_RECV*lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpMsg->result == 1)
	{
		if(lpObj->Level < gServerInfo.m_GuildCreateMinLevel[lpObj->AccountLevel])
		{
			goto CLEAR_JUMP;
		}

		if(lpObj->Reset < gServerInfo.m_GuildCreateMinReset[lpObj->AccountLevel])
		{
			goto CLEAR_JUMP;
		}

		PBMSG_HEAD pMsg;

		pMsg.set(0x55,sizeof(pMsg));

		DataSend(aIndex,(BYTE*)&pMsg,pMsg.size); 

		return;
	}
	else
	{
		goto CLEAR_JUMP;
	}

	CLEAR_JUMP:

	if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
	{
		lpObj->Interface.use = 0;
	}
}

void CGuild::CGGuildCreateRecv(PMSG_GUILD_CREATE_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char GuildName[9] = {0};

	memcpy(GuildName,lpMsg->GuildName,sizeof(lpMsg->GuildName));

	// Update 90 2.4.8 -> 97K - Validacao de caracteres maliciosos em nomes de guild
	if(CheckNameSyntax(GuildName) == 0 || CheckSymbol(GuildName) != 0)
	{
		gLog.Output(LOG_HACK,"[CheckNameSyntax][%s][%s] Guild Create error. [%s]",lpObj->Account,lpObj->Name,GuildName);
		this->GCGuildCreateResultSend(aIndex,5);
	}
	else if(strlen(GuildName) < 3 || strlen(GuildName) > 9)
	{
		this->GCGuildCreateResultSend(aIndex,2);
	}
	else
	{
		SDHP_GUILD_CREATE_SEND pMsg;

		pMsg.header.set(0xA0,0x00,sizeof(pMsg));

		memcpy(pMsg.Name,GuildName,sizeof(pMsg.Name));

		memcpy(pMsg.Master,lpObj->Name,sizeof(pMsg.Master));

		memcpy(pMsg.Mark,lpMsg->Mark,sizeof(pMsg.Mark));

		pMsg.Server = gServerInfo.m_ServerCode;

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
	}

	if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
	{
		lpObj->Interface.use = 0;
	}
}

void CGuild::CGGuildMasterCancelRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Guild != 0)
	{
		return;
	}

	if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
	{
		lpObj->Interface.use = 0;
	}
}

void CGuild::CGGuildWarRequestResultRecv(PMSG_GUILD_WAR_REQUEST_RESULT_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	GUILD_INFO* lpGuild1 = lpObj->Guild;

	if(lpGuild1 == 0)
	{
		this->GCGuildWarResultSend(aIndex,0);
		return;
	}

	GUILD_INFO* lpGuild2 = lpGuild1->WarEnemy;

	if(lpGuild2 != 0 && lpMsg->result != 0)
	{
		LPOBJ lpTarget = &gObj[lpGuild2->Member[0].Index];

		if(gCustomArena.CheckMap(lpTarget->Map) != 0 || BC_MAP_RANGE(lpTarget->Map) != 0 || DS_MAP_RANGE(lpTarget->Map) != 0)
		{
			lpMsg->result = 0;
		}
	}

	if(lpMsg->result == 0)
	{
		if(lpGuild2 != 0)
		{
			if(lpGuild2->WarDeclare == GUILD_WAR_STATE_DECLARE)
			{
				lpGuild1->WarDeclare = GUILD_WAR_STATE_NONE;

				lpGuild2->WarDeclare = GUILD_WAR_STATE_NONE;

				if(lpGuild2->Member[0].Index >= 0)
				{
					this->GCGuildWarResultSend(lpGuild2->Member[0].Index,6);
				}
			}
		}
	}
	else
	{
		if(lpGuild1->WarDeclare == GUILD_WAR_STATE_DECLARE)
		{
			if(lpGuild2 != 0)
			{
				if(lpGuild2->WarDeclare == GUILD_WAR_STATE_DECLARE)
				{
					if(lpGuild1->WarType == GUILD_WAR_TYPE_SOCCER)
					{
						if(gBattleSoccer.GetState() != BS_STATE_EMPTY)
						{
							lpGuild1->WarDeclare = GUILD_WAR_STATE_NONE;
							lpGuild1->WarState = GUILD_WAR_STATE_NONE;

							lpGuild2->WarDeclare = GUILD_WAR_STATE_NONE;
							lpGuild2->WarState = GUILD_WAR_STATE_NONE;

							this->GCGuildWarResultSend(aIndex,4);

							return;
						}

						lpGuild1->WarScore = 0;
						lpGuild2->WarScore = 0;
						
						gBattleSoccer.SetSoccerInfo(lpGuild1,lpGuild2);
					}

					lpGuild1->WarDeclare = GUILD_WAR_STATE_ACTIVE;
					lpGuild1->WarState = GUILD_WAR_STATE_DECLARE;
					lpGuild1->WarScore = 0;
					lpGuild1->WarTeam = GUILD_WAR_TEAM_BLUE;

					lpGuild2->WarDeclare = GUILD_WAR_STATE_ACTIVE;
					lpGuild2->WarState = GUILD_WAR_STATE_DECLARE;
					lpGuild2->WarScore = 0;
					lpGuild2->WarTeam = GUILD_WAR_TEAM_RED;

					for(int n=0;n < MAX_GUILD_MEMBER;n++)
					{
						if(lpGuild1->Member[n].IsValid() != 0)
						{
							if(n > 0 && lpGuild1->WarType == 1)
							{
								if(gObj[lpGuild1->Member[0].Index].PartyNumber >= 0 && gObj[lpGuild1->Member[0].Index].PartyNumber == gObj[lpGuild1->Member[n].Index].PartyNumber)
								{
									this->GuildWarDeclareSet(lpGuild1->Member[n].Index,lpGuild2->Name,lpGuild1->WarType,lpGuild1->WarTeam);
								}
							}
							else
							{
								this->GuildWarDeclareSet(lpGuild1->Member[n].Index,lpGuild2->Name,lpGuild1->WarType,lpGuild1->WarTeam);
							}
						}
					}

					for(int n=0;n < MAX_GUILD_MEMBER;n++)
					{
						if(lpGuild2->Member[n].IsValid() != 0)
						{
							if(n > 0 && lpGuild1->WarType == 1)
							{
								if(gObj[lpGuild2->Member[0].Index].PartyNumber >= 0 && gObj[lpGuild2->Member[0].Index].PartyNumber == gObj[lpGuild2->Member[n].Index].PartyNumber)
								{
									this->GuildWarDeclareSet(lpGuild2->Member[n].Index,lpGuild1->Name,lpGuild2->WarType,lpGuild2->WarTeam);
								}
							}
							else 
							{
								this->GuildWarDeclareSet(lpGuild2->Member[n].Index,lpGuild1->Name,lpGuild2->WarType,lpGuild2->WarTeam);
							}
						}
					}

					if(lpGuild1->WarType == GUILD_WAR_TYPE_SOCCER)
					{
						gObjAddMsgSendDelay(&gObj[aIndex],5,aIndex,10000,0);

						this->NoticeSendToAll(lpGuild1,1,gMessage.GetMessage(287));

						this->NoticeSendToAll(lpGuild2,1,gMessage.GetMessage(287));					
					}
				}
			}
		}
	}
}

void CGuild::GCGuildResultSend(int aIndex,int result) // OK
{
	PMSG_GUILD_RESULT_SEND pMsg;

	pMsg.header.set(0x51,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildDeleteSend(int aIndex,int result) // OK
{
	PMSG_GUILD_DELETE_SEND pMsg;

	pMsg.header.set(0x53,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildCreateResultSend(int aIndex,int result) // OK
{
	PMSG_GUILD_CREATE_RESULT_SEND pMsg;

	pMsg.header.set(0x56,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildWarResultSend(int aIndex,int result) // OK
{
	PMSG_GUILD_WAR_REQUEST_RESULT_SEND pMsg;

	pMsg.header.set(0x60,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildWarRequestSend(int aIndex,char* GuildName,int type) // OK
{
	PMSG_GUILD_WAR_REQUEST_SEND pMsg;

	pMsg.header.set(0x61,sizeof(pMsg));

	memcpy(pMsg.GuildName,GuildName,sizeof(pMsg.GuildName));

	pMsg.type = type;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildWarDeclareSend(int aIndex,char* GuildName,int type,int team) // OK
{
	PMSG_GUILD_WAR_START_SEND pMsg;

	pMsg.header.set(0x62,sizeof(pMsg));

	memcpy(pMsg.GuildName,GuildName,sizeof(pMsg.GuildName));

	pMsg.type = type;

	pMsg.team = team;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildWarEndSend(int aIndex,int result,char* GuildName) // OK
{
	PMSG_GUILD_WAR_END_SEND pMsg;

	pMsg.header.set(0x63,sizeof(pMsg));

	pMsg.result = result;

	memcpy(pMsg.GuildName,GuildName,sizeof(pMsg.GuildName));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildWarScoreSend(int aIndex,int type) // OK
{
	PMSG_GUILD_WAR_SCORE_SEND pMsg;

	pMsg.header.set(0x64,sizeof(pMsg));

	pMsg.score[0] = ((gObj[aIndex].Guild==0)?0:gObj[aIndex].Guild->WarScore);

	pMsg.score[1] = ((gObj[aIndex].Guild->WarEnemy==0)?0:gObj[aIndex].Guild->WarEnemy->WarScore);

	pMsg.type = type;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

int CGuild::GetMaxGuildUser(LPOBJ lpObj) // OK
{
	int MaxGuildUser = 0;

	if(lpObj->Class == CLASS_DL)
	{
		MaxGuildUser = ((lpObj->Reset==0)?((lpObj->Level/10)+(lpObj->Leadership/10)):gServerInfo.m_GuildInsertMaxUser2);

		MaxGuildUser = ((MaxGuildUser>gServerInfo.m_GuildInsertMaxUser2)?gServerInfo.m_GuildInsertMaxUser2:MaxGuildUser);
	}
	else
	{
		MaxGuildUser = ((lpObj->Reset==0)?(lpObj->Level/10):gServerInfo.m_GuildInsertMaxUser1);

		MaxGuildUser = ((MaxGuildUser>gServerInfo.m_GuildInsertMaxUser1)?gServerInfo.m_GuildInsertMaxUser1:MaxGuildUser);
	}

	return ((MaxGuildUser>MAX_GUILD_MEMBER)?MAX_GUILD_MEMBER:MaxGuildUser);
}

void CGuild::GuildWarDeclareSet(int aIndex,char* GuildName,int type,int team) // OK
{
	if(gObj[aIndex].Guild == 0)
	{
		return;
	}

	this->GCGuildWarDeclareSend(aIndex,GuildName,type,team);

	this->GCGuildWarScoreSend(aIndex,type);

	if(type == GUILD_WAR_TYPE_SOCCER)
	{
		if(gServerInfo.m_PKLimitFree == 0 && gObj[aIndex].PKLevel > 4)
		{
			return;
		}

		if(team == GUILD_WAR_TEAM_BLUE)
		{
			gObjTeleport(aIndex,6,63,153);
		}
		else
		{
			gObjTeleport(aIndex,6,63,168);
		}
	}
}

void CGuild::GuildWarRequestSend(int aIndex,char* GuildName,int type) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	// Update 89 2.4.7 -> 97K - Guild War Switch
	if(gServerInfo.m_GuildWarSwitch == 0)
	{
		return;
	}

	if(gMapManager.GetMapGuildWarEnable(lpObj->Map) == 0)
	{
		return;
	}

	if(lpObj->Guild == 0)
	{
		this->GCGuildWarResultSend(aIndex,3);
		return;
	}

	if(lpObj->Guild->WarState == GUILD_WAR_STATE_DECLARE || lpObj->Guild->WarDeclare == GUILD_WAR_STATE_DECLARE)
	{
		this->GCGuildWarResultSend(aIndex,4);
		return;
	}

	if(gServerInfo.m_PKLimitFree == 0 && lpObj->PKLevel > 4)
	{
		this->GCGuildWarResultSend(aIndex,4);
		return;
	}

	if(strcmp(lpObj->Guild->Master,lpObj->Name) != 0)
	{
		this->GCGuildWarResultSend(aIndex,5);
		return;
	}

	if(!strncmp(lpObj->Guild->Name,GuildName,8))
	{
		return;
	}

	GUILD_INFO* lpInfo = gGuildManager.GetGuild(GuildName);

	if(lpInfo == 0)
	{
		this->GCGuildWarResultSend(aIndex,0);
		return;
	}

	if(lpInfo->WarState == GUILD_WAR_STATE_DECLARE || lpInfo->WarDeclare == GUILD_WAR_STATE_DECLARE)
	{
		this->GCGuildWarResultSend(aIndex,4);
		return;
	}

	LPOBJ lpTarget = gObjFind(lpInfo->Master);

	if(lpTarget == 0)
	{
		this->GCGuildWarResultSend(aIndex,2);
		return;
	}

	if(gServerInfo.m_PKLimitFree == 0 && lpTarget->PKLevel > 4)
	{
		this->GCGuildWarResultSend(aIndex,4);
		return;
	}

	if(gMapManager.GetMapGuildWarEnable(lpTarget->Map) == 0)
	{
		return;
	}

	if((lpTarget->Option & 1) != 1)
	{
		this->GCGuildWarResultSend(aIndex,4);
		return;
	}

	this->GCGuildWarResultSend(aIndex,1);

	this->GCGuildWarRequestSend(lpTarget->Index,lpObj->Guild->Name,type);

	lpObj->Guild->WarDeclare = GUILD_WAR_STATE_DECLARE;
	lpObj->Guild->WarType = type;
	lpObj->Guild->WarEnemy = lpInfo;

	lpInfo->WarDeclare = GUILD_WAR_STATE_DECLARE;
	lpInfo->WarType = type;
	lpInfo->WarEnemy = lpObj->Guild;
}

int CGuild::gObjTargetGuildWarCheck(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Type != OBJECT_USER || lpTarget->Type != OBJECT_USER)
	{
		return 0;
	}

	if(lpObj->Guild == 0 || lpTarget->Guild == 0)
	{
		return 0;
	}

	if(lpObj->Guild->WarState != GUILD_WAR_STATE_DECLARE || lpTarget->Guild->WarState != GUILD_WAR_STATE_DECLARE)
	{
		return 0;
	}

	if(strcmp(lpObj->Guild->WarEnemy->Name,lpTarget->Guild->Name))
	{
		return 0;
	}

	return 1;
}

void CGuild::gObjGuildWarEndSend(LPOBJ lpObj,int result1,int result2) // OK
{
	if(lpObj == 0)
	{
		return;
	}

	if(lpObj->Guild == 0)
	{
		return;
	}

	if(lpObj->Guild->WarState != GUILD_WAR_STATE_DECLARE)
	{
		return;
	}

	if(lpObj->Guild->WarEnemy == 0)
	{
		return;
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpObj->Guild->Member[n].IsValid() != 0)
		{
			this->GCGuildWarEndSend(lpObj->Guild->Member[n].Index,result1,lpObj->Guild->WarEnemy->Name);
		}
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpObj->Guild->WarEnemy->Member[n].IsValid() != 0)
		{
			this->GCGuildWarEndSend(lpObj->Guild->WarEnemy->Member[n].Index,result2,lpObj->Guild->Name);
		}
	}
}

void CGuild::gObjGuildWarEndSend(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2,int result1,int result2) // OK
{
	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return;
	}

	if(lpGuild1->WarState != GUILD_WAR_STATE_DECLARE || lpGuild2->WarState != GUILD_WAR_STATE_DECLARE)
	{
		return;
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpGuild1->Member[n].IsValid() != 0)
		{
			gGuild.GCGuildWarEndSend(lpGuild1->Member[n].Index,result1,lpGuild2->Name);
		}
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpGuild2->Member[n].IsValid() != 0)
		{
			gGuild.GCGuildWarEndSend(lpGuild2->Member[n].Index,result2,lpGuild1->Name);
		}
	}
}

void CGuild::gObjGuildWarEnd(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2) // OK
{
	if(lpGuild1 != 0)
	{
		lpGuild1->WarDeclare = GUILD_WAR_STATE_NONE;
		lpGuild1->WarState = GUILD_WAR_STATE_NONE;

		if(lpGuild1->WarEnemy != 0)
		{
			lpGuild1->WarEnemy = 0;
		}
	}

	if(lpGuild2 != 0)
	{
		lpGuild2->WarDeclare = GUILD_WAR_STATE_NONE;
		lpGuild2->WarState = GUILD_WAR_STATE_NONE;

		if(lpGuild2->WarEnemy != 0)
		{
			lpGuild2->WarEnemy = 0;
		}
	}
}

// Update 89 2.4.7 -> 97K - Pontuação configurável e correção no cálculo de pontos de vitória
int CGuild::gObjGuildWarProc(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2,int score) // OK
{
	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return 0;
	}

	int MaxScore = (lpGuild1->WarType==GUILD_WAR_TYPE_SOCCER)?gServerInfo.m_GuildWarScoreMax2:gServerInfo.m_GuildWarScoreMax1;

	lpGuild1->WarScore += score;

	if(lpGuild1->WarScore >= MaxScore)
	{
		if(lpGuild2->WarScore == 0)
		{
			lpGuild1->Score += gServerInfo.m_GuildWarWinnerScore1;
		}
		else if(lpGuild2->WarScore <= (MaxScore/2))
		{
			lpGuild1->Score += gServerInfo.m_GuildWarWinnerScore2;
		}
		else
		{
			lpGuild1->Score += gServerInfo.m_GuildWarWinnerScore3;
		}

		this->GDGuildScoreSend(lpGuild1->Name,lpGuild1->Score);

		this->GDGuildScoreSend(lpGuild2->Name,lpGuild2->Score);

		return 1;
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpGuild1->Member[n].IsValid() != 0)
		{
			this->GCGuildWarScoreSend(lpGuild1->Member[n].Index,lpGuild1->WarType);
		}
	}
	
	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpGuild2->Member[n].IsValid() != 0)
		{
			this->GCGuildWarScoreSend(lpGuild2->Member[n].Index,lpGuild2->WarType);
		}
	}

	return 0;
}

// Update 89 2.4.7 -> 97K - Pontuação por morte configurável
int CGuild::gObjGuildWarCheck(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(gObjTargetGuildWarCheck(lpObj,lpTarget) == 0)
	{
		return 0;
	}

	GUILD_INFO* lpGuild1 = lpObj->Guild;

	GUILD_INFO* lpGuild2 = lpTarget->Guild;

	int Score = (strcmp(lpTarget->Name,lpTarget->Guild->Master)==0)?gServerInfo.m_GuildWarKillScore2:gServerInfo.m_GuildWarKillScore1;

	if(gObjGuildWarProc(lpGuild1,lpGuild2,Score) == 1)
	{
		if(lpGuild1->WarType == GUILD_WAR_TYPE_SOCCER)
		{
			gObjGuildWarEndSend(lpGuild1,lpGuild2,1,0);
			gObjGuildWarEnd(lpGuild1,lpGuild2);
			gBattleSoccer.SetState(BS_STATE_CLEAN);
		}
		else
		{
			gObjGuildWarEndSend(lpObj,1,0);
			gObjAddMsgSendDelay(lpObj,4,lpObj->Index,2000,0);
		}
	}

	return 1;
}

int CGuild::gObjGuildWarMasterClose(LPOBJ lpObj) // OK
{
	if(lpObj->Guild == 0)
	{
		return 0;
	}

	if(strcmp(lpObj->Guild->Master,lpObj->Name))
	{
		return 0;
	}

	gObjGuildWarEndSend(lpObj,3,2);

	gObjGuildWarEnd(lpObj->Guild,lpObj->Guild->WarEnemy);

	if(lpObj->Guild->WarType == GUILD_WAR_TYPE_SOCCER)
	{
		gBattleSoccer.SetState(BS_STATE_CLEAN);
	}

	return 1;
}

void CGuild::NoticeSendToAll(GUILD_INFO* lpGuild,int type,char* message,...) // OK
{
	if(lpGuild != 0)
	{
		char buff[256];

		va_list arg;
		va_start(arg,message);
		vsprintf_s(buff,message,arg);
		va_end(arg);

		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(lpGuild->Member[n].IsValid() != 0)
			{
				gNotice.GCNoticeSend(lpGuild->Member[n].Index,type,0,0,0,0,0,buff);
			}
		}
	}
}

void CGuild::DataSendToAll(GUILD_INFO* lpGuild,BYTE* lpMsg,int size) // OK
{
	if(lpGuild != 0)
	{
		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(lpGuild->Member[n].IsValid() != 0)
			{
				DataSend(lpGuild->Member[n].Index,lpMsg,size);
			}
		}
	}
}

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 99 (2.5.8) - Atualizacao da lista de membros para o Master da Guild
void CGuild::GuildMasterUpdate(char* Name)
{
	GUILD_INFO* lpGuild = gGuildManager.GetGuild(Name);

	if(lpGuild != 0)
	{
		LPOBJ lpMaster = gObjFind(lpGuild->Master);

		if(lpMaster != 0)
		{
			this->CGGuildListRecv(lpMaster->Index);
		}
	}
}