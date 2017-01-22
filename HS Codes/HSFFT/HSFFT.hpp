/*
HSFFT(hirosof Fast Fourier Transform)
C++ Header File

Copyright (C) 2015 hirosof(Hiroaki Software). All rights reserved.
*/

#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef __HSFFT__

#define __HSFFT__


//���f���\����
typedef struct tagHSFFTComplex {
	double Real;				//����
	double Imaginary;			//����
}HSFFTComplex , *PHSFFTComplex , *LPHSFFTComplex;

typedef struct tagHSFFTButterflyData {

	struct _input {
		HSFFTComplex A;			//����A
		HSFFTComplex B;			//����B
	}Input;

	struct _output {
		HSFFTComplex A;			//�o��A
		HSFFTComplex B;			//�o��B
	}Output;

}HSFFTButterflyData , *PHSFFTButterflyData , *LPHSFFTButterflyData;

//�����t�[���G�ϊ��̃��[�h
enum HSFFT_MODE {
	HSFFT_MODE_POSITIVE = 0,	//������(���Ԏ������g����)
	HSFFT_MODE_INVERSE			//�t����(���g���������Ԏ�)
};

//�r�b�g���o�[�X(�����Ȃ�32bit)
unsigned __int32 HSFFT_BitReverseU32 ( unsigned __int32 num , int bits );

//2���Ƃ����ΐ��֐�
double HSFFT_Log2 ( double real );

//�����𐮐��Ƃ��Ĉ����邩���m�F����֐�
bool HSFFT_IsInteger ( double real );



//���f���̉��Z
HSFFTComplex HSFFTComplex_Add ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 );

//���f���̌��Z
HSFFTComplex HSFFTComplex_Minus ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 );

//���f���̏�Z
HSFFTComplex HSFFTComplex_Multiple ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 );


//��]���q(twiddle factor) �֐�
HSFFTComplex HSFFT_TwiddleFactor ( int n , int k );

//�o�^�t���C�v�Z�֐�
bool HSFFT_Butterfly ( LPHSFFTButterflyData pData , int n , int k );


//FFT�EIFFT�֐�
bool HSFFT_Execute ( LPHSFFTComplex pInputArray , LPHSFFTComplex pOutputArray , unsigned int NumberOfSamples , HSFFT_MODE Mode = HSFFT_MODE_POSITIVE );

//�����pFFT�֐�
bool HSFFT_Positive_Execute ( double *pInputArray , LPHSFFTComplex pOutputArray , unsigned int NumberOfSamples );

//�����pIFFT�֐�
bool HSFFT_Inverse_Execute ( LPHSFFTComplex pInputArray , double *pOutputArray , unsigned int NumberOfSamples );




#endif