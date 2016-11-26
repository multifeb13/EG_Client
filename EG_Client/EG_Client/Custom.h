#ifndef	__CUSTOM_H__
#define	__CUSTOM_H__

#include "EG.h"

int GetUserID(char *cpBuffer, unsigned int nBufferSize);
int StartGame(void);
int EndGame(GAMEENDINFO GameEndInfo);
int MovePhase( GAMEINFO GameInfo, MOVEPIECEINFO *pMovePiece );
int MovePhaseResult(MOVEPIECERESULT result);

void GetNextMapPosition( unsigned int *uiPosX, unsigned int* uiPosY, GAMEINFO* pGameInfo, PIECE* pPiece );
int constrain( int value, int min, int max );
bool IsExistFriendPiece( PLAYERPIECE* pPlayer, int x, int y );
unsigned int GetTerritoryType( GAMEINFO* pGameInfo, int x, int y );
unsigned char ToDirection( PIECE* pPiece, unsigned int iDestX, unsigned int iDestY );
void SetMovePieceDirection( MOVEPIECEINFO *pMovePiece, unsigned char ucDirection );
#endif
