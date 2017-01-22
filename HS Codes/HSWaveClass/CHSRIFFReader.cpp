/************************************************************************************
	CHSRIFFReader Class
	- RIFF(Resource Interchange File Format) Reader Class -
	Copyright (C) 2014 HiroakiSoftware. All rights reserved.
************************************************************************************/

#include "CHSRIFFReader.hpp"

CHSRIFFReader::CHSRIFFReader(TCHAR *lpszRiffFilePath) {
	this->hFile = NULL;
	memset(this->szFilePath, 0, sizeof(this->szFilePath));
	if (lpszRiffFilePath) this->Open(lpszRiffFilePath);
}

CHSRIFFReader::~CHSRIFFReader() {
	this->Close();
}


bool CHSRIFFReader::OpenedCallBack(void) {
	return true;
}

bool CHSRIFFReader::BeforeCloseCallBack(void) {

	return true;
}

bool CHSRIFFReader::Open(TCHAR *lpszRiffFilePath) {

	if (!lpszRiffFilePath) return false;

	if (this->hFile) return false;


	HANDLE FileHandle;

	FileHandle = CreateFile(lpszRiffFilePath , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL);

	if (FileHandle == INVALID_HANDLE_VALUE) {
		return false;
	}
	
	THSRIFF_CHUNKHEADER header;
	DWORD readsize = 0;
	SetFilePointer(FileHandle , 0 , 0 , FILE_BEGIN);

	ReadFile(FileHandle , &header , sizeof(header) , &readsize , 0);
	
	if (readsize != sizeof(header)) {
		CloseHandle(FileHandle);
		return false;
	}

	if (_strnicmp(header.Name, "RIFF", 4)) {
		CloseHandle(FileHandle);
		return false;
	}


	LARGE_INTEGER li;
	if (GetFileSizeEx(FileHandle, &li) == false) {
		CloseHandle(FileHandle);
		return false;
	}

	if (li.QuadPart != (header.Size + 8)) {
		CloseHandle(FileHandle);
		return false;
	}

	SetFilePointer(FileHandle , 0 , 0 , FILE_BEGIN);
	this->hFile = FileHandle;

	if (PathIsRelative(lpszRiffFilePath)) {
		PTCHAR lplpFilename;
		if (GetFullPathName(lpszRiffFilePath, MAX_PATH, this->szFilePath, &lplpFilename) == NULL) {
			lstrcpy(this->szFilePath , lpszRiffFilePath);
		}

	} else {
		lstrcpy(this->szFilePath , lpszRiffFilePath);
	}

	if (this->OpenedCallBack() == false) {
		this->Close();
		return false;
	}

	return true;
}
bool CHSRIFFReader::Close(void) {
	if (this->hFile == NULL) return false;
	if (this->BeforeCloseCallBack() == false) return false;
	CloseHandle(this->hFile);
	this->hFile = NULL;
	memset(this->szFilePath, 0, sizeof(this->szFilePath));
	return true;
}

//RIFF�t�@�C����ς���
bool CHSRIFFReader::Change(TCHAR *lpszNewRiffFilePath) {
	if (!lpszNewRiffFilePath) return false;
	if (PathFileExists(lpszNewRiffFilePath) == false) return false;
	if (this->CheckPresent()){
		if(this->Close()){
			if (this->Open(lpszNewRiffFilePath)){
				return true;
			}
		}
	}

	return false;
}

bool CHSRIFFReader::CheckPresent(void) {
	if (this->hFile) return true;
	return false;
}


bool CHSRIFFReader::GetFilePath(TCHAR *lpszRiffFilePath) {
	if ((this->hFile)&&(lstrlen(this->szFilePath))) {
		if (lpszRiffFilePath) {
			lstrcpy(lpszRiffFilePath , this->szFilePath);
			return true;
		}
	}
	return false;
}


