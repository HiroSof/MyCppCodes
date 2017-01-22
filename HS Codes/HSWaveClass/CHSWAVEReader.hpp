/************************************************************************************
	CHSWAVEReader Class
	- WAVE File Reader Class header-
	Copyright (C) 2014 HiroakiSoftware. All rights reserved.
************************************************************************************/

#pragma once
#include "CHSRIFFReader.hpp"

class CHSWAVEReader : public CHSRIFFReader{
private:
	bool OpenedCallBack(void);
	bool BeforeCloseCallBack(void);
	UINT32 dataChunkPos;

public:
	CHSWAVEReader(TCHAR *lpszWaveFilePath = nullptr);
	~CHSWAVEReader();

	//フォーマット取得
	bool GetFormat(PCMWAVEFORMAT *lpFormat);
	bool GetFormat(WAVEFORMAT *lpFormat);
	bool GetFormat(WAVEFORMATEX *lpFormat);
	UINT32 GetFormat(void *lpFormat , UINT32 Size);
	UINT32 GetFormatSize(void);
	
	//再生データのサイズをバイト単位で取得
	UINT32 GetPlayDataSize(void);

	//再生サンプル数を取得
	UINT32 GetPlaySamples(void);

	//再生時間を取得
	UINT32 GetPlayTimeSeconds(void);
	UINT32 GetPlayTimeMilliSeconds(void);

	//全再生データを取得
	bool ReadAllPlayData(void *lpData , UINT32 *lpNumberOfSamplesByRead = nullptr);

	//再生データを取得
	bool ReadPlayData(void *lpData , UINT32 NumberOfReadPositionSamples , UINT32 NumberOfReadSamples , UINT32 *lpNumberOfSamplesByRead);
	bool ReadPlayDataIsInSeconds(void *lpData , UINT32 NumberOfReadPositionSeconds , UINT32 NumberOfReadSeconds , UINT32 *lpNumberOfSamplesByRead);
	bool ReadPlayDataIsInMilliSeconds(void *lpData , UINT32 NumberOfReadPositionMilliSeconds , UINT32 NumberOfReadMilliSeconds , UINT32 *lpNumberOfSamplesByRead);

	//必要なメモリサイズを計算する
	UINT32 CalculateMemorySize(UINT32 Samples);
	UINT32 CalculateMemorySizeIsInSeconds(UINT32 Seconds);
	UINT32 CalculateMemorySizeIsInMilliSeconds(UINT32 MilliSeconds);

	//時間からサンプル数を取得（秒単位とミリ秒単位の２種類用意)
	UINT32 CalculateNumberOfSamplesIsInSeconds(UINT32 Seconds);
	UINT32 CalculateNumberOfSamplesIsInMilliSeconds(UINT32 MilliSeconds);

	//曲名取得
	UINT32 GetSongName(char *lpName , UINT32 size);

	//曲名のサイズを取得
	UINT32 GetSongNameLength(void);

	//アーティスト名
	UINT32 GetSongArtistName(char *lpArtistName , UINT32 size);
	
	//アーティスト名のサイズを取得
	UINT32 GetSongArtistNameLength(void);

	//プロダクト名
	UINT32 GetSongProductName(char *lpProductName , UINT32 size);

	//プロダクト名のサイズを取得
	UINT32 GetSongProductNameLength(void);

	//ジャンル
	UINT32 GetSongGenreText(char *lpGenreText , UINT32 size);

	//ジャンルのサイズを取得
	UINT32 GetSongGenreTextLength(void);

};


