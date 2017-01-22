#include "CHSSHABASE.h"

#ifndef __CHSSHABASE__
#define __CHSSHABASE__


CHSSHABASE::CHSSHABASE(int elements , const unsigned __int32 *lpDefaultMessageDijest , unsigned __int32 *lpMessageDijest){
	this->NumberOfMessageDijestElements = elements;
	this->lpDefaultMessageDijest = lpDefaultMessageDijest;
	this->lpMessageDijest = lpMessageDijest;
	this->Initialization();
}

void CHSSHABASE::New(void){
	this->Initialization();
}

void CHSSHABASE::Initialization(void){

	memcpy(this->lpMessageDijest , this->lpDefaultMessageDijest , 
		sizeof(unsigned __int32) * this->NumberOfMessageDijestElements);

	this->DataSize = 0;
	memset(this->TempData , NULL , sizeof(this->TempData));
	this->TmpDataIdx = 0;
	this->QuitPublish = false;
}



/*
	������̃T�C�Y���o�C�g�P�ʂŎ擾
*/
int CHSSHABASE::StrlenByte(char *lpStringA){
	int size = 0;
	if(!lpStringA)return 0;
	while(*lpStringA){
		size++;
		lpStringA++;
	}
	return size * sizeof(char);
}
int CHSSHABASE::StrlenByte(unsigned char *lpStringA){
	int size = 0;
	if(!lpStringA)return 0;
	while(*lpStringA){
		size++;
		lpStringA++;
	}
	return size * sizeof(unsigned char);
}

int CHSSHABASE::StrlenByte(wchar_t *lpStringW){
	int size = 0;
	if(!lpStringW)return 0;
	while(*lpStringW){
		size++;
		lpStringW++;
	}
	return size * sizeof(wchar_t); 
}


/*
	�n�b�V���Ώ̂̃f�[�^�̒ǉ�
*/
CHSSHA_ERROR	CHSSHABASE::Insert(char *lpStringA){
	return this->Insert(lpStringA , this->StrlenByte(lpStringA));
}
CHSSHA_ERROR	CHSSHABASE::Insert(unsigned char *lpStringA){
	return this->Insert(lpStringA , this->StrlenByte(lpStringA));
}

CHSSHA_ERROR	CHSSHABASE::Insert(wchar_t *lpStringW) {
	return this->Insert(lpStringW , this->StrlenByte(lpStringW));
}
CHSSHA_ERROR	CHSSHABASE::Insert(void *lpData , __int64  size){
	__int8 *lpByteData;

	//tmpidx�E�E�E�ꎞ�f�[�^�ۑ��̈�̈ʒu
	__int64 tmpidx= this->TmpDataIdx;
	
	//�G���[�`�F�b�N
	if(!lpData  || (size <= 0))return CHSSHA_ERROR_FALSE;
	else if( this->QuitPublish ) return CHSSHA_ERROR_QUITPABLISHED;


	//�f�[�^�̃|�C���^���
	lpByteData = (__int8*)lpData;

	if(tmpidx + size >= 64){
		//�ꎞ�f�[�^�̈�� �����œn���ꂽ�f�[�^�T�C�Y�� 64�o�C�g�𒴂��Ă������̏���

		__int64 tmpaddfirst = 64 - tmpidx;
		__int64 NumBlock , NumRest;

		//�ꎞ�f�[�^�̈�� �n���ꂽ�f�[�^�̂��� tmpaddfirst �o�C�g�ǉ�
		memcpy(this->TempData + tmpidx , lpByteData , (__int32)tmpaddfirst);

		//�n�b�V���u���b�N�����s
		this->HashBlock();

		//tmpidx�� 0�ɂ���
		tmpidx = 0; 

		//lpByteData�̎����ʒu�� tmpaddfirst �o�C�g���炷
		lpByteData += tmpaddfirst;

		//�ǉ��f�[�^ �̂���u���b�N��
		NumBlock = (size - tmpaddfirst) / 64;

		//�ꎞ�̈�ɕۑ����ׂ��̂���f�[�^�T�C�Y
		NumRest = (size - tmpaddfirst) % 64;

		//�ǉ��f�[�^ �̂���u���b�N ����
		for(int i = 0 ;  i < NumBlock ; i++){

			//64�o�C�g���ꎞ�f�[�^�̈�ɓ����
			memcpy(this->TempData , lpByteData + 64*i  , 64);

			//�n�b�V���u���b�N�����s
			this->HashBlock();
		}

		//�ꎞ�̈�ɕۑ����ׂ��̂���f�[�^ ���R�s�[
		memcpy(this->TempData , lpByteData + 64*NumBlock  , (__int32)NumRest);

	}else{
		
		//�ꎞ�f�[�^�̈�� �����œn���ꂽ�f�[�^�T�C�Y�� 64�o�C�g�����̎��̏���

		//�ꎞ�f�[�^�̈�ɒǉ�
		memcpy(this->TempData + tmpidx , lpByteData , (__int32)size);
	}

	//�ꎞ�f�[�^�̈�̐V�����ʒu
	this->TmpDataIdx = (this->TmpDataIdx + size) % 64;
	
	return CHSSHA_ERROR_TRUE;
}


