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

enum {
	RESULT_FOUND_UNOCCUPATION = 1,
	RESULT_FOUND_ENEMYOCCUPATION,
	RESULT_FOUND_FAMILYOCCUPATION,
	RESULT_NOT_FOUND,
};

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
	cpUserID = (char*)"EatMoreCake";
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
int MovePhase( GAMEINFO GameInfo, MOVEPIECEINFO *pMovePiece )
{
	int				i;
	PIECE*			pPiece = NULL;
	unsigned int	uiPosX;
	unsigned int	uiPosY;
	unsigned char	ucDirection;

	if( pMovePiece == NULL ) {
		return	1;
	}
	else {
		memset( pMovePiece, 0, sizeof( MOVEPIECEINFO ) );
		pMovePiece->nOrder = GameInfo.nOrder;
	}
	
	for( i = 0; i < MAX_PIECE; i++ ) {
		if( GameInfo.nOrder == GameInfo.Player[0].Piece[i].nOrder ) {
			break;
		}
	}
	if( i >= MAX_PIECE ) {
		return 1;
	}
	else {
		pPiece = &GameInfo.Player[0].Piece[i];
		if( pPiece == NULL ) {
			return 1;
		}
	}

	GetNextMapPosition( &uiPosX, &uiPosY, &GameInfo, pPiece );
	ucDirection = ToDirection( pPiece, uiPosX, uiPosY );
	SetMovePieceDirection( pMovePiece, ucDirection );

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

void GetNextMapPosition( unsigned int* uiPosX, unsigned int* uiPosY, GAMEINFO* pGameInfo, PIECE* pPiece )
{
	int				x;
	int				y;

	unsigned int	uiRangeXMin;
	unsigned int	uiRangeXMax;
	unsigned int	uiRangeYMin;
	unsigned int	uiRangeYMax;

	bool			bMovableArea[MAP_HEIGHT][MAP_WIDTH];

	if( pGameInfo == NULL || pPiece == NULL ) {
		return;
	}
	if( pPiece->nSleep != 0 ) {
		*uiPosX = pPiece->nX;
		*uiPosY = pPiece->nY;
		return;
	}

	uiRangeXMin = (unsigned int)constrain( pPiece->nX - 1, 0, MAP_WIDTH  - 1 );
	uiRangeXMax = (unsigned int)constrain( pPiece->nX + 1, 0, MAP_WIDTH  - 1 );
	uiRangeYMin = (unsigned int)constrain( pPiece->nY - 1, 0, MAP_HEIGHT - 1 );
	uiRangeYMax = (unsigned int)constrain( pPiece->nY + 1, 0, MAP_HEIGHT - 1 );

	for( y = 0; y < MAP_HEIGHT; y++ ){
		for( x = 0; x < MAP_WIDTH; x++ ){
			bMovableArea[y][x] = false;
		}
	}
	for( y = (int)uiRangeYMin; y <= (int)uiRangeYMax; y++ ) {
		for( x = (int)uiRangeXMin; x <= (int)uiRangeXMax; x++ ) {
			/* 移動範囲内で、自身も含めて味方のコマがいない場所 */
			if( IsExistFriendPiece( &( pGameInfo->Player[0] ), x, y ) == false ) {
				if( abs( x - pPiece->nX ) > 0 && abs( y - pPiece->nY ) > 0 ) {
					/* 斜めの位置は、特殊コマで斜め移動回数が残っているときだけ移動可能 */
					if( (pPiece->nType != PIECETYPE_COMMON) && (pGameInfo->Player[0].nBootCount > 0) ) {
						bMovableArea[y][x] = true;
					}
				}
				else{
					bMovableArea[y][x] = true;
				}
			}
		}
	}

	/* 移動すべき位置を探す */
	switch( pPiece->nType ) {
		case PIECETYPE_COMMON:
			for( y = (int)uiRangeYMin; y <= (int)uiRangeYMax; y++ ) {
				for( x = (int)uiRangeXMin; x <= (int)uiRangeXMax; x++ ) {
					if( bMovableArea[y][x] == false ) {
						continue;
					}

					if( GetTerritoryType( pGameInfo, x, y ) == TERRITORY_BLANK ) {
						/* 1.未占領地 */
						*uiPosX = x;
						*uiPosY = y;
						return;
					}
					else if( GetTerritoryType( pGameInfo, x, y ) == TERRITORY_ENEMY ) {
						/* 2.敵占領地 */
						*uiPosX = x;
						*uiPosY = y;
						return;
					}
					else if( GetTerritoryType( pGameInfo, x, y ) == TERRITORY_FRIEND ) {
						/* 3.味方占領地 */
						*uiPosX = x;
						*uiPosY = y;
						return;
					}
				}
			}
			break;
		default:
			for( y = (int)uiRangeYMin; y <= (int)uiRangeYMax; y++ ) {
				for( x = (int)uiRangeXMin; x <= (int)uiRangeXMax; x++ ) {
					if( bMovableArea[y][x] == false ) {
						continue;
					}

					if( pGameInfo->Map[y][x].cTerritory == TERRITORY_BLANK ) {
						/* 1.敵占領地 */
						*uiPosX = x;
						*uiPosY = y;
						return;
					}
					else if( GetTerritoryType( pGameInfo, x, y ) == TERRITORY_BLANK ) {
						/* 2.未占領地 */
						*uiPosX = x;
						*uiPosY = y;
						return;
					}
					else if( GetTerritoryType( pGameInfo, x, y ) == TERRITORY_FRIEND ) {
						/* 3.味方占領地 */
						*uiPosX = x;
						*uiPosY = y;
						return;
					}
				}
			}
			break;
	}

	return;
}

int constrain( int value, int min, int max )
{
	int iConstrained;

	if( min > value ) {
		iConstrained = min;
	}
	else if( value > max ) {
		iConstrained = max;
	}
	else {
		iConstrained = value;
	}

	return iConstrained;
}

bool IsExistFriendPiece( PLAYERPIECE* pPlayer, int x, int y )
{
	int	i;

	for( i = 0; i < MAX_PIECE; i++ ) {
		if( pPlayer[0].Piece[i].nX == x && pPlayer[0].Piece[i].nY == y ) {
			return true;
		}
	}

	return false;
}

unsigned int GetTerritoryType( GAMEINFO* pGameInfo, int x, int y )
{
	if( pGameInfo->Map[y][x].cTerritory == TERRITORY_BLANK ) {
		return TERRITORY_BLANK;
	}
	else {
		for( int i = 0; i < MAX_PIECE; i++ ) {
			if( pGameInfo->Player[0].Piece[i].nOrder == pGameInfo->Map[y][x].cTerritory ) {
				return TERRITORY_FRIEND;
			}
			return TERRITORY_ENEMY;
		}
	}

	return TERRITORY_BLANK;
}

unsigned char ToDirection( PIECE* pPiece, unsigned int iDestX, unsigned int iDestY )
{
	int iDistanceX;
	int iDistanceY;
	int	i = 0;
	unsigned int uiDirection = DIRECTION_NONE;

	iDistanceX = iDestX - pPiece->nX;
	if( iDistanceX < 0 ) {
		uiDirection |= DIRECTION_LEFT;
	}
	else if( iDistanceX > 0 ) {
		uiDirection |= DIRECTION_RIGHT;
	}

	iDistanceY = iDestY - pPiece->nY;
	if( iDistanceY < 0 ) {
		uiDirection |= DIRECTION_UP;
	}
	else if( iDistanceY > 0 ) {
		uiDirection |= DIRECTION_DOWN;
	}

	return uiDirection;
}

void SetMovePieceDirection( MOVEPIECEINFO *pMovePiece, unsigned char ucDirection )
{
	if( ucDirection & DIRECTION_UP ) {
		pMovePiece->cDirection[pMovePiece->nStep] = DIRECTION_UP;
		pMovePiece->nStep++;
	}
	if( ucDirection & DIRECTION_DOWN ) {
		pMovePiece->cDirection[pMovePiece->nStep] = DIRECTION_DOWN;
		pMovePiece->nStep++;
	}
	if( ucDirection & DIRECTION_RIGHT ) {
		pMovePiece->cDirection[pMovePiece->nStep] = DIRECTION_RIGHT;
		pMovePiece->nStep++;
	}
	if( ucDirection & DIRECTION_LEFT ) {
		pMovePiece->cDirection[pMovePiece->nStep] = DIRECTION_LEFT;
		pMovePiece->nStep++;
	}
}