//�t�@�C���̃T�C�Y���擾
UINT32 CHSRIFFReader::GetFileSize(void) {
	if (this->hFile) {

		LARGE_INTEGER li;
		if (GetFileSizeEx(this->hFile, &li) == false) {
			return 0;
		}

		return (UINT32)li.QuadPart;
	}
	return 0;
}

//RIFF�`�����N�̃f�[�^�T�C�Y(�t�@�C���T�C�Y-8)���擾
UINT32 CHSRIFFReader::GetRiffDataSize(void) {
	
	if (this->hFile) {
		SetFilePointer(this->hFile , 4 , 0 , FILE_BEGIN);
		UINT32 size;
		DWORD rs = 0;

		ReadFile(this->hFile , &size , 4 , &rs , 0);
		if (rs != 4) return 0;
		return size;
	}
	return 0;
}


bool CHSRIFFReader::GetIdentifier(char *lpFileType) {
	if (this->hFile == NULL) return false;

	char Type[4];
	DWORD dwLoad = 0;
	SetFilePointer(this->hFile , 8 , 0 , FILE_BEGIN);

	ReadFile(this->hFile , Type , 4 , &dwLoad , 0);

	if (dwLoad != 4) return false;

	memcpy(lpFileType , Type , 4);

	return true;
}


bool CHSRIFFReader::CheckIdentifier(char *lpFileType) {
	char FileType[4];
	if (this->GetIdentifier(FileType)) {
		if (_strnicmp(FileType , lpFileType , 4))return false;
		return true;
	}
	return false;
}


//�w�肵���`�����N�̈ʒu���擾����
bool CHSRIFFReader::GetChunkPos(char *lpChunkName, UINT32 *lpChunkPos) {

	LARGE_INTEGER LI;
	char ChunkText[5];
	THSRIFF_CHUNKHEADER ChunkHeader;
	bool bFound = false;
	DWORD	rs;

	//�����Ȃǂɖ�肠�邩���m�F
	if(!this->hFile | !lpChunkName)return FALSE;	
	if((lstrlenA(lpChunkName)>=5) || (lstrlenA(lpChunkName)==0))return FALSE;

	//�����Ώۂ̃`�����N���̏���(lpChunk�̕�����̃T�C�Y�ɍő�3�o�C�g�̋󔒂�ǉ�����B)
	int iChunkTextNums = lstrlenA(lpChunkName); 
	memset(ChunkText,0,5);
	memcpy(ChunkText,lpChunkName,iChunkTextNums);
	if(iChunkTextNums!=4)memset(ChunkText+iChunkTextNums,' ',(4-iChunkTextNums));

	//�X�^�[�g�ʒu
	LI.QuadPart = DHSRIFF_FIRSTSUBCHUNKPOS;

	/*
		�ȉ��ɒ񎦂��������ꂩ�̏����ɊY������܂Ŗ������[�v�ŃX�L����
		����1�F������
		����2�F�t�@�C���̏I�[�ɂ��ǂ蒅��
		����3�F���[�v���Ŏg�p�����֐��ŃG���[���N����

	*/
	while (1) {

		//�t�@�C���|�C���^�̈ړ��Ɏ��s�����ꍇ�̓��[�v����O���
		if(!SetFilePointerEx(this->hFile,LI,NULL,FILE_BEGIN))break;

		//8�o�C�g(�`�����N��+�`�����N�T�C�Y)���̓ǂݍ��݂Ɏ��s�����ꍇ�̓��[�v����O���
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)break;
			
		//8�o�C�g�ǂ߂Ă��Ȃ���΃��[�v����O���
		if(rs != 8)break;
		
		//�w�肳�ꂽ�`�����N�Ȃ��CKFound = TRUE �Ƃ��� ���[�v���O���(����)
		if(!_strnicmp(ChunkText,ChunkHeader.Name,4)){
			bFound = true;
			break;
		}
		/*
			�����[�v�ł̓ǂݍ��݈ʒu�����̎菇�Ōv�Z
			�菇1.�`�����N�T�C�Y����Ȃ��1���Z���ċ����ɂ���
			�菇2.�菇1�̌��� + 8������B
			�菇3.LI.QuadPart�Ɏ菇2�̌��ʂ����Z

		*/
	
		//chunksize����Ȃ��1���Z���ċ����ɂ���
		if (ChunkHeader.Size % 2)ChunkHeader.Size++;

		//LI.QuadPart�Ɏ��̃`�����N�ւ̃I�t�Z�b�g�����Z����
		LI.QuadPart += ChunkHeader.Size + 8;
	}

	if (bFound == false) return false;
	if (lpChunkPos) *lpChunkPos = (UINT32)LI.QuadPart;
	return true;
}

