/************************************************************************************
	CHSRIFFReader Class
	- RIFF(Resource Interchange File Format) Reader Class -
	Copyright (C) 2014 HiroakiSoftware. All rights reserved.
************************************************************************************/

#include "CHSRIFFReader.hpp"

CHSRIFFReader::CHSRIFFReader(TCHAR *lpszRiffFilePath) {
	this->hFile = NULL;
	memset(this->szFilePath, 0, sizeof(this->szFilePath));
	if (lpszRiffFilePath) this->Open(lpszRiffFilePath);
}

CHSRIFFReader::~CHSRIFFReader() {
	this->Close();
}


bool CHSRIFFReader::OpenedCallBack(void) {
	return true;
}

bool CHSRIFFReader::BeforeCloseCallBack(void) {

	return true;
}

bool CHSRIFFReader::Open(TCHAR *lpszRiffFilePath) {

	if (!lpszRiffFilePath) return false;

	if (this->hFile) return false;


	HANDLE FileHandle;

	FileHandle = CreateFile(lpszRiffFilePath , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL);

	if (FileHandle == INVALID_HANDLE_VALUE) {
		return false;
	}
	
	THSRIFF_CHUNKHEADER header;
	DWORD readsize = 0;
	SetFilePointer(FileHandle , 0 , 0 , FILE_BEGIN);

	ReadFile(FileHandle , &header , sizeof(header) , &readsize , 0);
	
	if (readsize != sizeof(header)) {
		CloseHandle(FileHandle);
		return false;
	}

	if (_strnicmp(header.Name, "RIFF", 4)) {
		CloseHandle(FileHandle);
		return false;
	}


	LARGE_INTEGER li;
	if (GetFileSizeEx(FileHandle, &li) == false) {
		CloseHandle(FileHandle);
		return false;
	}

	if (li.QuadPart != (header.Size + 8)) {
		CloseHandle(FileHandle);
		return false;
	}

	SetFilePointer(FileHandle , 0 , 0 , FILE_BEGIN);
	this->hFile = FileHandle;

	if (PathIsRelative(lpszRiffFilePath)) {
		PTCHAR lplpFilename;
		if (GetFullPathName(lpszRiffFilePath, MAX_PATH, this->szFilePath, &lplpFilename) == NULL) {
			lstrcpy(this->szFilePath , lpszRiffFilePath);
		}

	} else {
		lstrcpy(this->szFilePath , lpszRiffFilePath);
	}

	if (this->OpenedCallBack() == false) {
		this->Close();
		return false;
	}

	return true;
}
bool CHSRIFFReader::Close(void) {
	if (this->hFile == NULL) return false;
	if (this->BeforeCloseCallBack() == false) return false;
	CloseHandle(this->hFile);
	this->hFile = NULL;
	memset(this->szFilePath, 0, sizeof(this->szFilePath));
	return true;
}

//RIFFファイルを変える
bool CHSRIFFReader::Change(TCHAR *lpszNewRiffFilePath) {
	if (!lpszNewRiffFilePath) return false;
	if (PathFileExists(lpszNewRiffFilePath) == false) return false;
	if (this->CheckPresent()){
		if(this->Close()){
			if (this->Open(lpszNewRiffFilePath)){
				return true;
			}
		}
	}

	return false;
}

bool CHSRIFFReader::CheckPresent(void) {
	if (this->hFile) return true;
	return false;
}


bool CHSRIFFReader::GetFilePath(TCHAR *lpszRiffFilePath) {
	if ((this->hFile)&&(lstrlen(this->szFilePath))) {
		if (lpszRiffFilePath) {
			lstrcpy(lpszRiffFilePath , this->szFilePath);
			return true;
		}
	}
	return false;
}


//ファイルのサイズを取得
UINT32 CHSRIFFReader::GetFileSize(void) {
	if (this->hFile) {

		LARGE_INTEGER li;
		if (GetFileSizeEx(this->hFile, &li) == false) {
			return 0;
		}

		return (UINT32)li.QuadPart;
	}
	return 0;
}

