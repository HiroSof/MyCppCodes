/*
	HSFFT(hirosof Fast Fourier Transform)
	C++ Source File

	Copyright (C) 2015 hirosof(Hiroaki Software). All rights reserved.
*/

#include "HSFFT.hpp"


//ビットリバース(符号なし32bit)
unsigned __int32 HSFFT_BitReverseU32 ( unsigned __int32 num , int bits ) {
	unsigned __int32 rev_num;
	int cnt;
	if ( bits > 32 )return 0;
	else if ( bits == 0 ) bits = 32;
	for ( rev_num = cnt = 0; cnt < bits; cnt++ )
		if ( ( num & ( 1 << cnt ) ) )
			rev_num += ( 1 << ( bits - 1 - cnt ) );
	return rev_num;
}

//2を底とした対数関数
double HSFFT_Log2 ( double real ) {
	return log ( real ) / log ( 2.0 );
}

//実数が整数として扱えるかを確認する関数
bool HSFFT_IsInteger ( double real ) {
	double integer;
	if ( modf ( real , &integer ) ) return false;
	return true;
}

//複素数の加算
HSFFTComplex HSFFTComplex_Add ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 ) {
	HSFFTComplex c;
	c.Real = cmp1.Real + cmp2.Real;
	c.Imaginary = cmp1.Imaginary + cmp2.Imaginary;
	return c;
}
//複素数の減算
HSFFTComplex HSFFTComplex_Minus ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 ) {
	HSFFTComplex c;
	c.Real = cmp1.Real - cmp2.Real;
	c.Imaginary = cmp1.Imaginary - cmp2.Imaginary;
	return c;
}
//複素数の乗算
HSFFTComplex HSFFTComplex_Multiple ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 ) {
	HSFFTComplex c;
	c.Real = cmp1.Real * cmp2.Real - cmp1.Imaginary * cmp2.Imaginary;
	c.Imaginary = cmp1.Real * cmp2.Imaginary + cmp1.Imaginary * cmp2.Real;
	return c;
}


//回転因子(twiddle factor) 関数
HSFFTComplex HSFFT_TwiddleFactor ( int n , int k ) {
	HSFFTComplex cmp;
	double index;

	//指数関数に渡す指数値の虚部
	index = ( 2.0 * M_PI * k ) / n;

	//オイラーの公式
	cmp.Real = cos ( index );
	cmp.Imaginary = -1.0 * sin ( index );

	return cmp;
}

//バタフライ計算関数
bool HSFFT_Butterfly ( LPHSFFTButterflyData pData , int n , int k ) {
	if ( pData == nullptr )return false;
	HSFFTComplex tf = HSFFT_TwiddleFactor ( n , k );
	pData->Output.A = HSFFTComplex_Add ( pData->Input.A , pData->Input.B );
	pData->Output.B = HSFFTComplex_Minus ( pData->Input.A , pData->Input.B );
	pData->Output.B = HSFFTComplex_Multiple ( pData->Output.B , tf );
	return true;
}


