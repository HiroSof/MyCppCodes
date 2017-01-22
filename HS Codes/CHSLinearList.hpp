//更新日時
//----------
//2013/08/23
//2013/08/24
//2013/08/25
//2013/08/26
//2013/08/31
//----------
//2013/09/01
//2014/04/14 ・・・ 配列へエクスポート機能追加
//2014/07/10 ・・・ IsEmpty関数追加
//2014/12/07 ・・・GetListData関数追加
#ifndef _CHSLinearList_HPP_
#define _CHSLinearList_HPP_

#define  HSLinearList_DefaultFirstSerialNumber  1

//HSLinearList_Data構造体
template<typename T> struct HSLinearList_Data{
	T Data;
	int SerialNumber;
	HSLinearList_Data *lpBack;
	HSLinearList_Data *lpNext;
};

//CHSLinearListクラス
template<typename T = int> class CHSLinearList
{

private:
	HSLinearList_Data<T> *lpData;
	int NextSerialNumber;
	int FirstSerialNumber;
	HSLinearList_Data<T>* Alloc();
	HSLinearList_Data<T>* Scan(int Idx , bool bIdxIsSerialNumber = false);

public:
	CHSLinearList(int FirstSerialNumber = HSLinearList_DefaultFirstSerialNumber);
	~CHSLinearList();

	int Append(T AddData);
	int GetSerialNumber(int idx);
	int GetIndex(int serialnumber);
	int GetNums(void);
	int GetMinSerialNumber(void);
	int GetMaxSerialNumber(void);
	bool IsEmpty(void);
	

	bool SetReAllocSerialNumber(void);
	bool Delete(int Idx ,  T *lpBackup = nullptr,bool bIdxIsSerialNumber = false);
	bool AllDelete(void);
	bool Get(int Idx , T *lpOutData , bool bIdxIsSerialNumber = false);
	bool Set(int Idx , T InData , bool bIdxIsSerialNumber = false);
	bool IsExistSerialNumber(int serialnumber);


	int BeforeInsert(unsigned int InsertPos , T Data);
	int AfterInsert(unsigned int InsertPos , T Data);

	bool Swap(int idx1 , int idx2);
	bool Reverse(int startidx , int endidx);
	bool ReverseAll(void);

	bool SetSerialNumber(int idx , int NewSerialNumber);
	bool Clone(CHSLinearList<T> *lpList, bool bTakeOverSerialNumber = false);
	bool CreateCloneInstance(CHSLinearList<T> **lplpList , bool bTakeOverSerialNumber = false);
	bool FreeCloneInstance(CHSLinearList<T> **lplpList);

	bool ExportArray(T *lpArray);

	bool Move(int OldIdx , int NewIdx);


	T* GetDataPointer(int Idx , bool bIdxIsSerialNumber = false);

	HSLinearList_Data<T>* GetListData(unsigned int idx = 0);

	//関数呼び出し演算子のオーバーロード
	T operator()(int idx , bool bIdxIsSerialNumber = false);

	//添え字　演算子のオーバーロード
	T operator[](int idx);

	//<<演算子のオーバーロード
	CHSLinearList<T>& operator<< (T AddData);

	//>>演算子のオーバーロード
	CHSLinearList<T>& operator>> (T &OutData);

	//型キャスト演算子（int) のオーバーロード
	operator int();

	//代入演算子のオーバーロード
	void operator = (T Data);
};

template<typename T>  CHSLinearList<T>::CHSLinearList(int FirstSerialNumber)
{
	this->NextSerialNumber = this->FirstSerialNumber = FirstSerialNumber;
	this->lpData = nullptr;
}

template<typename T> CHSLinearList<T>::~CHSLinearList()
{
	this->AllDelete();
}

template<typename T> HSLinearList_Data<T>* CHSLinearList<T>::Alloc(){

	HSLinearList_Data<T> *lpNewData = nullptr;
	HSLinearList_Data<T> *lpClassData = this->lpData;
	
	lpNewData = new HSLinearList_Data<T>;
	lpNewData->lpBack = lpNewData->lpNext = nullptr;
	lpNewData->SerialNumber = this->NextSerialNumber;
	this->NextSerialNumber++;
	while(this->IsExistSerialNumber(this->NextSerialNumber)) this->NextSerialNumber++;
	if(this->lpData == nullptr){
		this->lpData = lpNewData;

	}else{
		while(lpClassData->lpNext != nullptr) lpClassData = lpClassData->lpNext;

		lpClassData->lpNext = lpNewData;
		lpNewData->lpBack = lpClassData;
	}


	return lpNewData;

}



