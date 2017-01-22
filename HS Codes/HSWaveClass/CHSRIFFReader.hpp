/************************************************************************************
	CHSRIFFReader Class
	- RIFF(Resource Interchange File Format) Reader Class Header -
	Copyright (C) 2014 HiroakiSoftware. All rights reserved.
************************************************************************************/
#pragma once
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#pragma comment(lib , "Shlwapi.lib")
#define DHSRIFF_FIRSTSUBCHUNKPOS 12
struct THSRIFF_CHUNKHEADER {
	char Name[4];
	UINT32 Size;
};
struct THSRIFF_LISTCHUNKHEADER {
	THSRIFF_CHUNKHEADER ChunkHeader;
	char Type[4];
};
struct THSRIFF_ENUMCHUNKINFO {
	UINT32 Position;
	char Name[5];
	UINT32 Size;
	char Type[5]; /*LIST Chunk Only*/
};
class CHSRIFFReader;
typedef bool (__stdcall *LPFHSRIFFENUMPROC)(CHSRIFFReader *pReader ,THSRIFF_ENUMCHUNKINFO *lpInfo);

class CHSRIFFReader {
protected:
	TCHAR szFilePath[MAX_PATH];	//ファイル名
	HANDLE hFile;		//ファイルハンドル

	virtual bool OpenedCallBack(void);
	virtual bool BeforeCloseCallBack(void);
public:
	CHSRIFFReader(TCHAR *lpszRiffFilePath = nullptr);
	~CHSRIFFReader();

	//RIFFファイルを開く
	bool Open(TCHAR *lpszRiffFilePath);

	//RIFFファイルを変える
	bool Change(TCHAR *lpszNewRiffFilePath);

	//RIFFファイルを閉じる
	bool Close(void);

	//RIFFファイルが開かれてるかの確認
	bool CheckPresent(void);

	//開かれているファイルのパスを取得する
	bool GetFilePath(TCHAR *lpszRiffFilePath);

	//ファイルのサイズを取得
	UINT32 GetFileSize(void);

	//RIFFチャンクのデータサイズ(ファイルサイズ-8)を取得
	UINT32 GetRiffDataSize(void);

	//RIFFファイルの識別子を照会する
	bool CheckIdentifier(char *lpFileType);

	//RIFFファイルの識別子を取得する
	bool GetIdentifier(char *lpFileType);

	//指定したチャンクの位置を取得する
	bool GetChunkPos(char *lpChunkName , UINT32 *lpChunkPos);

	//指定したチャンクのサイズを取得する
	bool GetChunkSize(char *lpChunkName , UINT32 *lpChunkSize);

	//指定したチャンクのデータを取得する
	UINT32 GetChunkData(char *lpChunkName , void *lpValueData , UINT32 ValueDataSize);

	//チャンクを列挙する
	bool EnumChunk(LPFHSRIFFENUMPROC pProc);

	//指定したLISTチャンクの位置を取得
	bool GetListChunkPos(char *lpType ,  UINT32 *lpChunkPos);

	//指定したLISTチャンクのサイズを取得
	bool GetListChunkSize(char *lpType ,  UINT32 *lpChunkSize);

	//指定したLISTチャンクにある指定したサブチャンクの位置を取得
	bool GetListSubChunkPos(char *lpType , char *lpChunkName , UINT32 *lpChunkPos);
	
	//指定したLISTチャンクにあるサブチャンクを列挙する
	bool EnumListSubChunk(char *lpType , LPFHSRIFFENUMPROC pProc);

	//指定したLISTチャンクにある指定したサブチャンクのサイズを取得
	bool GetListSubChunkSize(char *lpType , char *lpChunkName , UINT32 *lpChunkSize);
	
	//指定したLISTチャンクにある指定したサブチャンクを取得
	UINT32 GetListSubChunkData(char *lpType , char *lpChunkName , void *lpValueData , UINT32 ValueDataSize);
};