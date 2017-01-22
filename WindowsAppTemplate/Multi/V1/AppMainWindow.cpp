#include "AppWindowFunc.h"

BOOL InitMainWindowClass ( HINSTANCE hInstance ) {
	ATOM atMain = RegisterAppWindowClass ( hInstance , MAINWINDOW_CLASSNAME , MainWndProc , MAINWINDOW_BACKGROUNDCOLOR );
	if ( atMain == NULL )return FALSE;
	return TRUE;
}

BOOL InitMainWindow ( HINSTANCE hInstance , int ShowState ) {

	int WindowStyle;

	//ウィンドウスタイル
	WindowStyle = WS_OVERLAPPEDWINDOW;

	//ウィンドウサイズ固定ならばオーバーラップウィンドウから
	//サイズ変更境界と最大化ボタンを無効化にする
	if ( MAINWINDOW_SIZE_VARIABLE == 0 )
		WindowStyle &= ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

	HWND hwnd = CreateAppWindow ( hInstance , MAINWINDOW_CLASSNAME , MAINWINDOW_DEFAULTTITLE ,
		WindowStyle , CW_USEDEFAULT , CW_USEDEFAULT , MAINWINDOW_CLIENT_WIDTH , MAINWINDOW_CLIENT_HEIGHT , ShowState );
	if ( hwnd == NULL )return FALSE;
	return TRUE;
}

/*
メインウィンドウプロシージャー
*/
LRESULT CALLBACK MainWndProc ( HWND hwnd , UINT msg , WPARAM wp , LPARAM lp ) {
	//クライアントサイズ保管場所
	RECT rcClientSize;

	//クライアントサイズ取得
	GetClientRect ( hwnd , &rcClientSize );

	//メッセージごとの処理
	switch ( msg ) {
		case WM_DESTROY:
			PostQuitMessage ( 0 );
			break;
		default:
			return DefWindowProc ( hwnd , msg , wp , lp );
	}
	return 0;
}