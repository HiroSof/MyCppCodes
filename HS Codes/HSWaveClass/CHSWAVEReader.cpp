/************************************************************************************
	CHSWAVEReader Class
	- WAVE File Reader Class -
	Copyright (C) 2014 HiroakiSoftware. All rights reserved.
************************************************************************************/

#include "CHSWAVEReader.hpp"

CHSWAVEReader::CHSWAVEReader(TCHAR *lpszWaveFilePath){
	this->dataChunkPos = 0;
	if (lpszWaveFilePath)this->Open(lpszWaveFilePath);
}

CHSWAVEReader::~CHSWAVEReader() {
	this->Close();
}


bool CHSWAVEReader::OpenedCallBack(void) {
	if (this->CheckIdentifier("WAVE")) {
		if (this->GetChunkPos("fmt", nullptr)) {
			if (this->GetChunkPos("data", &this->dataChunkPos)) {
				return true;
			}
		}
	}
	return false;
}
bool CHSWAVEReader::BeforeCloseCallBack(void) {
	this->dataChunkPos = 0;
	return true;
}

bool CHSWAVEReader::GetFormat(PCMWAVEFORMAT *lpFormat) {

	if (!lpFormat)return false;
	PCMWAVEFORMAT Format;
	UINT32 ReadSize;
	ReadSize =  this->GetChunkData("fmt", &Format, sizeof(PCMWAVEFORMAT));
	if (ReadSize == sizeof(PCMWAVEFORMAT)) {
		*lpFormat = Format;
		return true;
	}
	return false;
}
bool CHSWAVEReader::GetFormat(WAVEFORMAT *lpFormat) {

	if (!lpFormat)return false;
	WAVEFORMAT Format;
	UINT32 ReadSize;
	ReadSize =  this->GetChunkData("fmt", &Format, sizeof(WAVEFORMAT));
	if (ReadSize == sizeof(WAVEFORMAT)) {
		*lpFormat = Format;
		return true;
	}
	return false;
}
bool CHSWAVEReader::GetFormat(WAVEFORMATEX *lpFormat) {
	if (!lpFormat)return false;
	WAVEFORMATEX Format;
	UINT32 ReadSize;

	memset(&Format, 0, sizeof(WAVEFORMATEX));

	ReadSize =  this->GetChunkData("fmt", &Format, sizeof(WAVEFORMATEX));
	if ((ReadSize == sizeof(WAVEFORMATEX))||(ReadSize == sizeof(WAVEFORMAT))||(ReadSize == sizeof(PCMWAVEFORMAT))) {
		*lpFormat = Format;
		return true;
	}
	return false;
}

UINT32 CHSWAVEReader::GetFormat(void *lpFormat, UINT32 Size) {
	if (!lpFormat || (Size == 0))return false;
	return this->GetChunkData("fmt", lpFormat, Size);
}

UINT32 CHSWAVEReader::GetFormatSize(void) {
	UINT32 Size;
	if (this->GetChunkSize("fmt" , &Size))return Size;
	return 0;
}

//再生データのサイズをバイト単位で取得
UINT32 CHSWAVEReader::GetPlayDataSize(void) {
	UINT32 PlayDataSize;
	if (this->GetChunkSize("data" , &PlayDataSize))return PlayDataSize;
	return 0;
}



//再生サンプル数を取得
UINT32 CHSWAVEReader::GetPlaySamples(void) {

	PCMWAVEFORMAT pcmwf;

	UINT32 PlayDataSize;
	PlayDataSize = this->GetPlayDataSize();
	if (PlayDataSize == 0)return 0;

	if (this->GetFormat(&pcmwf) == false) return 0;

	return PlayDataSize  / pcmwf.wf.nBlockAlign;
}

