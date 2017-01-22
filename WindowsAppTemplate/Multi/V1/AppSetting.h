/*----------------------------------------------------------------------------
プログラム名:
バージョン:
プログラム概要：
開発言語：Microsoft Visual C++ 2013
統合開発環境：Microsoft Visual Studio 2013 Ultimate Edition
開発開始日：
最終更新日：
-------------------------------------------------------------------------------
更新日一覧
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
/*-------------------------マクロ定義部------------------------*/
#define PRONAME		"Proname"							//プログラム名
#define PROVER		"1.0.1.0"							//プログラムバージョン
#define PRONAME_TXT	TEXT(PRONAME)						//プログラム名(テキスト)
#define PROVER_TXT	TEXT(PROVER)						//プログラムバージョン(テキスト)
//----------------------------------------------------------------
#define MAINWINDOW_CLIENT_WIDTH		800					//メインウィンドウのクライアントサイズ（幅）
#define MAINWINDOW_CLIENT_HEIGHT	600					//メインウィンドウのクライアントサイズ（高さ）
#define MAINWINDOW_SIZE_VARIABLE	1					//メインウィンドウがサイズ可変かどうか(0:固定、 それ以外：可変)
#define MAINWINDOW_BACKGROUNDCOLOR	RGB(240,240,240)	//メインウィンドウの背景色
#define MAINWINDOW_DEFAULTTITLE		PRONAME_TXT			//メインウィンドウの初期タイトル
#define MAINWINDOW_CLASSNAME		PRONAME_TXT			//メインウィンドウのウィンドウクラス名
/*-------------------------------------------------------------*/
