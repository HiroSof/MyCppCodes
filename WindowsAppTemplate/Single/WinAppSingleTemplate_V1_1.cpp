/*----------------------------------------------------------------------------
プログラム名:Template
バージョン:1.0.1.0
プログラム概要：
開発言語：Microsoft Visual C++ 2013
統合開発環境：Microsoft Visual Studio 2013 Ultimate Edition
開発開始日：
最終更新日：
-------------------------------------------------------------------------------
更新日一覧
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
*/
/*-----------------------インクルード部------------------------*/
#include <windows.h>
/*------------------------------------------------------------*/
/*-----------------マニフェスト登録----------------------------*/
#pragma comment(linker,"\"/manifestdependency:type='win32' "	\
	"name='Microsoft.Windows.Common-Controls' "					\
	"version='6.0.0.0' "										\
	"processorArchitecture='*' "								\
	"publicKeyToken='6595b64144ccf1df' "						\
	"language='*'\"")
/*-------------------------------------------------------------*/
/*-------------------プロトタイプ宣言部------------------------*/

/*
アプリケーションサポート
*/
ATOM RegisterAppWindowClass ( HINSTANCE hInstance , TCHAR *lpszClassName , WNDPROC funcWndProc , COLORREF bgColor = RGB ( 255 , 255 , 255 ) , int ResMenuID = 0 );
HWND CreateAppWindow ( HINSTANCE hInstance , TCHAR *lpszClassName , TCHAR *lpszTitle , DWORD WindowStyle , int x , int y , int ClientWidth , int ClientHeight , int ShowState );


BOOL InitAppWindowClass ( HINSTANCE hInstance );
BOOL InitAppWindow ( HINSTANCE hInstance , int ShowState );

BOOL InitMainWindow ( HINSTANCE hInstance , int ShowState );

LRESULT CALLBACK MainWndProc ( HWND hwnd , UINT msg , WPARAM wp , LPARAM lp );		/* メインウィンドウプロシージャー */

/*-------------------------------------------------------------*/
/*-------------------------マクロ定義部------------------------*/
#define PRONAME		"Template"							//プログラム名
#define PROVER		"1.0.1.0"							//プログラムバージョン
#define PRONAME_TXT	TEXT(PRONAME)						//プログラム名(テキスト)
#define PROVER_TXT	TEXT(PROVER)						//プログラムバージョン(テキスト)
//----------------------------------------------------------------
#define MAINWINDOW_CLIENT_WIDTH		800					//メインウィンドウのクライアントサイズ（幅）
#define MAINWINDOW_CLIENT_HEIGHT	600					//メインウィンドウのクライアントサイズ（高さ）
#define MAINWINDOW_SIZE_VARIABLE	1					//メインウィンドウがサイズ可変かどうか(0:固定、 それ以外：可変)
#define MAINWINDOW_BACKGROUNDCOLOR	RGB(255,255,255)	//メインウィンドウの背景色
#define MAINWINDOW_DEFAULTTITLE		PRONAME_TXT			//メインウィンドウの初期タイトル
#define MAINWINDOW_CLASSNAME		PRONAME_TXT			//メインウィンドウのウィンドウクラス名
/*-------------------------------------------------------------*/
/*-------------------グローバル変数定義部----------------------*/
HINSTANCE hInstance = NULL;
/*-------------------------------------------------------------*/

/*----------------------------メイン関数定義部--------------------------------------------*/
int WINAPI WinMain ( HINSTANCE hCurInst , HINSTANCE hPrevInst , LPSTR lpsCmdLine , int nCmdShow ) {
	MSG msg;
	BOOL bRet;

	//COM初期化
	CoInitializeEx ( NULL , COINIT_APARTMENTTHREADED );

	//インスタンスハンドルをグローバル変数にコピー
	hInstance = hCurInst;

	//ウィンドウクラスの登録
	if ( !InitAppWindowClass ( hCurInst ) ) return FALSE;

	//ウィンドウ作成
	if ( !InitAppWindow ( hCurInst , nCmdShow ) ) return FALSE;

	//メッセージループ
	while ( ( bRet = GetMessage ( &msg , NULL , 0 , 0 ) ) ) {
		if ( bRet == -1 ) {
			MessageBox ( NULL , TEXT ( "GetMessageエラー" ) , TEXT ( "Error" ) , MB_OK );
			break;
		} else {
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );
		}
	}
	//COM 解放
	CoUninitialize ();
	return (int) msg.wParam;
}

ATOM RegisterAppWindowClass ( HINSTANCE hInstance , TCHAR *lpszClassName , WNDPROC funcWndProc , COLORREF bgColor , int ResMenuID ) {
	WNDCLASSEX wc; //WNDCLASSEX構造体
	wc.cbSize = sizeof ( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = funcWndProc; //プロシージャ名
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance; //インスタンス
	wc.hIcon = (HICON) LoadImage ( NULL , MAKEINTRESOURCE ( IDI_APPLICATION ) , IMAGE_ICON , 0 , 0 , LR_DEFAULTSIZE | LR_SHARED );
	wc.hCursor = (HCURSOR) LoadImage ( NULL , MAKEINTRESOURCE ( IDC_ARROW ) , IMAGE_CURSOR , 0 , 0 , LR_DEFAULTSIZE | LR_SHARED );
	wc.hbrBackground = CreateSolidBrush ( bgColor );
	wc.lpszMenuName = MAKEINTRESOURCE ( ResMenuID ); //メニュー名
	wc.lpszClassName = lpszClassName;
	wc.hIconSm = (HICON) LoadImage ( NULL , MAKEINTRESOURCE ( IDI_APPLICATION ) , IMAGE_ICON , 0 , 0 , LR_DEFAULTSIZE | LR_SHARED );
	return RegisterClassEx ( &wc );
}

HWND CreateAppWindow ( HINSTANCE hInstance , TCHAR *lpszClassName , TCHAR *lpszTitle , DWORD WindowStyle , int x , int y , int ClientWidth , int ClientHeight , int ShowState ) {

	WNDCLASSEX wc; //WNDCLASSEX構造体
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


	//ウィンドウ表示
	ShowWindow ( hWnd , ShowState );

	//ウィンドウ更新
	UpdateWindow ( hWnd );

	return hWnd;
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

BOOL InitAppWindowClass ( HINSTANCE hInstance ) {
	ATOM atMain = RegisterAppWindowClass ( hInstance , MAINWINDOW_CLASSNAME , MainWndProc , MAINWINDOW_BACKGROUNDCOLOR );
	if ( atMain == NULL )return FALSE;
	return TRUE;
}

BOOL InitAppWindow ( HINSTANCE hInstance , int ShowState ) {
	return InitMainWindow ( hInstance , ShowState );
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