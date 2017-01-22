#include "CHSSHA256.h"

#ifndef __CHSSHA256__
#define __CHSSHA256__

//定数データ
const  unsigned __int32
	CHSSHA256::ConstantData[64] =  {
		0x428a2f98 , 0x71374491 , 0xb5c0fbcf , 0xe9b5dba5 ,
		0x3956c25b , 0x59f111f1 , 0x923f82a4 , 0xab1c5ed5 ,
		0xd807aa98 , 0x12835b01 , 0x243185be , 0x550c7dc3 , 
		0x72be5d74 , 0x80deb1fe , 0x9bdc06a7 , 0xc19bf174 ,
		0xe49b69c1 , 0xefbe4786 , 0x0fc19dc6 , 0x240ca1cc , 
		0x2de92c6f , 0x4a7484aa , 0x5cb0a9dc , 0x76f988da ,
		0x983e5152 , 0xa831c66d , 0xb00327c8 , 0xbf597fc7 , 
		0xc6e00bf3 , 0xd5a79147 , 0x06ca6351 , 0x14292967 ,
		0x27b70a85 , 0x2e1b2138 , 0x4d2c6dfc , 0x53380d13 , 
		0x650a7354 , 0x766a0abb , 0x81c2c92e , 0x92722c85 ,
		0xa2bfe8a1 , 0xa81a664b , 0xc24b8b70 , 0xc76c51a3 , 
		0xd192e819 , 0xd6990624 , 0xf40e3585 , 0x106aa070 ,
		0x19a4c116 , 0x1e376c08 , 0x2748774c , 0x34b0bcb5 , 
		0x391c0cb3 , 0x4ed8aa4a , 0x5b9cca4f , 0x682e6ff3 ,
		0x748f82ee , 0x78a5636f , 0x84c87814 , 0x8cc70208 , 
		0x90befffa , 0xa4506ceb , 0xbef9a3f7 , 0xc67178f2
};
//デフォルトメッセージダイジェスト
const unsigned __int32
	CHSSHA256::DefaultMessageDijest[8] = {
		0x6a09e667 , 0xbb67ae85 , 0x3c6ef372 , 0xa54ff53a , 
		0x510e527f , 0x9b05688c , 0x1f83d9ab , 0x5be0cd19
};

CHSSHA256::CHSSHA256():CHSSHABASE(8,DefaultMessageDijest , MessageDijest){
	return;
}

/*
	右ローテート関数
*/
unsigned __int32 CHSSHA256::Rotate(unsigned __int32 x,int bit){
	unsigned __int32 rot = bit%32;
	return (x >> rot) | (x << (32-rot));
}


unsigned __int32 CHSSHA256::ShiftCalc(int mode, unsigned __int32 x){
	switch(mode){
		case 0:
			return this->Rotate(x,7) ^ this->Rotate(x,18) ^ (x >> 3);
		case 1:
			return this->Rotate(x,17) ^ this->Rotate(x,19) ^ (x >> 10);

		case 2:
			return this->Rotate(x,2) ^ this->Rotate(x,13) ^ this->Rotate(x,22);

		case 3:
			return this->Rotate(x,6) ^ this->Rotate(x,11) ^ this->Rotate(x,25);
	}
	return 0;
}

unsigned __int32 CHSSHA256::LogicalExp(int mode, unsigned __int32 x ,unsigned __int32 y ,unsigned __int32 z){
		if(mode) return (x&y)^(x&z)^(y&z);
		else 	return (x & y)^(~x & z);
}



//ハッシュブロック
void CHSSHA256::HashBlock(){

	unsigned __int32 md[8];		//メッセージダイジェストに加算する値
	unsigned __int32 Data[64];	//ハッシュブロックデータ


	//入力データを16ブロックに分割
	for(int i = 0 , j = 0 ;  i < 16 ; i++ , j+=4){
		Data[i]  = this->TempData[j] << 24;
		Data[i] |= this->TempData[j + 1] << 16;
		Data[i] |= this->TempData[j + 2] << 8;
		Data[i] |= this->TempData[j + 3];
	}

	//Data[16]～Data[63]まで準備
	for(int i=16 ; i <= 63 ; i++){
		Data[i] = this->ShiftCalc(1,Data[i-2]) + this->ShiftCalc(0,Data[i-15]) + Data[i-7] + Data[i-16];
	}

	//メッセージダイジェストをコピー
	memcpy(md , this->MessageDijest , sizeof(md));

	//Data[0] ～ Data[63]まで使用してメッセージダイジェストに加算する値を算出

	for(int i=0 ; i <= 63 ; i++){
		unsigned __int32 temp[2];
		temp[0] = this->ShiftCalc(3 , md[4]) + this->LogicalExp(0 , md[4] , md[5] , md[6]);
		temp[0] += md[7] + this->ConstantData[i] + Data[i];
		temp[1] = this->ShiftCalc(2 , md[0]) + this->LogicalExp(1 , md[0], md[1], md[2]);

		md[7] = md[6];
		md[6] = md[5];
		md[5] = md[4];
		md[4] = md[3] + temp[0];
		md[3] = md[2];
		md[2] = md[1];
		md[1] = md[0];
		md[0] = temp[0] + temp[1];

	}

	//メッセージダイジェストに加算
	for(int i = 0 ; i < 8 ; i++)
		this->MessageDijest[i] += md[i];

	//64byte DataSize加算
	this->DataSize += 64;
}

#endif // !__CHSSHA256__