//FFT・IFFT関数
bool HSFFT_Execute ( LPHSFFTComplex pInputArray , LPHSFFTComplex pOutputArray , unsigned int NumberOfSamples , HSFFT_MODE Mode ) {


	if ( pInputArray == nullptr )return false;
	if ( pOutputArray == nullptr )return false;

	double d_stage = HSFFT_Log2 ( NumberOfSamples );
	if ( HSFFT_IsInteger ( d_stage ) == false ) return false;

	unsigned int NumberOfStage = (unsigned int) d_stage;
	if ( NumberOfStage == 0 )return false;

	int Direction;
	switch ( Mode ) {
		case HSFFT_MODE_POSITIVE:
			Direction = 1;
			break;
		case HSFFT_MODE_INVERSE:
			Direction = -1;
			break;
		default:
			return false;
	}

	//バタフライ演算をする際のペアへのオフセットの初期値
	unsigned int pair_offset = NumberOfSamples;

	//1ブロックあたりのペア数
	unsigned int NumberOfPair = NumberOfSamples;

	//回転因子の指数の差分格納変数
	unsigned int diff_tw_index;

	//1ステージあたりの処理ループ数
	unsigned int NumberOfUnitStage;

	//ループカウンター
	unsigned int stage_cnt , unit_cnt , item_cnt;

	unsigned int index,index_pair;

	HSFFTButterflyData butterflayData;

	//FFTステージループ
	for ( stage_cnt = 0 ; stage_cnt < NumberOfStage; stage_cnt++ ) {

		//バタフライ演算をする際のペアへのオフセット
		pair_offset >>= 1;

		//1ブロックあたりのペア数
		NumberOfPair >>= 1;

		//回転因子の指数の差分
		diff_tw_index = 1 << stage_cnt;

		//1ステージあたりの処理ループ数
		NumberOfUnitStage = 1 << stage_cnt;

		//printf ( "<<Stage %u>>\n" , stage_cnt+1 );
		//1ステージあたりの処理ループ
		for ( unit_cnt = 0; unit_cnt < NumberOfUnitStage; unit_cnt++ ) {

			//1ペア当たりの処理
			for ( item_cnt = 0; item_cnt < NumberOfPair; item_cnt++ ) {


				//インデックス１
				index = item_cnt + ( NumberOfPair * 2 ) * unit_cnt;

				//インデックス２
				index_pair = index + pair_offset;

				//printf ( "%u %u\n" , index ,index_pair);

				//バタフライ演算関数の入力
				if ( stage_cnt == 0 ) {
					butterflayData.Input.A = *( pInputArray + index );
					butterflayData.Input.B = *( pInputArray + index_pair );
				} else {
					butterflayData.Input.A = *( pOutputArray + index );
					butterflayData.Input.B = *( pOutputArray + index_pair );
				}

				
				//バタフライ演算
				HSFFT_Butterfly ( &butterflayData , NumberOfSamples , Direction * diff_tw_index * item_cnt );

				//バタフライ演算の出力
				*( pOutputArray + index ) = butterflayData.Output.A;
				*( pOutputArray + index_pair ) = butterflayData.Output.B;
			}
		}
	}


	if ( Mode == HSFFT_MODE_INVERSE ) {
		for ( index = 0;index < NumberOfSamples; index++ ) {
			( pOutputArray + index )->Real /= NumberOfSamples;
			( pOutputArray + index )->Imaginary /= NumberOfSamples;
		}
	}


	HSFFTComplex cmp_buf;
	for (index  = 0; index < NumberOfSamples ; index++){

		//交換先のインデックス
		index_pair = HSFFT_BitReverseU32 ( index , NumberOfStage );

		//必要なデータを交換する

		if ( index < index_pair ) {
			cmp_buf = *( pOutputArray + index );
			*( pOutputArray + index ) = *( pOutputArray + index_pair );
			*( pOutputArray + index_pair ) = cmp_buf;
		}

	}
	return true;
}


//実数用FFT関数
bool HSFFT_Positive_Execute ( double *pInputArray , LPHSFFTComplex pOutputArray , unsigned int NumberOfSamples ) {
	if ( pInputArray == nullptr )return false;
	if ( pOutputArray == nullptr )return false;

	if ( NumberOfSamples == 0 )return false;


	LPHSFFTComplex pcmp_in = (LPHSFFTComplex) malloc ( sizeof ( HSFFTComplex ) * NumberOfSamples );
	if ( pcmp_in == nullptr )return false;



	for ( unsigned int i = 0; i < NumberOfSamples; i++ ) {
		( pcmp_in + i )->Real = *( pInputArray + i );
		( pcmp_in + i )->Imaginary = 0;
	}


	bool bRet = HSFFT_Execute ( pcmp_in , pOutputArray , NumberOfSamples );

	free ( pcmp_in );

	return bRet;
}

//実数用IFFT関数
bool HSFFT_Inverse_Execute ( LPHSFFTComplex pInputArray , double *pOutputArray , unsigned int NumberOfSamples ) {
	if ( pInputArray == nullptr )return false;
	if ( pOutputArray == nullptr )return false;

	if ( NumberOfSamples == 0 )return false;


	LPHSFFTComplex pcmp_out = (LPHSFFTComplex) malloc ( sizeof ( HSFFTComplex ) * NumberOfSamples );
	if ( pcmp_out == nullptr )return false;


	bool bRet = HSFFT_Execute ( pInputArray , pcmp_out , NumberOfSamples , HSFFT_MODE_INVERSE );

	if ( bRet ) {
		for ( unsigned int i = 0; i < NumberOfSamples; i++ ) {
			*( pOutputArray + i ) = ( pcmp_out + i )->Real;
		}
	}

	free ( pcmp_out );

	return bRet;
}
