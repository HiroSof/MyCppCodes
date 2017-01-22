#ifndef __CHSSHABASE_H__
#define __CHSSHABASE_H__

#ifndef NULL
#define NULL 0
#endif // !NULL

#include <memory>
#include <cstdio>


enum CHSSHA_ERROR
{
	CHSSHA_ERROR_FALSE = 0,
	CHSSHA_ERROR_TRUE,
	CHSSHA_ERROR_ISNOTQUITPABLISH,
	CHSSHA_ERROR_QUITPABLISHED
};

enum CHSSHA_TOSTRFLAG
{
	CHSSHA_TOSTRFLAG_SMALL = 0,
	CHSSHA_TOSTRFLAG_SMALL_DELIMITER,
	CHSSHA_TOSTRFLAG_BIG,
	CHSSHA_TOSTRFLAG_BIG_DELIMITER,
};



class CHSSHABASE
{


private :
	const unsigned __int32 *lpDefaultMessageDijest;
	unsigned __int32 *lpMessageDijest;
	int NumberOfMessageDijestElements;

//protected ïœêî
protected:

	__int64		DataSize;
	unsigned __int8		TempData[64];
	int			TmpDataIdx;
	bool		QuitPublish;

	virtual	void HashBlock(void) = NULL;

public:
	CHSSHABASE(int,const unsigned __int32 * , unsigned __int32 *);

	void	New(void);
	void	Initialization(void);

	int StrlenByte(char*);
	int StrlenByte(unsigned char*);
	int StrlenByte(wchar_t*);

	CHSSHA_ERROR	Insert(void* , __int64);
	CHSSHA_ERROR	Insert(char*);
	CHSSHA_ERROR	Insert(wchar_t*);
	CHSSHA_ERROR	Insert(unsigned char*);

	CHSSHA_ERROR	Publish(void *InputData = NULL , __int64  Size = NULL);
	CHSSHA_ERROR	Publish(char*);
	CHSSHA_ERROR	Publish(wchar_t*);
	CHSSHA_ERROR	Publish(unsigned char*);
	
	int Get(char *lpTextA , int size , CHSSHA_TOSTRFLAG  flag = CHSSHA_TOSTRFLAG_BIG_DELIMITER, char split_char = ' ');
	int Get(wchar_t *lpTextW , int size , CHSSHA_TOSTRFLAG  flag = CHSSHA_TOSTRFLAG_BIG_DELIMITER, wchar_t split_char = ' ');
	int Get(unsigned __int32 *);
};




#endif // !__CHSSHABASE_H__

