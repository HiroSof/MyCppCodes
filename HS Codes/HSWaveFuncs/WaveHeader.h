/*****************************************************************
*   WAVEファイル情報ヘッダ                                       *
*   ファイル名：WaveHeader.h                                     *
******************************************************************
* Copyright (C) 2011-2012 HiroakiSoftware. All rights reserved.  *
*****************************************************************/

//windows.hのインクルード
#include <windows.h>

//_HS_WaveHeader_というマクロが登録されていなければ以後の情報を読む
#ifndef _HS_WaveHeader_
	
	//_HS_WaveHeader_というマクロを登録
	#define _HS_WaveHeader_

	//構造体宣言部
	//WaveFileHeader構造体
	typedef struct tagWaveFileBaseHeader{
		char	sChunkID[4];						//"RIFF"
		unsigned __int32 uChunkSize;				//以後のファイルサイズ（ファイルサイズ-8）
		char	szFromType[4];						//"WAVE"
		
		//Fmtチャンク
		struct{
			char	sChunkID[4];					//"fmt "
			unsigned __int32 uPCMWaveFormatSize;	//フォーマット情報のサイズ
			PCMWAVEFORMAT tagPCMWaveFormat;			//フォーマット情報
		}Fmt;

		//Dataチャンク
		struct{
			char	sChunkID[4];					//"data"
			unsigned __int32 uWaveFormSize;			//波形データのサイズ
		}Data;

	}WaveFileBaseHeader,*PWaveFileBaseHeader,*LPWaveFileBaseHeader;

	//WaveFileInfo構造体
	typedef struct tagWaveFileInfo{
		struct{
			unsigned __int32 uPlayTime;				//再生時間（ミリ秒)
			unsigned __int32 uFileSize;				//ファイルサイズ
			unsigned __int32 uSamples;				//サンプル数
			unsigned __int32 uFmtChunkOffset;		//fmt チャンクへのオフセット
			unsigned __int32 uDataChunkOffset;		//dataチャンクへのオフセット
		}FileInfomation;
		WaveFileBaseHeader tagWaveFileBaseHeader;	//WaveFileHeader構造体
	}WaveFileInfo,*PWaveFileInfo,*LPWaveFileInfo;

#endif	/* _HS_WaveHeader_ */