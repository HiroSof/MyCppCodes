#include "AppSetting.h"




ATOM RegisterAppWindowClass ( HINSTANCE hInstance , TCHAR *lpszClassName , WNDPROC funcWndProc , COLORREF bgColor = RGB ( 255 , 255 , 255 ) , int ResMenuID = 0 );
HWND CreateAppWindow ( HINSTANCE hInstance , TCHAR *lpszClassName , TCHAR *lpszTitle , DWORD WindowStyle , int x , int y , int ClientWidth , int ClientHeight , int ShowState );

BOOL InitAppWindowClass ( HINSTANCE hInstance );
BOOL InitAppWindow ( HINSTANCE hInstance , int ShowState );

BOOL InitMainWindowClass ( HINSTANCE hInstance );
BOOL InitMainWindow ( HINSTANCE hInstance , int ShowState );

LRESULT CALLBACK MainWndProc ( HWND hwnd , UINT msg , WPARAM wp , LPARAM lp );		/* メインウィンドウプロシージャー */
