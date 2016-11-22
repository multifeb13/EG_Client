#include "stdafx.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <string.h>
#include <stdlib.h>
#endif //_WIN32
#include "Custom.h"
#include "StringUtility.h"
#include <time.h>	//Sample//


/// <summary>
/// ユーザIDを取得する
/// </summary>
/// <param name="cpBuffer">ユーザIDを格納するバッファ</param>
/// <param name="nBufferSize">ユーザIDを格納するバッファサイズ</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int GetUserID(char *cpBuffer, unsigned int nBufferSize)
{
	int		nRes = 0;
	bool	bRes;
	char	*cpUserID;

	//TODO: 自分のユーザIDを設定してください

	//////////
	// Sample

	// UserIDに「My UserID」を設定する
	cpUserID = (char*)"My UserID";
	// Sample
	//////////


	if( NULL == cpBuffer )
	{
		return	1;
	}
	
    bRes = true;

#ifdef _WIN32
	bRes = ShiftJisToUtf8(cpUserID, cpBuffer, nBufferSize);
	if ( true != bRes )
	{
		return	1;
	}
#else
    strcpy(cpBuffer,cpUserID);
#endif	//_WIN32
	
	return	nRes;
}


/// <summary>
/// ゲームの開始通知
/// </summary>
/// <returns>結果（0:成功, その他:エラー）</returns>
int StartGame(void)
{
	//TODO: 初期化が必要な場合はここで行う

	//////////
	// Sample
	
	//ゲーム開始を表示する
	printf("\r\n");
	printf("/////////////////////////\r\n");
	printf("// StartGame             \r\n");
	printf("//                       \r\n");
	srand((unsigned int)time(NULL));
	// Sample
	//////////

	return	0;
}


/// <summary>
/// ゲームの終了通知
/// </summary>
/// <param name="GameEndInfo">終了結果</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int EndGame(GAMEENDINFO GameEndInfo)
{
	//TODO: 終了処理が必要な場合はここで行う

	//////////
	// Sample
	
	//ゲームの結果を表示する
	char	*cpState;
	switch (GameEndInfo.nWinner)
	{
		case GAMERESULT_WIN:
			cpState = (char*)"Win";
			break;
		case GAMERESULT_LOSE:
			cpState = (char*)"Lose";
			break;
		case GAMERESULT_DRAW:
			cpState = (char*)"Draw";
			break;
		default:
			cpState = (char*)"?";
			break;
	}
	printf("\r\n");
	printf("/////////////////////////\r\n");
	printf("// MovePhaseResult       \r\n");
	printf("//                       \r\n");
	printf("// Winner = %s           \r\n", cpState);
	printf("// MyTerritory = %d      \r\n", GameEndInfo.nMyTerritory);
	printf("// OtherTerritory = %d   \r\n", GameEndInfo.nOtherTerritory);
	// Sample
	//////////

	return	0;
}


/// <summary>
/// 移動フェーズ通知
/// </summary>
/// <param name="GameInfo">地図やコマの情報</param>
/// <param name="pMovePiece">移動情報へのポインタ</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int MovePhase(GAMEINFO GameInfo, MOVEPIECEINFO *pMovePiece)
{
	if( pMovePiece == NULL ) {
		return	1;
	}

	//TODO: GameInfoを元にpMovePieceに移動データを入力する
	//      動かせない場合はpMovePiece->nStepに0を設定する

	//ランダムで移動する
	const char	cDirection[] = {	DIRECTION_UP,
									DIRECTION_DOWN,
									DIRECTION_LEFT,
									DIRECTION_RIGHT };
	int		i;
	PIECE	*pPiece;

	memset(pMovePiece, 0, sizeof(MOVEPIECEINFO) );
	pMovePiece->nOrder = GameInfo.nOrder;
	
	pPiece = NULL;
	for( i=0 ; i<MAX_PIECE ; i++ ) {
		if( GameInfo.nOrder == GameInfo.PlayerPiece[0].Piece[i].nOrder) {
			pPiece = &GameInfo.PlayerPiece[0].Piece[i];
			break;
		}
	}

	if( pPiece != NULL ) {
		if( pPiece->nSleep == 0 ) {
			if( GameInfo.PlayerPiece[0].nBootCount > 0 ) {
				pMovePiece->nStep = (0 == (rand() % 50)) ? 2 : 1;
			}
			else {
				pMovePiece->nStep = 1;
			}

			for( i = 0; i < pMovePiece->nStep; i++ ) {
				pMovePiece->cDirection[i] = cDirection[rand() % 4];
			}
		}
		else {
			pMovePiece->nStep = 0;
		}
	}
	// Sample
	//////////

	return	0;
}


/// <summary>
/// 移動フェーズの結果通知
/// </summary>
/// <param name="result">移動結果</param>
/// <returns>結果（0:成功, その他:エラー）</returns>
int MovePhaseResult( MOVEPIECERESULT	result )
{
	//TODO: 移動フェーズの結果で処理が必要な場合はここで行う

	//////////
	// Sample 

	//移動結果を表示する
	printf("\r\n");
	printf("/////////////////////////\r\n");
	printf("// MovePhaseResult       \r\n");
	printf("//                       \r\n");
	printf("// Result = %d           \r\n", result.nResult );
	// Sample 
	//////////

	return	0;
}
