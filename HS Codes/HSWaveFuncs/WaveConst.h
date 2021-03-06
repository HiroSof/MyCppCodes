#ifndef _HS_WaveConst_
#define _HS_WaveConst_

	//マクロ登録
	#ifndef WAVE_FORMAT_PCM
		//これはWindows SDKのMMSystem.hに定義されている。
		//又、Windows.hをインクルードすればMMSystem.hは内部でインクルードされる。
		#define WAVE_FORMAT_PCM			0x0001 //Pulse Code Modulation format
	#endif

	#ifndef WAVE_FORMAT_IEEE_FLOAT
		/*
			32/64 bit float format(値の範囲：-1.0～1.0)
		*/
		#define WAVE_FORMAT_IEEE_FLOAT	0x0003 		//Pulse Code Modulation float-point format
	#endif
	
	//最初のサブチャンクへのオフセット
	#define RIFF_FIRST_SUBCHUNKPOS 12

	//8bit
	#define	WAVE8_MIN 0
	#define WAVE8_MAX 255

	//8bit(作業上符号付きにした場合)
	#define	WAVE8S_MIN -128
	#define WAVE8S_MAX 127

	//16bit
	#define	WAVE16_MIN -32768
	#define WAVE16_MAX 32767

	//32bit
	#define	WAVE32_MIN (-2147483647-1)
	#define WAVE32_MAX 2147483647

	//32bit(float-data)
	#define	WAVE32F_MIN -1.0f
	#define WAVE32F_MAX 1.0f

	//64bit(float-data)
	//#define	WAVE64F_MIN -1.0
	//#define	WAVE64F_MAX 1.0

	//24bit
	#define	WAVE24_MIN -8388608
	#define WAVE24_MAX 8388607

#endif