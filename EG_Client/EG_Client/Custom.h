#ifndef	__CUSTOM_H__
#define	__CUSTOM_H__

#include "EG.h"
#include "Types.h"

typedef struct tagPIECEAPPEND
{
	bool		fFirstMove;
	MAP_POS		DestX;
	MAP_POS		DestY;
}PIECEAPPEND;

int GetUserID(char *cpBuffer, unsigned int nBufferSize);
int StartGame(void);
int EndGame(GAMEENDINFO GameEndInfo);
int MovePhase( GAMEINFO GameInfo, MOVEPIECEINFO *pMovePiece );
int MovePhaseResult(MOVEPIECERESULT result);

/* for Destination */
void		FindNearTerritory( GAMEINFO* pGameInfo, PIECE* pPiece, int TerritoryType, MAP_POS* pMapX, MAP_POS* pMapY );
void		FindNearEnemy( GAMEINFO* pGameInfo, PIECE* pPiece, MAP_POS* pMapX, MAP_POS* pMapY );
void		FindEnemy( GAMEINFO* pGameInfo, PIECE* pPiece, UINT8 PieceType, MAP_POS* pMapX, MAP_POS* pMapY );

/* Territory */
UINT		GetTerritoryType( GAMEINFO* pGameInfo, int x, int y );

/* Piece */
bool		IsExistFriendPiece( PLAYERPIECE* pPlayer, int x, int y );
void		GetPiecePosForNext( GAMEINFO* pGameInfo, PIECE* pPiece, MAP_POS* pMapX, MAP_POS* pMapY );
void		GetPiecePosForMove( PIECE* pPiece, MAP_POS DestX, MAP_POS DestY, MAP_POS* pMapX, MAP_POS* pMapY );

/* Direction */
DIRECTION	ToDirection( GAMEINFO* pGameInfo, PIECE* pPiece, MAP_POS iDestX, MAP_POS iDestY );
void		SetPieceDirection( MOVEPIECEINFO *pMovePiece, DIRECTION ucDirection );

/* etc */
bool		IsPossibleBoostMove( GAMEINFO* pGameInfo, PIECE* pPiece );
int			constrain( int value, int min, int max );
#endif