//�w�肵���`�����N�̃T�C�Y���擾����
bool CHSRIFFReader::GetChunkSize(char *lpChunkName, UINT32 *lpChunkSize) {
	UINT32 ChunkPos;
	if (!lpChunkSize)return false;

	if (this->GetChunkPos(lpChunkName, &ChunkPos)) {
		LARGE_INTEGER li;
		DWORD rs;
		THSRIFF_CHUNKHEADER ChunkHeader;
		li.QuadPart = ChunkPos;

		//�t�@�C���|�C���^�̈ړ��Ɏ��s�����ꍇ��false��Ԃ�
		if(!SetFilePointerEx(this->hFile,li,NULL,FILE_BEGIN))return false;

		//8�o�C�g(�`�����N��+�`�����N�T�C�Y)���̓ǂݍ��݂Ɏ��s�����ꍇ��false��Ԃ�
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)return false;

		*lpChunkSize = ChunkHeader.Size;

		return true;
	}
	return false;
}


//�w�肵���`�����N�̃f�[�^���擾����
UINT32 CHSRIFFReader::GetChunkData(char *lpChunkName, void *lpValueData, UINT32 ValueDataSize) {
	UINT32 ChunkPos;
	UINT32 ChunkSize;

	UINT32 LoadSize;
	DWORD readSize;

	if (!lpValueData)return 0;

	if (this->GetChunkPos(lpChunkName, &ChunkPos)) {
		if (this->GetChunkSize(lpChunkName, &ChunkSize)) {

			LoadSize = (ChunkSize < ValueDataSize) ? ChunkSize : ValueDataSize;

			LARGE_INTEGER li;
			li.QuadPart = ChunkPos + 8;

			//�t�@�C���|�C���^�̈ړ��Ɏ��s�����ꍇ��false��Ԃ�
			if(!SetFilePointerEx(this->hFile,li,NULL,FILE_BEGIN))return 0;

			//�`�����N�f�[�^��ǂݍ���
			if(ReadFile(this->hFile,lpValueData,LoadSize,&readSize,NULL)==NULL)return 0;

			return readSize;
		}
	}
	return 0;
}