/*
	�n�b�V���l���s
*/
CHSSHA_ERROR	CHSSHABASE::Publish(char *lpStringA){
	return this->Publish(lpStringA , this->StrlenByte(lpStringA));
}
CHSSHA_ERROR	CHSSHABASE::Publish(unsigned char *lpStringA){
	return this->Publish(lpStringA , this->StrlenByte(lpStringA));
}
CHSSHA_ERROR	CHSSHABASE::Publish(wchar_t *lpStringW) {
	return this->Publish(lpStringW , this->StrlenByte(lpStringW));
}


CHSSHA_ERROR	CHSSHABASE::Publish(void *InputData , __int64  Size){
	__int64 bitsize , revbitsize = 0;
	__int8 *lpbitsize,*lprevbitsize;

	//���ɔ��s�ς݂̏ꍇ�̓G���[��Ԃ�
	if(this->QuitPublish)return CHSSHA_ERROR_QUITPABLISHED;
	
	//�ŏI�ǉ�
	this->Insert(InputData,Size);

	//�f�[�^�T�C�Y(�r�b�g�P��)
	bitsize = (this->DataSize + this->TmpDataIdx) * 8;

	//�f�[�^�T�C�Y��0�̎��̓G���[��Ԃ�
	if(bitsize == 0) return CHSSHA_ERROR_FALSE;

	//�ꎞ�f�[�^�̈�̍Ō�̈ʒu��0x80����
	this->TempData[this->TmpDataIdx] = (unsigned __int8)0x80;
	
	//0x80����������ׂ�0�ɂ���
	memset(this->TempData + this->TmpDataIdx + 1 , 0 , 64 - (this->TmpDataIdx + 1 ));

	//0x80���������ʒu�� 56 �ȏゾ������
	//�n�b�V���u���b�N�����s���� �ŏ���56�o�C�g��0�ɂ���
	if(this->TmpDataIdx >= 56){
		this->HashBlock();
		memset(this->TempData  , 0 , 56);
	}

	//�f�[�^�T�C�Y ���o�C�g�P�ʂŋt���ɂ���
	lpbitsize = (__int8*)&bitsize;
	lprevbitsize = (__int8*)&revbitsize;
	for(int i = 0 ; i < 8 ; i++)*(lprevbitsize + 7 - i) = *(lpbitsize + i);

	//�f�[�^�T�C�Y �� �R�s�[
	memcpy(this->TempData + 56 , lprevbitsize , 8);

	//�n�b�V���u���b�N�����s
	this->HashBlock();

	this->QuitPublish = true;
	return CHSSHA_ERROR_TRUE;
}

