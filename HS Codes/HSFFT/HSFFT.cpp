/*
	HSFFT(hirosof Fast Fourier Transform)
	C++ Source File

	Copyright (C) 2015 hirosof(Hiroaki Software). All rights reserved.
*/

#include "HSFFT.hpp"


//�r�b�g���o�[�X(�����Ȃ�32bit)
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

//2���Ƃ����ΐ��֐�
double HSFFT_Log2 ( double real ) {
	return log ( real ) / log ( 2.0 );
}

//�����������Ƃ��Ĉ����邩���m�F����֐�
bool HSFFT_IsInteger ( double real ) {
	double integer;
	if ( modf ( real , &integer ) ) return false;
	return true;
}

//���f���̉��Z
HSFFTComplex HSFFTComplex_Add ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 ) {
	HSFFTComplex c;
	c.Real = cmp1.Real + cmp2.Real;
	c.Imaginary = cmp1.Imaginary + cmp2.Imaginary;
	return c;
}
//���f���̌��Z
HSFFTComplex HSFFTComplex_Minus ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 ) {
	HSFFTComplex c;
	c.Real = cmp1.Real - cmp2.Real;
	c.Imaginary = cmp1.Imaginary - cmp2.Imaginary;
	return c;
}
//���f���̏�Z
HSFFTComplex HSFFTComplex_Multiple ( HSFFTComplex  cmp1 , HSFFTComplex  cmp2 ) {
	HSFFTComplex c;
	c.Real = cmp1.Real * cmp2.Real - cmp1.Imaginary * cmp2.Imaginary;
	c.Imaginary = cmp1.Real * cmp2.Imaginary + cmp1.Imaginary * cmp2.Real;
	return c;
}


//��]���q(twiddle factor) �֐�
HSFFTComplex HSFFT_TwiddleFactor ( int n , int k ) {
	HSFFTComplex cmp;
	double index;

	//�w���֐��ɓn���w���l�̋���
	index = ( 2.0 * M_PI * k ) / n;

	//�I�C���[�̌���
	cmp.Real = cos ( index );
	cmp.Imaginary = -1.0 * sin ( index );

	return cmp;
}

//�o�^�t���C�v�Z�֐�
bool HSFFT_Butterfly ( LPHSFFTButterflyData pData , int n , int k ) {
	if ( pData == nullptr )return false;
	HSFFTComplex tf = HSFFT_TwiddleFactor ( n , k );
	pData->Output.A = HSFFTComplex_Add ( pData->Input.A , pData->Input.B );
	pData->Output.B = HSFFTComplex_Minus ( pData->Input.A , pData->Input.B );
	pData->Output.B = HSFFTComplex_Multiple ( pData->Output.B , tf );
	return true;
}


//FFT�EIFFT�֐�
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

	//�o�^�t���C���Z������ۂ̃y�A�ւ̃I�t�Z�b�g�̏����l
	unsigned int pair_offset = NumberOfSamples;

	//1�u���b�N������̃y�A��
	unsigned int NumberOfPair = NumberOfSamples;

	//��]���q�̎w���̍����i�[�ϐ�
	unsigned int diff_tw_index;

	//1�X�e�[�W������̏������[�v��
	unsigned int NumberOfUnitStage;

	//���[�v�J�E���^�[
	unsigned int stage_cnt , unit_cnt , item_cnt;

	unsigned int index,index_pair;

	HSFFTButterflyData butterflayData;

	//FFT�X�e�[�W���[�v
	for ( stage_cnt = 0 ; stage_cnt < NumberOfStage; stage_cnt++ ) {

		//�o�^�t���C���Z������ۂ̃y�A�ւ̃I�t�Z�b�g
		pair_offset >>= 1;

		//1�u���b�N������̃y�A��
		NumberOfPair >>= 1;

		//��]���q�̎w���̍���
		diff_tw_index = 1 << stage_cnt;

		//1�X�e�[�W������̏������[�v��
		NumberOfUnitStage = 1 << stage_cnt;

		//printf ( "<<Stage %u>>\n" , stage_cnt+1 );
		//1�X�e�[�W������̏������[�v
		for ( unit_cnt = 0; unit_cnt < NumberOfUnitStage; unit_cnt++ ) {

			//1�y�A������̏���
			for ( item_cnt = 0; item_cnt < NumberOfPair; item_cnt++ ) {


				//�C���f�b�N�X�P
				index = item_cnt + ( NumberOfPair * 2 ) * unit_cnt;

				//�C���f�b�N�X�Q
				index_pair = index + pair_offset;

				//printf ( "%u %u\n" , index ,index_pair);

				//�o�^�t���C���Z�֐��̓���
				if ( stage_cnt == 0 ) {
					butterflayData.Input.A = *( pInputArray + index );
					butterflayData.Input.B = *( pInputArray + index_pair );
				} else {
					butterflayData.Input.A = *( pOutputArray + index );
					butterflayData.Input.B = *( pOutputArray + index_pair );
				}

				
				//�o�^�t���C���Z
				HSFFT_Butterfly ( &butterflayData , NumberOfSamples , Direction * diff_tw_index * item_cnt );

				//�o�^�t���C���Z�̏o��
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

		//������̃C���f�b�N�X
		index_pair = HSFFT_BitReverseU32 ( index , NumberOfStage );

		//�K�v�ȃf�[�^����������

		if ( index < index_pair ) {
			cmp_buf = *( pOutputArray + index );
			*( pOutputArray + index ) = *( pOutputArray + index_pair );
			*( pOutputArray + index_pair ) = cmp_buf;
		}

	}
	return true;
}


//�����pFFT�֐�
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

//�����pIFFT�֐�
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
