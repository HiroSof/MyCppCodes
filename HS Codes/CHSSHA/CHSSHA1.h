
#ifndef __CHSSHA1_H__
#define __CHSSHA1_H__

#include "CHSSHABASE.h"


class CHSSHA1 : public CHSSHABASE
{
private:
	static const unsigned __int32 DefaultMessageDijest[5];
	unsigned __int32 MessageDijest[5];


	unsigned __int32 Rotate(unsigned __int32,int);
	unsigned __int32 GetConstData(int);
	unsigned __int32 LogicalExp(int , unsigned __int32,unsigned __int32,unsigned __int32);
	void HashBlock(void);
public:
	CHSSHA1();
};

#endif // !__CHSSHA1_H__
