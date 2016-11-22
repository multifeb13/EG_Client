
#include "stdafx.h"
#include "SocketUtility.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif //_WIN32


#ifdef _WIN32
//winsockのライブラリをリンクする
#pragma comment(lib, "ws2_32.lib")
#endif //_WIN32


/// <summary>
/// コンストラクタ
/// </summary>
CSocketUtility::CSocketUtility()
{
	m_Socket = INVALID_SOCKET;
	m_bAutoRelease = true;
}


/// <summary>
/// デストラクタ
/// </summary>
CSocketUtility::~CSocketUtility()
{
	Disconnect();
}


/// <summary>
/// サーバーと接続する
/// </summary>
/// <param name="szIPAddress">サーバーのIPアドレス</param>
/// <param name="nPort">サーバーのポート番号</param>
/// <returns>接続結果</returns>
bool CSocketUtility::Connect(const char *szIPAddress, int nPort)
{
	int		nRes;
	struct sockaddr_in server;
#ifdef _WIN32
	u_long	dwNonBlocking;
#else
	int     nNonBlocking;
#endif //_WIN32
	
	if (NULL == szIPAddress)
	{
		return	false;
	}
	
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_Socket )
	{
		return	false;
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(nPort);

#ifdef _WIN32
	nRes = inet_pton(AF_INET, szIPAddress, &server.sin_addr);
	if (1 != nRes)
	{
		return	false;
	}
#else
    server.sin_addr.s_addr = inet_addr(szIPAddress);
	if (0xFFFFFFFF == server.sin_addr.s_addr)
	{
		return	false;
	}
#endif //_WIN32
	
	nRes = connect(m_Socket, (struct sockaddr *)&server, sizeof(server));
	if( 0 != nRes )
	{
		return	false;
	}
	
#ifdef _WIN32
	dwNonBlocking = 1;
	nRes = ioctlsocket(m_Socket, FIONBIO, &dwNonBlocking);
#else
	nNonBlocking = 1;
	nRes = ioctl(m_Socket, FIONBIO, &nNonBlocking);
#endif	//_WIN32
	if (NO_ERROR != nRes)
	{
		return	false;
	}
    
	return	true;
}


/// <summary>
/// ソケットにアタッチする
/// </summary>
/// <param name="szIPAddress">サーバーのIPアドレス</param>
/// <param name="nPort">サーバーのポート番号</param>
/// <returns>アタッチ結果</returns>
bool CSocketUtility::Attach(SOCKET sock, bool bAutoRelease )
{
	if ( true != Disconnect() )
	{
		return	false;
	}

	m_Socket = sock;
	m_bAutoRelease = bAutoRelease;
	
	return	true;
}


/// <summary>
/// サーバーと切断する
/// </summary>
/// <returns>切断結果</returns>
bool CSocketUtility::Disconnect( void )
{
	if (INVALID_SOCKET == m_Socket)
	{
		return	true;
	}
	
	if (true == m_bAutoRelease)
	{
		closesocket(m_Socket);
	}
	else
	{
		m_bAutoRelease = true;
	}
	m_Socket = INVALID_SOCKET;
	
	return	true;
}


/// <summary>
/// データを送信する
/// </summary>
/// <param name="vpBuffer">送信データ</param>
/// <param name="nBufferSize">送信するデータサイズ</param>
/// <param name="npSendLen">送信されたデータサイズ</param>
/// <returns>送信結果</returns>
bool CSocketUtility::Send(void *vpBuffer, int nBufferSize, int *npSendLen)
{
	int		nRes;


	if( (INVALID_SOCKET == m_Socket) || (NULL == vpBuffer) )
	{
		return	false;
	}

	nRes = (int)::send(m_Socket, (char*)vpBuffer, nBufferSize, 0 );
	if (SOCKET_ERROR == nRes)
	{
		return	false;
	}
	
	if ( NULL != npSendLen )
	{
		*npSendLen = nRes;
	}

	return	true;
}


/// <summary>
/// データを受信する
/// </summary>
/// <param name="vpBuffer">受信バッファ</param>
/// <param name="nBufferSize">受信バッファのサイズ</param>
/// <param name="npRecvLen">受信したデータサイズ</param>
/// <returns>受信結果</returns>
bool CSocketUtility::Recv(void *vpBuffer, int nBufferSize, int *npRecvLen)
{
	int		nRes;
	
	if( (INVALID_SOCKET == m_Socket) || (NULL == vpBuffer) )
	{
		return	false;
	}

	nRes = (int)::recv(m_Socket, (char*)vpBuffer, nBufferSize, 0 );
	if (SOCKET_ERROR == nRes)
	{
		if (WSAEWOULDBLOCK != WSAGetLastError())
		{
			return	false;
		}

		nRes = 0;
	}
	
	if (NULL != npRecvLen)
	{
		*npRecvLen = nRes;
	}

	return	true;
}


