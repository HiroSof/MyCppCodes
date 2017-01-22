#include "CHSPipe.h"


CHSPipe::CHSPipe() {
	this->m_hPipeRead = NULL;
	this->m_hPipeWrite = NULL;

	//SECURITY_ATTRIBUTES構造体のセット
	this->m_sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	this->m_sa.lpSecurityDescriptor = NULL;
	this->m_sa.bInheritHandle = TRUE;

}


CHSPipe::~CHSPipe() {
	this->Close();
}

BOOL CHSPipe::Create(DWORD PipeSize) {
	if (this->m_hPipeRead) return FALSE;
	if (this->m_hPipeWrite) return FALSE;
	BOOL bRet = CreatePipe(&this->m_hPipeRead, &this->m_hPipeWrite, &this->m_sa, PipeSize);
	return bRet;
}

BOOL CHSPipe::CloseReadHandle(void) {
	BOOL bRet =  CloseHandle(this->m_hPipeRead);
	if (bRet) this->m_hPipeRead = NULL;
	return bRet;
}

BOOL CHSPipe::CloseWriteHandle(void) {
	BOOL bRet =  CloseHandle(this->m_hPipeWrite);
	if (bRet) this->m_hPipeWrite = NULL;
	return bRet;

}

BOOL CHSPipe::Close(void) {
	BOOL bRet = TRUE;
	if (this->m_hPipeRead) bRet &= this->CloseReadHandle();
	if (this->m_hPipeWrite) bRet &= this->CloseWriteHandle();
	return bRet;
}


HANDLE CHSPipe::GetReadHandle(void) {
	return this->m_hPipeRead;
}
HANDLE CHSPipe::GetWriteHandle(void) {
	return this->m_hPipeWrite;
}

//汎用書き込み
DWORD CHSPipe::Write(void *lpWriteData, DWORD WriteSize) {
	if (lpWriteData == nullptr)return 0;
	DWORD dwWrote;
	BOOL bRet = WriteFile(this->m_hPipeWrite, lpWriteData, WriteSize, &dwWrote , nullptr);
	if (bRet == FALSE) return 0;
	return dwWrote;
}

DWORD CHSPipe::Read(void *lpOutBuffer, DWORD ReadSize) {
	if (lpOutBuffer == nullptr)return 0;
	DWORD dwRead;
	BOOL bRet = ReadFile(this->m_hPipeRead, lpOutBuffer, ReadSize, &dwRead , nullptr);
	if (bRet == FALSE) return 0;
	return dwRead;
}

//ANSI(MBCS)版文字列書き込み
DWORD CHSPipe::WriteString(char *lpszWriteString) {
	if (lpszWriteString==nullptr)return 0;
	DWORD dwWrote = this->Write(lpszWriteString, lstrlenA(lpszWriteString) * sizeof(char));
	return dwWrote / sizeof(char);
}
DWORD CHSPipe::ReadString(char *lpszOutString, DWORD ReadSize) {
	if (lpszOutString == nullptr)return 0;
	DWORD dwRead = this->Read(lpszOutString, ReadSize * sizeof(char));
	return dwRead / sizeof(char);
}

//Wide(Unicode)版文字列書き込み
DWORD CHSPipe::WriteString(wchar_t *lpszWriteString) {
	if (lpszWriteString==nullptr)return 0;
	DWORD dwWrote = this->Write(lpszWriteString, lstrlenW(lpszWriteString) * sizeof(wchar_t));
	return dwWrote / sizeof(wchar_t);

}
DWORD CHSPipe::ReadString(wchar_t *lpszOutString, DWORD ReadSize) {
	if (lpszOutString == nullptr)return 0;
	DWORD dwRead = this->Read(lpszOutString, ReadSize * sizeof(wchar_t));
	return dwRead / sizeof(wchar_t);
}

