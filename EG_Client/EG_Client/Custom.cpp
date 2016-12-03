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
	cpUserID = (char*)"EatMoreCakes";
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
	DIRECTION	ucDirection;

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

	ucDirection = GetNextPieceDirection( &GameInfo, pPiece );
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
		}
		return TERRITORY_ENEMY;
	}
}

void GetMovableTerritoryCount( GAMEINFO* pGameInfo, PIECE* pPiece, int TerritoryType, int* paTerritoryCount )
{
	int	x;
	int	y;

	int	iRangeXMin;
	int	iRangeXMax;
	int	iRangeYMin;
	int	iRangeYMax;

	if( pPiece->nSleep != 0 ) {
		return;
	}

	iRangeXMin = constrain( pPiece->nX - 1, 0, MAP_WIDTH  - 1 );
	iRangeXMax = constrain( pPiece->nX + 1, 0, MAP_WIDTH  - 1 );
	iRangeYMin = constrain( pPiece->nY - 1, 0, MAP_HEIGHT - 1 );
	iRangeYMax = constrain( pPiece->nY + 1, 0, MAP_HEIGHT - 1 );

#if 0
	y = iRangeYMin;
	x = iRangeXMin;
	if( ToDirection( pPiece, x, y ) == DIRECTION_UP_LEFT ) {
		if( IsMovableTerritory( pGameInfo, pPiece, x, y ) ) {
			if( GetTerritoryType( pGameInfo, x, y ) == TerritoryType ) {
				paTerritoryCount[DIRECTION_UP_LEFT] = 1;
			}
		}
	}
	x = iRangeXMax;
	if( ToDirection( pPiece, x, y ) == DIRECTION_UP_RIGHT ) {
		if( IsMovableTerritory( pGameInfo, pPiece, x, y ) ) {
			if( GetTerritoryType( pGameInfo, x, y ) == TerritoryType ) {
				paTerritoryCount[DIRECTION_UP_RIGHT] = 1;
			}
		}
	}

	y = iRangeYMax;
	x = iRangeXMin;
	if( ToDirection( pPiece, x, y ) == DIRECTION_DOWN_LEFT ) {
		if( IsMovableTerritory( pGameInfo, pPiece, x, y ) ) {
			if( GetTerritoryType( pGameInfo, x, y ) == TerritoryType ) {
				paTerritoryCount[DIRECTION_DOWN_LEFT] = 1;
			}
		}
	}
	x = iRangeXMax;
	if( ToDirection( pPiece, x, y ) == DIRECTION_DOWN_RIGHT ) {
		if( IsMovableTerritory( pGameInfo, pPiece, x, y ) ){
			if( GetTerritoryType( pGameInfo, x, y ) == TerritoryType ) {
				paTerritoryCount[DIRECTION_DOWN_RIGHT] = 1;
			}
		}
	}
#endif

	y = pPiece->nY;
	for( x = pPiece->nX + 1; x < MAP_WIDTH; x++ ) {
		if( IsMovableTerritory( pGameInfo, pPiece, x, y ) &&
			GetTerritoryType( pGameInfo, x, pPiece->nY ) == TerritoryType )
		{
			paTerritoryCount[DIRECTION_RIGHT]++;
		}
	}
	for( x = pPiece->nX - 1; x >= 0; x-- ) {
		if( IsMovableTerritory( pGameInfo, pPiece, x, y ) &&
			GetTerritoryType( pGameInfo, x, pPiece->nY ) == TerritoryType )
		{
			paTerritoryCount[DIRECTION_LEFT]++;
		}
	}

	x = pPiece->nX;
	for( y = pPiece->nY + 1; y < MAP_HEIGHT; y++ ) {
		if( IsMovableTerritory( pGameInfo, pPiece, x, y ) &&
			GetTerritoryType( pGameInfo, pPiece->nX, y ) == TerritoryType )
		{
			paTerritoryCount[DIRECTION_DOWN]++;
		}
	}
	for( y = pPiece->nY - 1; y >= 0; y-- ) {
		if( IsMovableTerritory( pGameInfo, pPiece, x, y ) &&
			GetTerritoryType( pGameInfo, pPiece->nX, y ) == TerritoryType )
		{
			paTerritoryCount[DIRECTION_UP]++;
		}
	}
}

