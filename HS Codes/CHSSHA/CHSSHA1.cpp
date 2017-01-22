#include "CHSSHA1.h"

#ifndef __CHSSHA1__
#define __CHSSHA1__
//���b�Z�[�W�_�C�W�F�X�g�����l
const unsigned __int32  CHSSHA1::DefaultMessageDijest[5] = { 
	0x67452301 , 
	0xefcdab89 , 
	0x98badcfe , 
	0x10325476 , 
	0xc3d2e1f0 
}; 

CHSSHA1::CHSSHA1():CHSSHABASE(5,DefaultMessageDijest , MessageDijest){
	return;
}

/*
	�����[�e�[�g�֐�
*/
unsigned __int32 CHSSHA1::Rotate(unsigned __int32 x,int bit){
	unsigned __int32 rot = bit%32;
	return (x >> (32 - rot)) | (x << rot);
}

/*
	�萔�擾�֐�
*/
unsigned __int32 CHSSHA1::GetConstData(int constant){
	if(constant<=19)return 0x5a827999;
	else if(constant<=39)return 0x6ed9eba1;
	else if(constant<=59)return 0x8f1bbcdc;
	else if(constant<=79)return 0xca62c1d6;
	return 0;

}

/*
	�_�����Z��
*/
unsigned __int32 CHSSHA1::LogicalExp(int constant, unsigned __int32 x ,unsigned __int32 y ,unsigned __int32 z){
	if(constant<=19)return (x&y)|(~x&z);
	else if(constant<=39)return x^y^z;
	else if(constant<=59)return (x&y)|(x&z)|(y&z);
	else if(constant<=79)return x^y^z;	
	return 0;
}

//�n�b�V���u���b�N
void CHSSHA1::HashBlock(){

	unsigned __int32 md[5];		//���b�Z�[�W�_�C�W�F�X�g�ɉ��Z����l
	unsigned __int32 Data[80];	//�n�b�V���u���b�N�f�[�^


	//���̓f�[�^��16�u���b�N�ɕ���
	for(int i = 0 , j = 0 ;  i < 16 ; i++ , j+=4){
		Data[i]  = this->TempData[j] << 24;
		Data[i] |= this->TempData[j + 1] << 16;
		Data[i] |= this->TempData[j + 2] << 8;
		Data[i] |= this->TempData[j + 3];
	}

	//Data[16]�`Data[79]�܂Ń��[�e�[�g�֐����g���ăf�[�^����
	for(int i=16 ; i <= 79 ; i++){
		unsigned __int32  rotbase;
		rotbase = Data[i-3] ^ Data[i-8] ^ Data[i-14] ^ Data[i-16];
		Data[i] = this->Rotate(rotbase , 1);
	}

	//���b�Z�[�W�_�C�W�F�X�g���R�s�[
	memcpy(md , this->MessageDijest , sizeof(md));

	//Data[0] �` Data[79]�܂Ŏg�p���ă��b�Z�[�W�_�C�W�F�X�g�ɉ��Z����l���Z�o

	for(int i=0 ; i <= 79 ; i++){
		unsigned __int32 temp;
		temp = this->Rotate(md[0] , 5) + this->LogicalExp(i , md[1] , md[2] , md[3]) + md[4];
		temp+=Data[i] + this->GetConstData(i);
		md[4] = md[3];
		md[3] = md[2];
		md[2] = this->Rotate(md[1],30);
		md[1] = md[0];
		md[0] = temp;
	}

	//���b�Z�[�W�_�C�W�F�X�g�ɉ��Z
	for(int i = 0 ; i < 5 ; i++)
		this->MessageDijest[i] += md[i];

	//64byte DataSize���Z
	this->DataSize += 64;
}

#endif // !__CHSSHA1__