//�`�����N��񋓂���
bool CHSRIFFReader::EnumChunk(LPFHSRIFFENUMPROC pProc) {

	if (!pProc) return false;

	LARGE_INTEGER LI;
	LI.QuadPart = DHSRIFF_FIRSTSUBCHUNKPOS;

	THSRIFF_CHUNKHEADER ChunkHeader;
	bool bFound = false;
	DWORD	rs;
	char ChunkType[5] = { 0 };
	THSRIFF_ENUMCHUNKINFO info;

	/*
		�ȉ��ɒ񎦂��������ꂩ�̏����ɊY������܂Ŗ������[�v�ŃX�L����
		����1�F�t�@�C���̏I�[�ɂ��ǂ蒅��
		����2�F���[�v���Ŏg�p�����֐��ŃG���[���N����

	*/
	while (1) {

		memset(&info, 0, sizeof(THSRIFF_ENUMCHUNKINFO));

		//�t�@�C���|�C���^�̈ړ��Ɏ��s�����ꍇ�̓��[�v����O���
		if(!SetFilePointerEx(this->hFile,LI,NULL,FILE_BEGIN))break;

		//8�o�C�g(�`�����N��+�`�����N�T�C�Y)���̓ǂݍ��݂Ɏ��s�����ꍇ�̓��[�v����O���
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)break;
			
		//8�o�C�g�ǂ߂Ă��Ȃ���΃��[�v����O���
		if(rs != 8)break;
		
		//LIST�`�����N�Ȃ炻�̃^�C�v���擾
		if (!_strnicmp("LIST", ChunkHeader.Name, 4)) {

			//4�o�C�g�̓ǂݍ��݂ɐ��������ꍇ�̂݃R�[���o�b�N�֐��ɓn���\���̂ɃR�s�[
			if (ReadFile(this->hFile, ChunkType, 4, &rs, NULL)) {
				if (rs == 4) {
					lstrcpyA(info.Type , ChunkType);
				}
			}
		}


		info.Position = (UINT32)LI.QuadPart;
		info.Size = ChunkHeader.Size;


		memcpy(info.Name , ChunkHeader.Name ,  4);


		//�R�[���o�b�N�֐����Ă�ŁAfalse���Ԃ��Ă�����񋓂��I������
		if (pProc(this , &info) == false) break;


		/*
			�����[�v�ł̓ǂݍ��݈ʒu�����̎菇�Ōv�Z
			�菇1.�`�����N�T�C�Y����Ȃ��1���Z���ċ����ɂ���
			�菇2.�菇1�̌��� + 8������B
			�菇3.LI.QuadPart�Ɏ菇2�̌��ʂ����Z

		*/
	
		//chunksize����Ȃ��1���Z���ċ����ɂ���
		if (ChunkHeader.Size % 2)ChunkHeader.Size++;

		//LI.QuadPart�Ɏ��̃`�����N�ւ̃I�t�Z�b�g�����Z����
		LI.QuadPart += ChunkHeader.Size + 8;

	}

	return true;
}



//�w�肵��LIST�`�����N�̈ʒu���擾
bool CHSRIFFReader::GetListChunkPos(char *lpType, UINT32 *lpChunkPos) {

	LARGE_INTEGER LI;
	char TypeText[5];
	THSRIFF_LISTCHUNKHEADER ListChunkHeader;
	bool bFound = false;
	DWORD	rs;

	//�����Ȃǂɖ�肠�邩���m�F
	if(!this->hFile | !lpType)return FALSE;	
	if((lstrlenA(lpType)>=5) || (lstrlenA(lpType)==0))return FALSE;

	//�����Ώۂ̃`�����N���̏���(lpChunk�̕�����̃T�C�Y�ɍő�3�o�C�g�̋󔒂�ǉ�����B)
	int iTypeTextNums = lstrlenA(lpType); 
	memset(TypeText,0,5);
	memcpy(TypeText,lpType,iTypeTextNums);
	if(iTypeTextNums!=4)memset(TypeText+iTypeTextNums,' ',(4-iTypeTextNums));

	//�X�^�[�g�ʒu
	LI.QuadPart = DHSRIFF_FIRSTSUBCHUNKPOS;

	/*
		�ȉ��ɒ񎦂��������ꂩ�̏����ɊY������܂Ŗ������[�v�ŃX�L����
		����1�F������
		����2�F�t�@�C���̏I�[�ɂ��ǂ蒅��
		����3�F���[�v���Ŏg�p�����֐��ŃG���[���N����

	*/
	while (1) {

		//�t�@�C���|�C���^�̈ړ��Ɏ��s�����ꍇ�̓��[�v����O���
		if(!SetFilePointerEx(this->hFile,LI,NULL,FILE_BEGIN))break;

		//8�o�C�g(�`�����N��+�`�����N�T�C�Y)���̓ǂݍ��݂Ɏ��s�����ꍇ�̓��[�v����O���
		if(ReadFile(this->hFile,&ListChunkHeader,sizeof(THSRIFF_LISTCHUNKHEADER),&rs,NULL)==NULL)break;
			
		//8�o�C�g�ǂ߂Ă��Ȃ���΃��[�v����O���
		if(rs != sizeof(THSRIFF_LISTCHUNKHEADER))break;
		
		//�w�肳�ꂽ�`�����N�Ȃ��CKFound = TRUE �Ƃ��� ���[�v���O���(����)
		if(!_strnicmp("LIST",ListChunkHeader.ChunkHeader.Name,4)){
			if (!_strnicmp(TypeText, ListChunkHeader.Type, 4)) {
				bFound = true;
				break;
			}
		}
		/*
			�����[�v�ł̓ǂݍ��݈ʒu�����̎菇�Ōv�Z
			�菇1.�`�����N�T�C�Y����Ȃ��1���Z���ċ����ɂ���
			�菇2.�菇1�̌��� + 8������B
			�菇3.LI.QuadPart�Ɏ菇2�̌��ʂ����Z

		*/
	
		//chunksize����Ȃ��1���Z���ċ����ɂ���
		if (ListChunkHeader.ChunkHeader.Size % 2)ListChunkHeader.ChunkHeader.Size++;

		//LI.QuadPart�Ɏ��̃`�����N�ւ̃I�t�Z�b�g�����Z����
		LI.QuadPart += ListChunkHeader.ChunkHeader.Size + 8;
	}

	if (bFound == false) return false;
	if (lpChunkPos) *lpChunkPos = (UINT32)LI.QuadPart;

	return true;
}

