
#ifndef	__CUSTOM_H__
#define	__CUSTOM_H__


#include "EG.h"


int GetUserID(char *cpBuffer, unsigned int nBufferSize);
int StartGame(void);
int EndGame(GAMEENDINFO GameEndInfo);
int MovePhase( GAMEINFO GameInfo, MOVEPIECEINFO *pMovePiece );
int MovePhaseResult(MOVEPIECERESULT result);


#endif
