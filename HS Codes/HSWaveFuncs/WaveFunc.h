#include "WaveHeader.h"
#include "WaveConst.h"
#include "WaveType.h"
#include <Shlwapi.h>
#include <math.h>
#ifndef	WAVEFUNC_H
	#define	WAVEFUNC_H
	

	//いくつかのライブラリをリンク
	#pragma comment( lib , "Shlwapi.lib" )
	#pragma comment( lib , "Kernel32.lib" )
	#pragma comment( lib , "Comdlg32.lib" )
	#pragma comment( lib , "winmm.lib" )
	
	/*
		マクロ登録
	*/
	#define GDWFI_CD		1

	/*
		I/O系関数
	*/
	//WAVEファイルを開く関数
	#ifdef __cplusplus
		HANDLE		OpenWaveFile(TCHAR *lpWaveFileAd , BOOL isWrite = FALSE , BOOL isShareRead = FALSE);
	#else
		HANDLE		OpenWaveFile(TCHAR *lpWaveFileAd , BOOL isWrite , BOOL isShareRead);
	#endif

	//WAVEファイルを空にして開く関数
	HANDLE			OpenWaveFileWithClear(TCHAR *lpWaveFileAd);

	//WAVEファイルを作成する関数
	HANDLE			CreateWaveFile(TCHAR *lpWaveFileAd);

	//WAVEファイルを作成する関数(自動判別版)
	HANDLE			CreateWaveFileAuto(TCHAR *lpWaveFileAd);
	
	//WAVEファイルがあるかを確認する関数
	BOOL			ExistWaveFile(TCHAR *lpWaveFileAd);

	//WAVEファイルを削除する関数
	BOOL			DeleteWaveFile(TCHAR *lpWaveFileAd);

	//WAVEファイルを閉じる関数
	BOOL			CloseWaveFile(HANDLE hWaveFile);


	/*
		情報取得系関数
	*/
	//WAVEファイルのファイルサイズを取得
	unsigned __int32	GetWaveFileSize(HANDLE hWaveFile);

	//WAVEファイルのヘッダーからファイルサイズを取得
	unsigned __int32	GetWaveFileSizeFromHeader(HANDLE hWaveFile);

	//WAVEファイルを検査する関数
	BOOL			CheckWaveFile(HANDLE	hWaveFile);

	//WAVEファイルの情報を取得
	BOOL			GetWaveInfo(HANDLE	hWaveFile , LPWaveFileInfo lpWFI);

	//指定されたフォーマットかを確認する関数
	BOOL			CheckWaveFormat(LPWaveFileBaseHeader lpwfh,WORD NumberOfFormatTag , WORD NumberOfChannels , DWORD SamplePerSec   , WORD BitPerSample);

	//指定されたフォーマットかを確認する関数(ファイルバージョン)
	BOOL			CheckWaveFormatFromFile(HANDLE hWaveFile,WORD NumberOfFormatTag , WORD NumberOfChannels , DWORD SamplePerSec   , WORD BitPerSample);

	//対応している方式か確認する関数
	BOOL			CheckWaveFormatType(LPWaveFileBaseHeader lpwfh);
	//対応している方式か確認する関数
	BOOL			CheckWaveFormatTypeFromFile(HANDLE hWaveFile);






	/*
		チャンク情報取得系関数
	*/
	//WAVEファイルの指定したチャンクを検索してその位置を返す関数
	unsigned __int32	GetWaveChunkPos(HANDLE hWaveFile , char *lpChunk);

	//WAVEファイルの指定したチャンクのサイズを返す関数
	unsigned __int32	GetWaveChunkSize(HANDLE hWaveFile , char *lpChunk);
	
	//WAVEファイルの指定したチャンクデータを取得する関数
#ifdef __cplusplus
	unsigned __int32	ReadWaveChunkData(HANDLE hWaveFile , char *lpChunk, BYTE *lpData , unsigned __int32 readsize = 0);
#else
	unsigned __int32	ReadWaveChunkData(HANDLE hWaveFile , char *lpChunk, BYTE *lpData , unsigned __int32 readsize);
