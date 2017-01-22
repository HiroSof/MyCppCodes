#pragma once
#include <Windows.h>
#include <stdio.h>

class CHSPipe {
private:
	HANDLE m_hPipeRead;
	HANDLE m_hPipeWrite;
	SECURITY_ATTRIBUTES m_sa;


public:
	CHSPipe();
	~CHSPipe();

	BOOL Create(DWORD PipeSize = 0);


	BOOL CloseReadHandle(void);
	BOOL CloseWriteHandle(void);
	BOOL Close(void);
	HANDLE GetReadHandle(void);
	HANDLE GetWriteHandle(void);
	_declspec(property(get = GetReadHandle)) HANDLE ReadHandle;
	_declspec(property(get = GetWriteHandle)) HANDLE WriteHandle;

	//”Ä—p‘‚«‚İ
	DWORD Write(void *lpWriteData, DWORD WriteSize);
	DWORD Read(void *lpOutBuffer, DWORD ReadSize);

	//ANSI(MBCS)”Å•¶š—ñ‘‚«‚İ
	DWORD WriteString(char *lpszWriteString);
	DWORD ReadString(char *lpszOutString, DWORD ReadSize);

	//Wide(Unicode)”Å•¶š—ñ‘‚«‚İ
	DWORD WriteString(wchar_t *lpszWriteString);
	DWORD ReadString(wchar_t *lpszOutString, DWORD ReadSize);

};

