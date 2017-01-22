/*----------------------------------------------------
<<CHSQueueテンプレートクラス>>
機能：汎用キュー
作成日：2015/12/13
作成者：ヒロソフ
----------------------------------------------------*/
#ifndef __HS_QUEUE__
#define __HS_QUEUE__
template<typename T> class CHSQueue {
private:
	template<typename T> struct THSQueueData {
		T Data;
		THSQueueData<T> *pBackSide;
	};

	THSQueueData<T> *pLastEntry;
	THSQueueData<T> *pFront;
public:
	CHSQueue ( ) {
		pLastEntry = nullptr;
		pFront = nullptr;
	}

	~CHSQueue ( ){
		Free ( );
	}
	void Enqueue ( T Data ) {
		THSQueueData<T> *pCurrent = new THSQueueData<T>;
		pCurrent->Data = Data;
		pCurrent->pBackSide = nullptr;
	
		if( pLastEntry )
			pLastEntry->pBackSide = pCurrent;
		
		pLastEntry = pCurrent;

		if ( pFront == nullptr )pFront = pCurrent;
	
	}
	
	bool Dequeue ( T *pData ) {
		if ( Peek ( pData ) ) {
			this->Skip ( );
			return true;
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
			THSQueueData<T> *pCurrent = pFront;
			if ( pCurrent == pLastEntry )pLastEntry = nullptr;
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
	void Free ( ) {
		while ( pFront ) {
			THSQueueData<T> *pCurrent = pFront;
			pFront = pFront->pBackSide;
			delete pCurrent;
		}
		pLastEntry = nullptr;
	}

};

#endif /*__HS_QUEUE__*/