//RIFFチャンクのデータサイズ(ファイルサイズ-8)を取得
UINT32 CHSRIFFReader::GetRiffDataSize(void) {
	
	if (this->hFile) {
		SetFilePointer(this->hFile , 4 , 0 , FILE_BEGIN);
		UINT32 size;
		DWORD rs = 0;

		ReadFile(this->hFile , &size , 4 , &rs , 0);
		if (rs != 4) return 0;
		return size;
	}
	return 0;
}


bool CHSRIFFReader::GetIdentifier(char *lpFileType) {
	if (this->hFile == NULL) return false;

	char Type[4];
	DWORD dwLoad = 0;
	SetFilePointer(this->hFile , 8 , 0 , FILE_BEGIN);

	ReadFile(this->hFile , Type , 4 , &dwLoad , 0);

	if (dwLoad != 4) return false;

	memcpy(lpFileType , Type , 4);

	return true;
}


bool CHSRIFFReader::CheckIdentifier(char *lpFileType) {
	char FileType[4];
	if (this->GetIdentifier(FileType)) {
		if (_strnicmp(FileType , lpFileType , 4))return false;
		return true;
	}
	return false;
}


//指定したチャンクの位置を取得する
bool CHSRIFFReader::GetChunkPos(char *lpChunkName, UINT32 *lpChunkPos) {

	LARGE_INTEGER LI;
	char ChunkText[5];
	THSRIFF_CHUNKHEADER ChunkHeader;
	bool bFound = false;
	DWORD	rs;

	//引数などに問題あるかを確認
	if(!this->hFile | !lpChunkName)return FALSE;	
	if((lstrlenA(lpChunkName)>=5) || (lstrlenA(lpChunkName)==0))return FALSE;

	//検索対象のチャンク名の準備(lpChunkの文字列のサイズに最大3バイトの空白を追加する。)
	int iChunkTextNums = lstrlenA(lpChunkName); 
	memset(ChunkText,0,5);
	memcpy(ChunkText,lpChunkName,iChunkTextNums);
	if(iChunkTextNums!=4)memset(ChunkText+iChunkTextNums,' ',(4-iChunkTextNums));

	//スタート位置
	LI.QuadPart = DHSRIFF_FIRSTSUBCHUNKPOS;

	/*
		以下に提示したいずれかの条件に該当するまで無限ループでスキャン
		条件1：見つかる
		条件2：ファイルの終端にたどり着く
		条件3：ループ内で使用した関数でエラーが起きた

	*/
	while (1) {

		//ファイルポインタの移動に失敗した場合はループから外れる
		if(!SetFilePointerEx(this->hFile,LI,NULL,FILE_BEGIN))break;

		//8バイト(チャンク名+チャンクサイズ)分の読み込みに失敗した場合はループから外れる
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)break;
			
		//8バイト読めていなければループから外れる
		if(rs != 8)break;
		
		//指定されたチャンクならばCKFound = TRUE として ループを外れる(成功)
		if(!_strnicmp(ChunkText,ChunkHeader.Name,4)){
			bFound = true;
			break;
		}
		/*
			次ループでの読み込み位置を次の手順で計算
			手順1.チャンクサイズが奇数ならば1加算して偶数にする
			手順2.手順1の結果 + 8をする。
			手順3.LI.QuadPartに手順2の結果を加算

		*/
	
		//chunksizeが奇数ならば1加算して偶数にする
		if (ChunkHeader.Size % 2)ChunkHeader.Size++;

		//LI.QuadPartに次のチャンクへのオフセットを加算する
		LI.QuadPart += ChunkHeader.Size + 8;
	}

	if (bFound == false) return false;
	if (lpChunkPos) *lpChunkPos = (UINT32)LI.QuadPart;
	return true;
}

