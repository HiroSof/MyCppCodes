/************************************************************************************
	CHSRIFFReader Class
	- RIFF(Resource Interchange File Format) Reader Class Header -
	Copyright (C) 2014 HiroakiSoftware. All rights reserved.
************************************************************************************/
#pragma once
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#pragma comment(lib , "Shlwapi.lib")
#define DHSRIFF_FIRSTSUBCHUNKPOS 12
struct THSRIFF_CHUNKHEADER {
	char Name[4];
	UINT32 Size;
};
struct THSRIFF_LISTCHUNKHEADER {
	THSRIFF_CHUNKHEADER ChunkHeader;
	char Type[4];
};
struct THSRIFF_ENUMCHUNKINFO {
	UINT32 Position;
	char Name[5];
	UINT32 Size;
	char Type[5]; /*LIST Chunk Only*/
};
class CHSRIFFReader;
typedef bool (__stdcall *LPFHSRIFFENUMPROC)(CHSRIFFReader *pReader ,THSRIFF_ENUMCHUNKINFO *lpInfo);

class CHSRIFFReader {
protected:
	TCHAR szFilePath[MAX_PATH];	//�t�@�C����
	HANDLE hFile;		//�t�@�C���n���h��

	virtual bool OpenedCallBack(void);
	virtual bool BeforeCloseCallBack(void);
public:
	CHSRIFFReader(TCHAR *lpszRiffFilePath = nullptr);
	~CHSRIFFReader();

	//RIFF�t�@�C�����J��
	bool Open(TCHAR *lpszRiffFilePath);

	//RIFF�t�@�C����ς���
	bool Change(TCHAR *lpszNewRiffFilePath);

	//RIFF�t�@�C�������
	bool Close(void);

	//RIFF�t�@�C�����J����Ă邩�̊m�F
	bool CheckPresent(void);

	//�J����Ă���t�@�C���̃p�X���擾����
	bool GetFilePath(TCHAR *lpszRiffFilePath);

	//�t�@�C���̃T�C�Y���擾
	UINT32 GetFileSize(void);

	//RIFF�`�����N�̃f�[�^�T�C�Y(�t�@�C���T�C�Y-8)���擾
	UINT32 GetRiffDataSize(void);

	//RIFF�t�@�C���̎��ʎq���Ɖ��
	bool CheckIdentifier(char *lpFileType);

	//RIFF�t�@�C���̎��ʎq���擾����
	bool GetIdentifier(char *lpFileType);

	//�w�肵���`�����N�̈ʒu���擾����
	bool GetChunkPos(char *lpChunkName , UINT32 *lpChunkPos);

	//�w�肵���`�����N�̃T�C�Y���擾����
	bool GetChunkSize(char *lpChunkName , UINT32 *lpChunkSize);

	//�w�肵���`�����N�̃f�[�^���擾����
	UINT32 GetChunkData(char *lpChunkName , void *lpValueData , UINT32 ValueDataSize);

	//�`�����N��񋓂���
	bool EnumChunk(LPFHSRIFFENUMPROC pProc);

	//�w�肵��LIST�`�����N�̈ʒu���擾
	bool GetListChunkPos(char *lpType ,  UINT32 *lpChunkPos);

	//�w�肵��LIST�`�����N�̃T�C�Y���擾
	bool GetListChunkSize(char *lpType ,  UINT32 *lpChunkSize);

	//�w�肵��LIST�`�����N�ɂ���w�肵���T�u�`�����N�̈ʒu���擾
	bool GetListSubChunkPos(char *lpType , char *lpChunkName , UINT32 *lpChunkPos);
	
	//�w�肵��LIST�`�����N�ɂ���T�u�`�����N��񋓂���
	bool EnumListSubChunk(char *lpType , LPFHSRIFFENUMPROC pProc);

	//�w�肵��LIST�`�����N�ɂ���w�肵���T�u�`�����N�̃T�C�Y���擾
	bool GetListSubChunkSize(char *lpType , char *lpChunkName , UINT32 *lpChunkSize);
	
	//�w�肵��LIST�`�����N�ɂ���w�肵���T�u�`�����N���擾
	UINT32 GetListSubChunkData(char *lpType , char *lpChunkName , void *lpValueData , UINT32 ValueDataSize);
};