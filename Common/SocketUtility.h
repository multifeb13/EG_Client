
#ifndef __SOCKETUTILITY_H__
#define __SOCKETUTILITY_H__

#ifdef _WIN32
#include <winsock2.h>	//windows.hより前にincludeしないとエラーになる
#include <ws2tcpip.h>
#else
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

typedef int SOCKET;
#define closesocket(sock)   close(sock)
#define INVALID_SOCKET      (SOCKET)(~0)
#define SOCKET_ERROR        (-1)
#define NO_ERROR            0
#endif //_WIN32



class CSocketUtility
{
private:
	SOCKET	m_Socket;
	bool	m_bAutoRelease;

public:
	operator SOCKET() { return m_Socket; };

	CSocketUtility();
	~CSocketUtility();
	bool Connect(const char *szIPAddress, int nPort);
	bool Attach(SOCKET sock, bool fAutoRelease=true);
	bool Disconnect(void);
	bool Send(void *vpBuffer, int nBufferSize, int *npSendLen=NULL );
	bool Recv(void *vpBuffer, int nBufferSize, int *npRecvLen=NULL );
};


#endif //__SOCKETUTILITY_H__

