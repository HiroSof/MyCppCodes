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


//複素数構造体
typedef struct tagHSFFTComplex {
	double Real;				//実部
	double Imaginary;			//虚部
}HSFFTComplex , *PHSFFTComplex , *LPHSFFTComplex;

typedef struct tagHSFFTButterflyData {

	struct _input {
		HSFFTComplex A;			//入力A
		HSFFTComplex B;			//入力B
	}Input;

	struct _output {
		HSFFTComplex A;			//出力A
		HSFFTComplex B;			//出力B
	}Output;

}HSFFTButterflyData , *PHSFFTButterflyData , *LPHSFFTButterflyData;

//高速フーリエ変換のモード
enum HSFFT_MODE {
	HSFFT_MODE_POSITIVE = 0,	//正方向(時間軸→周波数軸)
	HSFFT_MODE_INVERSE			//逆方向(周波数軸→時間軸)
};

//ビットリバース(符号なし32bit)
unsigned __int32 HSFFT_BitReverseU32 ( unsigned __int32 num , int bits );

//2を底とした対数関数
double HSFFT_Log2 ( double real );

//実数を整数として扱えるかを確認する関数
bool HSFFT_IsInteger ( double real );



//複素数の加算
HSFFTComplex HSFFTComplex_Add ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 );

//複素数の減算
HSFFTComplex HSFFTComplex_Minus ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 );

//複素数の乗算
HSFFTComplex HSFFTComplex_Multiple ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 );


//回転因子(twiddle factor) 関数
HSFFTComplex HSFFT_TwiddleFactor ( int n , int k );

//バタフライ計算関数
bool HSFFT_Butterfly ( LPHSFFTButterflyData pData , int n , int k );


//FFT・IFFT関数
bool HSFFT_Execute ( LPHSFFTComplex pInputArray , LPHSFFTComplex pOutputArray , unsigned int NumberOfSamples , HSFFT_MODE Mode = HSFFT_MODE_POSITIVE );

//実数用FFT関数
bool HSFFT_Positive_Execute ( double *pInputArray , LPHSFFTComplex pOutputArray , unsigned int NumberOfSamples );

//実数用IFFT関数
bool HSFFT_Inverse_Execute ( LPHSFFTComplex pInputArray , double *pOutputArray , unsigned int NumberOfSamples );




#endif