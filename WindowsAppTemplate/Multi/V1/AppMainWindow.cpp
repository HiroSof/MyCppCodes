#include "AppWindowFunc.h"

BOOL InitMainWindowClass ( HINSTANCE hInstance ) {
	ATOM atMain = RegisterAppWindowClass ( hInstance , MAINWINDOW_CLASSNAME , MainWndProc , MAINWINDOW_BACKGROUNDCOLOR );
	if ( atMain == NULL )return FALSE;
	return TRUE;
}

BOOL InitMainWindow ( HINSTANCE hInstance , int ShowState ) {

	int WindowStyle;

	//�E�B���h�E�X�^�C��
	WindowStyle = WS_OVERLAPPEDWINDOW;

	//�E�B���h�E�T�C�Y�Œ�Ȃ�΃I�[�o�[���b�v�E�B���h�E����
	//�T�C�Y�ύX���E�ƍő剻�{�^���𖳌����ɂ���
	if ( MAINWINDOW_SIZE_VARIABLE == 0 )
		WindowStyle &= ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

	HWND hwnd = CreateAppWindow ( hInstance , MAINWINDOW_CLASSNAME , MAINWINDOW_DEFAULTTITLE ,
		WindowStyle , CW_USEDEFAULT , CW_USEDEFAULT , MAINWINDOW_CLIENT_WIDTH , MAINWINDOW_CLIENT_HEIGHT , ShowState );
	if ( hwnd == NULL )return FALSE;
	return TRUE;
}

/*
���C���E�B���h�E�v���V�[�W���[
*/
LRESULT CALLBACK MainWndProc ( HWND hwnd , UINT msg , WPARAM wp , LPARAM lp ) {
	//�N���C�A���g�T�C�Y�ۊǏꏊ
	RECT rcClientSize;

	//�N���C�A���g�T�C�Y�擾
	GetClientRect ( hwnd , &rcClientSize );

	//���b�Z�[�W���Ƃ̏���
	switch ( msg ) {
		case WM_DESTROY:
			PostQuitMessage ( 0 );
			break;
		default:
			return DefWindowProc ( hwnd , msg , wp , lp );
	}
	return 0;
}