#include <DxLib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define GAME_NAME "ブロック崩し(2月21日版)"
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
	SetWindowText ( TEXT ( "初期化中・・・" ) );



	if ( DxLib_Init ( ) == -1 ) {
		MessageBoxA ( NULL , "DXライブラリの初期化に失敗しました。" , "エラー" , MB_OK | MB_ICONERROR );
		return 0;
	}


	SetWindowText ( TEXT ( GAME_NAME ) );

	while ( ProcessMessage ( ) == 0 && CheckHitKey ( KEY_INPUT_ESCAPE ) == 0 ) {






	}


	DxLib_End ( );

	return 0;
}
