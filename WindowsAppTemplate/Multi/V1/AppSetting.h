/*----------------------------------------------------------------------------
�v���O������:
�o�[�W����:
�v���O�����T�v�F
�J������FMicrosoft Visual C++ 2013
�����J�����FMicrosoft Visual Studio 2013 Ultimate Edition
�J���J�n���F
�ŏI�X�V���F
-------------------------------------------------------------------------------
�X�V���ꗗ
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
/*-------------------------�}�N����`��------------------------*/
#define PRONAME		"Proname"							//�v���O������
#define PROVER		"1.0.1.0"							//�v���O�����o�[�W����
#define PRONAME_TXT	TEXT(PRONAME)						//�v���O������(�e�L�X�g)
#define PROVER_TXT	TEXT(PROVER)						//�v���O�����o�[�W����(�e�L�X�g)
//----------------------------------------------------------------
#define MAINWINDOW_CLIENT_WIDTH		800					//���C���E�B���h�E�̃N���C�A���g�T�C�Y�i���j
#define MAINWINDOW_CLIENT_HEIGHT	600					//���C���E�B���h�E�̃N���C�A���g�T�C�Y�i�����j
#define MAINWINDOW_SIZE_VARIABLE	1					//���C���E�B���h�E���T�C�Y�ς��ǂ���(0:�Œ�A ����ȊO�F��)
#define MAINWINDOW_BACKGROUNDCOLOR	RGB(240,240,240)	//���C���E�B���h�E�̔w�i�F
#define MAINWINDOW_DEFAULTTITLE		PRONAME_TXT			//���C���E�B���h�E�̏����^�C�g��
#define MAINWINDOW_CLASSNAME		PRONAME_TXT			//���C���E�B���h�E�̃E�B���h�E�N���X��
/*-------------------------------------------------------------*/
