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

	//�t�H�[�}�b�g�擾
	bool GetFormat(PCMWAVEFORMAT *lpFormat);
	bool GetFormat(WAVEFORMAT *lpFormat);
	bool GetFormat(WAVEFORMATEX *lpFormat);
	UINT32 GetFormat(void *lpFormat , UINT32 Size);
	UINT32 GetFormatSize(void);
	
	//�Đ��f�[�^�̃T�C�Y���o�C�g�P�ʂŎ擾
	UINT32 GetPlayDataSize(void);

	//�Đ��T���v�������擾
	UINT32 GetPlaySamples(void);

	//�Đ����Ԃ��擾
	UINT32 GetPlayTimeSeconds(void);
	UINT32 GetPlayTimeMilliSeconds(void);

	//�S�Đ��f�[�^���擾
	bool ReadAllPlayData(void *lpData , UINT32 *lpNumberOfSamplesByRead = nullptr);

	//�Đ��f�[�^���擾
	bool ReadPlayData(void *lpData , UINT32 NumberOfReadPositionSamples , UINT32 NumberOfReadSamples , UINT32 *lpNumberOfSamplesByRead);
	bool ReadPlayDataIsInSeconds(void *lpData , UINT32 NumberOfReadPositionSeconds , UINT32 NumberOfReadSeconds , UINT32 *lpNumberOfSamplesByRead);
	bool ReadPlayDataIsInMilliSeconds(void *lpData , UINT32 NumberOfReadPositionMilliSeconds , UINT32 NumberOfReadMilliSeconds , UINT32 *lpNumberOfSamplesByRead);

	//�K�v�ȃ������T�C�Y���v�Z����
	UINT32 CalculateMemorySize(UINT32 Samples);
	UINT32 CalculateMemorySizeIsInSeconds(UINT32 Seconds);
	UINT32 CalculateMemorySizeIsInMilliSeconds(UINT32 MilliSeconds);

	//���Ԃ���T���v�������擾�i�b�P�ʂƃ~���b�P�ʂ̂Q��ޗp��)
	UINT32 CalculateNumberOfSamplesIsInSeconds(UINT32 Seconds);
	UINT32 CalculateNumberOfSamplesIsInMilliSeconds(UINT32 MilliSeconds);

	//�Ȗ��擾
	UINT32 GetSongName(char *lpName , UINT32 size);

	//�Ȗ��̃T�C�Y���擾
	UINT32 GetSongNameLength(void);

	//�A�[�e�B�X�g��
	UINT32 GetSongArtistName(char *lpArtistName , UINT32 size);
	
	//�A�[�e�B�X�g���̃T�C�Y���擾
	UINT32 GetSongArtistNameLength(void);

	//�v���_�N�g��
	UINT32 GetSongProductName(char *lpProductName , UINT32 size);

	//�v���_�N�g���̃T�C�Y���擾
	UINT32 GetSongProductNameLength(void);

	//�W������
	UINT32 GetSongGenreText(char *lpGenreText , UINT32 size);

	//�W�������̃T�C�Y���擾
	UINT32 GetSongGenreTextLength(void);

};


