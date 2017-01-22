#pragma once
#include <memory.h>
typedef unsigned __int8 HSUInt8;
typedef unsigned __int32 HSUInt32;

enum HSBIT_STATE{
	HSBIT_STATE_INVALID = -2,
	HSBIT_STATE_OVERACCESS,
	HSBIT_STATE_OFF,
	HSBIT_STATE_ON
};

class CHSBits
{
private:
	HSUInt8 *pData;
	HSUInt32 m_NumberOfBytes;
	HSUInt32 m_NumberOfRestBits;
public:
	CHSBits(void);
	~CHSBits(void);

	bool Initialize(HSUInt32 NumberOfBits);
	bool Uninitialize(void);

	HSUInt32 GetAllocatedBitCount(void);
	HSUInt32 GetAllocatedByteCount(void);


	HSBIT_STATE SetBit(HSUInt32 IndexOfBits , HSBIT_STATE State);
	HSBIT_STATE GetBit(HSUInt32 IndexOfBits);
	bool SetBitByBoolean(HSUInt32 IndexOfBits , bool bState);
	bool GetBitByBoolean(HSUInt32 IndexOfBits);
	int SetBitByInteger(HSUInt32 IndexOfBits , int iState);
	int GetBitByInteger(HSUInt32 IndexOfBits);

	HSUInt32 CountBits(HSBIT_STATE State);
	bool FillBits(HSBIT_STATE State);
	void InvertBits(void);


};