template<typename T> int CHSLinearList<T>::Append(T AddData){
	HSLinearList_Data<T> *lpListData;
	lpListData = this->Alloc();
	lpListData->Data = AddData;
	return lpListData->SerialNumber;
}


template<typename T> HSLinearList_Data<T>* CHSLinearList<T>::Scan(int Idx , bool bIdxIsSerialNumber){
	if((Idx < 0) && (bIdxIsSerialNumber)) return nullptr;
	if(this->lpData == nullptr)return nullptr;
	HSLinearList_Data<T>* lpTargetData = this->lpData;

	if(bIdxIsSerialNumber){
		while(lpTargetData != nullptr){
			if(lpTargetData->SerialNumber == Idx) break;
			lpTargetData = lpTargetData->lpNext;
		}
	}else{

		if(Idx >= 0){
			for (int i = 0; ((i < Idx) && (lpTargetData != nullptr)); i++)
				lpTargetData = lpTargetData->lpNext;
		}else{
			Idx *= -1;
			Idx--;

			while(lpTargetData->lpNext != nullptr) 
				lpTargetData = lpTargetData->lpNext;

			for (int i = 0; ((i < Idx) && (lpTargetData != nullptr)); i++)
				lpTargetData = lpTargetData->lpBack;
		}
	}
	return lpTargetData;
}

template<typename T> bool CHSLinearList<T>::Delete(int Idx ,  T *lpBackup , bool bIdxIsSerialNumber){
	if(this->lpData == nullptr)return false;
	HSLinearList_Data<T>* lpTargetData;

	lpTargetData = this->Scan(Idx , bIdxIsSerialNumber);
	
	if(lpTargetData == nullptr) return false;
	
	if(lpBackup != nullptr) *lpBackup = lpTargetData->Data;
	
	if(lpTargetData->lpBack != nullptr) lpTargetData->lpBack->lpNext = lpTargetData->lpNext;
	
	if(lpTargetData->lpNext != nullptr) lpTargetData->lpNext->lpBack = lpTargetData->lpBack;
	
	if(lpTargetData == this->lpData) this->lpData = this->lpData->lpNext;
	
	delete lpTargetData;
	return true;

}


template<typename T> bool CHSLinearList<T>::AllDelete(void){
	if(this->lpData == nullptr)return false;
	HSLinearList_Data<T>* lpClassData = this->lpData;

	while(lpClassData->lpNext) lpClassData = lpClassData->lpNext;

	//前のデータのアドレス
	HSLinearList_Data<T>* oldad = nullptr;
	//破棄しながら前のデータに移動する
	do{
		oldad = lpClassData->lpBack;
		delete lpClassData;
	}while(lpClassData = oldad);
	this->lpData = nullptr;
	this->NextSerialNumber = this->FirstSerialNumber;
	return true;
}

template<typename T> bool CHSLinearList<T>::Get(int Idx , T *lpOutData , bool bIdxIsSerialNumber){
	if(lpOutData == nullptr) return false;
	if(this->lpData == nullptr)return false;
	HSLinearList_Data<T>* lpTargetData;
	lpTargetData = this->Scan(Idx , bIdxIsSerialNumber);
	if(lpTargetData == nullptr) return false;
	*lpOutData = lpTargetData->Data;
	return true;

}

template<typename T> bool CHSLinearList<T>::Set(int Idx , T InData , bool bIdxIsSerialNumber){
	HSLinearList_Data<T>* lpTargetData;
	lpTargetData = this->Scan(Idx , bIdxIsSerialNumber);
	if(lpTargetData == nullptr) return false;
	lpTargetData->Data = InData;
	return true;

}

template<typename T> int CHSLinearList<T>::GetNums(void){
	if(this->lpData == nullptr)return 0;
	HSLinearList_Data<T>* lpClassData = this->lpData;
	int nums = 0;
	while(lpClassData != nullptr){
		nums++;
		lpClassData = lpClassData->lpNext;
	}
	return nums;
}

template<typename T> bool CHSLinearList<T>::SetReAllocSerialNumber(void){
	if(this->lpData == nullptr)return 0;
	HSLinearList_Data<T>* lpClassData = this->lpData;
	int nums = this->FirstSerialNumber;
	while(lpClassData != nullptr){
		lpClassData->SerialNumber = nums;
		nums++;
		lpClassData = lpClassData->lpNext;
	}
	this->NextSerialNumber = nums;
	return true;
}

