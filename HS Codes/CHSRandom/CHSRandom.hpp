//xorshiftを使った乱数生成クラス
//http://ja.wikipedia.org/wiki/Xorshift
//http://hexadrive.sblo.jp/article/63660775.html

#include <time.h>

#pragma once


//初期シード値
const unsigned int HSRANDOM_BASIC_SEED_X = 123456789;
const unsigned int HSRANDOM_BASIC_SEED_Y = 362436069;
const unsigned int HSRANDOM_BASIC_SEED_Z = 521288629;
const unsigned int HSRANDOM_BASIC_SEED_W = 88675123;



class CHSRandom {

private:
	unsigned int seed_x;
	unsigned int seed_y;
	unsigned int seed_z;
	unsigned int seed_w;

public:

	CHSRandom ();

	void SetSeed ( unsigned int seed);
	void SetSeedEx ( unsigned int seed );
	void SetSeedFromLocalTime ( void );
	void SetSeedFromLocalTimeEx ( void );


	unsigned int Next ( void );

	unsigned int RangeNext ( unsigned int minValue , unsigned int maxValue );

	int RangeNextSigned ( int minValue , int maxValue );

	template<typename T> bool Shuffle ( T *pValue , unsigned int NumberOfElements ) {

		if ( pValue == nullptr )return false;
		if ( NumberOfElements == 0 )return false;

		for ( unsigned int i = NumberOfElements - 1; i > 0; i-- ) {
			unsigned int j = this->RangeNext ( 0 , i );
			T tmp = *( pValue + i );
			*( pValue + i ) = *( pValue + j );
			*( pValue + j ) = tmp;
		}

		return true;
	}

	unsigned int operator()() {
		return Next ();
	}

	unsigned int operator()( unsigned int minValue , unsigned int maxValue ) {
		return RangeNext ( minValue , maxValue );
	}


	unsigned int operator%( unsigned int val ) {
		return Next () % val;
	}

};