/*----------------------------------------------------
<<CHSSecureDequeテンプレートクラス>>
機能：安全な汎用両端キュー
作成日：2015/12/13
作成者：ヒロソフ
----------------------------------------------------*/

#ifndef __HS_SECUREDEQUE__
#define __HS_SECUREDEQUE__

template<typename T> class CHSSecureDeque {
private:
	template<typename T> struct THSDequeData {
		T Data;
		THSDequeData<T> *pFrontSide;	//先頭側
		THSDequeData<T> *pBackSide;		//終端側
	};

	template<typename T>  class CHSDequeStack {
	private:
		THSDequeData<T> **ppFront;
		THSDequeData<T> **ppBack;
		bool bSwap;
	public:
		CHSDequeStack ( THSDequeData<T> **_ppFront , THSDequeData<T> **_ppBack , bool bSwapFlag ) {
			this->ppFront = _ppFront;
			this->ppBack = _ppBack;
			bSwap = bSwapFlag;
		}

		void Push ( T Data ) {

			//データ領域確保
			THSDequeData<T> *pCurrent = new THSDequeData<T>;

			//追加データ設定
			pCurrent->Data = Data;
			if ( bSwap == false ) {
				pCurrent->pFrontSide = nullptr;
				pCurrent->pBackSide = *ppFront;

				//追加したデータを新たな先頭にする
				*ppFront = pCurrent;

				//先頭から2番目の前のデータへのポインタを先頭にする
				if ( ( *ppFront )->pBackSide )
					( *ppFront )->pBackSide->pFrontSide = *ppFront;

				if ( *ppBack == nullptr ) {
					//終端側がなかった場合追加したデータを終端にする
					*ppBack = pCurrent;
				}
			} else {

				pCurrent->pFrontSide = *ppBack;
				pCurrent->pBackSide = nullptr;

				//追加したデータを終端にする
				*ppBack = pCurrent;

				//終端から２番目の次のデータへのポインタを終端にする
				if ( ( *ppBack )->pFrontSide )
					( *ppBack )->pFrontSide->pBackSide = *ppBack;

				if ( *ppFront == nullptr ) {
					//先端側がなかった場合追加したデータを先端にする
					*ppFront = pCurrent;
				}
			}
		}

		bool Pop ( T *pData ) {
			if ( ( *ppFront != nullptr ) && ( *ppBack != nullptr ) ) {
				if ( Peek ( pData ) )
					return Skip ( );
			}
			return false;
		}

		bool Peek ( T *pData ) {
			if ( ( *ppFront != nullptr ) && ( *ppBack != nullptr ) ) {
				if ( pData ) {
					*pData = ( bSwap == false ) ? ( *ppFront )->Data : ( *ppBack )->Data;
					return true;
				}
			}
			return false;
		}

		bool Skip ( ) {
			if ( ( *ppFront != nullptr ) && ( *ppBack != nullptr ) ) {
				THSDequeData<T> *pSkip;

				if ( bSwap == false ) {
					pSkip = *ppFront;
					*ppFront = ( *ppFront )->pBackSide;
					if ( *ppFront == nullptr ) *ppBack = nullptr;
					else ( *ppFront )->pFrontSide = nullptr;

				} else {
					pSkip = *ppBack;
					*ppBack = ( *ppBack )->pFrontSide;
					if ( *ppBack == nullptr ) *ppFront = nullptr;
					else ( *ppBack )->pBackSide = nullptr;
				}
				delete pSkip;
				return true;
			}
			return false;
		}
	};

	THSDequeData<T> *pFront;
	THSDequeData<T> *pBack;
	CHSDequeStack<T> InnerFront;
	CHSDequeStack<T> InnerBack;
public:

	CHSSecureDeque ( ) : InnerFront ( &pFront , &pBack , false ) , InnerBack ( &pFront , &pBack , true ) {
		pFront = nullptr;
		pBack = nullptr;
	}

	~CHSSecureDeque ( ) {
		Free ( );
	}

	CHSDequeStack<T>& GetFront ( void ) {
		return InnerFront;
	}
	CHSDequeStack<T>& GetBack ( void ) {
		return InnerBack;
	}

	__declspec( property( get = GetFront ) ) CHSDequeStack<T>& Front;
	__declspec( property( get = GetBack ) ) CHSDequeStack<T>& Back;

	bool IsEmpty ( void ) {
		return ( pFront == nullptr ) ? true : false;
	}

	bool IsValid ( void ) {
		return ( pFront != nullptr ) ? true : false;
	}
	int Count ( void ) {
		int c = 0;
		THSDequeData<T> *pCurrent = pFront;
		while ( pCurrent ) {
			c++;
			pCurrent = pCurrent->pBackSide;
		}
		return c;
	}
	void Free ( ) {
		while ( pFront ) {
			THSDequeData<T> *pCurrent = pFront;
			pFront = pFront->pBackSide;
			delete pCurrent;
		}
		pBack = nullptr;
	}

};
#endif /*__HS_SECUREDEQUE__*/