#endif

	/*
		波形データ読み込み関連
	*/

	//波形データをサンプル単位で読み込む関数
	unsigned __int32	ReadWaveDataSamples(HANDLE hWaveFile , void *lpdata , unsigned __int32 StartSamples  , unsigned __int32 ReadSamples);
	
	//波形データを秒単位で読み込む関数
	unsigned __int32	ReadWaveDataSeconds(HANDLE hWaveFile , void *lpdata ,  unsigned __int32 StartSeconds  , unsigned __int32 ReadSeconds);

	/*
		書き込み用フォーマット情報準備関連関数
	*/

	//指定されたフォーマット情報を構造体に入れる
	BOOL			SetWaveFormatToStruct(LPWaveFileBaseHeader lpwfh ,WORD NumberOfFormatTag , WORD NumberOfChannels , DWORD SamplePerSec   , WORD BitPerSample);

	//既定のWAVEファイルヘッダーを取得する
#ifdef __cplusplus
	BOOL			GetWaveDefaultHeader(LPWaveFileBaseHeader lpwfh,int mode = 0);
#else
	BOOL			GetWaveDefaultHeader(LPWaveFileBaseHeader lpwfh,int mode );
#endif
	/*
		書き込み関連関数
	*/

	//ヘッダーを書き込む
#ifdef __cplusplus
	BOOL			WriteWaveHeader(HANDLE hWaveFile , LPWaveFileBaseHeader lpwfh = NULL);
#else
	BOOL			WriteWaveHeader(HANDLE hWaveFile , LPWaveFileBaseHeader lpwfh);
#endif

	//波形データを書き込む
	BOOL			WriteWaveData(HANDLE hWaveFile , void *lpData , unsigned __int32 size);

	/*ビットレート変換用*/

	//(8/16/24/32)bit -> 32bit float
	WAVE32F			WaveNBitSampleTo32BitFloat(void *data,int bitrate);
	
	//32bit float ->(8/16/24/32)bit
	BOOL			Wave32BitFloatSampleToNBit(WAVE32F indata , void *outdata , int targetbit);

	/*
		データ編集援助用
	*/
	BOOL			WaveNormalize(void *indata  , unsigned __int32 insize, double *outdata , PCMWAVEFORMAT *informat);
	BOOL			WaveUnNormalize(double *indata  ,unsigned __int32 insize ,void *outdata ,PCMWAVEFORMAT  *outformat);
	BOOL			WaveCutChannel(void *indata ,unsigned __int32 insize, void *outL_Channel , void *outR_Channel , int bitrate);
	BOOL			WaveMixChannel(void *inL_Channel , void *inR_Channel , unsigned __int32 insize , void *outdata , int bitrate);


	/*
		ダイアログ系
	*/
	//#define WaveFileOpenDlg WaveFileOpenDialog
	//#define WaveFileSaveDlg WaveFileSaveDialog

	#ifndef __cplusplus
		//WAVEファイル選択ダイアログ（開く用)
		BOOL			WaveFileOpenDialog(HWND hwnd , TCHAR *lpszFileName , UINT32 size ,  TCHAR *lpszDefaultDirectory);
		
		//WAVEファイル選択ダイアログ（保存用)
		BOOL			WaveFileSaveDialog(HWND hwnd , TCHAR *lpszFileName , UINT32 size , TCHAR *lpszDefaultDirectory);
	#else
		//WAVEファイル選択ダイアログ（開く用)
		BOOL			WaveFileOpenDialog(HWND hwnd , TCHAR *lpszFileName , UINT32 size ,  TCHAR *lpszDefaultDirectory = NULL);
		
		//WAVEファイル選択ダイアログ（保存用)
		BOOL			WaveFileSaveDialog(HWND hwnd , TCHAR *lpszFileName , UINT32 size , TCHAR *lpszDefaultDirectory = NULL);
	#endif


	/*
		メモリ関係
	*/

	void *				WaveMemoryAlloc(WORD NumberOfChannels , DWORD NumberOfSamples   , WORD BitPerSample);

	BOOL				WaveMemoryFree(void *lpData);
#endif	/*WAVEFUNC_H*/