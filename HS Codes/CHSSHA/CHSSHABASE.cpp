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
	文字列のサイズをバイト単位で取得
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
	ハッシュ対称のデータの追加
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

	//tmpidx・・・一時データ保存領域の位置
	__int64 tmpidx= this->TmpDataIdx;
	
	//エラーチェック
	if(!lpData  || (size <= 0))return CHSSHA_ERROR_FALSE;
	else if( this->QuitPublish ) return CHSSHA_ERROR_QUITPABLISHED;


	//データのポインタ代入
	lpByteData = (__int8*)lpData;

	if(tmpidx + size >= 64){
		//一時データ領域と 引数で渡されたデータサイズが 64バイトを超えていた時の処理

		__int64 tmpaddfirst = 64 - tmpidx;
		__int64 NumBlock , NumRest;

		//一時データ領域に 渡されたデータのうち tmpaddfirst バイト追加
		memcpy(this->TempData + tmpidx , lpByteData , (__int32)tmpaddfirst);

		//ハッシュブロックを実行
		this->HashBlock();

		//tmpidxを 0にする
		tmpidx = 0; 

		//lpByteDataの示す位置を tmpaddfirst バイトずらす
		lpByteData += tmpaddfirst;

		//追加データ のこりブロック数
		NumBlock = (size - tmpaddfirst) / 64;

		//一時領域に保存すべきのこりデータサイズ
		NumRest = (size - tmpaddfirst) % 64;

		//追加データ のこりブロック 処理
		for(int i = 0 ;  i < NumBlock ; i++){

			//64バイトずつ一時データ領域に入れる
			memcpy(this->TempData , lpByteData + 64*i  , 64);

			//ハッシュブロックを実行
			this->HashBlock();
		}

		//一時領域に保存すべきのこりデータ をコピー
		memcpy(this->TempData , lpByteData + 64*NumBlock  , (__int32)NumRest);

	}else{
		
		//一時データ領域と 引数で渡されたデータサイズが 64バイト未満の時の処理

		//一時データ領域に追加
		memcpy(this->TempData + tmpidx , lpByteData , (__int32)size);
	}

	//一時データ領域の新しい位置
	this->TmpDataIdx = (this->TmpDataIdx + size) % 64;
	
	return CHSSHA_ERROR_TRUE;
}


/*
	ハッシュ値発行
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

	//既に発行済みの場合はエラーを返す
	if(this->QuitPublish)return CHSSHA_ERROR_QUITPABLISHED;
	
	//最終追加
	this->Insert(InputData,Size);

	//データサイズ(ビット単位)
	bitsize = (this->DataSize + this->TmpDataIdx) * 8;

	//データサイズが0の時はエラーを返す
	if(bitsize == 0) return CHSSHA_ERROR_FALSE;

	//一時データ領域の最後の位置に0x80を代入
	this->TempData[this->TmpDataIdx] = (unsigned __int8)0x80;
	
	//0x80から後ろをすべて0にする
	memset(this->TempData + this->TmpDataIdx + 1 , 0 , 64 - (this->TmpDataIdx + 1 ));

	//0x80を代入した位置が 56 以上だったら
	//ハッシュブロックを実行して 最初の56バイトを0にする
	if(this->TmpDataIdx >= 56){
		this->HashBlock();
		memset(this->TempData  , 0 , 56);
	}

	//データサイズ をバイト単位で逆順にする
	lpbitsize = (__int8*)&bitsize;
	lprevbitsize = (__int8*)&revbitsize;
	for(int i = 0 ; i < 8 ; i++)*(lprevbitsize + 7 - i) = *(lpbitsize + i);

	//データサイズ を コピー
	memcpy(this->TempData + 56 , lprevbitsize , 8);

	//ハッシュブロックを実行
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
