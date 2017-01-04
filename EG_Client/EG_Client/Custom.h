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

/* Destination */
void		FindNearTerritory( GAMEINFO* pGameInfo, PIECE* pPiece, TERRITORY_TYPE TerritoryType, MAP_POS* pMapX, MAP_POS* pMapY );
void		FindNearEnemy( GAMEINFO* pGameInfo, PIECE* pPiece, MAP_POS* pMapX, MAP_POS* pMapY );
void		GetNextPieceDestination( GAMEINFO* pGameInfo, PIECE* pPiece, MAP_POS* pMapX, MAP_POS* pMapY );
void		GetNextPieceMapPosition( PIECE* pPiece, MAP_POS DestX, MAP_POS DestY, MAP_POS* pMapX, MAP_POS* pMapY );

/* Territory */
TERRITORY_TYPE	GetTerritoryType( GAMEINFO* pGameInfo, int x, int y );

/* Piece */
bool		IsExistFriendPiece( PLAYERPIECE* pPlayer, int x, int y );

/* Direction */
DIRECTION	ToDirection( GAMEINFO* pGameInfo, PIECE* pPiece, MAP_POS iDestX, MAP_POS iDestY );
void		SetPieceDirection( MOVEPIECEINFO *pMovePiece, DIRECTION ucDirection );

/* etc */
bool		IsPossibleBoostMove( GAMEINFO* pGameInfo, PIECE* pPiece );
#endif
