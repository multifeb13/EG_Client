
#ifndef	__QUEUE_H__
#define	__QUEUE_H__


template <typename TYPE>
class CQueue
{
private:
	unsigned long	m_dwBufferSize;
	TYPE			*m_pBuffer;
	unsigned long	m_dwHead;
	unsigned long	m_dwTail;

private:
	unsigned long MovePos(unsigned long dwNow, unsigned long dwStep=1);

public:

	CQueue(unsigned long dwSize);
	~CQueue();

	unsigned long GetSize(void);
	unsigned long GetFreeSize(void);
	bool Enqueue(TYPE Data);
	bool Enqueue(TYPE *pData, unsigned long dwSize);
	bool Peek(TYPE *pData, unsigned long dwSize=1);
	bool Dequeue(TYPE *pData, unsigned long dwSize=1);


};


/// <summary>
/// コンストラクタ
/// </summary>
template <typename TYPE>
CQueue<TYPE>::CQueue(unsigned long dwSize)
{
	m_dwHead = 0;
	m_dwTail = 0;
	m_dwBufferSize = dwSize+1;
	m_pBuffer = new TYPE[m_dwBufferSize];
}


/// <summary>
/// デストラクタ
/// </summary>
template <typename TYPE>
CQueue<TYPE>::~CQueue()
{
	if (NULL != m_pBuffer)
	{
		delete[]m_pBuffer;
		m_pBuffer = NULL;
	}
	m_dwBufferSize = 0;
}


/// <summary>
/// キューに入っている要素数を取得する
/// </summary>
/// <returns>要素数</returns>
template <typename TYPE>
unsigned long CQueue<TYPE>::GetSize(void)
{
	unsigned long	dwSize;

	if (m_dwHead <= m_dwTail)
	{
		dwSize = m_dwTail - m_dwHead;
	}
	else
	{
		dwSize = m_dwBufferSize - m_dwHead + m_dwTail;
	}

	return	dwSize;
}


/// <summary>
/// キューの空き領域サイズを取得する
/// </summary>
/// <returns>空き領域サイズ</returns>
template <typename TYPE>
unsigned long CQueue<TYPE>::GetFreeSize(void)
{
	unsigned long	dwFreeSize;
	unsigned long	dwSize;
	unsigned long	dwValid;


	dwSize = GetSize();
	dwValid = m_dwBufferSize - 1;

	dwFreeSize = 0;
	if (dwSize <= dwValid)
	{
		dwFreeSize = dwValid - dwSize;
	}
	
	return	dwFreeSize;
}


/// <summary>
/// リングバッファの位置を移動する
/// </summary>
/// <param name="dwNow">現在の位置</param>
/// <param name="dwStep">移動幅</param>
/// <returns>新しい位置</returns>template <typename TYPE>
template <typename TYPE>
unsigned long CQueue<TYPE>::MovePos(unsigned long dwNow, unsigned long dwStep)
{
	return	(dwNow + dwStep) % m_dwBufferSize;
}


/// <summary>
/// キューに要素を格納する
/// </summary>
/// <param name="Data">格納する要素</param>
/// <returns>格納結果</returns>template <typename TYPE>
template <typename TYPE>
bool CQueue<TYPE>::Enqueue(TYPE Data)
{
	return	Enqueue(&Data, 1);
}


/// <summary>
/// キューに要素を格納する
/// </summary>
/// <param name="pData">格納する要素へのポインタ</param>
/// <param name="dwSize">格納する要素数</param>
/// <returns>格納結果</returns>template <typename TYPE>
template <typename TYPE>
bool CQueue<TYPE>::Enqueue(TYPE *pData, unsigned long dwSize)
{
	unsigned long	i;

	
	if(( NULL == pData) || (GetFreeSize() < dwSize))
	{
		return	false;
	}

	if (0 == dwSize)
	{
		return	true;
	}
	
	for (i = 0; i < dwSize; i++ )
	{
		m_pBuffer[m_dwTail] = pData[i];
		m_dwTail = MovePos(m_dwTail);
	}
	
	return	true;
}


/// <summary>
/// キューの要素をコピーする
/// </summary>
/// <param name="pData">保存先へのポインタ</param>
/// <param name="dwSize">コピー数</param>
/// <returns>コピー結果</returns>template <typename TYPE>
template <typename TYPE>
bool CQueue<TYPE>::Peek(TYPE *pData, unsigned long dwSize)
{
	unsigned long	i;
	unsigned long	dwPos;


	if ((NULL == pData) || (GetSize() < dwSize))
	{
		return	false;
	}

	if (0 == dwSize)
	{
		return	true;
	}
	
	dwPos = m_dwHead;
	for (i = 0; i < dwSize; i++)
	{
		pData[i] = m_pBuffer[dwPos];
		dwPos = MovePos(dwPos);
	}

	return	true;
}


/// <summary>
/// キューから要素を取得する
/// </summary>
/// <param name="pData">保存先へのポインタ</param>
/// <param name="dwSize">取得数</param>
/// <returns>取得結果</returns>template <typename TYPE>
template <typename TYPE>
bool CQueue<TYPE>::Dequeue(TYPE *pData, unsigned long dwSize)
{
	bool	bRes;

	bRes = Peek(pData, dwSize);
	if ( true != bRes)
	{
		return	false;
	}

	m_dwHead = MovePos(m_dwHead, dwSize);

	return	true;
}


#endif	//__QUEUE_H__
