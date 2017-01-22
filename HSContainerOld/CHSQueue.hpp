

//çXêVì˙éû
//2015-05-05
//2015-05-06


#ifndef __HSQUEUE__
#define __HSQUEUE__

template <typename T> struct THSQueueData {
	T Data;
	THSQueueData<T> *pNext;
};



template<typename T> class CHSQueue {
private:
	THSQueueData<T> *pLast;
	THSQueueData<T> *pFirst;
public:


	CHSQueue () {
		pLast = nullptr;
		pFirst = nullptr;
	}

	~CHSQueue () {
		this->Free ();

	}


	void Enqueue ( T data ) {
		THSQueueData<T> *pCuurent = new THSQueueData<T>;

		pCuurent->Data = data;
		pCuurent->pNext = nullptr;


		if ( pLast == nullptr ) {
			pLast = pCuurent;
		} else {
			pLast->pNext = pCuurent;
			pLast = pCuurent;
		}

		if ( pFirst == nullptr ) pFirst = pCuurent;
	}


	bool Dequeue (T *pData) {
		if ( Peek ( pData ) ) {
			this->Skip ();
			return true;
		}
		return false;
	}

	bool Peek ( T *pData ) {
		if ( pData == nullptr )return false;
		if ( pFirst == nullptr ) return false;
		*pData = pFirst->Data;
		return true;
	}


	void Skip ( void ) {
		THSQueueData<T> *pCurrent = pFirst;
		pFirst = pFirst->pNext;
		delete pCurrent;
	}

	bool IsEmpty ( void ) {
		return ( pFirst == nullptr ) ? true : false;
	}


	void Free ( void ) {

		while ( pFirst ) {

			THSQueueData<T> *pCurrent = pFirst;

			pFirst = pFirst->pNext;

			delete pCurrent;
		}

		pLast = nullptr;
	}



};

typedef CHSQueue<int> CHSQueueInt;
typedef CHSQueue<char> CHSQueueChar;
typedef CHSQueue<float> CHSQueueFloat;
typedef CHSQueue<double> CHSQueueDouble;



#endif // !__HSQUEUE__
