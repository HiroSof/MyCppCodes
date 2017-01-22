#pragma once

#include <Windows.h>
#include <stdint.h>
#include <stdexcept>

template <typename T> class CHSBuffer {

private:
	T *pBuffer;

	uint32_t NumberOfAllocElements;

	bool IsRangeIndex ( uint32_t index ) {
		return ( index < this->GetBufferSize ( ) );
	}

public:

	CHSBuffer ( ) {
		this->pBuffer = nullptr;
		this->NumberOfAllocElements = 0;
	}


	CHSBuffer ( uint32_t NumberOfElements ) {
		this->pBuffer = nullptr;
		this->NumberOfAllocElements = 0;
		this->Alloc ( NumberOfElements );
	}

	~CHSBuffer ( ) {
		this->Free ( );
	}


	bool Alloc ( uint32_t NumberOfElements ) {
		if ( NumberOfElements == 0 ) return false;
		if ( this->IsAllocated ( ) ) return false;
		this->pBuffer = reinterpret_cast< T* >( HeapAlloc ( GetProcessHeap ( ) ,
			HEAP_ZERO_MEMORY , sizeof ( T ) *  NumberOfElements ) );
		if ( this->pBuffer ) this->NumberOfAllocElements = NumberOfElements;
		return this->IsAllocated ( );
	}

	bool ReAlloc ( uint32_t NumberOfNewElements ) {
		if ( this->IsAllocated ( ) ) {
			void *pnewbuf = HeapReAlloc ( GetProcessHeap ( ) , HEAP_ZERO_MEMORY ,
				this->pBuffer , sizeof ( T ) *  NumberOfElements );
			if ( pnewbuf != nullptr ) {
				this->pBuffer = reinterpret_cast< T* >( pnewbuf );
				this->NumberOfAllocElements = NumberOfElements;
				return true;
			}
		}
		return false;
	}


	bool Free ( void ) {
		if ( this->IsAllocated ( ) ) {
			if ( HeapFree ( GetProcessHeap ( ) , NULL , this->pBuffer ) ) {
				this->pBuffer = nullptr;
				this->NumberOfAllocElements = 0;
				return true;
			}
		}
		return false;
	}

	uint32_t GetAllocSize ( void ) const {
		return this->NumberOfAllocElements;
	}

	uint32_t GetAllocBytesSize ( void ) const {
		return this->GetAllocSize ( ) * this->GetBufferTypeSize ( );
	}

	uint32_t GetBufferSize ( void ) const {
		return this->GetBufferBytesSize ( ) / this->GetBufferTypeSize ( );
	}

	static uint32_t GetBufferTypeSize ( void ) {
		return  static_cast< uint32_t >( sizeof ( T ) );
	}


	uint32_t GetBufferBytesSize ( void ) const {
		if ( this->IsAllocated ( ) ) {
			int64_t s = HeapSize ( GetProcessHeap ( ) , NULL , this->pBuffer );
			return ( s == -1 ) ? 0 : static_cast< uint32_t >( s );
		}
		return 0;
	}

	bool IsAllocated ( void ) const {
		return  ( this->pBuffer != nullptr );
	}

	T* GetBufferPointer ( uint32_t idx = 0 ){
		if ( IsRangeIndex ( idx ) ) {
			return this->pBuffer + idx;
		}
		return nullptr;
	}

	T& GetBufferReference ( uint32_t idx = 0 ) {
		T *ptr = this->GetBufferPointer ( idx );
		if ( ptr == nullptr ) {
			throw std::out_of_range("");
		} else {
			return *ptr;
		}

	}

	bool SetValue ( uint32_t idx , T value ) {
		if ( this->IsAllocated ( ) ) {
			if ( IsRangeIndex ( idx ) ) {
				*( this->pBuffer + idx ) = value;
				return true;
			}
		}
		return false;
	}



	bool GetValue ( uint32_t idx , T *pvalue ) const {
		if ( pvalue == nullptr ) return 0;
		if ( this->IsAllocated ( ) ) {
			if ( IsRangeIndex ( idx ) ) {
				*pvalue = *( this->pBuffer + idx );
				return true;
			}
		}
		return false;
	}

	T& operator[] ( uint32_t idx ) {
		return this->GetBufferReference ( idx );
	}


	bool Swap ( uint32_t idx1 , uint32_t idx2 ) {
		if ( this->IsRangeIndex ( idx1 ) && this->IsRangeIndex ( idx2 ) ) {
			T data = *( this->pBuffer + idx1 );
			*( this->pBuffer + idx1 ) = *( this->pBuffer + idx2 );
			*( this->pBuffer + idx2 ) = data;
			return true;
		}
		return false;
	}


	bool Reverse ( void ) {
		if ( this->IsAllocated ( ) ) {
			uint32_t FirstSide = 0;
			uint32_t LastSide = this->GetBufferSize ( ) - 1;
			while ( FirstSide < LastSide ) {
				this->Swap ( FirstSide , LastSide );
				FirstSide++;
				LastSide--;
			}
			return true;
		}
		return false;
	}



};