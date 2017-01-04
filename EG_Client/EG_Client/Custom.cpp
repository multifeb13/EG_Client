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

PIECEAPPEND		PieceAppend[MAX_PIECE];

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
	cpUserID = (char*)"The Last Hope";
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
	int		i;
	int		iPieceNum;

	iPieceNum = sizeof( PieceAppend ) / sizeof( PieceAppend[0] );
	for( i = 0; i < iPieceNum; i++ ) {
		PieceAppend[i].fFirstMove = true;
		PieceAppend[i].DestX = 0;
		PieceAppend[i].DestY = 0;
	}

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
	int			i;
	PIECE*		pPiece = NULL;
	MAP_POS		x;
	MAP_POS		y;
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
		else if( pPiece->nSleep > 0 ) {
			return 0;
		}
	}

	if( PieceAppend[i].fFirstMove == false ) {
		if( pPiece->nType == PIECETYPE_COMMON ) {
			if( pPiece->nX == PieceAppend[i].DestX && pPiece->nY == PieceAppend[i].DestY ) {
				GetPiecePosForNext( &GameInfo, pPiece, &x, &y );
				PieceAppend[i].DestX = x;
				PieceAppend[i].DestY = y;
			}
			else {
				GetPiecePosForMove( pPiece, PieceAppend[i].DestX, PieceAppend[i].DestY, &x, &y );
			}
		}
		else {
			GetPiecePosForNext( &GameInfo, pPiece, &x, &y );
		}
	}
	else {
		GetPiecePosForNext( &GameInfo, pPiece, &x, &y );
		PieceAppend[i].fFirstMove = false;
		PieceAppend[i].DestX = x;
		PieceAppend[i].DestY = y;
	}
	ucDirection = ToDirection( &GameInfo, pPiece, x, y );
	SetPieceDirection( pMovePiece, ucDirection );

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

void FindNearTerritory( GAMEINFO* pGameInfo, PIECE* pPiece, int TerritoryType, MAP_POS* pMapX, MAP_POS* pMapY )
{
	int		x;
	int		y;

	int		iDistanceX;
	int		iDistanceY;
	UINT	uiDistanceXY;

	UINT	uiNeariDistanceXY = 9999;
	UINT	uiNearPosX = 0;
	UINT	uiNearPosY = 0;

	bool	fFound = FALSE;

	for( y = 0; y < MAP_HEIGHT; y++ ) {
		for( x = 0; x < MAP_WIDTH; x++ ) {
			if( GetTerritoryType( pGameInfo, x, y ) != TerritoryType ) {
				continue;
			}
			if( x == pPiece->nX && y == pPiece->nY ) {
				continue;
			}
			if( IsExistFriendPiece( &( pGameInfo->Player[0] ), x, y ) ) {
				continue;
			}

			iDistanceX = pPiece->nX - x;
			iDistanceY = pPiece->nY - y;
			uiDistanceXY = abs( iDistanceX ) + abs( iDistanceY );

			if( uiNeariDistanceXY > uiDistanceXY ) {
				fFound = TRUE;
				uiNeariDistanceXY = uiDistanceXY;
				uiNearPosX = x;
				uiNearPosY = y;
			}
		}
	}

	if( fFound ) {
		*pMapX = uiNearPosX;
		*pMapY = uiNearPosY;
	}
	else {
		*pMapX = pPiece->nX;
		*pMapY = pPiece->nY;
	}
}

void FindNearEnemy( GAMEINFO* pGameInfo, PIECE* pPiece, MAP_POS* pMapX, MAP_POS* pMapY )
{
	int		x;
	int		y;

	int		iDistanceX;
	int		iDistanceY;
	UINT	uiDistanceXY;

	UINT	uiNearDistanceXY = 9999;
	UINT	uiNearPosX = 0;
	UINT	uiNearPosY = 0;

	UINT	uiPieceOrder;
	UINT	uiEnemyOrder;

	bool	fFound = FALSE;

	uiPieceOrder = pPiece->nOrder;

	for( y = 0; y < MAP_HEIGHT; y++ ) {
		for( x = 0; x < MAP_WIDTH; x++ ) {
			if( x == pPiece->nX && y == pPiece->nY ) {
				continue;
			}
			if( IsExistFriendPiece( &( pGameInfo->Player[0] ), x, y )  ) {
				continue;
			}

			if( GetTerritoryType( pGameInfo, x, y ) & TERRITORY_EXIST_ENEMY ) {
				iDistanceX = pPiece->nX - x;
				iDistanceY = pPiece->nY - y;
				uiDistanceXY = abs( iDistanceX ) + abs( iDistanceY );

				if( uiPieceOrder == ( MAX_GAMEPLAYER * MAX_PIECE ) ) {
					if( uiDistanceXY != 1 ) {
						continue;
					}
				}
				else{
					uiEnemyOrder = pGameInfo->Map[y][x].cTerritory;
					if( uiEnemyOrder < uiPieceOrder ) {
						continue;
					}
				}

				if( uiNearDistanceXY > uiDistanceXY && uiDistanceXY != 0 ) {
					fFound = TRUE;
					uiNearDistanceXY = uiDistanceXY;
					uiNearPosX = x;
					uiNearPosY = y;
				}
			}
		}
	}

	if( fFound ) {
		*pMapX = uiNearPosX;
		*pMapY = uiNearPosY;
	}
	else {
		FindNearTerritory( pGameInfo, pPiece, TERRITORY_BLANK, pMapX, pMapY );
	}
}

