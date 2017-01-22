/*----------------------------------------------------
<<CHSDeque�e���v���[�g�N���X>>
�@�\�F�ėp���[�L���[
�쐬���F2015/12/13
�쐬�ҁF�q���\�t
----------------------------------------------------*/

#ifndef __HS_DEQUE__
#define __HS_DEQUE__

template<typename T> class CHSDeque {
private:
	template<typename T> struct THSDequeData {
		T Data;
		THSDequeData<T> *pFrontSide;	//�擪��
		THSDequeData<T> *pBackSide;		//�I�[��
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

			//�f�[�^�̈�m��
			THSDequeData<T> *pCurrent = new THSDequeData<T>;

			//�ǉ��f�[�^�ݒ�
			pCurrent->Data = Data;
			if ( bSwap == false ) {
				pCurrent->pFrontSide = nullptr;
				pCurrent->pBackSide = *ppFront;

				//�ǉ������f�[�^��V���Ȑ擪�ɂ���
				*ppFront = pCurrent;

				//�擪����2�Ԗڂ̑O�̃f�[�^�ւ̃|�C���^��擪�ɂ���
				if ( ( *ppFront )->pBackSide )
					( *ppFront )->pBackSide->pFrontSide = *ppFront;

				if ( *ppBack == nullptr ) {
					//�I�[�����Ȃ������ꍇ�ǉ������f�[�^���I�[�ɂ���
					*ppBack = pCurrent;
				}
			} else {

				pCurrent->pFrontSide = *ppBack;
				pCurrent->pBackSide = nullptr;

				//�ǉ������f�[�^���I�[�ɂ���
				*ppBack = pCurrent;

				//�I�[����Q�Ԗڂ̎��̃f�[�^�ւ̃|�C���^���I�[�ɂ���
				if ( ( *ppBack )->pFrontSide )
					( *ppBack )->pFrontSide->pBackSide = *ppBack;

				if ( *ppFront == nullptr ) {
					//��[�����Ȃ������ꍇ�ǉ������f�[�^���[�ɂ���
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
public:
	CHSDequeStack<T> Front;
	CHSDequeStack<T> Back;

	CHSDeque ( ) : Front(&pFront , &pBack , false) , Back ( &pFront , &pBack  , true){
		pFront = nullptr;
		pBack = nullptr;
	}

	~CHSDeque ( ){
		Free ( );
	}


	bool IsEmpty ( void ) {
		return ( pFront == nullptr ) ? true : false;
	}

	bool IsValid ( void ) {
		return ( pFront != nullptr ) ? true : false;
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
#endif /*__HS_DEQUE__*/