/*----------------------------------------------------------------------------
�v���O������:Template
�o�[�W����:1.0.1.0
�v���O�����T�v�F
�J������FMicrosoft Visual C++ 2013
�����J�����FMicrosoft Visual Studio 2013 Ultimate Edition
�J���J�n���F
�ŏI�X�V���F
-------------------------------------------------------------------------------
�X�V���ꗗ
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
*/
/*-----------------------�C���N���[�h��------------------------*/
#include <windows.h>
/*------------------------------------------------------------*/
/*-----------------�}�j�t�F�X�g�o�^----------------------------*/
#pragma comment(linker,"\"/manifestdependency:type='win32' "	\
	"name='Microsoft.Windows.Common-Controls' "					\
	"version='6.0.0.0' "										\
	"processorArchitecture='*' "								\
	"publicKeyToken='6595b64144ccf1df' "						\
	"language='*'\"")
/*-------------------------------------------------------------*/
/*-------------------�v���g�^�C�v�錾��------------------------*/

/*
�A�v���P�[�V�����T�|�[�g
*/
ATOM RegisterAppWindowClass ( HINSTANCE hInstance , TCHAR *lpszClassName , WNDPROC funcWndProc , COLORREF bgColor = RGB ( 255 , 255 , 255 ) , int ResMenuID = 0 );
HWND CreateAppWindow ( HINSTANCE hInstance , TCHAR *lpszClassName , TCHAR *lpszTitle , DWORD WindowStyle , int x , int y , int ClientWidth , int ClientHeight , int ShowState );


BOOL InitAppWindowClass ( HINSTANCE hInstance );
BOOL InitAppWindow ( HINSTANCE hInstance , int ShowState );

BOOL InitMainWindow ( HINSTANCE hInstance , int ShowState );

LRESULT CALLBACK MainWndProc ( HWND hwnd , UINT msg , WPARAM wp , LPARAM lp );		/* ���C���E�B���h�E�v���V�[�W���[ */

/*-------------------------------------------------------------*/
/*-------------------------�}�N����`��------------------------*/
#define PRONAME		"Template"							//�v���O������
#define PROVER		"1.0.1.0"							//�v���O�����o�[�W����
#define PRONAME_TXT	TEXT(PRONAME)						//�v���O������(�e�L�X�g)
#define PROVER_TXT	TEXT(PROVER)						//�v���O�����o�[�W����(�e�L�X�g)
//----------------------------------------------------------------
#define MAINWINDOW_CLIENT_WIDTH		800					//���C���E�B���h�E�̃N���C�A���g�T�C�Y�i���j
#define MAINWINDOW_CLIENT_HEIGHT	600					//���C���E�B���h�E�̃N���C�A���g�T�C�Y�i�����j
#define MAINWINDOW_SIZE_VARIABLE	1					//���C���E�B���h�E���T�C�Y�ς��ǂ���(0:�Œ�A ����ȊO�F��)
#define MAINWINDOW_BACKGROUNDCOLOR	RGB(255,255,255)	//���C���E�B���h�E�̔w�i�F
#define MAINWINDOW_DEFAULTTITLE		PRONAME_TXT			//���C���E�B���h�E�̏����^�C�g��
#define MAINWINDOW_CLASSNAME		PRONAME_TXT			//���C���E�B���h�E�̃E�B���h�E�N���X��
/*-------------------------------------------------------------*/
/*-------------------�O���[�o���ϐ���`��----------------------*/
HINSTANCE hInstance = NULL;
/*-------------------------------------------------------------*/

/*----------------------------���C���֐���`��--------------------------------------------*/
int WINAPI WinMain ( HINSTANCE hCurInst , HINSTANCE hPrevInst , LPSTR lpsCmdLine , int nCmdShow ) {
	MSG msg;
	BOOL bRet;

	//COM������
	CoInitializeEx ( NULL , COINIT_APARTMENTTHREADED );

	//�C���X�^���X�n���h�����O���[�o���ϐ��ɃR�s�[
	hInstance = hCurInst;

	//�E�B���h�E�N���X�̓o�^
	if ( !InitAppWindowClass ( hCurInst ) ) return FALSE;

	//�E�B���h�E�쐬
	if ( !InitAppWindow ( hCurInst , nCmdShow ) ) return FALSE;

	//���b�Z�[�W���[�v
	while ( ( bRet = GetMessage ( &msg , NULL , 0 , 0 ) ) ) {
		if ( bRet == -1 ) {
			MessageBox ( NULL , TEXT ( "GetMessage�G���[" ) , TEXT ( "Error" ) , MB_OK );
			break;
		} else {
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );
		}
	}
	//COM ���
	CoUninitialize ();
	return (int) msg.wParam;
}

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

BOOL InitAppWindowClass ( HINSTANCE hInstance ) {
	ATOM atMain = RegisterAppWindowClass ( hInstance , MAINWINDOW_CLASSNAME , MainWndProc , MAINWINDOW_BACKGROUNDCOLOR );
	if ( atMain == NULL )return FALSE;
	return TRUE;
}

BOOL InitAppWindow ( HINSTANCE hInstance , int ShowState ) {
	return InitMainWindow ( hInstance , ShowState );
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