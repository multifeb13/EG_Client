
#include "stdafx.h"
#include "SocketProtcol.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <string.h>
#endif //_WIN32
#include "ByteOrder.h"


#define		MIN_RECV	2		//キャッシュ可能なメッセージ数


/// <summary>
/// コンストラクタ
/// </summary>
CSocketProtcol::CSocketProtcol()
	: m_RecvQueue(sizeof(EGPROTCOL) * MIN_RECV)
{
}


/// <summary>
/// デストラクタ
/// </summary>
CSocketProtcol::~CSocketProtcol()
{
}


/// <summary>
/// ソケット管理クラスを取得する
/// </summary>
/// <returns>ソケット管理クラスへのポインタ</returns>
CSocketUtility* CSocketProtcol::GetSocketUtility(void)
{
	return	&m_CSocket;
}


/// <summary>
/// キャッシュしているデータからコマンドを取得する
/// </summary>
/// <param name="wpCommand">取得したコマンドを保存するポインタ</param>
/// <param name="vpBuffer">取得したコマンド付属情報を保存するバッファへのポインタ</param>
/// <param name="nBufferSize">コマンド付属情報のバッファサイズ</param>
/// <param name="npRecvBufferSize">取得したコマンド付属情報のサイズ</param>
/// <returns>結果取得</returns>
bool CSocketProtcol::GetCommand(unsigned short *wpCommand, void *vpBuffer, int nBufferSize, int *npRecvBufferSize )
{
	bool			bRes = true;
	unsigned short	wDataSize;
	EGPROTCOL		recvData;
	int				nSize;
	int				nParamSize;
	
	
	if (sizeof(wDataSize) > m_RecvQueue.GetSize())
	{
		return	false;
	}

	//コマンドのサイズ分Queueにたまっているか確認する
	bRes = m_RecvQueue.Peek((unsigned char*)&wDataSize, sizeof(wDataSize));
	if (true != bRes)
	{
		return	false;
	}

	wDataSize = ENDIAN_SWAP2BYTE(ENDIAN_LITTLE, wDataSize);
	
	if (wDataSize > m_RecvQueue.GetSize())
	{
		return	false;
	}

	memset(&recvData, 0, sizeof(recvData));
	bRes = m_RecvQueue.Dequeue((unsigned char*)&recvData, wDataSize);
	if (true != bRes)
	{
		return	false;
	}
	
	*wpCommand = ENDIAN_SWAP2BYTE(ENDIAN_LITTLE, recvData.CmdInfo.wCommand);

	//コマンド情報を取得する
	nSize = 0;
	if ((NULL != vpBuffer) && (0 < nBufferSize))
	{
		nSize = nBufferSize;
		nParamSize = wDataSize - (sizeof(recvData.wDataSize) + sizeof(recvData.CmdInfo.wCommand));
		if (nParamSize < nSize)
		{
			nSize = nParamSize;
		}
		//共用体なので読み込み時にどのメンバを指定しても同じ結果になる
		memcpy(vpBuffer, &recvData.CmdInfo.Parqam.GameInfo, nSize);
	}

	if (NULL != npRecvBufferSize)
	{
		*npRecvBufferSize = nSize;
	}
	
	return	true;
}


/// <summary>
/// キャッシュしているデータからコマンドを取得する
/// </summary>
/// <param name="pEgp">コマンドを保存する構造体へのポインタ</param>
/// <returns>結果取得</returns>
bool CSocketProtcol::RecvCommand(EGPROTCOL *pEgp)
{
	bool			bRes = true;
	int				nSize;
	unsigned char	*cpInfo;
	int				nInfoSize;
	int				nRecvInfoSize;


	if (NULL == pEgp)
	{
		return	false;
	}

	nSize = sizeof(pEgp->wDataSize) + sizeof(pEgp->CmdInfo.wCommand);
	cpInfo = (unsigned char*)pEgp + nSize;
	nInfoSize = sizeof(EGPROTCOL) - nSize;

	memset(pEgp, 0, sizeof(EGPROTCOL));
	pEgp->CmdInfo.wCommand = CMD_NONE;
	nRecvInfoSize = 0;
	bRes = RecvCommand(&(pEgp->CmdInfo.wCommand), cpInfo, nInfoSize, &nRecvInfoSize);
	if (true != bRes)
	{
		return	false;
	}
	
	pEgp->wDataSize = (unsigned short)(nSize + nRecvInfoSize);

	return	bRes;
}