template<typename T> int CHSLinearList<T>::GetSerialNumber(int idx){
	if(this->lpData == nullptr)return this->FirstSerialNumber - 1;
	HSLinearList_Data<T>* lpTargetData;
	lpTargetData = this->Scan(idx , false);
	if(lpTargetData == nullptr) return this->FirstSerialNumber - 1;
	return lpTargetData->SerialNumber;

}
template<typename T> int CHSLinearList<T>::GetIndex(int serialnumber){
	if(serialnumber < 0) return -1;
	if(this->lpData == nullptr) return - 1;
	HSLinearList_Data<T>* lpTargetData = this->lpData;

	int idx = 0;
	while(lpTargetData != nullptr){
		if(lpTargetData->SerialNumber == serialnumber) break;
		idx++;
		lpTargetData = lpTargetData->lpNext;
	}
	if(lpTargetData == nullptr) return -1;
	return  idx;
}

template<typename T> int CHSLinearList<T>::GetMinSerialNumber(void){
	if(this->lpData == nullptr) return this->FirstSerialNumber - 1;
	HSLinearList_Data<T>* lpTargetData = this->lpData;	
	int fsn = lpTargetData->SerialNumber;
	while(lpTargetData = lpTargetData->lpNext)
		if(fsn > lpTargetData->SerialNumber)
			fsn = lpTargetData->SerialNumber;
	return fsn;
}

template<typename T> int CHSLinearList<T>::GetMaxSerialNumber(void){
	if(this->lpData == nullptr) return this->FirstSerialNumber - 1;
	HSLinearList_Data<T>* lpTargetData = this->lpData;	
	int lsn = lpTargetData->SerialNumber;
	while(lpTargetData = lpTargetData->lpNext)
		if(lsn < lpTargetData->SerialNumber)
			lsn = lpTargetData->SerialNumber;
	return lsn;
}
template<typename T> bool CHSLinearList<T>::IsEmpty(void) {
	if(this->lpData == nullptr) return true;
	return false;
}

template<typename T> bool CHSLinearList<T>::IsExistSerialNumber(int serialnumber){
	if(this->lpData == nullptr) return false;
	HSLinearList_Data<T>* lpTargetData;
	lpTargetData = this->Scan(serialnumber , true);
	return (lpTargetData == nullptr) ? false : true;
}

template<typename T> T* CHSLinearList<T>::GetDataPointer(int Idx , bool bIdxIsSerialNumber = false){
	if(this->lpData == nullptr) return false;
	HSLinearList_Data<T>* lpTargetData;
	lpTargetData = this->Scan(Idx , bIdxIsSerialNumber);
	if(lpTargetData == nullptr) return nullptr;
	else return &lpTargetData->Data;
}



template<typename T> int CHSLinearList<T>:: BeforeInsert(unsigned int InsertPos , T Data){

	if(this->GetNums() == 0){
		if(InsertPos == 0) return  this->Append(Data);
		else return this->FirstSerialNumber - 1;
	}
	HSLinearList_Data<T> *lpAdminData;
	lpAdminData = this->Scan(InsertPos);


	if(!lpAdminData) return  this->FirstSerialNumber - 1;
	

	HSLinearList_Data<T> *lpNewData;
	lpNewData = new HSLinearList_Data<T>;
	lpNewData->lpBack = lpNewData->lpNext = nullptr;
	lpNewData->SerialNumber = this->NextSerialNumber;
	lpNewData->Data = Data;

	this->NextSerialNumber++;
	while(this->IsExistSerialNumber(this->NextSerialNumber)) this->NextSerialNumber++;

	if(InsertPos == 0){
		this->lpData = lpNewData;
	}else{
		lpNewData->lpBack = lpAdminData->lpBack;
		lpNewData->lpBack->lpNext = lpNewData;
	}
	lpNewData->lpNext = lpAdminData;
	lpAdminData->lpBack = lpNewData;

	return lpNewData->SerialNumber;

}

template<typename T> int CHSLinearList<T>::AfterInsert(unsigned int InsertPos , T Data){
	return this->BeforeInsert(InsertPos + 1 ,Data); 
}

template<typename T> bool CHSLinearList<T>:: Swap(int idx1 , int idx2){
	HSLinearList_Data<T> *lpData1;
	HSLinearList_Data<T> *lpData2;
	HSLinearList_Data<T> Tmp;


	lpData1 = this->Scan(idx1);
	lpData2 = this->Scan(idx2);

	if(!lpData1 || !lpData2) return false;

	Tmp = *lpData1;

	lpData1->Data = lpData2->Data;
	lpData1->SerialNumber = lpData2->SerialNumber;

	lpData2->Data = Tmp.Data;
	lpData2->SerialNumber = Tmp.SerialNumber;

	return true;
}
template<typename T> bool CHSLinearList<T>:: Reverse(int startidx , int endidx){

	if(startidx > endidx) return false;

	if((!this->Scan(startidx)) || (!this->Scan(endidx))) return false;

	int nums = endidx - startidx + 1;

	int idx1 = startidx;
	int idx2 = endidx;

	for (int i = 0; i < (nums/2); i++)
	{
		this->Swap(idx1 , idx2);
		idx1++;
		idx2--;
	}

	return true;
}
template<typename T> bool CHSLinearList<T>:: ReverseAll(void){
	if(this->GetNums() == 0) return false;
	return this->Reverse(0 , this->GetNums() - 1);
}


