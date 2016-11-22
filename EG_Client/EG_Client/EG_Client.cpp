// EG_Client.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "C2SProtcol.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif //_WIN32
#include "EG.h"
#include "EGProtcol.h"
#include "Custom.h"


//Clientの状態
enum ClientState
{
	WaitRequestUserID = 0,
	WaitGameStart,
	PlayGame,
};


#define		RECV_WAIT		1
#define		WAIT_COUNT		768


//プロトタイプ宣言
int MainLoop(char *szIPAddress, int nPort);
void SleepWait(unsigned long dwWait = RECV_WAIT);
int OnStateWaitRequestUserID(CC2SProtcol *CpC2SP, char *szUserID, EGPROTCOL egp, ClientState *pState);
int OnStateWaitGameStart(CC2SProtcol *CpC2SP, EGPROTCOL egp, ClientState *pState);
int OnStatePlayGame(CC2SProtcol *CpC2SP, EGPROTCOL egp, ClientState *pState);
int OnCmdGetUserID(CC2SProtcol *CpC2SP, char *szUserID);
int OnCmdStartGame(CC2SProtcol *CpC2SP);
int OnCmdEndGame(CC2SProtcol *CpC2SP, EGPROTCOL egp);
int OnCmdMovePhase(CC2SProtcol *CpC2SP, EGPROTCOL egp);
int OnCmdMoveResult(CC2SProtcol *CpC2SP, EGPROTCOL egp);
int OnCmdExistApp(CC2SProtcol *CpC2SP);


/// <summary>
/// メイン
/// </summary>
/// <param name="argc">引数の数</param>
/// <param name="argv">引数の文字列リスト</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int main(int argc, char *argv[])
{
	int		nRes = 0;
	char	*cpIPAddress = NULL;
	int		nPort = 0;
	
	
	if( 3 > argc )
	{
		printf("引数が不足しています。");
		return	1;
	}

	//引数から情報を取得する
	//argv[1]: サーバーのIPアドレス
	//argv[2]: サーバーのポート番号
	cpIPAddress = argv[1];
	nPort = atoi(argv[2]);
	
	
	//winsockを初期化する
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(WINSOCK_VERSION, &wsaData);
#endif //_WIN32

	//メイン処理を行う
	nRes = MainLoop( cpIPAddress, nPort );

	//winsockを終了する
#ifdef _WIN32
	WSACleanup();
#endif //_WIN32

    return nRes;
}


/// <summary>
/// メインループ
/// </summary>
/// <param name="szIPAddress">サーバーのIPアドレス</param>
/// <param name="nPort">サーバーのポート番号</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int MainLoop(char *szIPAddress, int nPort)
{
	int						nRes = 0;
	char					cUserID[MAX_USERID];
	CC2SProtcol				CC2SP;
	bool					bEnd;
	bool					bRes;
	ClientState				state;
	EGPROTCOL				egp;


	//ユーザIDを取得する
	memset(cUserID, 0, sizeof(cUserID));
	nRes = GetUserID( cUserID, sizeof(cUserID) );
	if( (0!= nRes) || (0 == strlen(cUserID)))
	{
		return	1;
	}
	
	//サーバーと接続する
	bRes = CC2SP.Connect(szIPAddress, nPort);
	if (true != bRes)
	{
		return	1;
	}
	
	state = WaitRequestUserID;

	bEnd = false;
	while (true != bEnd)
	{
		//コマンドを受信する
		memset(&egp, 0, sizeof(egp));
		egp.CmdInfo.wCommand = CMD_NONE;
		bRes = CC2SP.RecvCommand(&egp);
		if ( true != bRes)
		{
			nRes = 1;
			break;
		}

		if (CMD_EXIT_APP == egp.CmdInfo.wCommand)
		{
			//終了コマンドはどの状態よりも優先される
			nRes = OnCmdExistApp( &CC2SP);
			bEnd = true;
		}
		else if (CMD_NONE != egp.CmdInfo.wCommand)
		{
			//状態に応じた処理を行う
			switch(state)
			{
				case WaitRequestUserID:
					nRes = OnStateWaitRequestUserID( &CC2SP, cUserID, egp, &state);
					break;
				case WaitGameStart:
					nRes = OnStateWaitGameStart(&CC2SP, egp, &state);
					break;
				case PlayGame:
					nRes = OnStatePlayGame(&CC2SP, egp, &state);
					break;
				default:
					break;
			}

			if (0 != nRes)
			{
				bEnd = true;
			}
		}

		SleepWait();
	}
	
	//サーバーから切断する
	bRes = CC2SP.Disconnect();
	if (true != bRes)
	{
		nRes = 1;
	}
	
	return	nRes;
}


/// <summary>
/// Sleepでウェイトをかける
/// </summary>
/// <param name="dwWait">Sleepの時間(ミリ秒)</param>
void SleepWait(unsigned long dwWait)
{
	static int	nCounter = 0;
	const int	nMaxCounter = WAIT_COUNT;

	if (0 < dwWait)
	{
		nCounter++;
		if (nMaxCounter < nCounter)
		{
			Sleep(dwWait);
			nCounter = 0;
		}
	}
}


/// <summary>
/// ユーザーID取得要求待ち状態
/// </summary>
/// <param name="CpC2SP">クライアント通信監理クラス</param>
/// <param name="szUserID">ユーザID</param>
/// <param name="egp">受信したコマンド</param>
/// <param name="pState">処理の状態</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int OnStateWaitRequestUserID(CC2SProtcol *CpC2SP, char *szUserID, EGPROTCOL egp, ClientState *pState)
{
	int		nRes = 0;


	if ( (NULL == CpC2SP) || (NULL == szUserID) || (NULL == pState))
	{
		return	1;
	}

	switch( egp.CmdInfo.wCommand )
	{
		case CMD_GET_USERID:
			nRes = OnCmdGetUserID(CpC2SP, szUserID);
			if (0 == nRes)
			{
				*pState = WaitGameStart;
			}
			break;
		default:
			return	0;
	}

	return	nRes;
}