//�w�肵��LIST�`�����N�̃T�C�Y���擾
bool CHSRIFFReader::GetListChunkSize(char *lpType, UINT32 *lpChunkSize) {

	UINT32 ChunkPos;
	if (!lpChunkSize)return false;

	if (this->GetListChunkPos(lpType, &ChunkPos)) {
		LARGE_INTEGER li;
		DWORD rs;
		THSRIFF_CHUNKHEADER ChunkHeader;
		li.QuadPart = ChunkPos;

		//�t�@�C���|�C���^�̈ړ��Ɏ��s�����ꍇ��false��Ԃ�
		if(!SetFilePointerEx(this->hFile,li,NULL,FILE_BEGIN))return false;

		//8�o�C�g(�`�����N��+�`�����N�T�C�Y)���̓ǂݍ��݂Ɏ��s�����ꍇ��false��Ԃ�
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)return false;

		*lpChunkSize = ChunkHeader.Size;

		return true;
	}

	return false;
}


//�w�肵��LIST�`�����N�ɂ���w�肵���T�u�`�����N�̈ʒu���擾
bool CHSRIFFReader::GetListSubChunkPos(char *lpType, char *lpChunkName, UINT32 *lpChunkPos) {

	LARGE_INTEGER LI;
	char ChunkText[5];
	THSRIFF_CHUNKHEADER ChunkHeader;
	bool bFound = false;
	DWORD	rs;

	//�����Ȃǂɖ�肠�邩���m�F
	if(!this->hFile | !lpChunkName)return FALSE;	
	if((lstrlenA(lpChunkName)>=5) || (lstrlenA(lpChunkName)==0))return FALSE;

	//�����Ώۂ̃`�����N���̏���(lpChunk�̕�����̃T�C�Y�ɍő�3�o�C�g�̋󔒂�ǉ�����B)
	int iChunkTextNums = lstrlenA(lpChunkName); 
	memset(ChunkText,0,5);
	memcpy(ChunkText,lpChunkName,iChunkTextNums);
	if(iChunkTextNums!=4)memset(ChunkText+iChunkTextNums,' ',(4-iChunkTextNums));

	//�X�^�[�g�ʒu
	UINT32 ListPos , ListSize , QuitPos;

	if(this->GetListChunkPos(lpType ,&ListPos) == false) return false;
	if(this->GetListChunkSize(lpType ,&ListSize) == false) return false;

	ListSize -= 4;
	LI.QuadPart = ListPos + 12;

	QuitPos = ListPos + 12 + ListSize;

	/*
		�ȉ��ɒ񎦂��������ꂩ�̏����ɊY������܂Ŗ������[�v�ŃX�L����
		����1�F������
		����2�F�t�@�C���̏I�[�ɂ��ǂ蒅��
		����3�F���[�v���Ŏg�p�����֐��ŃG���[���N����

	*/
	while (1) {

		//�t�@�C���|�C���^�̈ړ��Ɏ��s�����ꍇ�̓��[�v����O���
		if(!SetFilePointerEx(this->hFile,LI,NULL,FILE_BEGIN))break;

		//8�o�C�g(�`�����N��+�`�����N�T�C�Y)���̓ǂݍ��݂Ɏ��s�����ꍇ�̓��[�v����O���
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)break;
			
		//8�o�C�g�ǂ߂Ă��Ȃ���΃��[�v����O���
		if(rs != 8)break;
		
		//�w�肳�ꂽ�`�����N�Ȃ��CKFound = TRUE �Ƃ��� ���[�v���O���(����)
		if(!_strnicmp(ChunkText,ChunkHeader.Name,4)){
			bFound = true;
			break;
		}
		/*
			�����[�v�ł̓ǂݍ��݈ʒu�����̎菇�Ōv�Z
			�菇1.�`�����N�T�C�Y����Ȃ��1���Z���ċ����ɂ���
			�菇2.�菇1�̌��� + 8������B
			�菇3.LI.QuadPart�Ɏ菇2�̌��ʂ����Z

		*/
	
		//chunksize����Ȃ��1���Z���ċ����ɂ���
		if (ChunkHeader.Size % 2)ChunkHeader.Size++;

		//LI.QuadPart�Ɏ��̃`�����N�ւ̃I�t�Z�b�g�����Z����
		LI.QuadPart += ChunkHeader.Size + 8;

		if (LI.QuadPart >= QuitPos) break;
	}

	if (bFound == false) return false;
	if (lpChunkPos) *lpChunkPos = (UINT32)LI.QuadPart;
	return true;
}
	