//指定したチャンクのサイズを取得する
bool CHSRIFFReader::GetChunkSize(char *lpChunkName, UINT32 *lpChunkSize) {
	UINT32 ChunkPos;
	if (!lpChunkSize)return false;

	if (this->GetChunkPos(lpChunkName, &ChunkPos)) {
		LARGE_INTEGER li;
		DWORD rs;
		THSRIFF_CHUNKHEADER ChunkHeader;
		li.QuadPart = ChunkPos;

		//ファイルポインタの移動に失敗した場合はfalseを返す
		if(!SetFilePointerEx(this->hFile,li,NULL,FILE_BEGIN))return false;

		//8バイト(チャンク名+チャンクサイズ)分の読み込みに失敗した場合はfalseを返す
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)return false;

		*lpChunkSize = ChunkHeader.Size;

		return true;
	}
	return false;
}


//指定したチャンクのデータを取得する
UINT32 CHSRIFFReader::GetChunkData(char *lpChunkName, void *lpValueData, UINT32 ValueDataSize) {
	UINT32 ChunkPos;
	UINT32 ChunkSize;

	UINT32 LoadSize;
	DWORD readSize;

	if (!lpValueData)return 0;

	if (this->GetChunkPos(lpChunkName, &ChunkPos)) {
		if (this->GetChunkSize(lpChunkName, &ChunkSize)) {

			LoadSize = (ChunkSize < ValueDataSize) ? ChunkSize : ValueDataSize;

			LARGE_INTEGER li;
			li.QuadPart = ChunkPos + 8;

			//ファイルポインタの移動に失敗した場合はfalseを返す
			if(!SetFilePointerEx(this->hFile,li,NULL,FILE_BEGIN))return 0;

			//チャンクデータを読み込む
			if(ReadFile(this->hFile,lpValueData,LoadSize,&readSize,NULL)==NULL)return 0;

			return readSize;
		}
	}
	return 0;
}

//チャンクを列挙する
bool CHSRIFFReader::EnumChunk(LPFHSRIFFENUMPROC pProc) {

	if (!pProc) return false;

	LARGE_INTEGER LI;
	LI.QuadPart = DHSRIFF_FIRSTSUBCHUNKPOS;

	THSRIFF_CHUNKHEADER ChunkHeader;
	bool bFound = false;
	DWORD	rs;
	char ChunkType[5] = { 0 };
	THSRIFF_ENUMCHUNKINFO info;

	/*
		以下に提示したいずれかの条件に該当するまで無限ループでスキャン
		条件1：ファイルの終端にたどり着く
		条件2：ループ内で使用した関数でエラーが起きた

	*/
	while (1) {

		memset(&info, 0, sizeof(THSRIFF_ENUMCHUNKINFO));

		//ファイルポインタの移動に失敗した場合はループから外れる
		if(!SetFilePointerEx(this->hFile,LI,NULL,FILE_BEGIN))break;

		//8バイト(チャンク名+チャンクサイズ)分の読み込みに失敗した場合はループから外れる
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)break;
			
		//8バイト読めていなければループから外れる
		if(rs != 8)break;
		
		//LISTチャンクならそのタイプを取得
		if (!_strnicmp("LIST", ChunkHeader.Name, 4)) {

			//4バイトの読み込みに成功した場合のみコールバック関数に渡す構造体にコピー
			if (ReadFile(this->hFile, ChunkType, 4, &rs, NULL)) {
				if (rs == 4) {
					lstrcpyA(info.Type , ChunkType);
				}
			}
		}


		info.Position = (UINT32)LI.QuadPart;
		info.Size = ChunkHeader.Size;


		memcpy(info.Name , ChunkHeader.Name ,  4);


		//コールバック関数を呼んで、falseが返ってきたら列挙を終了する
		if (pProc(this , &info) == false) break;


		/*
			次ループでの読み込み位置を次の手順で計算
			手順1.チャンクサイズが奇数ならば1加算して偶数にする
			手順2.手順1の結果 + 8をする。
			手順3.LI.QuadPartに手順2の結果を加算

		*/
	
		//chunksizeが奇数ならば1加算して偶数にする
		if (ChunkHeader.Size % 2)ChunkHeader.Size++;

		//LI.QuadPartに次のチャンクへのオフセットを加算する
		LI.QuadPart += ChunkHeader.Size + 8;

	}

	return true;
}



