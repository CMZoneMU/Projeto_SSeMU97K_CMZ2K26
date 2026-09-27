// Guild.h: interface for the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"

enum eGuildRank
{
	GUILD_RANK_NONE = -1,
	GUILD_RANK_NORMAL = 0,
	GUILD_RANK_MASTER = 128,
};

enum eGuildWarType
{
	GUILD_WAR_TYPE_NORMAL = 0,
	GUILD_WAR_TYPE_SOCCER = 1,
};

enum eGuildWarState
{
	GUILD_WAR_STATE_NONE = 0,
	GUILD_WAR_STATE_DECLARE = 1,
	GUILD_WAR_STATE_ACTIVE = 2,
};

enum eGuildWarTeam
{
	GUILD_WAR_TEAM_NONE = -1,
	GUILD_WAR_TEAM_BLUE = 0,
	GUILD_WAR_TEAM_RED = 1,
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_RECV
{
	PSBMSG_HEAD header; // C1:A0:00
	DWORD Index;
	char Name[9];
	char Master[11];
	BYTE Mark[32];
	BYTE result;
};

struct SDHP_GUILD_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:A0:01
	char Name[9];
	char Member[11];
	BYTE result;
};

struct SDHP_GUILD_MEMBER_ADD_RECV
{
	PSBMSG_HEAD header; // C1:A0:02
	char Name[9];
	char Member[11];
	int Server;
	BYTE result;
};

struct SDHP_GUILD_MEMBER_DEL_RECV
{
	PSBMSG_HEAD header; // C1:A0:03
	char Name[9];
	char Member[11];
	BYTE result;
};

struct SDHP_GUILD_NOTICE_RECV
{
	PSBMSG_HEAD header; // C1:A0:04
	char Name[9];
	char Notice[60];
};

struct SDHP_GUILD_POST_RECV
{
	PSBMSG_HEAD header; // C1:A0:05
	char Name[9];
	char Member[11];
	char Message[60];
};

struct SDHP_GUILD_SCORE_RECV
{
	PSBMSG_HEAD header; // C1:A0:06
	char Name[9];
	DWORD Score;
};

struct SDHP_GUILD_INFO_RECV
{
	PSWMSG_HEAD header; // C2:A0:07
	DWORD Index;
	char Name[9];
	char Master[11];
	BYTE Mark[32];
	DWORD Score;
	char Notice[60];
	BYTE count;
};

struct SDHP_GUILD_INFO
{
	char Name[11];
	int Server;
};

struct SDHP_GUILD_MEMBER_UPDATE_RECV
{
	PSBMSG_HEAD header; // C1:A0:08
	char Name[9];
	char Member[11];
	int Server;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_SEND
{
	PSBMSG_HEAD header; // C1:A0:00
	char Name[9];
	char Master[11];
	BYTE Mark[32];
	int Server;
};

struct SDHP_GUILD_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:A0:01
	char Name[9];
	char Member[11];
};

struct SDHP_GUILD_MEMBER_ADD_SEND
{
	PSBMSG_HEAD header; // C1:A0:02
	char Name[9];
	char Member[11];
	int Server;
};

struct SDHP_GUILD_MEMBER_DEL_SEND
{
	PSBMSG_HEAD header; // C1:A0:03
	char Name[9];
	char Member[11];
};

struct SDHP_GUILD_NOTICE_SEND
{
	PSBMSG_HEAD header; // C1:A0:04
	char Name[9];
	char Notice[60];
};

struct SDHP_GUILD_POST_SEND
{
	PSBMSG_HEAD header; // C1:A0:05
	char Name[9];
	char Member[11];
	char Message[60];
};

