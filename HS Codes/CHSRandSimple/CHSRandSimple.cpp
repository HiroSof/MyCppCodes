#include "CHSRandSimple.h"

#include <stdio.h>

CHSRandSimple::CHSRandSimple(void)
{
	seed_x = HSRS_BASIC_SEED::HSRS_BASIC_SEED_X;
	seed_y = HSRS_BASIC_SEED::HSRS_BASIC_SEED_Y;
	seed_z = HSRS_BASIC_SEED::HSRS_BASIC_SEED_Z;
	seed_w = HSRS_BASIC_SEED::HSRS_BASIC_SEED_W;

}


CHSRandSimple::~CHSRandSimple(void)
{

}

void CHSRandSimple::SetSeed(unsigned int seed){
	if(seed == 0){
		seed_x = HSRS_BASIC_SEED::HSRS_BASIC_SEED_X;
		seed_y = HSRS_BASIC_SEED::HSRS_BASIC_SEED_Y;
		seed_z = HSRS_BASIC_SEED::HSRS_BASIC_SEED_Z;
		seed_w = HSRS_BASIC_SEED::HSRS_BASIC_SEED_W;
	}else{

		unsigned int b1,b2;
		seed_x = seed;
		seed_y = ((seed & 0xFFFF0000) >> 16) | ((seed & 0x0000FFFF) << 16);

		b1 = seed & 0x0000000F;
		b1 |= (seed & 0x00000F00) >> 4;
		b1 |= (seed & 0x000F0000) >> 8;
		b1 |= (seed & 0x0F000000) >> 12;

		b2 = (seed & 0x000000F0) >> 4;
		b2 |= (seed & 0x0000F000) >> 8;
		b2 |= (seed & 0x00F00000) >> 12;
		b2 |= (seed & 0xF0000000) >> 16;

		seed_z = b1 | (b2 << 16);
		seed_w = (b1 << 16) | b2;
	}
}


unsigned int CHSRandSimple::Next(void){

	unsigned int t = seed_x ^ (seed_x << 11);
	seed_x = seed_y; 
	seed_y = seed_z;
	seed_z = seed_w;

	seed_w = (seed_w ^ (seed_w>>19)) ^ (t^(t>>8));

	return seed_w;
}
