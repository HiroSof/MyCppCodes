#include <DxLib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define GAME_NAME "�u���b�N����(2��21����)"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800

#pragma comment(linker,"\"/manifestdependency:type='win32' "	\
	"name='Microsoft.Windows.Common-Controls' "					\
	"version='6.0.0.0' "										\
	"processorArchitecture='*' "								\
	"publicKeyToken='6595b64144ccf1df' "						\
	"language='*'\"")


int WINAPI WinMain ( HINSTANCE hCurInst , HINSTANCE hPrevInst , LPSTR lpsCmdLine , int nCmdShow ) {


#ifndef _DEBUG
	SetOutApplicationLogValidFlag ( FALSE );
#endif

	ChangeWindowMode ( TRUE );
	SetGraphMode ( WINDOW_WIDTH , WINDOW_HEIGHT , 32 );
	SetWindowText ( TEXT ( "���������E�E�E" ) );



	if ( DxLib_Init ( ) == -1 ) {
		MessageBoxA ( NULL , "DX���C�u�����̏������Ɏ��s���܂����B" , "�G���[" , MB_OK | MB_ICONERROR );
		return 0;
	}


	SetWindowText ( TEXT ( GAME_NAME ) );

	while ( ProcessMessage ( ) == 0 && CheckHitKey ( KEY_INPUT_ESCAPE ) == 0 ) {






	}


	DxLib_End ( );

	return 0;
}
