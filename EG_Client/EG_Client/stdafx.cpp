// stdafx.cpp : 標準インクルード EG_Client.pch のみを
// 含むソース ファイルは、プリコンパイル済みヘッダーになります。
// stdafx.obj にはプリコンパイル済み型情報が含まれます。

#include "stdafx.h"

// TODO: このファイルではなく、STDAFX.H で必要な
// 追加ヘッダーを参照してください。


#ifdef _WIN32

#else

int WSAGetLastError()
{
    return  errno;
}

#endif	//_WIN32