/// <summary>
/// データを受信しコマンドを取得する
/// </summary>
/// <param name="wpCommand">取得したコマンドを格納するポインタ</param>
/// <param name="vpBuffer">取得したコマンド付属情報を保存するバッファ</param>
/// <param name="nBufferSize">コマンド付属情報バッファのサイズ</param>
/// <param name="npRecvBufferSize">取得したコマンド付属情報サイズ</param>
/// <returns>結果取得</returns>
bool CSocketProtcol::RecvCommand(unsigned short *wpCommand, void *vpBuffer, int nBufferSize, int *npRecvBufferSize)
{
	bool			bRes;
	EGPROTCOL		recvData;
	int				nSize, nRecv;
	
	
	if (NULL == wpCommand)
	{
		return	false;
	}
	*wpCommand = CMD_NONE;
	
	//Queueにコマンドが存在するときはQueueから取り出す
	bRes = GetCommand(wpCommand, vpBuffer, nBufferSize, npRecvBufferSize);
	if ( true == bRes)
	{
		return	true;
	}
	
	//ソケットからデータを受信する
	nSize = sizeof(recvData);
	nRecv = 0;
	bRes = m_CSocket.Recv(&recvData, nSize, &nRecv);
	if (true != bRes)
	{
		return	false;
	}

	if (0 == nRecv)
	{
		return	true;
	}

	if (m_RecvQueue.GetFreeSize() < (unsigned long)nRecv)
	{
		return	false;
	}

	bRes = m_RecvQueue.Enqueue((unsigned char*)&recvData, nRecv);
	if (true != bRes)
	{
		return	false;
	}

	//Queueに追加したデータにコマンドが存在するときはQueueから取り出す
	bRes = GetCommand(wpCommand, vpBuffer, nBufferSize, npRecvBufferSize);
	if (true != bRes)
	{
		return	false;
	}

	return	true;
}


/// <summary>
/// コマンドを送信する
/// </summary>
/// <param name="wCommand">送信コマンド</param>
/// <param name="vpData">コマンド付属情報のバッファ</param>
/// <param name="nDataSize">コマンド付属情報のサイズ</param>
/// <returns>送信結果</returns>
bool CSocketProtcol::SendCommand(unsigned short wCommand, const void* vpData, int nDataSize)
{
	EGPROTCOL		egp;
	unsigned short	wDataSize;
	bool			bRes;
	int				nSend;
	int				nHeaderSize;
	int				nSize;
	unsigned char	*cpData;
    
    
	memset(&egp, 0, sizeof(egp));
	wDataSize = 0;
    
	if( INVALID_SOCKET == m_CSocket )
	{
		return	false;
	}
    
	//コマンドを設定する
	egp.CmdInfo.wCommand = wCommand;
	wDataSize += sizeof(egp.CmdInfo.wCommand);

	//コマンド情報を設定する
	if ((NULL != vpData) && (0 < nDataSize))
	{
		nSize = nDataSize;
		nHeaderSize = sizeof(egp.wDataSize) + sizeof(egp.CmdInfo.wCommand);
		cpData = (unsigned char*)(&egp) + nHeaderSize;
		if ((sizeof(egp) - nHeaderSize) < (unsigned int)nSize)
		{
			return	false;
		}

		memcpy(cpData, vpData, nSize);
		wDataSize += nSize;
	}

	wDataSize += sizeof(egp.wDataSize);
	egp.wDataSize = wDataSize;

	nSend = 0;
	bRes = m_CSocket.Send(&egp, wDataSize, &nSend);
	if ((true != bRes) || (wDataSize != nSend))
	{
		return	false;
	}

	return	true;
}

