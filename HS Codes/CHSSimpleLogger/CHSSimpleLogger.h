#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <tchar.h>
#include <io.h>

#pragma once

class CHSSimpleLogger {
private:
	HANDLE hFile;

private:
	BOOL Save(TCHAR *lpszClassName, TCHAR *lpszFuncName, TCHAR *lpszTitleString, TCHAR *lpszLogFormatMessage, va_list  ArgListEx);
	static BOOL SaveToFile(HANDLE hFile, TCHAR *lpszLineString);
	static std::basic_string<TCHAR> MakeWriteString(TCHAR *lpszClassName, TCHAR *lpszFuncName,TCHAR *lpszTitleString, TCHAR *lpszLogMessage);

	static std::basic_string<TCHAR> MakeTimeStamp(void);

public:
	CHSSimpleLogger();
	~CHSSimpleLogger();

	BOOL Write(TCHAR *lpszLogFormatMessage ,...);
	BOOL Write(TCHAR *lpszFuncName, TCHAR *lpszLogFormatMessage, ...);
	BOOL Write(TCHAR *lpszClassName, TCHAR *lpszFuncName, TCHAR *lpszLogFormatMessage, ...);
	BOOL WriteForFuncInNamespace(TCHAR *lpszNamespaceName, TCHAR *lpszFuncName, TCHAR *lpszLogFormatMessage, ...);
	BOOL WriteWithTitle(TCHAR *lpszTitleName, TCHAR *lpszLogFormatMessage, ...);

	BOOL Create(TCHAR *lpszFilePath);
	BOOL Close(void);
};

