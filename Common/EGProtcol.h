
#ifndef	__EGPROTCOL_H__
#define	__EGPROTCOL_H__


#include "EG.h"


//コマンドID
#define	CMD_NONE			0x0000		//コマンド無し
//client<->server
#define	CMD_EXIT_APP		0x0001		//プログラム終了要求
#define	CMD_GET_USERID		0x0100		//ユーザID取得要求
#define	CMD_START_GAME		0x0200		//ゲーム開始通知
#define	CMD_END_GAME		0x0201		//ゲーム終了通知
#define	CMD_MOVE_PHASE		0x0300		//コマの移動通知
#define	CMD_MOVE_RESULT		0x0301		//コマの移動結果通知
//gui<->server
#define	CMD_EXIT			0x1001		//プログラム終了要求
#define	CMD_GET_USERINFO	0x1100		//ユーザ情報取得要求
#define	CMD_START			0x1200		//ゲーム開始通知
#define	CMD_END				0x1201		//ゲーム終了通知
#define	CMD_NEXT			0x1300		//行動依頼通知
#define	CMD_GAMEINFO		0x1301		//ゲームの情報通知
#define	CMD_TIMELEFT		0x1302		//移動残り時間通知


#ifdef _WIN32
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif	//_WIN32


//以下のプロトコルの構造体のエンディアンはリトルエンディアンとする


//コマンド
typedef struct tagEGCOMMAND
{
	unsigned short	wCommand;		//コマンドID
	union tagParqam
	{											//					send(recv)			reply
		char			cUserID[MAX_USERID];	// UTF-8								CMD_GET_USERID		
		GAMEINFO		GameInfo;				//					CMD_MOVE_PHASE		CMD_NEXT
		MOVEPIECEINFO	MovePiece;				//										CMD_MOVE_PHASE
		MOVEPIECERESULT	MoveResult;				//					CMD_MOVE_RESULT		
		GAMEENDINFO		GameEndInfo;			//					CMD_END_GAME		
		PLAYERINFO		PlayerInfo;				//										CMD_GET_USERINFO
		unsigned int	nTimeLeft;				// Millisecond		CMD_TIMELEFT		
		GAMESTARTINFO	GameStartInfo;			//					CMD_START			
		ENDINFO			EndInfo;				//					CMD_END				
	} Parqam;
} EGCOMMAND;


//プロトコル
typedef struct tagEGPROTCOL
{
	unsigned short	wDataSize;		//本メンバを含む通信データサイズ
	EGCOMMAND		CmdInfo;		//コマンド情報
} EGPROTCOL;


#ifdef _WIN32
#pragma pack(pop)
#else
#pragma pack(0)
#endif	//_WIN32


#endif	//__EGPROTCOL_H__

