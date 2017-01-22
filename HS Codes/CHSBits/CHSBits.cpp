#include "CHSBits.h"
CHSBits::CHSBits(void)
{
	pData = nullptr;
	m_NumberOfBytes = 0;
	m_NumberOfRestBits = 0;
}


CHSBits::~CHSBits(void)
{
	this->Uninitialize();
}


bool CHSBits::Initialize(HSUInt32 NumberOfBits){
	if(this->pData) return false;

	this->m_NumberOfBytes = NumberOfBits / 8;
	this->m_NumberOfRestBits = NumberOfBits % 8;

	HSUInt32  bytes = m_NumberOfBytes;
	if(m_NumberOfRestBits) bytes++;

	this->pData = new HSUInt8[bytes];
	memset(this->pData , 0 , bytes);
	return true;
}

bool CHSBits::Uninitialize(void){
	if(this->pData){
		delete []this->pData;
		pData = nullptr;
		m_NumberOfBytes = 0;
		m_NumberOfRestBits = 0;
		return true;
	}
	return false;
}

HSUInt32 CHSBits::GetAllocatedBitCount(void){
	return m_NumberOfBytes * 8 + m_NumberOfRestBits;
}

HSUInt32 CHSBits::GetAllocatedByteCount(void){
	if(m_NumberOfRestBits) return  m_NumberOfBytes + 1;
	return m_NumberOfBytes;
}


HSBIT_STATE CHSBits::SetBit(HSUInt32 IndexOfBits , HSBIT_STATE State){
	if(this->pData == nullptr) return HSBIT_STATE_OVERACCESS;
	if((m_NumberOfBytes * 8 + m_NumberOfRestBits) <= IndexOfBits) return HSBIT_STATE_OVERACCESS;
	if((State == HSBIT_STATE_INVALID)||(State == HSBIT_STATE_OVERACCESS)) return HSBIT_STATE_INVALID;
	HSUInt32 ByteIdx , BitIdx;
	ByteIdx = IndexOfBits / 8;
	BitIdx = IndexOfBits % 8;
	HSUInt8 *pCurrent = this->pData + ByteIdx;
	*pCurrent &= ~(1 << BitIdx);
	if(State == HSBIT_STATE_ON) *pCurrent |= (1 << BitIdx);
	return State;
}

HSBIT_STATE CHSBits::GetBit(HSUInt32 IndexOfBits){
	if(this->pData == nullptr) return HSBIT_STATE_OVERACCESS;
	if((m_NumberOfBytes * 8 + m_NumberOfRestBits) <= IndexOfBits) return HSBIT_STATE_OVERACCESS;
	HSUInt32 ByteIdx , BitIdx;
	ByteIdx = IndexOfBits / 8;
	BitIdx = IndexOfBits % 8;
	HSUInt8 *pCurrent = this->pData + ByteIdx;
	if((*pCurrent) & (1 << BitIdx)) return HSBIT_STATE_ON;
	return HSBIT_STATE_OFF;
}

HSUInt32 CHSBits::CountBits(HSBIT_STATE State){
	if(this->pData == nullptr) return 0;

	if((State == HSBIT_STATE_INVALID)||(State == HSBIT_STATE_OVERACCESS)) return 0;

	HSUInt32 CountOfOnBit = 0;

	HSUInt32 AllBits = m_NumberOfBytes * 8 + m_NumberOfRestBits;
	HSUInt32 ByteIdx , BitIdx;

	for(HSUInt32 IndexOfBits = 0 ; IndexOfBits < AllBits ; IndexOfBits++){
		ByteIdx = IndexOfBits / 8;
		BitIdx = IndexOfBits % 8;
		if(*(pData + ByteIdx) & (1 << BitIdx))CountOfOnBit++;
	}


	if(State == HSBIT_STATE_ON) return CountOfOnBit;

	return AllBits - CountOfOnBit;

}

bool CHSBits::FillBits(HSBIT_STATE State){
	if(this->pData == nullptr) return 0;

	if((State == HSBIT_STATE_INVALID)||(State == HSBIT_STATE_OVERACCESS)) return false;

	HSUInt8 ByteData = 0;
	if(State == HSBIT_STATE_ON) ByteData = 255;

	HSUInt32 NumberOfLoops = m_NumberOfBytes;
	if(this->m_NumberOfRestBits) NumberOfLoops++;

	for(HSUInt32 i = 0 ; i < NumberOfLoops ; i++) *(pData + i) = ByteData;
	
	return true;
}


void CHSBits::InvertBits(void){
	if(this->pData == nullptr) return;
	HSUInt32 NumberOfLoops = m_NumberOfBytes;
	if(this->m_NumberOfRestBits) NumberOfLoops++;
	for(HSUInt32 i = 0 ; i < NumberOfLoops ; i++) *(pData + i) =  ~(*(pData + i));
}



bool CHSBits::SetBitByBoolean(HSUInt32 IndexOfBits , bool bState){
	HSBIT_STATE s = (bState) ? HSBIT_STATE_ON : HSBIT_STATE_OFF;
	if(this->SetBit(IndexOfBits , s) == s) return  true;
	return false;
}


bool CHSBits::GetBitByBoolean(HSUInt32 IndexOfBits){
	HSBIT_STATE s;
	s = this->GetBit(IndexOfBits);
	if(s == HSBIT_STATE_ON) return true;
	return false;
}

int CHSBits::SetBitByInteger(HSUInt32 IndexOfBits , int iState){
	HSBIT_STATE s = (iState) ? HSBIT_STATE_ON : HSBIT_STATE_OFF;
	if(this->SetBit(IndexOfBits , s) == s) return  iState;
	return -1;
}
int CHSBits::GetBitByInteger(HSUInt32 IndexOfBits){
	HSBIT_STATE s;
	s = this->GetBit(IndexOfBits);
	if(s == HSBIT_STATE_ON) return 1;
	if(s == HSBIT_STATE_OFF) return 0;
	return -1;
}
