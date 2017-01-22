#include "CHSGameRandom.hpp"


CHSGameRandom::CHSGameRandom () {
	this->seed_x = HSRANDOM_BASIC_SEED_X;
	this->seed_y = HSRANDOM_BASIC_SEED_Y;
	this->seed_z = HSRANDOM_BASIC_SEED_Z;
	this->seed_w = HSRANDOM_BASIC_SEED_W;
}


void CHSGameRandom::SetSeed ( unsigned int seed ) {
	if ( seed == 0 ) {
		this->seed_x = HSRANDOM_BASIC_SEED_X;
		this->seed_y = HSRANDOM_BASIC_SEED_Y;
		this->seed_z = HSRANDOM_BASIC_SEED_Z;
		this->seed_w = HSRANDOM_BASIC_SEED_W;
	} else {

		unsigned int b1 , b2;
		seed_x = seed;
		seed_y = ( ( seed & 0xFFFF0000 ) >> 16 ) | ( ( seed & 0x0000FFFF ) << 16 );

		b1 = seed & 0x0000000F;
		b1 |= ( seed & 0x00000F00 ) >> 4;
		b1 |= ( seed & 0x000F0000 ) >> 8;
		b1 |= ( seed & 0x0F000000 ) >> 12;

		b2 = ( seed & 0x000000F0 ) >> 4;
		b2 |= ( seed & 0x0000F000 ) >> 8;
		b2 |= ( seed & 0x00F00000 ) >> 12;
		b2 |= ( seed & 0xF0000000 ) >> 16;

		seed_z = b1 | ( b2 << 16 );
		seed_w = ( b1 << 16 ) | b2;
	}
}


void CHSGameRandom::SetSeedEx ( unsigned int seed ) {
	unsigned int seed2 = reinterpret_cast<unsigned int>( this );
	seed_x = seed;
	seed_y = seed2;
	seed_z = ( ( seed & 0xFFFF0000 ) >> 16 ) | ( ( seed & 0x0000FFFF ) << 16 );
	seed_w = ( ( seed2 & 0xFFFF0000 ) >> 16 ) | ( ( seed2 & 0x0000FFFF ) << 16 );
}


void CHSGameRandom::SetSeedFromLocalTime ( void ) {

	this->SetSeed ( (unsigned) time ( NULL ) );
}
void CHSGameRandom::SetSeedFromLocalTimeEx ( void ) {

	this->SetSeedEx ( (unsigned) time ( NULL ) );
}

unsigned int CHSGameRandom::Next ( void ) {
	unsigned int t = seed_x ^ ( seed_x << 11 );
	seed_x = seed_y;
	seed_y = seed_z;
	seed_z = seed_w;
	seed_w = ( seed_w ^ ( seed_w >> 19 ) ) ^ ( t ^ ( t >> 8 ) );
	return seed_w;
}



unsigned int CHSGameRandom::RangeNext ( unsigned int minValue , unsigned int maxValue ) {
	unsigned int range = maxValue - minValue + 1;
	return ( Next () % range ) + minValue;
}

int CHSGameRandom::RangeNextSigned ( int minValue , int maxValue ) {
	int range = maxValue - minValue + 1;
	return ( Next () % range ) + minValue;
}
