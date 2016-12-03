#ifndef	__CUSTOM_H__
#define	__CUSTOM_H__

#include "EG.h"
#include "Types.h"

int GetUserID(char *cpBuffer, unsigned int nBufferSize);
int StartGame(void);
int EndGame(GAMEENDINFO GameEndInfo);
int MovePhase( GAMEINFO GameInfo, MOVEPIECEINFO *pMovePiece );
int MovePhaseResult(MOVEPIECERESULT result);

int			constrain( int value, int min, int max );

UINT		GetTerritoryType( GAMEINFO* pGameInfo, int x, int y );
void		GetMovableTerritoryCount( GAMEINFO* pGameInfo, PIECE* pPiece, int TerritoryType, int* paTerritoryCount );
UINT		GetTerritoryType( GAMEINFO* pGameInfo, int x, int y );
bool		IsMovableTerritory( GAMEINFO* pGameInfo, PIECE* pPiece, int x, int y );

DIRECTION	GetMinTerritoryDirection( int* paTerritoryCount );
DIRECTION	GetMaxTerritoryDirection( int* paTerritoryCount );
DIRECTION	GetNextPieceDirection( GAMEINFO* pGameInfo, PIECE* pPiece );
DIRECTION	ToDirection( PIECE* pPiece, UINT iDestX, UINT iDestY );
void		SetMovePieceDirection( MOVEPIECEINFO *pMovePiece, DIRECTION ucDirection );

bool		IsExistFriendPiece( PLAYERPIECE* pPlayer, int x, int y );

#endif