int CHSSHABASE::Get(char *lpTextA , int size , CHSSHA_TOSTRFLAG  flag, char split_char){
	char *lpText;
	int idx;

	int bsize;
	bsize = this->NumberOfMessageDijestElements * 9 + 1;
	lpText = new char[bsize];

	if(!this->QuitPublish)return 0;

	switch (flag){
		case CHSSHA_TOSTRFLAG_SMALL:
			for(idx = 0 ; idx < this->NumberOfMessageDijestElements ; idx++){
				if(idx){
					sprintf_s(lpText , bsize , "%s%08x" ,lpText,  this->lpMessageDijest[idx]);
				}else{
					sprintf_s(lpText , bsize , "%08x" ,  this->lpMessageDijest[0]);
				}
			}
			break;
		case CHSSHA_TOSTRFLAG_SMALL_DELIMITER:
			for(idx = 0 ; idx < this->NumberOfMessageDijestElements ; idx++){
				if(idx){
					sprintf_s(lpText , bsize , "%s%08x%c" ,lpText,  this->lpMessageDijest[idx] , split_char);
				}else{
					sprintf_s(lpText , bsize , "%08x%c" ,  this->lpMessageDijest[0] , split_char);
				}
			}
			lpText[this->StrlenByte(lpText)/sizeof(char) - 1] = NULL;
			break;
		case CHSSHA_TOSTRFLAG_BIG:
			for(idx = 0 ; idx < this->NumberOfMessageDijestElements ; idx++){
				if(idx){
					sprintf_s(lpText , bsize , "%s%08X" ,lpText,  this->lpMessageDijest[idx]);
				}else{
					sprintf_s(lpText , bsize , "%08X" ,  this->lpMessageDijest[0]);
				}
			}
			break;
		case CHSSHA_TOSTRFLAG_BIG_DELIMITER:
			for(idx = 0 ; idx < this->NumberOfMessageDijestElements ; idx++){
				if(idx){
					sprintf_s(lpText , bsize , "%s%08X%c" ,lpText,  this->lpMessageDijest[idx] , split_char);
				}else{
					sprintf_s(lpText , bsize , "%08X%c" ,  this->lpMessageDijest[0] , split_char);
				}
			}
			lpText[this->StrlenByte(lpText)/sizeof(char) - 1] = NULL;
			break;
	}
	int isize = this->StrlenByte(lpText);
	if(lpTextA && (isize + 1 <= size)) sprintf_s(lpTextA , size ,  "%s" , lpText);
	delete []lpText;
	return isize;
}


int CHSSHABASE::Get(wchar_t *lpTextW , int size , CHSSHA_TOSTRFLAG  flag, wchar_t split_char){
	wchar_t *lpText;
	int idx;
	if(!this->QuitPublish)return 0;
	int bsize;
	bsize = this->NumberOfMessageDijestElements * 9 + 1;
	lpText = new wchar_t[bsize];

	switch (flag){
		case CHSSHA_TOSTRFLAG_SMALL:
			for(idx = 0 ; idx < this->NumberOfMessageDijestElements ; idx++){
				if(idx){
					swprintf_s(lpText , bsize ,L"%s%08x" ,lpText,  this->lpMessageDijest[idx]);
				}else{
					swprintf_s(lpText , bsize ,L"%08x" ,  this->lpMessageDijest[0]);
				}
			}
			break;
		case CHSSHA_TOSTRFLAG_SMALL_DELIMITER:
			for(idx = 0 ; idx < this->NumberOfMessageDijestElements ; idx++){
				if(idx){
					swprintf_s(lpText , bsize ,L"%s%08x%c" ,lpText,  this->lpMessageDijest[idx] , split_char);
				}else{
					swprintf_s(lpText , bsize ,L"%08x%c" ,  this->lpMessageDijest[0] , split_char);
				}
			}
			lpText[this->StrlenByte(lpText)/sizeof(wchar_t) - 1] = NULL;
			break;
		case CHSSHA_TOSTRFLAG_BIG:
			for(idx = 0 ; idx < this->NumberOfMessageDijestElements ; idx++){
				if(idx){
					swprintf_s(lpText , bsize ,L"%s%08X" ,lpText,  this->lpMessageDijest[idx]);
				}else{
					swprintf_s(lpText , bsize ,L"%08X" ,  this->lpMessageDijest[0]);
				}
			}
			break;
		case CHSSHA_TOSTRFLAG_BIG_DELIMITER:
			for(idx = 0 ; idx < this->NumberOfMessageDijestElements ; idx++){
				if(idx){
					swprintf_s(lpText , bsize ,L"%s%08X%c" ,lpText,  this->lpMessageDijest[idx] , split_char);
				}else{
					swprintf_s(lpText , bsize ,L"%08X%c" ,  this->lpMessageDijest[0] , split_char);
				}
			}
			lpText[this->StrlenByte(lpText)/sizeof(wchar_t) - 1] = NULL;
			break;
	}
	int isize = this->StrlenByte(lpText)/sizeof(wchar_t);
	if(lpTextW && (isize + 1 <= size)) swprintf_s(lpTextW , size ,  L"%s" , lpText);
	delete[] lpText;
	return isize;
}

int CHSSHABASE::Get(unsigned __int32 *lpData){
	if(!this->QuitPublish)return 0;
	if(lpData){
		memcpy(lpData , this->lpMessageDijest ,
			sizeof(unsigned __int32) * this->NumberOfMessageDijestElements);
	}
	return this->NumberOfMessageDijestElements;
}

#endif // !__CHSSHABASE__
