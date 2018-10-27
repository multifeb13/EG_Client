
#include "stdafx.h"
#include "C2SProtcol.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <string.h>
#endif //_WIN32
#include "ByteOrder.h"


/// <summary>
/// コンストラクタ
/// </summary>
CC2SProtcol::CC2SProtcol()
{
}


/// <summary>
/// デストラクタ
/// </summary>
CC2SProtcol::~CC2SProtcol()
{
	Disconnect();
}


/// <summary>
/// ソケットを接続する
/// </summary>
/// <param name="szIPAddress">IPアドレス</param>
/// <param name="nPort">ポート番号</param>
/// <returns>接続結果</returns>
bool CC2SProtcol::Connect(const char *szIPAddress, int nPort)
{
	return	m_CSocket.Connect(szIPAddress, nPort);
}


/// <summary>
/// ソケットを切断する
/// </summary>
/// <returns>切断結果</returns>
bool CC2SProtcol::Disconnect(void)
{
	return	m_CSocket.Disconnect();
}


/// <summary>
/// コマンドを受信する
/// </summary>
/// <param name="pEgp">受信したコマンドを格納する構造体へのポインタ</param>
/// <returns>受信結果</returns>
bool CC2SProtcol::RecvCommand(EGPROTCOL *pEgp)
{
	bool			bRes;
	GAMEINFO		*pGameInfo;
	MOVEPIECERESULT	*pMoveResult;
	GAMEENDINFO		*pGameEndInfo;
	int				i, j;

	
	bRes = CSocketProtcol::RecvCommand(pEgp);
	if (true != bRes)
	{
		return	bRes;
	}
	
	switch( pEgp->CmdInfo.wCommand )
	{
		case CMD_MOVE_PHASE:
			pGameInfo = &(pEgp->CmdInfo.Parqam.GameInfo);
			pGameInfo->nTurn = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pGameInfo->nTurn);
			pGameInfo->nOrder = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pGameInfo->nOrder);
			for (i = 0; i < MAX_GAMEPLAYER; i++)
			{
				pGameInfo->PlayerPiece[i].nBootCount = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pGameInfo->PlayerPiece[i].nBootCount);
				
				for (j = 0; j < MAX_PIECE; j++)
				{
					pGameInfo->PlayerPiece[i].Piece[j].nX = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pGameInfo->PlayerPiece[i].Piece[j].nX);
					pGameInfo->PlayerPiece[i].Piece[j].nY = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pGameInfo->PlayerPiece[i].Piece[j].nY);
					pGameInfo->PlayerPiece[i].Piece[j].nOrder = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pGameInfo->PlayerPiece[i].Piece[j].nOrder);
					pGameInfo->PlayerPiece[i].Piece[j].nType = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pGameInfo->PlayerPiece[i].Piece[j].nType);
					pGameInfo->PlayerPiece[i].Piece[j].nSleep = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pGameInfo->PlayerPiece[i].Piece[j].nSleep);
				}
			}
			break;
		case CMD_MOVE_RESULT:
			pMoveResult = &(pEgp->CmdInfo.Parqam.MoveResult);
			pMoveResult->nResult = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pMoveResult->nResult);
			break;
		case CMD_END_GAME:
			pGameEndInfo = &(pEgp->CmdInfo.Parqam.GameEndInfo);
			pGameEndInfo->nWinner = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pGameEndInfo->nWinner);
			pGameEndInfo->nMyTerritory = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pGameEndInfo->nMyTerritory);
			pGameEndInfo->nOtherTerritory = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, pGameEndInfo->nOtherTerritory);
			break;
	}
	
	return	true;
}


/// <summary>
/// ユーザID取得に応答する
/// </summary>
/// <param name="szUserID">ユーザID</param>
/// <returns>応答結果</returns>
bool CC2SProtcol::ReplyGetUserID(const char* szUserID)
{
	EGPROTCOL		egp;
	
	if (NULL == szUserID)
	{
		return	false;
	}
	
	memset(&egp, 0, sizeof(egp));
	memcpy_s(egp.CmdInfo.Parqam.cUserID, sizeof(egp.CmdInfo.Parqam.cUserID), szUserID, strlen(szUserID));

	return	SendCommand( CMD_GET_USERID, egp.CmdInfo.Parqam.cUserID, sizeof(egp.CmdInfo.Parqam.cUserID));
}


/// <summary>
/// ゲーム開始に応答する
/// </summary>
/// <returns>応答結果</returns>
bool CC2SProtcol::ReplyStartGame(void)
{
	return	SendCommand(CMD_START_GAME);
}


/// <summary>
/// ゲーム終了に応答する
/// </summary>
/// <returns>応答結果</returns>
bool CC2SProtcol::ReplyEndGame(void)
{
	return	SendCommand(CMD_END_GAME);
}


/// <summary>
/// 移動依頼に応答する
/// </summary>
/// <param name="MovePieceInfo">移動情報</param>
/// <returns>応答結果</returns>
bool CC2SProtcol::ReplyMovePhase(MOVEPIECEINFO MovePieceInfo)
{
	MOVEPIECEINFO	mpi;

	mpi = MovePieceInfo;
	mpi.nOrder = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, mpi.nOrder);
	mpi.nStep = ENDIAN_SWAP4BYTE(ENDIAN_LITTLE, mpi.nStep);
	
	return	SendCommand(CMD_MOVE_PHASE, &mpi, sizeof(mpi) );
}


/// <summary>
/// 移動結果に応答する
/// </summary>
/// <returns>応答結果</returns>
bool CC2SProtcol::ReplyMoveResult(void)
{
	return	SendCommand(CMD_MOVE_RESULT);
}


/// <summary>
/// アプリケーション終了に応答する
/// </summary>
/// <returns>応答結果</returns>
bool CC2SProtcol::ReplyExitApp(void)
{
	return	SendCommand(CMD_EXIT_APP);
}