//指定したLISTチャンクの位置を取得
bool CHSRIFFReader::GetListChunkPos(char *lpType, UINT32 *lpChunkPos) {

	LARGE_INTEGER LI;
	char TypeText[5];
	THSRIFF_LISTCHUNKHEADER ListChunkHeader;
	bool bFound = false;
	DWORD	rs;

	//引数などに問題あるかを確認
	if(!this->hFile | !lpType)return FALSE;	
	if((lstrlenA(lpType)>=5) || (lstrlenA(lpType)==0))return FALSE;

	//検索対象のチャンク名の準備(lpChunkの文字列のサイズに最大3バイトの空白を追加する。)
	int iTypeTextNums = lstrlenA(lpType); 
	memset(TypeText,0,5);
	memcpy(TypeText,lpType,iTypeTextNums);
	if(iTypeTextNums!=4)memset(TypeText+iTypeTextNums,' ',(4-iTypeTextNums));

	//スタート位置
	LI.QuadPart = DHSRIFF_FIRSTSUBCHUNKPOS;

	/*
		以下に提示したいずれかの条件に該当するまで無限ループでスキャン
		条件1：見つかる
		条件2：ファイルの終端にたどり着く
		条件3：ループ内で使用した関数でエラーが起きた

	*/
	while (1) {

		//ファイルポインタの移動に失敗した場合はループから外れる
		if(!SetFilePointerEx(this->hFile,LI,NULL,FILE_BEGIN))break;

		//8バイト(チャンク名+チャンクサイズ)分の読み込みに失敗した場合はループから外れる
		if(ReadFile(this->hFile,&ListChunkHeader,sizeof(THSRIFF_LISTCHUNKHEADER),&rs,NULL)==NULL)break;
			
		//8バイト読めていなければループから外れる
		if(rs != sizeof(THSRIFF_LISTCHUNKHEADER))break;
		
		//指定されたチャンクならばCKFound = TRUE として ループを外れる(成功)
		if(!_strnicmp("LIST",ListChunkHeader.ChunkHeader.Name,4)){
			if (!_strnicmp(TypeText, ListChunkHeader.Type, 4)) {
				bFound = true;
				break;
			}
		}
		/*
			次ループでの読み込み位置を次の手順で計算
			手順1.チャンクサイズが奇数ならば1加算して偶数にする
			手順2.手順1の結果 + 8をする。
			手順3.LI.QuadPartに手順2の結果を加算

		*/
	
		//chunksizeが奇数ならば1加算して偶数にする
		if (ListChunkHeader.ChunkHeader.Size % 2)ListChunkHeader.ChunkHeader.Size++;

		//LI.QuadPartに次のチャンクへのオフセットを加算する
		LI.QuadPart += ListChunkHeader.ChunkHeader.Size + 8;
	}

	if (bFound == false) return false;
	if (lpChunkPos) *lpChunkPos = (UINT32)LI.QuadPart;

	return true;
}

//指定したLISTチャンクのサイズを取得
bool CHSRIFFReader::GetListChunkSize(char *lpType, UINT32 *lpChunkSize) {

	UINT32 ChunkPos;
	if (!lpChunkSize)return false;

	if (this->GetListChunkPos(lpType, &ChunkPos)) {
		LARGE_INTEGER li;
		DWORD rs;
		THSRIFF_CHUNKHEADER ChunkHeader;
		li.QuadPart = ChunkPos;

		//ファイルポインタの移動に失敗した場合はfalseを返す
		if(!SetFilePointerEx(this->hFile,li,NULL,FILE_BEGIN))return false;

		//8バイト(チャンク名+チャンクサイズ)分の読み込みに失敗した場合はfalseを返す
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)return false;

		*lpChunkSize = ChunkHeader.Size;

		return true;
	}

	return false;
}