struct SDHP_GUILD_SCORE_SEND
{
	PSBMSG_HEAD header; // C1:A0:06
	char Name[9];
	DWORD Score;
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_GUILD_REQUEST_RECV
{
	PBMSG_HEAD header; // C1:50
	BYTE index[2];
};

struct PMSG_GUILD_RESULT_RECV
{
	PBMSG_HEAD header; // C1:51
	BYTE result;
	BYTE index[2];
};

struct PMSG_GUILD_DELETE_RECV
{
	PBMSG_HEAD header; // C1:53
	char name[10];
	char PersonalCode[10];
};

struct PMSG_GUILD_MASTER_OPEN_RECV
{
	PBMSG_HEAD header; // C1:54
	BYTE result;
};

struct PMSG_GUILD_CREATE_RECV
{
	PBMSG_HEAD header; // C1:55
	char GuildName[8];
	BYTE Mark[32];
};

struct PMSG_GUILD_WAR_REQUEST_RESULT_RECV
{
	PBMSG_HEAD header; // C1:61
	BYTE result;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_GUILD_REQUEST_SEND
{
	PBMSG_HEAD header; // C1:50
	BYTE index[2];
};

struct PMSG_GUILD_RESULT_SEND
{
	PBMSG_HEAD header; // C1:51
	BYTE result;
};

struct PMSG_GUILD_LIST_SEND
{
	PWMSG_HEAD header; // C2:52
	BYTE result;
	BYTE count;
	DWORD TotalScore;
	BYTE score;
};

struct PMSG_GUILD_LIST
{
	char name[10];
	BYTE number;
	BYTE connected;
};

struct PMSG_GUILD_DELETE_SEND
{
	PBMSG_HEAD header; // C1:53
	BYTE result;
};

struct PMSG_GUILD_CREATE_RESULT_SEND
{
	PBMSG_HEAD header; // C1:56
	BYTE result;
};

struct PMSG_GUILD_WAR_REQUEST_RESULT_SEND
{
	PBMSG_HEAD header; // C1:60
	BYTE result;
};

struct PMSG_GUILD_WAR_REQUEST_SEND
{
	PBMSG_HEAD header; // C1:61
	char GuildName[8];
	BYTE type;
};

struct PMSG_GUILD_WAR_START_SEND
{
	PBMSG_HEAD header; // C1:62
	char GuildName[8];
	BYTE type;
	BYTE team;
};

struct PMSG_GUILD_WAR_END_SEND
{
	PBMSG_HEAD header; // C1:63
	BYTE result;
	char GuildName[8];
};

struct PMSG_GUILD_WAR_SCORE_SEND
{
	PBMSG_HEAD header; // C1:64
	BYTE score[2];
	BYTE type;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGuild
{
public:
	CGuild();
	virtual ~CGuild();
	void DGGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg);
	void DGGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg);
	void DGGuildMemberAddRecv(SDHP_GUILD_MEMBER_ADD_RECV* lpMsg);
	void DGGuildMemberDelRecv(SDHP_GUILD_MEMBER_DEL_RECV* lpMsg);
	void DGGuildNoticeRecv(SDHP_GUILD_NOTICE_RECV* lpMsg);
	void DGGuildPostRecv(SDHP_GUILD_POST_RECV* lpMsg);
	void DGGuildScoreRecv(SDHP_GUILD_SCORE_RECV* lpMsg);
	void DGGuildInfoRecv(SDHP_GUILD_INFO_RECV* lpMsg);
	void DGGuildMemberUpdateRecv(SDHP_GUILD_MEMBER_UPDATE_RECV* lpMsg);
	void GDGuildMemberAddSend(int aIndex,char* Name,char* Member);
	void GDGuildMemberDelSend(int aIndex,char* Name,char* Member);
	void GDGuildNoticeSend(char* Name,char* Notice);
	void GDGuildPostSend(char* Name,char* Member,char* Message);
	void GDGuildScoreSend(char* Name,int score);
	void CGGuildRequestRecv(PMSG_GUILD_REQUEST_RECV* lpMsg,int aIndex);
	void CGGuildResultRecv(PMSG_GUILD_RESULT_RECV* lpMsg,int aIndex);
	void CGGuildListRecv(int aIndex);
	void CGGuildDeleteRecv(PMSG_GUILD_DELETE_RECV* lpMsg,int aIndex);
	void CGGuildMasterOpenRecv(PMSG_GUILD_MASTER_OPEN_RECV* lpMsg, int aIndex);
	void CGGuildCreateRecv(PMSG_GUILD_CREATE_RECV* lpMsg,int aIndex);
	void CGGuildMasterCancelRecv(int aIndex);
	void CGGuildWarRequestResultRecv(PMSG_GUILD_WAR_REQUEST_RESULT_RECV* lpMsg,int aIndex);
	void GCGuildResultSend(int aIndex,int result);
	void GCGuildDeleteSend(int aIndex,int result);
	void GCGuildCreateResultSend(int aIndex,int result);
	void GCGuildWarResultSend(int aIndex,int result);
	void GCGuildWarRequestSend(int aIndex,char* GuildName,int type);
	void GCGuildWarDeclareSend(int aIndex,char* GuildName,int type,int team);
	void GCGuildWarEndSend(int aIndex,int result,char* GuildName);
	void GCGuildWarScoreSend(int aIndex,int type);
	int GetMaxGuildUser(LPOBJ lpObj);
	void GuildWarDeclareSet(int aIndex,char* GuildName,int type,int team);
	void GuildWarRequestSend(int aIndex,char* GuildName,int type);
	int gObjTargetGuildWarCheck(LPOBJ lpObj,LPOBJ lpTarget);
	void gObjGuildWarEndSend(LPOBJ lpObj,int result1,int result2);
	void gObjGuildWarEndSend(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2,int result1,int result2);
	void gObjGuildWarEnd(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2);
	int gObjGuildWarProc(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2,int score);
	int gObjGuildWarCheck(LPOBJ lpObj,LPOBJ lpTargetObj);
	int gObjGuildWarMasterClose(LPOBJ lpObj);
	void NoticeSendToAll(GUILD_INFO* lpGuild,int type,char* message,...);
	void DataSendToAll(GUILD_INFO* lpGuild,BYTE* lpMsg,int size);
	// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 99 (2.5.8) - Atualizacao da lista de membros para o Master da Guild
	void GuildMasterUpdate(char* Name);
};

extern CGuild gGuild;