template<typename T> bool  CHSLinearList<T>::SetSerialNumber(int idx , int NewSerialNumber){
	if((!this->Scan(idx)) || (this->IsExistSerialNumber(NewSerialNumber))) return false;
	if(NewSerialNumber < this->FirstSerialNumber) return false;
	HSLinearList_Data<T> *lpAdminData;
	lpAdminData = this->Scan(idx);
	lpAdminData->SerialNumber = NewSerialNumber;
	this->NextSerialNumber = this->GetMaxSerialNumber() + 1;
	return true;
}
template<typename T> bool  CHSLinearList<T>::Clone(CHSLinearList<T> *lpList , bool bTakeOverSerialNumber){

	if(!lpList || (this->GetNums() == 0)) return false;


	lpList->AllDelete();

	for (int i = 0; i < this->GetNums(); i++)
	{
		T Data;
		this->Get(i , &Data);
		lpList->Append(Data);
		if(bTakeOverSerialNumber) lpList->SetSerialNumber( i , this->GetSerialNumber(i));
	}
	return true;
}
template<typename T> bool  CHSLinearList<T>::CreateCloneInstance(CHSLinearList<T> **lplpList , bool bTakeOverSerialNumber){

	if(!lplpList || (this->GetNums() == 0)) return false;

	*lplpList = new CHSLinearList<T>(this->FirstSerialNumber);

	return this->Clone(*lplpList , bTakeOverSerialNumber);
}
template<typename T> bool  CHSLinearList<T>::FreeCloneInstance(CHSLinearList<T> **lplpList){
	if(!lplpList) return false;
	if(!(*lplpList)) return false;

	delete *lplpList;
	*lplpList = nullptr;

	return true;

}

template<typename T> bool  CHSLinearList<T>::ExportArray(T *lpArray) {

	if (lpArray == NULL) return false;
	if (this->lpData == NULL)return false;

	HSLinearList_Data<T>* lpTargetData = this->lpData;

	T *lpExport = lpArray;
	do {
		*lpExport  = lpTargetData->Data;
		lpExport++;
		lpTargetData = lpTargetData->lpNext;
	} while (lpTargetData);
	return true;
}

template<typename T> bool  CHSLinearList<T>:: Move(int OldIdx , int NewIdx){
	if(this->GetNums() == 0) return false;
	if((this->GetNums() <=OldIdx) ||((this->GetNums() - 1) <=NewIdx)) return false;


	T Data;
	int SerialNumber = this->GetSerialNumber(OldIdx);

	this->Delete(OldIdx , &Data);

	this->BeforeInsert(NewIdx , Data);

	this->SetSerialNumber(NewIdx , SerialNumber);


	return true;
}
template<typename T> HSLinearList_Data<T>*  CHSLinearList<T>::GetListData(unsigned int idx) {
	HSLinearList_Data<T> *pRet = this->lpData;
	if (pRet == nullptr)return nullptr;
	for (unsigned int i = 0; i < idx; i++) {
		if (pRet == nullptr) break;
		pRet = pRet->lpNext;
	}
	return pRet;
}

//演算子のオーバーロード
template<typename T> T CHSLinearList<T>::operator()(int idx , bool bIdxIsSerialNumber){
	T Data  = { 0 };
	this->Get(idx , &Data , bIdxIsSerialNumber);
	return Data;
}


	//添え字　演算子のオーバーロード
template<typename T> T CHSLinearList<T>:: operator[](int idx) {
	T Data  = { 0 };
	this->Get(idx , &Data , false);
	return Data;	
}


template<typename T> CHSLinearList<T>& CHSLinearList<T>::operator<< (T AddData){
	this->Append(AddData);
	return *this;
}

template<typename T> CHSLinearList<T>& CHSLinearList<T>::operator>> (T &OutData){
	this->Delete(-1 , &OutData);
	return *this;
}

template<typename T> CHSLinearList<T>::operator int(){
	return this->GetNums();
}

template<typename T> void CHSLinearList<T>::operator=(T Data){
	this->AllDelete();
	this->Add(Data);
}

#endif /* _CHSLinearList_HPP_ */
