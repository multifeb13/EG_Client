
#ifndef __STRINGUTILITY_H__
#define	__STRINGUTILITY_H__


#ifdef _WIN32


bool ShiftJisToUtf8(const char *szString, char *cpBuffer, unsigned int nBufferSize);
bool Utf8ToShiftJis(const char *szString, char *cpBuffer, unsigned int nBufferSize);


#endif //_WIN32


#endif	//__STRINGUTILITY_H__


