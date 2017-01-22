#ifndef __CHSSHA256_H__
#define __CHSSHA256_H__
#include "CHSSHABASE.h"

class CHSSHA256 : public CHSSHABASE
{

private:

	static const  unsigned __int32 ConstantData[64];

	static const  unsigned __int32 DefaultMessageDijest[8];
	unsigned __int32 MessageDijest[8];

	unsigned __int32 ShiftCalc(int, unsigned __int32);
	unsigned __int32 LogicalExp(int, unsigned __int32,unsigned __int32,unsigned __int32);

	unsigned __int32 Rotate(unsigned __int32,int);
	void HashBlock(void);

public:
	CHSSHA256();



};



#endif // !__CHSSHA256_H__