//指定したLISTチャンクにある指定したサブチャンクの位置を取得
bool CHSRIFFReader::GetListSubChunkPos(char *lpType, char *lpChunkName, UINT32 *lpChunkPos) {

	LARGE_INTEGER LI;
	char ChunkText[5];
	THSRIFF_CHUNKHEADER ChunkHeader;
	bool bFound = false;
	DWORD	rs;

	//引数などに問題あるかを確認
	if(!this->hFile | !lpChunkName)return FALSE;	
	if((lstrlenA(lpChunkName)>=5) || (lstrlenA(lpChunkName)==0))return FALSE;

	//検索対象のチャンク名の準備(lpChunkの文字列のサイズに最大3バイトの空白を追加する。)
	int iChunkTextNums = lstrlenA(lpChunkName); 
	memset(ChunkText,0,5);
	memcpy(ChunkText,lpChunkName,iChunkTextNums);
	if(iChunkTextNums!=4)memset(ChunkText+iChunkTextNums,' ',(4-iChunkTextNums));

	//スタート位置
	UINT32 ListPos , ListSize , QuitPos;

	if(this->GetListChunkPos(lpType ,&ListPos) == false) return false;
	if(this->GetListChunkSize(lpType ,&ListSize) == false) return false;

	ListSize -= 4;
	LI.QuadPart = ListPos + 12;

	QuitPos = ListPos + 12 + ListSize;

	/*
		以下に提示したいずれかの条件に該当するまで無限ループでスキャン
		条件1：見つかる
		条件2：ファイルの終端にたどり着く
		条件3：ループ内で使用した関数でエラーが起きた

	*/
	while (1) {

		//ファイルポインタの移動に失敗した場合はループから外れる
		if(!SetFilePointerEx(this->hFile,LI,NULL,FILE_BEGIN))break;

		//8バイト(チャンク名+チャンクサイズ)分の読み込みに失敗した場合はループから外れる
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)break;
			
		//8バイト読めていなければループから外れる
		if(rs != 8)break;
		
		//指定されたチャンクならばCKFound = TRUE として ループを外れる(成功)
		if(!_strnicmp(ChunkText,ChunkHeader.Name,4)){
			bFound = true;
			break;
		}
		/*
			次ループでの読み込み位置を次の手順で計算
			手順1.チャンクサイズが奇数ならば1加算して偶数にする
			手順2.手順1の結果 + 8をする。
			手順3.LI.QuadPartに手順2の結果を加算

		*/
	
		//chunksizeが奇数ならば1加算して偶数にする
		if (ChunkHeader.Size % 2)ChunkHeader.Size++;

		//LI.QuadPartに次のチャンクへのオフセットを加算する
		LI.QuadPart += ChunkHeader.Size + 8;

		if (LI.QuadPart >= QuitPos) break;
	}

	if (bFound == false) return false;
	if (lpChunkPos) *lpChunkPos = (UINT32)LI.QuadPart;
	return true;
}
	