//再生時間を取得
UINT32 CHSWAVEReader::GetPlayTimeSeconds(void) {
	return this->GetPlayTimeMilliSeconds() / 1000;
}
UINT32 CHSWAVEReader::GetPlayTimeMilliSeconds(void) {

	PCMWAVEFORMAT pcmwf;

	UINT32 PlayDataSize , PlayTime;
	PlayDataSize = this->GetPlayDataSize();
	if (PlayDataSize == 0)return 0;

	if (this->GetFormat(&pcmwf) == false) return 0;

	PlayTime = (UINT32)((double)PlayDataSize / (double)pcmwf.wf.nAvgBytesPerSec * 1000.0);

	return PlayTime;
}


//曲名取得
UINT32 CHSWAVEReader::GetSongName(char *lpName, UINT32 size) {
	return this->GetListSubChunkData("INFO", "INAM" , lpName , size);
}

//曲名のサイズを取得
UINT32 CHSWAVEReader::GetSongNameLength(void) {
	UINT32 size;
	if(this->GetListSubChunkSize("INFO", "INAM" , &size)) return size;
	return 0;
}

//アーティスト名
UINT32 CHSWAVEReader::GetSongArtistName(char *lpArtistName, UINT32 size) {
	return this->GetListSubChunkData("INFO", "IART" , lpArtistName , size);
}
	
//アーティスト名のサイズを取得
UINT32 CHSWAVEReader::GetSongArtistNameLength(void) {
	UINT32 size;
	if(this->GetListSubChunkSize("INFO", "IART" , &size)) return size;
	return 0;
}

//プロダクト名
UINT32 CHSWAVEReader::GetSongProductName(char *lpProductName, UINT32 size) {
	return this->GetListSubChunkData("INFO", "IPRD" , lpProductName , size);
}

//プロダクト名のサイズを取得
UINT32 CHSWAVEReader::GetSongProductNameLength(void) {
	UINT32 size;
	if(this->GetListSubChunkSize("INFO", "IPRD" , &size)) return size;
	return 0;
}


//ジャンル
UINT32 CHSWAVEReader::GetSongGenreText(char *lpGenreText, UINT32 size) {
	return this->GetListSubChunkData("INFO", "IGNR" , lpGenreText , size);
}

//ジャンルのサイズを取得
UINT32 CHSWAVEReader::GetSongGenreTextLength(void) {
	UINT32 size;
	if(this->GetListSubChunkSize("INFO", "IGNR" , &size)) return size;
	return 0;
}


//再生データをサンプル単位で取得
bool CHSWAVEReader::ReadPlayData(void *lpData, UINT32 NumberOfReadPositionSamples, UINT32 NumberOfReadSamples, UINT32 *lpNumberOfSamplesByRead) {

	WAVEFORMAT wf;

	if (lpData == nullptr) return false;
	if (NumberOfReadSamples == 0)return false;
	if (lpNumberOfSamplesByRead == nullptr) return false;
	if (this->GetFormat(&wf) == false) return false;

	UINT32 AllSamples = this->GetPlaySamples();
	
	if (AllSamples == 0) return false;
	if (NumberOfReadPositionSamples >= AllSamples) return false;

	UINT32 ReadEndSamples = NumberOfReadPositionSamples + NumberOfReadSamples;

	if (ReadEndSamples > AllSamples) {
		NumberOfReadSamples = AllSamples - NumberOfReadPositionSamples;
	}

	UINT32 NumberOfReadPositionByte = NumberOfReadPositionSamples * wf.nBlockAlign;
	UINT32 NumberOfReadByte = NumberOfReadSamples * wf.nBlockAlign;

	LARGE_INTEGER li;

	//読み込み開始位置を計算(+8 は チャンク名とチャンクサイズの8バイト分）
	li.QuadPart = this->dataChunkPos + NumberOfReadPositionByte + 8;
	
	if (SetFilePointerEx(this->hFile , li , NULL , FILE_BEGIN) == FALSE) return false;

	DWORD dwReadEndSize;

	if(ReadFile(this->hFile , lpData , NumberOfReadByte , &dwReadEndSize , NULL) == FALSE) return false;

	*lpNumberOfSamplesByRead = dwReadEndSize / wf.nBlockAlign;

	return true;
}


