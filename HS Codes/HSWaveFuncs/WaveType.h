#include "WaveHeader.h"
#ifndef _HS_WaveType_
#define _HS_WaveType_

	//WAVEデータ型(bit数不明 or 不定用)
	typedef __int8	WAVEDN;

	//WAVEデータ型(8bit用)
	typedef unsigned __int8 WAVE8;
	typedef __int8	WAVE8S;
	
	//WAVEデータ型(16bit用)
	typedef	__int16 WAVE16;
	
	//WAVEデータ型(32bit用)
	typedef	__int32 WAVE32;

	//WAVEデータ型(32bit,float-data用)
	typedef	float WAVE32F;
	
	//WAVEデータ型(64bit,float-data用)
	//typedef	double WAVE64F;

	typedef  double WAVENORMDATA;


#ifdef __cplusplus
	//WAVEデータ型(24bit用)
	typedef struct{
		__int8 Num[3];
		void operator = (int nValue){
			Num[0] = nValue & 0xFF;
			Num[1] = (nValue & 0xFF00) >> 8;
			Num[2] = (nValue & 0xFF0000) >> 16;
		}

		operator int(){
			return Num[0]+ (Num[1] << 8) + (Num[2] << 16); 
		}
	}WAVE24;
#endif

#endif