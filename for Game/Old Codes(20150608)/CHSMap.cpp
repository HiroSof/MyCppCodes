#include "CHSMap.hpp"

CHSMap::CHSMap () {
	pMapData = nullptr;
	uWidth = uHeight = 0;
	ErrorCodeForIntReturn = -1;
}

CHSMap::~CHSMap () {
	this->Uninitialize ();
}
#include <stdio.h>
void CHSMap::Uninitialize () {
	if ( pMapData ) {
		delete [] pMapData;
		pMapData = nullptr;
		uWidth = uHeight = 0;
	}
}

int CHSMap::SetErrorReturnValueInt ( int ErrorCode ) {
	int OldValue = this->ErrorCodeForIntReturn;
	ErrorCodeForIntReturn = ErrorCode;
	return OldValue;
}
int CHSMap::GetErrorReturnValueInt ( void ) {
	return ErrorCodeForIntReturn;
}


bool CHSMap::IsInitialized ( void ) {
	return ( pMapData != nullptr ) ? true : false;
}


bool  CHSMap::InnerInitialize ( unsigned int Width , unsigned int Height ) {
	if ( ( Width == 0 ) || ( Height == 0 ) ) return false;
	if ( pMapData ) return false;
	uWidth = Width;
	uHeight = Height;

	pMapData = new int [ uWidth * uHeight ];


	this->Clear ();

	return true;
}

bool CHSMap::Initialize ( unsigned int Width , unsigned int Height ) {
	return InnerInitialize ( Width , Height );
}


unsigned int CHSMap::GetWidth ( void ) {
	return uWidth;
}
unsigned int CHSMap::GetHeight ( void ) {
	return uHeight;
}

bool CHSMap::Clear ( void ) {

	if ( pMapData == nullptr ) return false;

	memset ( pMapData , 0 , sizeof ( int ) * uWidth * uHeight );

	return true;
}


bool CHSMap::SetValue ( unsigned int x , unsigned int y , int value ) {
	if ( pMapData == nullptr ) return false;

	if ( x >= uWidth ) return false;
	if ( y >= uHeight ) return false;



	int linepos = y * uWidth;

	*( pMapData + linepos + x ) = value;

	return true;
}
int CHSMap::GetValue ( unsigned int x , unsigned int y ) {

	if ( pMapData == nullptr ) return ErrorCodeForIntReturn;

	if ( x >= uWidth ) return ErrorCodeForIntReturn;
	if ( y >= uHeight ) return ErrorCodeForIntReturn;
	int linepos = y * uWidth;

	return *( pMapData + linepos + x );
}

bool CHSMap::FillValue ( int value ) {
	if ( pMapData == nullptr ) return false;
	int pos;
	for ( unsigned int y = 0; y < uHeight; y++ ) {
		for ( unsigned int x = 0; x < uWidth; x++ ) {
			pos = y * uWidth + x;
			*( pMapData + pos ) = value;
		}
	}
	return true;
}

bool CHSMap::Export ( int *pTargetMap ) {
	if ( pTargetMap == 0 )return false;
	if ( pMapData == nullptr ) return false;
	int pos;
	for ( unsigned int y = 0; y < uHeight; y++ ) {
		for ( unsigned int x = 0; x < uWidth; x++ ) {
			pos = y * uWidth + x;
			*( pTargetMap + pos ) = *( pMapData + pos );
		}
	}
	return true;
}

bool CHSMap::Import ( int *pTargetMap ) {

	if ( pTargetMap == 0 )return false;
	if ( pMapData == nullptr ) return false;
	int pos;
	for ( unsigned int y = 0; y < uHeight; y++ ) {
		for ( unsigned int x = 0; x < uWidth; x++ ) {
			pos = y * uWidth + x;
			*( pMapData + pos ) = *( pTargetMap + pos );
		}
	}
	return true;
}



bool CHSMap::InnerResize ( unsigned int Width , unsigned int Height , bool bTakeOver ) {

	if ( this->pMapData == nullptr )return false;

	int *pOldMap = this->pMapData;
	unsigned int OldWidth = uWidth , OldHeight = uHeight;


	pMapData = new int [ Width * Height ];

	uWidth = Width;
	uHeight = Height;

	this->Clear ();

	if ( bTakeOver ) {
		for (unsigned int y = 0; y < OldHeight; y++ ) {

			if ( y == uHeight )break;
			for ( unsigned int x = 0; x < OldWidth; x++ ) {
				if ( x == uWidth )break;

				int OldPos = y * OldWidth + x;
				int pos = y * uWidth + x;

				*( pMapData + pos ) = *( pOldMap + OldPos );

			}

		}
	}

	delete []pOldMap;

	return true;
}


bool CHSMap::Resize ( unsigned int Width , unsigned int Height , bool bTakeOver ) {
	return this->InnerResize ( Width , Height , bTakeOver );
}
