//çXêVì˙éû
//2015-05-06

#ifndef __HSSTACK__
#define __HSSTACK__

template <typename T> struct THSStackData {
	T Data;
	THSStackData<T> *pNext;
};


template <typename T> class CHSStack {
private:

	THSStackData<T> *pTop;

public:
	CHSStack () {
		pTop = nullptr;
	}

	~CHSStack () {
		this->Free ();
	
	}



	void Push ( T data ) {
		THSStackData<T> *pCurrent = new THSStackData < T > ;
		pCurrent->Data = data;
		pCurrent->pNext = pTop;
		pTop = pCurrent;
	}


	bool Peek ( T *pData ) {
		if ( pData == nullptr ) return false;
		if ( pTop == nullptr )return false;
		*pData = pTop->Data;
		return true;
	}

	bool Pop ( T *pData ) {
		if ( Peek ( pData ) ) {
			this->Skip ();
			return true;
		}
		return false;
	}


	bool IsEmpty ( void ) {
		return ( pTop == nullptr ) ? true : false;
	}



	void Free ( void ) {
		while ( pTop ) {
			THSStackData<T> *pCurrent = pTop;
			pTop = pTop->pNext;
			delete pCurrent;
		}
	}

	void Skip ( void ) {
		if ( pTop ) {
			THSStackData<T> *pCurrent = pTop;
			pTop = pTop->pNext;
			delete pCurrent;
		}
	}




};

#endif // !__HSSTACK__
