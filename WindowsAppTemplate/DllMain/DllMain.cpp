/*----------------------------------------------------------------------------
���C�u������:
�o�[�W����:1.0.1.0
���C�u�����T�v�F
�J������FMicrosoft Visual C++ 2015
�����J�����FMicrosoft Visual Studio 2015 Community Edition
�J���J�n���F
�ŏI�X�V���F
-------------------------------------------------------------------------------
�X�V���ꗗ
--------------------------------------------------------------------------------
*/

#include <Windows.h>

static HINSTANCE hInstance;

BOOL WINAPI DllMain ( HINSTANCE hDLL , DWORD dwReason , LPVOID lpReserved ) {
	switch ( dwReason ) {
		case DLL_PROCESS_ATTACH:
			hInstance = hDLL;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}