bool CHSWAVEReader::ReadPlayDataIsInSeconds(void *lpData, UINT32 NumberOfReadPositionSeconds, UINT32 NumberOfReadSeconds, UINT32 *lpNumberOfSamplesByRead) {
	UINT32 NumberOfReadPositionSamples,  NumberOfReadSamples;

	WAVEFORMAT wf;
	if (this->GetFormat(&wf) == false) return 0;

	NumberOfReadPositionSamples = this->CalculateMemorySizeIsInSeconds(NumberOfReadPositionSeconds) / wf.nBlockAlign;
	NumberOfReadSamples = this->CalculateMemorySizeIsInSeconds(NumberOfReadSeconds) / wf.nBlockAlign;



	return this->ReadPlayData(lpData , NumberOfReadPositionSamples , NumberOfReadSamples , lpNumberOfSamplesByRead);
}
bool CHSWAVEReader::ReadPlayDataIsInMilliSeconds(void *lpData, UINT32 NumberOfReadPositionMilliSeconds, UINT32 NumberOfReadMilliSeconds, UINT32 *lpNumberOfSamplesByRead) {
	UINT32 NumberOfReadPositionSamples,  NumberOfReadSamples;

	WAVEFORMAT wf;
	if (this->GetFormat(&wf) == false) return 0;

	NumberOfReadPositionSamples = this->CalculateMemorySizeIsInMilliSeconds(NumberOfReadPositionMilliSeconds) / wf.nBlockAlign;
	NumberOfReadSamples = this->CalculateMemorySizeIsInMilliSeconds(NumberOfReadMilliSeconds) / wf.nBlockAlign;

	return this->ReadPlayData(lpData , NumberOfReadPositionSamples , NumberOfReadSamples , lpNumberOfSamplesByRead);
}

//必要なメモリサイズを計算する
UINT32 CHSWAVEReader::CalculateMemorySize(UINT32 Samples) {
	WAVEFORMAT wf;
	if (this->GetFormat(&wf) == false) return 0;
	return Samples * wf.nBlockAlign;
}
UINT32 CHSWAVEReader::CalculateMemorySizeIsInSeconds(UINT32 Seconds) {
	WAVEFORMAT wf;
	if (this->GetFormat(&wf) == false) return 0;
	UINT32 Samples = wf.nSamplesPerSec * Seconds;
	return this->CalculateMemorySize(Samples);
}
UINT32 CHSWAVEReader::CalculateMemorySizeIsInMilliSeconds(UINT32 MilliSeconds) {
	WAVEFORMAT wf;
	if (this->GetFormat(&wf) == false) return 0;
	double ms = ((double)MilliSeconds) / 1000.0;
	UINT32 Samples = (UINT32)((double)wf.nSamplesPerSec * ms);
	return this->CalculateMemorySize(Samples);
}


//時間からサンプル数を取得（秒単位とミリ秒単位の２種類用意)
UINT32 CHSWAVEReader::CalculateNumberOfSamplesIsInSeconds(UINT32 Seconds) {
	WAVEFORMAT wf;
	if (this->GetFormat(&wf) == false) return 0;
	return wf.nSamplesPerSec * Seconds;
}
UINT32 CHSWAVEReader::CalculateNumberOfSamplesIsInMilliSeconds(UINT32 MilliSeconds) {
	WAVEFORMAT wf;
	if (this->GetFormat(&wf) == false) return 0;
	double ms = ((double)MilliSeconds) / 1000.0;
	return (UINT32)((double)wf.nSamplesPerSec * ms);
}


//全再生データを取得
bool CHSWAVEReader::ReadAllPlayData(void *lpData , UINT32 *lpNumberOfSamplesByRead) {

	UINT32 ReadSucceedSamples;
	UINT32 NumberOfSamples = this->GetPlaySamples();
	if (this->ReadPlayData(lpData, 0, NumberOfSamples, &ReadSucceedSamples)) {

		if (lpNumberOfSamplesByRead) {
			*lpNumberOfSamplesByRead = ReadSucceedSamples;
			return true;
		} else if (NumberOfSamples != ReadSucceedSamples){
			 return false;
		}
		return true;
	}
	return false;
}
