#include "CHSSimpleLogger.h"


CHSSimpleLogger::CHSSimpleLogger() {
	this->hFile = NULL;
}


CHSSimpleLogger::~CHSSimpleLogger() {
	this->Close();
}

std::basic_string<TCHAR> CHSSimpleLogger::MakeTimeStamp(void) {
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::basic_string<TCHAR> TargetString;
	TCHAR szString[24];	
	_stprintf_s(szString, TEXT("[%d/%02d/%02d][%02d:%02d:%02d]"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	TargetString = szString;
	return  TargetString;
}

std::basic_string<TCHAR> CHSSimpleLogger::MakeWriteString(TCHAR *lpszClassName, TCHAR *lpszFuncName, TCHAR *lpszTitleString, TCHAR *lpszLogMessage){

	std::basic_string<TCHAR> TargetString;
	std::basic_string<TCHAR> TimeStamp = CHSSimpleLogger::MakeTimeStamp();
	TargetString = TimeStamp;
	TCHAR *lpszBufString = nullptr;
	int length = 0;

	if (lpszClassName){
		length = lstrlen(lpszClassName);
		if (length) {
			if (lpszFuncName) {
				int curlength = lstrlen(lpszFuncName);
				if (curlength) {
					TCHAR strMethod[] = TEXT(" メソッド");
					length +=curlength +  lstrlen(strMethod) + 4;
					lpszBufString = new TCHAR[length + 1];
					memset(lpszBufString, 0, sizeof(TCHAR)*(length + 1));
					_stprintf_s(lpszBufString, length + 1, TEXT("[%s.%s%s]"), lpszClassName, lpszFuncName, strMethod);
				}
			}

			if (lpszBufString == nullptr) {
				TCHAR strClass[] = TEXT(" クラス");
				length += lstrlen(strClass) + 2;
				lpszBufString = new TCHAR[length + 1];
				memset(lpszBufString, 0, sizeof(TCHAR)*(length + 1));
				_stprintf_s(lpszBufString, length + 1, TEXT("[%s%s]"), lpszClassName,strClass);
			}
		}
	}
	
	if (lpszFuncName && (lpszBufString ==nullptr)) {
		length =  lstrlen(lpszFuncName);
		if (length) {
			TCHAR strFunction[] = TEXT(" 関数");
			length += lstrlen(strFunction) + 2;
			lpszBufString = new TCHAR[length + 1];
			memset(lpszBufString, 0, sizeof(TCHAR)*(length + 1));
			_stprintf_s(lpszBufString, length + 1, TEXT("[%s%s]"), lpszFuncName,strFunction);
		}
	}

	if (lpszBufString) {
		TargetString += lpszBufString;
		delete[] lpszBufString;
	}

	if (lpszTitleString) {
		TargetString += TEXT("[");
		TargetString += lpszTitleString;
		TargetString += TEXT("]");
	}

	if (lpszLogMessage) {
		TargetString += TEXT(" ");
		TargetString += lpszLogMessage;
	}
	return TargetString;
}

BOOL CHSSimpleLogger::Write(TCHAR *lpszLogFormatMessage, ...) {
	va_list arglist;
	va_start(arglist, lpszLogFormatMessage);
	BOOL ret = this->Save(nullptr, nullptr, nullptr, lpszLogFormatMessage, arglist);
	va_end(arglist);
	return ret;
}
BOOL CHSSimpleLogger::Write(TCHAR *lpszFuncName, TCHAR *lpszLogFormatMessage, ...) {
	va_list arglist;
	va_start(arglist, lpszLogFormatMessage);
	BOOL ret = this->Save(nullptr, lpszFuncName, nullptr, lpszLogFormatMessage, arglist);
	va_end(arglist);
	return ret;
}

BOOL CHSSimpleLogger::Write(TCHAR *lpszClassName, TCHAR *lpszFuncName, TCHAR *lpszLogFormatMessage, ...) {
	va_list arglist;
	va_start(arglist, lpszLogFormatMessage);
	BOOL ret = this->Save(lpszClassName, lpszFuncName, nullptr, lpszLogFormatMessage, arglist);
	va_end(arglist);
	return ret;
}

BOOL CHSSimpleLogger::WriteForFuncInNamespace(TCHAR *lpszNamespaceName, TCHAR *lpszFuncName, TCHAR *lpszLogFormatMessage, ...) {

	if ((lpszFuncName == nullptr) || (lpszNamespaceName == nullptr))return FALSE;

	va_list arglist;
	va_start(arglist, lpszLogFormatMessage);

	TCHAR *lpszFullFuncName;
	int length = lstrlen(lpszNamespaceName) + lstrlen(lpszFuncName) + 3;
	
	lpszFullFuncName = new TCHAR[length];

	_stprintf_s(lpszFullFuncName, length, TEXT("%s::%s"), lpszNamespaceName, lpszFuncName);

	BOOL ret = this->Save(nullptr, lpszFullFuncName, nullptr, lpszLogFormatMessage, arglist);
	va_end(arglist);
	delete[] lpszFullFuncName;
	return TRUE;
}
BOOL CHSSimpleLogger::WriteWithTitle(TCHAR *lpszTitleName, TCHAR *lpszLogFormatMessage, ...) {
	va_list arglist;
	va_start(arglist, lpszLogFormatMessage);
	BOOL ret = this->Save(nullptr, nullptr, lpszTitleName, lpszLogFormatMessage, arglist);
	va_end(arglist);
	return ret;
}

BOOL CHSSimpleLogger::Save(TCHAR *lpszClassName, TCHAR *lpszFuncName, TCHAR *lpszTitleString, TCHAR *lpszLogFormatMessage, va_list  ArgListEx) {
	TCHAR *lpszLogMessage = nullptr;
	int LogMessageSize = _vsctprintf(lpszLogFormatMessage, ArgListEx) + 1;
	lpszLogMessage = new TCHAR[LogMessageSize];
	_vstprintf_s(lpszLogMessage, LogMessageSize,lpszLogFormatMessage, ArgListEx);
	std::basic_string<TCHAR> writestring = this->MakeWriteString(lpszClassName, lpszFuncName, lpszTitleString,lpszLogMessage);
	delete[] lpszLogMessage;

	int WriteStringLength = writestring.length();
	TCHAR *lpWriteString = new TCHAR[WriteStringLength + 1];
	_tcscpy_s(lpWriteString, WriteStringLength + 1, writestring.c_str());
	BOOL ret = this->SaveToFile(this->hFile, lpWriteString);
	delete[] lpWriteString;

	return ret;
}

BOOL CHSSimpleLogger::SaveToFile(HANDLE hFile, TCHAR *lpszLineString) {	
	if (hFile) {
		if (SetFilePointer(hFile, 0, 0, FILE_END) == INVALID_SET_FILE_POINTER)return FALSE;
	
		TCHAR CRLF[] = TEXT("\r\n"); //改行コード(CR+LF)
		DWORD WroteSize;
		DWORD WriteSize = sizeof(TCHAR) * lstrlen(lpszLineString);
		BOOL bret = WriteFile(hFile, lpszLineString, WriteSize, &WroteSize, NULL);
		if ((bret == FALSE) || (WroteSize != WriteSize))return FALSE;

		WriteSize = sizeof(TCHAR) * lstrlen(CRLF);
		WroteSize = 0;
		bret = WriteFile(hFile, CRLF, WriteSize, &WroteSize, NULL);
		if ((bret == FALSE) || (WroteSize != WriteSize))return FALSE;

		return  TRUE;
	}
	return FALSE;
}


BOOL CHSSimpleLogger::Create(TCHAR *lpszFilePath) {

	if (this->hFile)return FALSE;
	if (!lpszFilePath)return FALSE;

	HANDLE FileHandle = NULL;

	int CreateFlag = TRUNCATE_EXISTING;
	if (_taccess_s(lpszFilePath, 0) == ENOENT)CreateFlag = CREATE_NEW;

	FileHandle = CreateFile(lpszFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CreateFlag, FILE_ATTRIBUTE_NORMAL, NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)return FALSE;
	this->hFile = FileHandle;

#ifdef _UNICODE //Unicode(UTF-16)の時、BOMを追加
	//BOM確保領域
	unsigned char BOM[2] = { 0xFE, 0xFF };//ビッグエンディアンのBOM
	int forBOMCheck = 1;
	if (*((char*)&forBOMCheck)) {
		//リトルエンディアン
		BOM[0] = 0xFF;
		BOM[1] = 0xFE;
	}
	DWORD dwWrote = 0;
	WriteFile(FileHandle, BOM, 2, &dwWrote, NULL);
#endif

	return TRUE;
}

BOOL CHSSimpleLogger::Close(void) {
	BOOL bRet = FALSE;
	if (this->hFile) {
		bRet = CloseHandle(this->hFile);
		if (bRet) this->hFile = NULL;
	}
	return bRet;
}