void FindEnemy( GAMEINFO* pGameInfo, PIECE* pPiece, UINT8 PieceType, MAP_POS* pMapX, MAP_POS* pMapY )
{
	int		i;

	for( i = 0; i < MAX_PIECE; i++ ){
		if( pGameInfo->Player[1].Piece[i].nType == PieceType ) {
			break;
		}
	}

	if( i < MAX_PIECE ){
		*pMapX = pGameInfo->Player[1].Piece[i].nX;
		*pMapY = pGameInfo->Player[1].Piece[i].nY;
	}
	else {
		*pMapX = pPiece->nX;
		*pMapY = pPiece->nY;
	}
}

UINT GetTerritoryType( GAMEINFO* pGameInfo, int x, int y )
{
	int	i;
	int	iTerritoryType = TERRITORY_BLANK;

	if( pGameInfo->Map[y][x].cTerritory == TERRITORY_BLANK ) {
		iTerritoryType |= TERRITORY_BLANK;
	}
	else {
		for( i = 0; i < MAX_PIECE; i++ ) {
			if( pGameInfo->Player[0].Piece[i].nOrder == pGameInfo->Map[y][x].cTerritory ) {
				iTerritoryType |= TERRITORY_FRIEND;
			}
		}
		if( i >= MAX_PIECE ) {
			iTerritoryType |= TERRITORY_ENEMY;
		}
	}

	for( i = 0; i < MAX_PIECE; i++ ) {
		if( pGameInfo->Player[0].Piece[i].nX == x && pGameInfo->Player[0].Piece[i].nY == y ) {
			iTerritoryType |= TERRITORY_EXIST_FRIEND;
			break;
		}
		else if( pGameInfo->Player[1].Piece[i].nX == x && pGameInfo->Player[1].Piece[i].nY == y ) {
			iTerritoryType |= TERRITORY_EXIST_ENEMY;
			break;
		}
	}

	return iTerritoryType;
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

void GetPiecePosForNext( GAMEINFO* pGameInfo, PIECE* pPiece, MAP_POS* pMapX, MAP_POS* pMapY )
{
	switch( pPiece->nType ) {
		case PIECETYPE_COMMON:
			FindNearTerritory( pGameInfo, pPiece, TERRITORY_BLANK, pMapX, pMapY );
			break;
		default:
			FindNearEnemy( pGameInfo, pPiece, pMapX, pMapY );
			break;
	}
}

void GetPiecePosForMove( PIECE* pPiece,  MAP_POS DestX, MAP_POS DestY, MAP_POS* pMapX, MAP_POS* pMapY )
{
	MAP_POS	x;
	MAP_POS	y;

	INT8	iDistanceX;
	INT8	iDistanceY;

	x = pPiece->nX;
	y = pPiece->nY;
	iDistanceX = DestX - x;
	iDistanceY = DestY - y;

	if( y != DestY ) {
		if( iDistanceY > 0 ) {
			y++;
		}
		else if( iDistanceY < 0 ) {
			y--;
		}
		else {
			/* Do Nothing */
		}
	}
	else {
		if( iDistanceX > 0 ) {
			x++;
		}
		else if( iDistanceX < 0 ) {
			x--;
		}
		else {
			/* Do Nothing */
		}
	}

	*pMapX = x;
	*pMapY = y;
}

DIRECTION ToDirection( GAMEINFO* pGameInfo, PIECE* pPiece, MAP_POS iDestX, MAP_POS iDestY )
{
	int iDistanceX;
	int iDistanceY;

	DIRECTION DirectionLR = DIRECTION_NONE;
	DIRECTION DirectionUD = DIRECTION_NONE;
	DIRECTION Direction;

	iDistanceX = iDestX - pPiece->nX;
	iDistanceY = iDestY - pPiece->nY;

	if( iDistanceX > 0 ) {
		DirectionLR |= DIRECTION_RIGHT;
	}
	else if( iDistanceX < 0 ) {
		DirectionLR |= DIRECTION_LEFT;
	}
	else {
		/* Do Nothing */
	}

	if( iDistanceY > 0 ) {
		DirectionUD |= DIRECTION_DOWN;
	}
	else if( iDistanceY < 0 ) {
		DirectionUD |= DIRECTION_UP;
	}
	else {
		/* Do Nothing */
	}

	if( IsPossibleBoostMove( pGameInfo, pPiece ) ) {
		Direction = DirectionLR | DirectionUD;
	}
	else {
		if( DirectionLR == DIRECTION_NONE || DirectionUD == DIRECTION_NONE ) {
			Direction = DirectionLR | DirectionUD;
		}
		else {
			if( pGameInfo->nTurn % 2 ) {
				Direction = DirectionLR;
			}
			else {
				Direction = DirectionUD;
			}
		}
	}

	return Direction;
}

void SetPieceDirection( MOVEPIECEINFO *pMovePiece, DIRECTION ucDirection )
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

bool IsPossibleBoostMove( GAMEINFO* pGameInfo, PIECE* pPiece )
{
	/* 斜めの位置は、特殊コマで斜め移動回数が残っているときだけ移動可能 */
	if( ( pPiece->nType != PIECETYPE_COMMON ) && ( pGameInfo->Player[0].nBootCount > 0 ) ) {
		return true;
	}
	return false;
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
