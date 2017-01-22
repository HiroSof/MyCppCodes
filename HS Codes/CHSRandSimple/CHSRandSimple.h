//xorshift‚ğg‚Á‚½—”¶¬ƒNƒ‰ƒX
//http://ja.wikipedia.org/wiki/Xorshift
//http://hexadrive.sblo.jp/article/63660775.html


#pragma once

namespace HSRS_BASIC_SEED{
	const unsigned int HSRS_BASIC_SEED_X = 123456789;
	const unsigned int HSRS_BASIC_SEED_Y = 362436069;
	const unsigned int HSRS_BASIC_SEED_Z = 521288629;
	const unsigned int HSRS_BASIC_SEED_W = 88675123;
}

class CHSRandSimple
{
private:
	unsigned int seed_x;
	unsigned int seed_y;
	unsigned int seed_z;
	unsigned int seed_w;
public:
	CHSRandSimple(void);
	~CHSRandSimple(void);

	void SetSeed(unsigned int seed);

	unsigned int Next(void);

};

