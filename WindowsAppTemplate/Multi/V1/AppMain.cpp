
#include "AppSetting.h"
#include "AppWindowFunc.h"

/*----------------------------メイン関数定義部--------------------------------------------*/
int WINAPI WinMain ( HINSTANCE hCurInst , HINSTANCE hPrevInst , LPSTR lpsCmdLine , int nCmdShow ) {
	MSG msg;
	BOOL bRet;

	//COM初期化
	CoInitializeEx ( NULL , COINIT_APARTMENTTHREADED );

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