//�w�肵��LIST�`�����N�ɂ���T�u�`�����N��񋓂���
bool CHSRIFFReader::EnumListSubChunk(char *lpType, LPFHSRIFFENUMPROC pProc) {

	if (!pProc) return false;

	LARGE_INTEGER LI;

	//�X�^�[�g�ʒu
	UINT32 ListPos , ListSize , QuitPos;

	if(this->GetListChunkPos(lpType ,&ListPos) == false) return false;
	if(this->GetListChunkSize(lpType ,&ListSize) == false) return false;

	ListSize -= 4;
	LI.QuadPart = ListPos + 12;

	QuitPos = ListPos + 12 + ListSize;

	THSRIFF_CHUNKHEADER ChunkHeader;
	bool bFound = false;
	DWORD	rs;
	char ChunkType[5] = { 0 };
	THSRIFF_ENUMCHUNKINFO info;

	/*
		�ȉ��ɒ񎦂��������ꂩ�̏����ɊY������܂Ŗ������[�v�ŃX�L����
		����1�F�t�@�C���̏I�[�ɂ��ǂ蒅��
		����2�F���[�v���Ŏg�p�����֐��ŃG���[���N����

	*/
	while (1) {

		memset(&info, 0, sizeof(THSRIFF_ENUMCHUNKINFO));

		lstrcpyA(info.Type , lpType);


		//�t�@�C���|�C���^�̈ړ��Ɏ��s�����ꍇ�̓��[�v����O���
		if(!SetFilePointerEx(this->hFile,LI,NULL,FILE_BEGIN))break;

		//8�o�C�g(�`�����N��+�`�����N�T�C�Y)���̓ǂݍ��݂Ɏ��s�����ꍇ�̓��[�v����O���
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)break;
			
		//8�o�C�g�ǂ߂Ă��Ȃ���΃��[�v����O���
		if(rs != 8)break;
		
		info.Position = (UINT32)LI.QuadPart;
		info.Size = ChunkHeader.Size;

		memcpy(info.Name , ChunkHeader.Name ,  4);


		//�R�[���o�b�N�֐����Ă�ŁAfalse���Ԃ��Ă�����񋓂��I������
		if (pProc(this , &info) == false) break;


		/*
			�����[�v�ł̓ǂݍ��݈ʒu�����̎菇�Ōv�Z
			�菇1.�`�����N�T�C�Y����Ȃ��1���Z���ċ����ɂ���
			�菇2.�菇1�̌��� + 8������B
			�菇3.LI.QuadPart�Ɏ菇2�̌��ʂ����Z

		*/
	
		//chunksize����Ȃ��1���Z���ċ����ɂ���
		if (ChunkHeader.Size % 2)ChunkHeader.Size++;

		//LI.QuadPart�Ɏ��̃`�����N�ւ̃I�t�Z�b�g�����Z����
		LI.QuadPart += ChunkHeader.Size + 8;

		if (LI.QuadPart >= QuitPos) break;

	}

	return true;
}