/// <summary>
/// ゲーム開始通知待ち状態
/// </summary>
/// <param name="CpC2SP">クライアント通信監理クラス</param>
/// <param name="egp">受信したコマンド</param>
/// <param name="pState">処理の状態</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int OnStateWaitGameStart(CC2SProtcol *CpC2SP, EGPROTCOL egp, ClientState *pState)
{
	int	nRes = 0;
	

	if ((NULL == CpC2SP) || (NULL == pState))
	{
		return	1;
	}

	switch (egp.CmdInfo.wCommand)
	{
		case CMD_START_GAME:
			nRes = OnCmdStartGame(CpC2SP);
			if ( 0 == nRes )
			{
				*pState = PlayGame;
			}
			break;
		default:
			return	0;
	}
	
	return	nRes;
}


/// <summary>
/// ゲームプレイ中状態
/// </summary>
/// <param name="CpC2SP">クライアント通信監理クラス</param>
/// <param name="egp">受信したコマンド</param>
/// <param name="pState">処理の状態</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int OnStatePlayGame(CC2SProtcol *CpC2SP, EGPROTCOL egp, ClientState *pState)
{
	int	nRes = 0;


	if ((NULL == CpC2SP) || (NULL == pState))
	{
		return	1;
	}
	
	switch(egp.CmdInfo.wCommand)
	{
		case CMD_END_GAME:
			nRes = OnCmdEndGame(CpC2SP, egp);
			if (0 == nRes)
			{
				*pState = WaitGameStart;
			}
			break;
		case CMD_MOVE_PHASE:
			nRes = OnCmdMovePhase(CpC2SP, egp);
			break;
		case CMD_MOVE_RESULT:
			nRes = OnCmdMoveResult(CpC2SP, egp);
			break;
		default:
			return	0;
	}

	return	nRes;
}


/// <summary>
/// ユーザID取得コマンドの処理を行う
/// </summary>
/// <param name="CpC2SP">クライアント通信監理クラス</param>
/// <param name="szUserID">ユーザID</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int OnCmdGetUserID(CC2SProtcol *CpC2SP, char *szUserID )
{
	bool	bRes;


	if ((NULL == CpC2SP) || (NULL == szUserID))
	{
		return	1;
	}

	bRes = CpC2SP->ReplyGetUserID(szUserID);
	if (true != bRes)
	{
		return	1;
	}
	
	return	0;
}


/// <summary>
/// ゲーム開始コマンドの処理を行う
/// </summary>
/// <param name="CpC2SP">クライアント通信監理クラス</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int OnCmdStartGame(CC2SProtcol *CpC2SP)
{
	int		nRes = 0;
	bool	bRes;


	if(NULL == CpC2SP)
	{
		return	1;
	}
	
	nRes = StartGame();
	
	bRes = CpC2SP->ReplyStartGame();
	if (true != bRes)
	{
		return	1;
	}

	return	nRes;
}


/// <summary>
/// ゲーム終了コマンドの処理を行う
/// </summary>
/// <param name="CpC2SP">クライアント通信監理クラス</param>
/// <param name="egp">コマンド情報</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int OnCmdEndGame(CC2SProtcol *CpC2SP, EGPROTCOL egp)
{
	int		nRes = 0;
	bool	bRes;

	
	if (NULL == CpC2SP)
	{
		return	1;
	}

	nRes = EndGame(egp.CmdInfo.Parqam.GameEndInfo);

	bRes = CpC2SP->ReplyEndGame();
	if (true != bRes)
	{
		return	1;
	}

	return	nRes;
}


/// <summary>
/// 移動フェーズコマンドの処理を行う
/// </summary>
/// <param name="CpC2SP">クライアント通信監理クラス</param>
/// <param name="egp">コマンド情報</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int OnCmdMovePhase(CC2SProtcol *CpC2SP, EGPROTCOL egp)
{
	int		nRes = 0;
	MOVEPIECEINFO	mpi;
	bool	bRes;
	
	
	if (NULL == CpC2SP)
	{
		return	1;
	}

	memset(&mpi, 0, sizeof(mpi));
	nRes = MovePhase(egp.CmdInfo.Parqam.GameInfo, &mpi);
	
	bRes = CpC2SP->ReplyMovePhase(mpi);
	if (true != bRes)
	{
		return	1;
	}

	return	nRes;
}


/// <summary>
/// 移動結果コマンドの処理を行う
/// </summary>
/// <param name="CpC2SP">クライアント通信監理クラス</param>
/// <param name="egp">コマンド情報</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int OnCmdMoveResult(CC2SProtcol *CpC2SP, EGPROTCOL egp)
{
	int		nRes = 0;
	bool	bRes;


	if (NULL == CpC2SP)
	{
		return	1;
	}

	nRes = MovePhaseResult(egp.CmdInfo.Parqam.MoveResult);

	bRes = CpC2SP->ReplyMoveResult();
	if (true != bRes)
	{
		return	1;
	}

	return	nRes;
}


/// <summary>
/// アプリケーション終了コマンドの処理を行う
/// </summary>
/// <param name="CpC2SP">クライアント通信監理クラス</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int OnCmdExistApp(CC2SProtcol *CpC2SP)
{
	int		nRes = 0;
	bool	bRes;


	if (NULL == CpC2SP)
	{
		return	1;
	}
	
	bRes = CpC2SP->ReplyExitApp();
	if (true != bRes)
	{
		return	1;
	}

	return	nRes;
}

