#pragma once
#include <memory.h>
class CHSMap {
private:
	unsigned int uWidth;
	unsigned int uHeight;
	int ErrorCodeForIntReturn;
	int *pMapData;

protected:
	bool InnerInitialize ( unsigned int Width , unsigned int Height );
	bool InnerResize ( unsigned int Width , unsigned int Height , bool bTakeOver );
	void Uninitialize ( void );

public:
	CHSMap ();
	~CHSMap ();


	int SetErrorReturnValueInt ( int ErrorCode );
	int GetErrorReturnValueInt ( void );


	bool IsInitialized ( void );


	unsigned int GetWidth ( void );
	unsigned int GetHeight ( void );



	
	bool Clear ( void );
	
	bool SetValue ( unsigned int x , unsigned int y , int value );
	int GetValue ( unsigned int x , unsigned int y  );

	bool FillValue ( int value );
	bool Export ( int *pTargetMap );
	bool Import ( int *pTargetMap );


	bool Initialize ( unsigned int Width , unsigned int Height );
	bool Resize ( unsigned int Width , unsigned int Height , bool bTakeOver );


};

