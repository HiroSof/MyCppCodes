#include "AppWindowFunc.h"


ATOM RegisterAppWindowClass ( HINSTANCE hInstance , TCHAR *lpszClassName , WNDPROC funcWndProc , COLORREF bgColor , int ResMenuID ) {
	WNDCLASSEX wc; //WNDCLASSEX�\����
	wc.cbSize = sizeof ( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = funcWndProc; //�v���V�[�W����
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance; //�C���X�^���X
	wc.hIcon = (HICON) LoadImage ( NULL , MAKEINTRESOURCE ( IDI_APPLICATION ) , IMAGE_ICON , 0 , 0 , LR_DEFAULTSIZE | LR_SHARED );
	wc.hCursor = (HCURSOR) LoadImage ( NULL , MAKEINTRESOURCE ( IDC_ARROW ) , IMAGE_CURSOR , 0 , 0 , LR_DEFAULTSIZE | LR_SHARED );
	wc.hbrBackground = CreateSolidBrush ( bgColor );
	wc.lpszMenuName = MAKEINTRESOURCE ( ResMenuID ); //���j���[��
	wc.lpszClassName = lpszClassName;
	wc.hIconSm = (HICON) LoadImage ( NULL , MAKEINTRESOURCE ( IDI_APPLICATION ) , IMAGE_ICON , 0 , 0 , LR_DEFAULTSIZE | LR_SHARED );
	return RegisterClassEx ( &wc );
}

HWND CreateAppWindow ( HINSTANCE hInstance , TCHAR *lpszClassName , TCHAR *lpszTitle , DWORD WindowStyle , int x , int y , int ClientWidth , int ClientHeight , int ShowState ) {

	WNDCLASSEX wc; //WNDCLASSEX�\����
	wc.cbSize = sizeof ( WNDCLASSEX );

	if ( GetClassInfoEx ( hInstance , lpszClassName , &wc ) == FALSE ) return NULL;

	int Width , Height;
	RECT adustRect;
	adustRect.left = 0;
	adustRect.top = 0;
	adustRect.right = ClientWidth;
	adustRect.bottom = ClientHeight;
	if ( AdjustWindowRectEx ( &adustRect , WindowStyle , ( wc.lpszMenuName ) ? TRUE : FALSE , 0 ) ) {
		Width = adustRect.right - adustRect.left;
		Height = adustRect.bottom - adustRect.top;
	} else {
		Width = ClientWidth;
		Height = ClientHeight;
	}

	HWND hWnd = CreateWindowEx ( NULL , lpszClassName , lpszTitle , WindowStyle , x , y , Width , Height , NULL , NULL , hInstance , NULL );

	if ( hWnd == NULL )return NULL;


	//�E�B���h�E�\��
	ShowWindow ( hWnd , ShowState );

	//�E�B���h�E�X�V
	UpdateWindow ( hWnd );

	return hWnd;
}



BOOL InitAppWindowClass ( HINSTANCE hInstance ) {
	BOOL bRet = InitMainWindowClass ( hInstance );
	return bRet;
}
BOOL InitAppWindow ( HINSTANCE hInstance , int ShowState ) {
	BOOL bRet = InitMainWindow ( hInstance , ShowState );
	return bRet;
}