DIRECTION GetNextPieceDirection( GAMEINFO* pGameInfo, PIECE* pPiece )
{
	int			aTerritoryCount[DIRECTION_ALL] = { 0 };
	DIRECTION	ucDirection = DIRECTION_NONE;

	if( pPiece->nSleep != 0 ) {
		return ucDirection;
	}

	if( pPiece->nType == PIECETYPE_COMMON ) {
		/* 1.未占領地 */
		GetMovableTerritoryCount( pGameInfo, pPiece, TERRITORY_BLANK, &aTerritoryCount[0] );
		ucDirection = GetMinTerritoryDirection( &aTerritoryCount[0] );
		if( ucDirection != DIRECTION_NONE ) {
			return ucDirection;
		}
		/* 2.敵占領地 */
		GetMovableTerritoryCount( pGameInfo, pPiece, TERRITORY_ENEMY, &aTerritoryCount[0] );
		ucDirection = GetMaxTerritoryDirection( &aTerritoryCount[0] );
		if( ucDirection != DIRECTION_NONE ) {
			return ucDirection;
		}
		/* 3.味方占領地 */
		GetMovableTerritoryCount( pGameInfo, pPiece, TERRITORY_FRIEND, &aTerritoryCount[0] );
		ucDirection = GetMaxTerritoryDirection( &aTerritoryCount[0] );
		if( ucDirection != DIRECTION_NONE ) {
			return ucDirection;
		}
	}
	else {
		/* 1.敵占領地 */
		GetMovableTerritoryCount( pGameInfo, pPiece, TERRITORY_ENEMY, &aTerritoryCount[0] );
		ucDirection = GetMaxTerritoryDirection( &aTerritoryCount[0] );
		if( ucDirection != DIRECTION_NONE ) {
			return ucDirection;
		}
		/* 2.未占領地 */
		GetMovableTerritoryCount( pGameInfo, pPiece, TERRITORY_BLANK, &aTerritoryCount[0] );
		ucDirection = GetMinTerritoryDirection( &aTerritoryCount[0] );
		if( ucDirection != DIRECTION_NONE ) {
			return ucDirection;
		}
		/* 3.味方占領地 */
		GetMovableTerritoryCount( pGameInfo, pPiece, TERRITORY_FRIEND, &aTerritoryCount[0] );
		ucDirection = GetMaxTerritoryDirection( &aTerritoryCount[0] );
		if( ucDirection != DIRECTION_NONE ) {
			return ucDirection;
		}
	}

	return ucDirection;
}

DIRECTION ToDirection( PIECE* pPiece, unsigned int iDestX, unsigned int iDestY )
{
	int iDistanceX;
	int iDistanceY;

	DIRECTION ucDirection = DIRECTION_NONE;

	iDistanceX = iDestX - pPiece->nX;
	iDistanceY = iDestY - pPiece->nY;

	if( iDistanceX > 0 ) {
		ucDirection |= DIRECTION_RIGHT;
	}
	else if( iDistanceX < 0 ) {
		ucDirection |= DIRECTION_LEFT;
	}
	else {
		/* Do Nothing */
	}

	if( iDistanceY > 0 ) {
		ucDirection |= DIRECTION_DOWN;
	}
	else if( iDistanceY < 0 ) {
		ucDirection |= DIRECTION_UP;
	}
	else {
		/* Do Nothing */
	}

	return ucDirection;
}

DIRECTION GetMinTerritoryDirection( int* paTerritoryCount )
{
	INT16		i;
	INT16		iMinCount = 9999;
	DIRECTION	MinDirection = DIRECTION_NONE;

	for( i = 0; i < DIRECTION_ALL; i++ ) {
		if( paTerritoryCount[i] == 0 ) {
			continue;
		}

		if( iMinCount > paTerritoryCount[i] ) {
			iMinCount = paTerritoryCount[i];
			MinDirection = (DIRECTION)i;
		}
	}

	return MinDirection;
}

DIRECTION GetMaxTerritoryDirection( int* paTerritoryCount )
{
	INT16		i;
	INT16		iMaxCount = -9999;
	DIRECTION	MaxDirection = DIRECTION_NONE;

	for( i = 0; i < DIRECTION_ALL; i++ ) {
		if( paTerritoryCount[i] == 0 ) {
			continue;
		}

		if( iMaxCount < paTerritoryCount[i] ) {
			iMaxCount = paTerritoryCount[i];
			MaxDirection = (DIRECTION)i;
		}
	}

	return MaxDirection;
}

void SetMovePieceDirection( MOVEPIECEINFO *pMovePiece, DIRECTION ucDirection )
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

bool IsMovableTerritory( GAMEINFO* pGameInfo, PIECE* pPiece, int x, int y )
{
	/* 移動範囲内で、自身も含めて味方のコマがいない場所 */
	if( IsExistFriendPiece( &( pGameInfo->Player[0] ), x, y ) == false ) {
		if( abs( x - pPiece->nX ) > 0 && abs( y - pPiece->nY ) > 0 ) {
			/* 斜めの位置は、特殊コマで斜め移動回数が残っているときだけ移動可能 */
			if( (pPiece->nType != PIECETYPE_COMMON) && (pGameInfo->Player[0].nBootCount > 0) ) {
				return true;
			}
		}
		else{
			return true;
		}
	}

	return false;
}
