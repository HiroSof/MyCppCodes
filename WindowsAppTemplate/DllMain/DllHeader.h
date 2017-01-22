#pragma once

/*--------------------------------------------------------------/
EXPORTÉ}ÉNÉçìoò^ïî
/--------------------------------------------------------------*/
#ifdef __DLL_EXPORT
	#define HSDLL_EXPORT extern "C" __declspec (dllexport)
#else
	#define HSDLL_EXPORT extern "C" __declspec (dllimport)
#endif



