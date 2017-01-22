#include <Windows.h>
#ifndef __HSCOMPTR__
#define __HSCOMPTR__



template <typename T> class HSComType : public T {

private:
	//IUnknown インタフェースクラス　メンバの２つをprivateにする
	virtual ULONG STDMETHODCALLTYPE AddRef ( void ) = 0;
	virtual ULONG STDMETHODCALLTYPE Release ( void ) = 0;

};


template <typename T> class HSComPtrBase {

protected :
	T *pm_Interface;

public:


	HSComPtrBase () {
		pm_Interface = nullptr;
	}
	~HSComPtrBase () {
		this->Release ();
	}

	HRESULT CreateInstance ( const IID Clsid , const IID riid = __uuidof( T )) {
		if ( pm_Interface != nullptr )return E_FAIL;
		return CoCreateInstance ( Clsid , NULL , CLSCTX_INPROC_SERVER , riid , (void**) &pm_Interface );
	}


	ULONG AddRef ( void ) {
		if ( pm_Interface == nullptr )return E_FAIL;
		return ( (IUnknown*) pm_Interface )->AddRef ();
	}

	ULONG Release ( void ) {
		if ( pm_Interface == nullptr )return E_FAIL;
		ULONG uRet = ( (IUnknown*) pm_Interface )->Release ();
		this->pm_Interface = nullptr;
		return uRet;
	}

	bool CheckCreatedInstance ( void ) {
		return ( pm_Interface ) ? true : false;
	}

	HSComType<T> * operator ->( ) {
		return reinterpret_cast<HSComType<T>*>( pm_Interface );
	}



};


template <typename T> class HSComPtr : public HSComPtrBase < T > {

public:



	T* Attach ( T* pInterface ) {
		if ( pInterface == nullptr ) return nullptr;
		T* old = this->pm_Interface;
		this->pm_Interface = pInterface;
		return old;
	}

	T* Detach ( void ) {
		T* det = this->pm_Interface;
		this->pm_Interface = nullptr;
		return det;
	}


	T** operator&( ) {
		if ( pm_Interface != nullptr )return nullptr;
		return &this->pm_Interface;
	}

	HSComType<T> * Get ( void ) {
		return reinterpret_cast<HSComType<T>*>( pm_Interface );
	}

	 operator T*( ) {

		 return  this->pm_Interface;

	}

};

#endif