//�w�肵��LIST�`�����N�ɂ���w�肵���T�u�`�����N�̃T�C�Y���擾
bool CHSRIFFReader::GetListSubChunkSize(char *lpType, char *lpChunkName, UINT32 *lpChunkSize) {

	UINT32 ChunkPos;
	if (!lpChunkSize)return false;

	if (this->GetListSubChunkPos(lpType , lpChunkName, &ChunkPos)) {
		LARGE_INTEGER li;
		DWORD rs;
		THSRIFF_CHUNKHEADER ChunkHeader;
		li.QuadPart = ChunkPos;

		//�t�@�C���|�C���^�̈ړ��Ɏ��s�����ꍇ��false��Ԃ�
		if(!SetFilePointerEx(this->hFile,li,NULL,FILE_BEGIN))return false;

		//8�o�C�g(�`�����N��+�`�����N�T�C�Y)���̓ǂݍ��݂Ɏ��s�����ꍇ��false��Ԃ�
		if(ReadFile(this->hFile,&ChunkHeader,8,&rs,NULL)==NULL)return false;

		*lpChunkSize = ChunkHeader.Size;

		return true;
	}
	return false;
}
	
//�w�肵��LIST�`�����N�ɂ���w�肵���T�u�`�����N���擾
UINT32 CHSRIFFReader::GetListSubChunkData(char *lpType, char *lpChunkName, void *lpValueData, UINT32 ValueDataSize) {


	UINT32 ChunkPos;
	UINT32 ChunkSize;

	UINT32 LoadSize;
	DWORD readSize;

	if (!lpValueData)return 0;

	if (this->GetListSubChunkPos(lpType , lpChunkName, &ChunkPos)) {
		if (this->GetListSubChunkSize(lpType , lpChunkName, &ChunkSize)) {

			LoadSize = (ChunkSize < ValueDataSize) ? ChunkSize : ValueDataSize;

			LARGE_INTEGER li;
			li.QuadPart = ChunkPos + 8;

			//�t�@�C���|�C���^�̈ړ��Ɏ��s�����ꍇ��false��Ԃ�
			if(!SetFilePointerEx(this->hFile,li,NULL,FILE_BEGIN))return 0;

			//8�o�C�g(�`�����N��+�`�����N�T�C�Y)���̓ǂݍ��݂Ɏ��s�����ꍇ��false��Ԃ�
			if(ReadFile(this->hFile,lpValueData,LoadSize,&readSize,NULL)==NULL)return 0;

			return readSize;
		}
	}
	return 0;
}
