/*----------------------------------------------------------------------------
ライブラリ名:
バージョン:1.0.1.0
ライブラリ概要：
開発言語：Microsoft Visual C++ 2015
統合開発環境：Microsoft Visual Studio 2015 Community Edition
開発開始日：
最終更新日：
-------------------------------------------------------------------------------
更新日一覧
--------------------------------------------------------------------------------
*/

#include <Windows.h>

static HINSTANCE hInstance;

BOOL WINAPI DllMain ( HINSTANCE hDLL , DWORD dwReason , LPVOID lpReserved ) {
	switch ( dwReason ) {
		case DLL_PROCESS_ATTACH:
			hInstance = hDLL;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}