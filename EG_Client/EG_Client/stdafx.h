// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#ifdef _WIN32
#include <tchar.h>
#else
#include <errno.h>
#define WSAEWOULDBLOCK  EAGAIN
#define memcpy_s(dest,numberOfElements,src,count)   memcpy(dest,src,count)
#define Sleep(msec)                                 usleep((useconds_t)(msec*1000))

int WSAGetLastError();
#endif //_WIN32


// TODO: プログラムに必要な追加ヘッダーをここで参照してください
