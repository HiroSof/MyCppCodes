/*----------------------------------------------------------------------------
ライブラリ名:
バージョン:1.0.1.0
ライブラリ概要：
開発言語：Microsoft Visual C++ 2013
統合開発環境：Microsoft Visual Studio 2013 Ultimate Edition
開発開始日：
最終更新日：
-------------------------------------------------------------------------------
更新日一覧
--------------------------------------------------------------------------------
*/

#include <Windows.h>

static HINSTANCE hInstance;

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved){
	hInstance = hDLL;
	return TRUE;
}