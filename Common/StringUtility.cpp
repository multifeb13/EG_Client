
#include "stdafx.h"
#ifdef _WIN32
#include <windows.h>
#include "StringUtility.h"


/// <summary>
/// シフトJISをUTF-8に変換する
/// </summary>
/// <param name="szString">シフトJISの文字列</param>
/// <param name="cpBuffer">変換した文字列を格納するバッファ</param>
/// <param name="nBufferSize">格納バッファのサイズ</param>
/// <returns>変換結果</returns>
bool ShiftJisToUtf8( const char *szString, char *cpBuffer, unsigned int nBufferSize )
{
	bool		bRes = true;
	int			nWideCharSize;
	wchar_t		*cpWideChar = NULL;

	if( (NULL == szString) || (NULL == cpBuffer) )
	{
		return	false;
	}

	nWideCharSize = MultiByteToWideChar( CP_ACP, 0, szString, (int)strlen(szString)+1, NULL, 0 );
	if (0 >= nWideCharSize)
	{
		bRes = false;
	}
	
	if (true == bRes)
	{
		cpWideChar = (wchar_t*)malloc(sizeof(wchar_t)*nWideCharSize);
		if (NULL == cpWideChar)
		{
			bRes = false;
		}
	}

	if (true == bRes)
	{
		MultiByteToWideChar( CP_ACP, 0, szString, (int)strlen(szString) + 1, cpWideChar, nWideCharSize);
		WideCharToMultiByte(CP_UTF8, 0, cpWideChar, nWideCharSize, cpBuffer, nBufferSize, NULL, NULL);
	}
	
	
	if (NULL != cpWideChar)
	{
		free(cpWideChar);
		cpWideChar = NULL;
	}
	
	return	bRes;
}


/// <summary>
/// UTF-8をシフトJISに変換する
/// </summary>
/// <param name="szString">UTF-8の文字列</param>
/// <param name="cpBuffer">変換した文字列を格納するバッファ</param>
/// <param name="nBufferSize">格納バッファのサイズ</param>
/// <returns>変換結果</returns>
bool Utf8ToShiftJis(const char *szString, char *cpBuffer, unsigned int nBufferSize)
{
	bool		bRes = true;
	int			nWideCharSize;
	wchar_t		*cpWideChar = NULL;

	if ((NULL == szString) || (NULL == cpBuffer))
	{
		return	false;
	}

	nWideCharSize = MultiByteToWideChar(CP_UTF8, 0, szString, (int)strlen(szString) + 1, NULL, 0);
	if (0 >= nWideCharSize)
	{
		bRes = false;
	}

	if (true == bRes)
	{
		cpWideChar = (wchar_t*)malloc(sizeof(wchar_t)*nWideCharSize);
		if (NULL == cpWideChar)
		{
			bRes = false;
		}
	}

	if (true == bRes)
	{
		MultiByteToWideChar(CP_UTF8, 0, szString, (int)strlen(szString) + 1, cpWideChar, nWideCharSize);
		WideCharToMultiByte(CP_ACP, 0, cpWideChar, nWideCharSize, cpBuffer, nBufferSize, NULL, NULL);
	}


	if (NULL != cpWideChar)
	{
		free(cpWideChar);
		cpWideChar = NULL;
	}

	return	bRes;
}


#endif //_WIN32

