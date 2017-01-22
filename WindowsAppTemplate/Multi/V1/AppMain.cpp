
#include "AppSetting.h"
#include "AppWindowFunc.h"

/*----------------------------���C���֐���`��--------------------------------------------*/
int WINAPI WinMain ( HINSTANCE hCurInst , HINSTANCE hPrevInst , LPSTR lpsCmdLine , int nCmdShow ) {
	MSG msg;
	BOOL bRet;

	//COM������
	CoInitializeEx ( NULL , COINIT_APARTMENTTHREADED );

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

