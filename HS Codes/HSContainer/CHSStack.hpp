/*----------------------------------------------------
<<CHSStackテンプレートクラス>>
機能：汎用スタック
作成日：2015/12/13
作成者：ヒロソフ
----------------------------------------------------*/

#ifndef __HS_STACK__
#define __HS_STACK__

template<typename T> class CHSStack {
private:

	template<typename T> struct THSStackData {
		T Data;
		THSStackData<T> *pBackSide;
	};

	THSStackData<T> *pFront;

public:
	CHSStack ( ) {
		pFront = nullptr;
	}
	~CHSStack ( ) {
		this->Free ( );
	}

	void Push ( T Data ) {
		THSStackData<T> *pCurrent = new THSStackData<T>;
		pCurrent->Data = Data;
		pCurrent->pBackSide = pFront;
		pFront = pCurrent;
	}

	bool Pop ( T *pData ) {
		if ( pFront ) {
			if ( Peek ( pData ) ) {
				return Skip ( );
			}
		}
		return false;
	}

	bool Peek ( T *pData ) {
		if ( pFront ) {
			if ( pData ) {
				*pData = pFront->Data;
				return true;
			}
		}
		return false;
	}
	bool Skip ( void ) {
		if ( pFront ) {
			THSStackData<T> *pCurrent = pFront;
			pFront = pFront->pBackSide;
			delete pCurrent;
			return true;
		}
		return false;
	}

	bool IsEmpty ( void ) {
		return ( pFront == nullptr ) ? true : false;
	}
	bool IsValid ( void ) {
		return ( pFront != nullptr ) ? true : false;
	}

	int Count ( void ) {
		int c = 0;
		THSStackData<T> *pCurrent = pFront;
		while ( pCurrent ) {
			c++;
			pCurrent = pCurrent->pBackSide;
		}
		return c;
	}

	void Free ( ) {
		while ( pFront ) {
			THSStackData<T> *pCurrent = pFront;
			pFront = pFront->pBackSide;
			delete pCurrent;
		}
	}

};

#endif /*__HS_STACK__*/