//指定したLISTチャンクにあるサブチャンクを列挙する
bool CHSRIFFReader::EnumListSubChunk(char *lpType, LPFHSRIFFENUMPROC pProc) {

	if (!pProc) return false;

	LARGE_INTEGER LI;

	//スタート位置
	UINT32 ListPos , ListSize , QuitPos;

	if(this->GetListChunkPos(lpType ,&ListPos) == false) return false;
	if(this->GetListChunkSize(lpType ,&ListSize) == false) return false;

	ListSize -= 4;
	LI.QuadPart = ListPos + 12;

	QuitPos = ListPos + 12 + ListSize;

	THSRIFF_CHUNKHEADER ChunkHeader;
	bool bFound = false;
	DWORD	rs;
	char ChunkType[5] = { 0 };
	THSRIFF_ENUMCHUNKINFO info;

	/*
		以下に提示したいずれかの条件に該当するまで無限ループでスキャン
		条件1：ファイルの終端にたどり着く
		条件2：ループ内で使用した関数でエラーが起きた

	*/
	while (1) {

		memset(&info, 0, sizeof(THSRIFF_ENUMCHUNKINFO));

		lstrcpyA(info.Type , lpType);


		//ファイルポインタの移動に失敗した場合はループから外れる
		if(!SetFilePointerEx(this->hFile,LI,NULL,FILE_BEGIN))break;

		//8バイト(チャンク名+チャンクサイズ)分の読み込みに失敗した場合はループから外れる
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)break;
			
		//8バイト読めていなければループから外れる
		if(rs != 8)break;
		
		info.Position = (UINT32)LI.QuadPart;
		info.Size = ChunkHeader.Size;

		memcpy(info.Name , ChunkHeader.Name ,  4);


		//コールバック関数を呼んで、falseが返ってきたら列挙を終了する
		if (pProc(this , &info) == false) break;


		/*
			次ループでの読み込み位置を次の手順で計算
			手順1.チャンクサイズが奇数ならば1加算して偶数にする
			手順2.手順1の結果 + 8をする。
			手順3.LI.QuadPartに手順2の結果を加算

		*/
	
		//chunksizeが奇数ならば1加算して偶数にする
		if (ChunkHeader.Size % 2)ChunkHeader.Size++;

		//LI.QuadPartに次のチャンクへのオフセットを加算する
		LI.QuadPart += ChunkHeader.Size + 8;

		if (LI.QuadPart >= QuitPos) break;

	}

	return true;
}

//指定したLISTチャンクにある指定したサブチャンクのサイズを取得
bool CHSRIFFReader::GetListSubChunkSize(char *lpType, char *lpChunkName, UINT32 *lpChunkSize) {

	UINT32 ChunkPos;
	if (!lpChunkSize)return false;

	if (this->GetListSubChunkPos(lpType , lpChunkName, &ChunkPos)) {
		LARGE_INTEGER li;
		DWORD rs;
		THSRIFF_CHUNKHEADER ChunkHeader;
		li.QuadPart = ChunkPos;

		//ファイルポインタの移動に失敗した場合はfalseを返す
		if(!SetFilePointerEx(this->hFile,li,NULL,FILE_BEGIN))return false;

		//8バイト(チャンク名+チャンクサイズ)分の読み込みに失敗した場合はfalseを返す
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)return false;

		*lpChunkSize = ChunkHeader.Size;

		return true;
	}
	return false;
}
	
//指定したLISTチャンクにある指定したサブチャンクを取得
UINT32 CHSRIFFReader::GetListSubChunkData(char *lpType, char *lpChunkName, void *lpValueData, UINT32 ValueDataSize) {


	UINT32 ChunkPos;
	UINT32 ChunkSize;

	UINT32 LoadSize;
	DWORD readSize;

	if (!lpValueData)return 0;

	if (this->GetListSubChunkPos(lpType , lpChunkName, &ChunkPos)) {
		if (this->GetListSubChunkSize(lpType , lpChunkName, &ChunkSize)) {

			LoadSize = (ChunkSize < ValueDataSize) ? ChunkSize : ValueDataSize;

			LARGE_INTEGER li;
			li.QuadPart = ChunkPos + 8;

			//ファイルポインタの移動に失敗した場合はfalseを返す
			if(!SetFilePointerEx(this->hFile,li,NULL,FILE_BEGIN))return 0;

			//8バイト(チャンク名+チャンクサイズ)分の読み込みに失敗した場合はfalseを返す
			if(ReadFile(this->hFile,lpValueData,LoadSize,&readSize,NULL)==NULL)return 0;

			return readSize;
		}
	}
	return 0;
}
