/*----------------------------------------------------------------------------
���C�u������:
�o�[�W����:1.0.1.0
���C�u�����T�v�F
�J������FMicrosoft Visual C++ 2013
�����J�����FMicrosoft Visual Studio 2013 Ultimate Edition
�J���J�n���F
�ŏI�X�V���F
-------------------------------------------------------------------------------
�X�V���ꗗ
--------------------------------------------------------------------------------
*/

#include <Windows.h>

static HINSTANCE hInstance;

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved){
	hInstance = hDLL;
	return TRUE;
}