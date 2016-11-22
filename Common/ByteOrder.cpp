
#include "stdafx.h"
#include "ByteOrder.h"


#ifdef _WIN32
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif	//_WIN32


//エンディアンを調べる構造体
typedef struct tagMULTIVALUE
{
	union tagval
	{
		char	cVal[4];
		int		nVal;
	} val;
} MULTIVALUE;


#ifdef _WIN32
#pragma pack(pop)
#else
#pragma pack(0)
#endif	//_WIN32


/// <summary>
/// 現在の環境のエンディアンを取得する
/// </summary>
/// <returns>エンディアン(ENDIAN_LITTLE:リトルエンディアン, ENDIAN_BIG:ビッグエンディアン）</returns>
int GetCurrentByteOrder(void)
{
	static bool		bCheckByteOrder = false;
	static int		nByteOrder = ENDIAN_LITTLE;
	MULTIVALUE		mv;


	if( true != bCheckByteOrder )
	{
		mv.val.nVal = 1;
		
		nByteOrder = (1 == mv.val.cVal[0]) ? ENDIAN_LITTLE : ENDIAN_BIG;

		bCheckByteOrder = true;
	}

	return	nByteOrder;
}

