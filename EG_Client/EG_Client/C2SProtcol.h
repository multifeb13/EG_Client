
#ifndef	__C2SPROTCOL_H__
#define	__C2SPROTCOL_H__


#include "SocketProtcol.h"


class CC2SProtcol : public CSocketProtcol
{
public:
	CC2SProtcol();
	~CC2SProtcol();

	bool Connect(const char *szIPAddress, int nPort);
	bool Disconnect(void);
	
	bool RecvCommand(EGPROTCOL *pEgp);
	bool ReplyGetUserID(const char* szUserID);
	bool ReplyStartGame(void);
	bool ReplyEndGame(void);
	bool ReplyMovePhase(MOVEPIECEINFO MovePieceInfo);
	bool ReplyMoveResult(void);
	bool ReplyExitApp(void);
};


#endif	//__CLIENTPROTCOL_H__



