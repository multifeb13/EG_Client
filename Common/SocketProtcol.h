
#ifndef	__SOCKETPROTCOL_H__
#define	__SOCKETPROTCOL_H__


#include "SocketUtility.h"
#include "EGProtcol.h"
#include "Queue.h"


class CSocketProtcol
{
protected:
	CSocketUtility			m_CSocket;
	CQueue<unsigned char>	m_RecvQueue;
public:

	CSocketProtcol();
	~CSocketProtcol();
	CSocketUtility* GetSocketUtility(void);
	bool GetCommand(unsigned short *wpCommand, void *vpBuffer = NULL, int nBufferSize = 0, int *npRecvBufferSize = NULL);
	bool RecvCommand(EGPROTCOL *pEgp);
	bool RecvCommand(unsigned short *wpCommand, void *vpBuffer = NULL, int nBufferSize = 0, int *npRecvBufferSize = NULL);
	bool SendCommand(unsigned short wCommand, const void* vpData = NULL, int nDataSize = 0);
};


#endif	//__SOCKETPROTCOL_H__



