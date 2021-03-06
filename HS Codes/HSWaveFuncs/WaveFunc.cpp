#include "WaveFunc.h"

#ifndef	WAVEFUNC_CPP
	#define	WAVEFUNC_CPP
	/*
		I/O系関数
	*/
	//WAVEファイルを開く関数
	HANDLE	OpenWaveFile(TCHAR *lpWaveFileAd , BOOL isWrite , BOOL isShareRead){
		HANDLE handle;
		UINT shared=0;
		if(isShareRead && (isWrite == FALSE))shared = FILE_SHARE_READ;
		handle =  (!lpWaveFileAd)?NULL:CreateFile(lpWaveFileAd , 
			GENERIC_READ  | ((isWrite) ? GENERIC_WRITE : 0) ,
			shared,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if(handle==INVALID_HANDLE_VALUE)handle = NULL;
		return handle; 
	}

	//WAVEファイルを空にして開く関数
	HANDLE	OpenWaveFileWithClear(TCHAR *lpWaveFileAd){
		HANDLE handle;
		handle = (!lpWaveFileAd)?NULL:CreateFile(lpWaveFileAd , 
			GENERIC_READ  | GENERIC_WRITE ,
			NULL,
			NULL,
			TRUNCATE_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if(handle==INVALID_HANDLE_VALUE)handle = NULL;
		return handle;
	}

	//WAVEファイルを作成する関数
	HANDLE	CreateWaveFile(TCHAR *lpWaveFileAd){
		HANDLE handle;
		handle = (!lpWaveFileAd)?NULL:CreateFile(lpWaveFileAd , 
			GENERIC_READ  | GENERIC_WRITE ,
			NULL,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL);		
		if(handle==INVALID_HANDLE_VALUE)handle = NULL;
		return handle;
	}
	//WAVEファイルを作成する関数(自動判別版)
	HANDLE	CreateWaveFileAuto(TCHAR *lpWaveFileAd){
		return (ExistWaveFile(lpWaveFileAd))?OpenWaveFileWithClear(lpWaveFileAd):CreateWaveFile(lpWaveFileAd);

	}
	//WAVEファイルがあるかを確認する関数
	BOOL	ExistWaveFile(TCHAR *lpWaveFileAd){
		return (!lpWaveFileAd)?FALSE:PathFileExists(lpWaveFileAd);
	}

	//WAVEファイルを削除する関数
	BOOL	DeleteWaveFile(TCHAR *lpWaveFileAd){
		return (!lpWaveFileAd)?FALSE:DeleteFile(lpWaveFileAd);
	}


	//WAVEファイルを閉じる関数
	BOOL	CloseWaveFile(HANDLE hWaveFile){ 
		return (!hWaveFile)?FALSE:CloseHandle(hWaveFile); 
	}

	
	/*
		情報取得系関数
	*/

	//WAVEファイルのファイルサイズを取得
	unsigned __int32	GetWaveFileSize(HANDLE hWaveFile){
		LARGE_INTEGER LI;
		LI.QuadPart = 0;
		if(!hWaveFile)return FALSE;
		GetFileSizeEx(hWaveFile,&LI);
		return (unsigned __int32)LI.QuadPart;
	}

	//WAVEファイルのヘッダーからファイルサイズを取得
	unsigned __int32	GetWaveFileSizeFromHeader(HANDLE hWaveFile){
		unsigned __int32 fsize;
		DWORD rs;
		if(!CheckWaveFile(hWaveFile))return 0;
		if(SetFilePointer(hWaveFile,4,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER)return FALSE;
		if(!ReadFile(hWaveFile , &fsize , 4 , &rs , NULL))return FALSE;
		if(rs!=4)return 0;
		return fsize+8;
	}

	//WAVEファイルを検査する関数
	BOOL	CheckWaveFile(HANDLE	hWaveFile){
		char data[5];
		unsigned __int32 fsize;
		LARGE_INTEGER LI;
		DWORD	rs;

		//ファイルハンドルがNULLならばFALSEを返す
		if(!hWaveFile)return FALSE;

		//dataの初期化
		memset(data , 0 , 5);

		/* 0バイト目～3バイト目が"RIFF"かを確認する  */
		if(SetFilePointer(hWaveFile,0,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER)return FALSE;
		if(!ReadFile(hWaveFile , data , 4 , &rs , NULL))return FALSE;
		if(rs!=4)return FALSE;
		if(strcmp(data,"RIFF"))return FALSE;

		/* 4バイト目～7バイト目がファイルサイズ-8と一致するかを調べる*/
		if(SetFilePointer(hWaveFile,4,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER)return FALSE;
		if(!ReadFile(hWaveFile , &fsize , 4 , &rs , NULL))return FALSE;
		if(rs!=4)return FALSE;
		if(!GetFileSizeEx(hWaveFile,&LI))return FALSE;
		if((fsize + 8) != LI.QuadPart)return FALSE;

		/* 8バイト目～11バイト目が"WAVE"かを確認する  */
		if(SetFilePointer(hWaveFile,8,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER)return FALSE;
		if(!ReadFile(hWaveFile , data , 4 , &rs , NULL))return FALSE;
		if(rs!=4)return FALSE;
		if(strcmp(data,"WAVE"))return FALSE;

		/*"fmt "チャンクがあるかを確認する*/
		if(!GetWaveChunkPos(hWaveFile,"fmt"))return FALSE;

		/*"data"チャンクがあるかを確認する*/
		if(!GetWaveChunkPos(hWaveFile,"data"))return FALSE;

		//全てのチェックを正常通過したのでTRUEを返す
		return TRUE;
	}

	//WAVEファイルの情報を取得
	BOOL	GetWaveInfo(HANDLE	hWaveFile , LPWaveFileInfo lpWFI){
		LPWaveFileBaseHeader lpWFBH;
		LARGE_INTEGER LI;
		DWORD rs;
		if(!lpWFI)return FALSE;
		lpWFBH = &(lpWFI->tagWaveFileBaseHeader);
		if(!CheckWaveFile(hWaveFile))return FALSE;

		//ファイル先頭12バイト分
		memcpy(lpWFBH->sChunkID,"RIFF",4);
		memcpy(lpWFBH->szFromType,"WAVE",4);
		lpWFI->FileInfomation.uFileSize = GetWaveFileSizeFromHeader(hWaveFile);
		lpWFBH->uChunkSize = lpWFI->FileInfomation.uFileSize - 8;

		//"fmt"チャンクを読み込む
		if(!(lpWFI->FileInfomation.uFmtChunkOffset = GetWaveChunkPos(hWaveFile,"fmt")))return FALSE;
		LI.QuadPart = lpWFI->FileInfomation.uFmtChunkOffset;
		if(!SetFilePointerEx(hWaveFile,LI,NULL,FILE_BEGIN))return 0;
		if(!ReadFile(hWaveFile,&(lpWFBH->Fmt),sizeof(lpWFBH->Fmt),&rs,NULL))return 0;

		//"data"チャンクの最初の8バイト分(波形データ本体以外)読み込む
		if(!(lpWFI->FileInfomation.uDataChunkOffset = GetWaveChunkPos(hWaveFile,"data")))return FALSE;
		LI.QuadPart = lpWFI->FileInfomation.uDataChunkOffset;
		if(!SetFilePointerEx(hWaveFile,LI,NULL,FILE_BEGIN))return 0;
		if(!ReadFile(hWaveFile,&(lpWFBH->Data),sizeof(lpWFBH->Data),&rs,NULL))return 0;

		//各種情報計算

		//再生時間
		//式：波形データサイズ÷1秒当たりのサイズ*1000
		lpWFI->FileInfomation.uPlayTime = (unsigned __int32)((double)lpWFBH->Data.uWaveFormSize/(double)lpWFBH->Fmt.tagPCMWaveFormat.wf.nAvgBytesPerSec*1000);
		
		//サンプル数
		//式：(波形データサイズ÷チャンネル数)÷(1サンプル当たりのバイト数) 
		//式2(これを使用)：波形データのサイズ÷ブロックサイズ
		lpWFI->FileInfomation.uSamples = lpWFBH->Data.uWaveFormSize / lpWFBH->Fmt.tagPCMWaveFormat.wf.nBlockAlign;
		return TRUE;
	}

	//指定されたフォーマットかを確認する関数
	BOOL	CheckWaveFormat(LPWaveFileBaseHeader lpwfh,WORD NumberOfFormatTag , WORD NumberOfChannels , DWORD SamplePerSec   , WORD BitPerSample){
		PCMWAVEFORMAT *lpformat;
		if(!lpwfh)return FALSE;
		lpformat = &(lpwfh->Fmt.tagPCMWaveFormat);
		if((lpformat->wf.wFormatTag !=  NumberOfFormatTag)&&(NumberOfFormatTag!=0))return FALSE;
		if((lpformat->wf.nChannels !=  NumberOfChannels)&&(NumberOfChannels!=0))return FALSE;
		if((lpformat->wf.nSamplesPerSec != SamplePerSec)&&(SamplePerSec!=0))return FALSE;
		if((lpformat->wBitsPerSample !=  BitPerSample)&&(BitPerSample!=0))return FALSE;
		return TRUE;
	}

	//指定されたフォーマットかを確認する関数(ファイルバージョン)
	BOOL	CheckWaveFormatFromFile(HANDLE hWaveFile,WORD NumberOfFormatTag , WORD NumberOfChannels , DWORD SamplePerSec   , WORD BitPerSample){
		WaveFileInfo	wfi;
		if(!GetWaveInfo(hWaveFile,&wfi))return FALSE;
		return CheckWaveFormat(&(wfi.tagWaveFileBaseHeader),NumberOfFormatTag,NumberOfChannels,SamplePerSec,BitPerSample);
	}


	//対応している方式か確認する関数
	BOOL			CheckWaveFormatType(LPWaveFileBaseHeader lpwfh){
		PCMWAVEFORMAT *lpformat;
		if(!lpwfh)return FALSE;
		lpformat = &(lpwfh->Fmt.tagPCMWaveFormat);
	
		//フォーマットID検査
		if((!CheckWaveFormat(lpwfh,WAVE_FORMAT_PCM,0,0,0))&&(!CheckWaveFormat(lpwfh,WAVE_FORMAT_IEEE_FLOAT,0,0,32)))
			return FALSE;

		//チャンネル検査
		if((!CheckWaveFormat(lpwfh,0,1,0,0))&&(!CheckWaveFormat(lpwfh,0,2,0,0)))
			return FALSE;

		return TRUE;
	}
	
	//対応している方式か確認する関数
	BOOL			CheckWaveFormatTypeFromFile(HANDLE hWaveFile){
		WaveFileInfo	wfi;
		if(!GetWaveInfo(hWaveFile,&wfi))return FALSE;
		return CheckWaveFormatType(&(wfi.tagWaveFileBaseHeader));
	}



	/*
		チャンク情報取得系関数
	*/
	//WAVEファイルの指定したチャンクを検索してその位置を返す関数
	unsigned __int32	GetWaveChunkPos(HANDLE hWaveFile , char *lpChunk){
		LARGE_INTEGER LI;
		unsigned __int32 chunksize;
		DWORD	rs;
		char ChunkText[5],data[8];
		BOOL CKFound = FALSE;
		

		//引数に問題あるかを確認
		if(!hWaveFile | !lpChunk)return FALSE;	
		if((lstrlenA(lpChunk)>=5) || (lstrlenA(lpChunk)==0))return FALSE;

		//検索対象のチャンク名の準備(lpChunkの文字列のサイズに最大3バイトの空白を追加する。)
		memset(ChunkText,0,5);
		memcpy(ChunkText,lpChunk,lstrlenA(lpChunk));
		if(lstrlenA(lpChunk)!=4)memset(ChunkText+lstrlenA(lpChunk),' ',(4-lstrlenA(lpChunk)));

		//最初のサブチャンクの位置を代入
		LI.QuadPart = RIFF_FIRST_SUBCHUNKPOS;
		/*
			以下に提示したいずれかの条件に該当するまで無限ループでスキャン
			条件1：見つかる
			条件2：ファイルの終端にたどり着く
			条件3：ループ内で使用した関数でエラーが起きた

		*/
		while(1){
			//ファイルポインタの移動に失敗した場合はループから外れる
			if(!SetFilePointerEx(hWaveFile,LI,NULL,FILE_BEGIN))break;

			//8バイト(チャンク名+チャンクサイズ)分の読み込みに失敗した場合はループから外れる
			if(ReadFile(hWaveFile,data,8,&rs,NULL)==NULL)break;
			
			//8バイト読めていなければループから外れる
			if(rs != 8)break;
		
			//指定されたチャンクならばCKFound = TRUE として ループを外れる(成功)
			if(!strncmp(ChunkText,data,4)){
				CKFound = TRUE;
				break;
			}
			/*
				次ループでの読み込み位置を次の手順で計算
				手順1.チャンクサイズを取得
				手順2.手順1の結果が奇数ならば1加算して偶数にする
				手順3.手順2の結果 + 8をする。
				手順4.LI.QuadPartに手順3の結果を加算

			*/
			//チャンクサイズを取得
			memcpy(&chunksize,data+4,sizeof(unsigned __int32));
			
			//chunksizeが奇数ならば1加算して偶数にする
			if(chunksize%2)chunksize++;

			//chunksizeに8(チャンク名 + チャンクサイズの分)を加算
			chunksize+=8;

			//LI.QuadPartにchunksizeを加算
			LI.QuadPart += chunksize;
		}

		//ループ内で見つかってない場合は0を代入
		if(!CKFound)LI.QuadPart = 0;

		//指定されたチャンクの位置を返す
		return (unsigned __int32)LI.QuadPart;
	}



	//WAVEファイルの指定したチャンクのサイズを返す関数
	unsigned __int32	GetWaveChunkSize(HANDLE hWaveFile , char *lpChunk){
		LARGE_INTEGER LI;
		unsigned __int32 size;
		DWORD rs;

		//チャンクの位置を取得
		if(!(LI.QuadPart=GetWaveChunkPos(hWaveFile,lpChunk)))return 0;

		//4バイト分加算
		LI.QuadPart+=4;

		//ファイルポインタの位置ををLI.QuadPartに変更
		if(!SetFilePointerEx(hWaveFile,LI,NULL,FILE_BEGIN))return 0;

		//4バイト分読みこむ
		if(ReadFile(hWaveFile,&size,4,&rs,NULL)==NULL)return 0;
		
		//4バイト読めてなければ0を返す
		if(rs != 4)	return 0;

		//チャンクのサイズを返す
		return size;
	}

	//WAVEファイルの指定したチャンクデータを取得する関数
	unsigned __int32	ReadWaveChunkData(HANDLE hWaveFile , char *lpChunk, BYTE *lpData , unsigned __int32 readsize){
		LARGE_INTEGER LI;
		unsigned __int32 Offset,readingsize;
		DWORD rs;
		if(!lpData || (readsize<0))return 0;
		if(!(Offset = GetWaveChunkPos(hWaveFile,lpChunk)))return 0;
		readingsize = GetWaveChunkSize(hWaveFile,lpChunk);
		if((readsize)&&(readingsize > readsize))readingsize = readsize;
		
		LI.QuadPart = Offset+8;
		if(!SetFilePointerEx(hWaveFile,LI,NULL,FILE_BEGIN))return 0;
		if(!ReadFile(hWaveFile,lpData,readingsize,&rs,NULL))return 0;
		return rs;
	}

	
	/*
		波形データ読み込み関連
	*/


	//波形データをサンプル単位で読み込む関数
	unsigned __int32	ReadWaveDataSamples(HANDLE hWaveFile , void *lpdata, unsigned __int32 StartSamples  , unsigned __int32 ReadSamples){
		WaveFileInfo WFI;
		LARGE_INTEGER LI;
		DWORD	rs;
		unsigned  __int32 StartPos,ReadSize;
		if((!hWaveFile)||(!lpdata))return 0;
		
		//情報を取得
		GetWaveInfo(hWaveFile,&WFI);

		//読み込み開始位置を計算
		StartPos = WFI.tagWaveFileBaseHeader.Fmt.tagPCMWaveFormat.wf.nBlockAlign * StartSamples;

		//読み込みサイズを計算
		ReadSize = WFI.tagWaveFileBaseHeader.Fmt.tagPCMWaveFormat.wf.nBlockAlign * ReadSamples;
		
		//読み込みサイズ調整
		if((StartPos + ReadSize - 1) >= WFI.tagWaveFileBaseHeader.Data.uWaveFormSize)
			ReadSize = WFI.tagWaveFileBaseHeader.Data.uWaveFormSize - StartPos;

		//ファイルポインタを移動
		LI.QuadPart = StartPos + WFI.FileInfomation.uDataChunkOffset + 8;
		if(!SetFilePointerEx(hWaveFile,LI,NULL,FILE_BEGIN))return 0;

		//読み込む
		if(!ReadFile(hWaveFile,lpdata,ReadSize,&rs,NULL))return 0;

		return rs;
	}
	
	//波形データを秒単位で読み込む関数
	unsigned __int32	ReadWaveDataSeconds(HANDLE hWaveFile , void *lpdata ,unsigned __int32 StartSeconds  , unsigned __int32 ReadSeconds){
		WaveFileInfo WFI;
		//LARGE_INTEGER LI;
		//DWORD	rs;
		unsigned  __int32 StartSamples,ReadSamples;
		if((!hWaveFile)||(!lpdata))return 0;
		
		//情報を取得
		GetWaveInfo(hWaveFile,&WFI);
		//読み込み開始位置を計算
		StartSamples = WFI.tagWaveFileBaseHeader.Fmt.tagPCMWaveFormat.wf.nSamplesPerSec * StartSeconds;

		//読み込みサンプル数を計算
		ReadSamples = WFI.tagWaveFileBaseHeader.Fmt.tagPCMWaveFormat.wf.nSamplesPerSec * ReadSeconds;
		
		//読み込む
		return ReadWaveDataSamples(hWaveFile,lpdata , StartSamples,ReadSamples);
	}

	/*
		書き込み用フォーマット情報準備関連関数
	*/
	//指定されたフォーマット情報を構造体に入れる
	BOOL	SetWaveFormatToStruct(LPWaveFileBaseHeader lpwfh ,WORD NumberOfFormatTag , WORD NumberOfChannels , DWORD SamplePerSec   , WORD BitPerSample){
		if(!lpwfh)return FALSE;
		
		//既定のWAVEファイルヘッダーを取得する
		GetWaveDefaultHeader(lpwfh,0);

		//フォーマット識別子を代入
		lpwfh->Fmt.tagPCMWaveFormat.wf.wFormatTag = NumberOfFormatTag;

		//チャンネル数を代入
		lpwfh->Fmt.tagPCMWaveFormat.wf.nChannels = NumberOfChannels;

		//サンプリング周波数を代入
		lpwfh->Fmt.tagPCMWaveFormat.wf.nSamplesPerSec = SamplePerSec;

		//1サンプル当たりのビット数(量子化ビット数)を代入
		lpwfh->Fmt.tagPCMWaveFormat.wBitsPerSample = BitPerSample;
		
		//ブロックサイズを計算(量子化ビット数÷8  × チャンネル数)
		lpwfh->Fmt.tagPCMWaveFormat.wf.nBlockAlign = BitPerSample/8  *  NumberOfChannels;

		//1秒間当たりのサイズを計算(サンプリング周波数 × ブロックサイズ)
		lpwfh->Fmt.tagPCMWaveFormat.wf.nAvgBytesPerSec  = SamplePerSec * lpwfh->Fmt.tagPCMWaveFormat.wf.nBlockAlign;
		return TRUE;
	}

	//既定のWAVEファイルヘッダーを取得する
	BOOL	GetWaveDefaultHeader(LPWaveFileBaseHeader lpwfh,int mode){
		if(!lpwfh)return FALSE;
		memset(lpwfh,0,sizeof(WaveFileBaseHeader));
		
		//各種チャンク名・固定なサイズを代入
		memcpy(lpwfh->sChunkID,"RIFF",4);
		memcpy(lpwfh->szFromType,"WAVE",4);
		memcpy(lpwfh->Fmt.sChunkID,"fmt ",4);
		memcpy(lpwfh->Data.sChunkID,"data",4);
		lpwfh->Fmt.uPCMWaveFormatSize = sizeof(PCMWAVEFORMAT);
		
		//引数：modeの判定
		switch(mode){
			case GDWFI_CD:
				//音楽CDのフォーマット情報を書き込む
				//(フォーマット：PCM , チャンネル数：2(ステレオ), 標本化周波数：44.1Khz  , 量子化ビット数：16bit
				return SetWaveFormatToStruct(lpwfh,WAVE_FORMAT_PCM,2,44100,16);
			default:
				return TRUE;
		}
	}
	
	/*
		書き込み関連関数
	*/

	//ヘッダーを書き込む
	BOOL	WriteWaveHeader(HANDLE hWaveFile , LPWaveFileBaseHeader lpwfh){
		//lpwfhがNULLの時の対策用に使用する
		WaveFileBaseHeader wfh;

		DWORD dw;

		//ファイルハンドルがNULLならFALSEを返す。
		if(!hWaveFile)return FALSE;

		//ファイルポインタを先頭に移動
		if(SetFilePointer(hWaveFile,0,0,FILE_BEGIN)==INVALID_SET_FILE_POINTER)return FALSE;

		//書き込むデータを準備
		if(lpwfh)memcpy(&wfh , lpwfh , sizeof(WaveFileBaseHeader));
		else GetWaveDefaultHeader(&wfh,0);

		//データ確認
		wfh.Fmt.uPCMWaveFormatSize = sizeof(wfh.Fmt) - 8;

		//書き込む
		if(!WriteFile(hWaveFile,&wfh,sizeof(WaveFileBaseHeader),&dw,NULL))return FALSE;

		return (dw == sizeof(WaveFileBaseHeader));
	}

	//波形データを書き込む
	BOOL	WriteWaveData(HANDLE hWaveFile , void *lpData , unsigned __int32 size){
		DWORD dw;
		if((!hWaveFile)||(!lpData)||(!size))return FALSE;
		
		//ファイルサイズがヘッダサイズに満たない場合は
		//チャンク名とNULLが書かれた仮定のデータを書き込む
		if(GetWaveFileSize(hWaveFile) < sizeof(WaveFileBaseHeader))
			if(!WriteWaveHeader(hWaveFile,0))return FALSE;

		//ファイルの最後にポインタを移動
		if(SetFilePointer(hWaveFile,0,0,FILE_END)==INVALID_SET_FILE_POINTER)return FALSE;

		//書き込む
		if(!WriteFile(hWaveFile,lpData,size,&dw,NULL))return FALSE;

		return (dw == size);
	}
	/*量子化ビット変換用*/
	//(8/16/24/32)bit -> 32bit float
	WAVE32F	WaveNBitSampleTo32BitFloat(void *data,int bitrate){
		WAVE32F w32f;
		if(!data)return 0;
		switch(bitrate){
			case 8:
				{
					WAVE8S w8;
					memcpy(&w8,data,1);
					w8-=128;
					if(w8 < 0)w32f= (WAVE32F)w8 /(WAVE32F) WAVE8S_MIN*WAVE32F_MIN;
					else w32f= (WAVE32F)w8 /(WAVE32F) WAVE8S_MAX*WAVE32F_MAX;
				}
				break;
			case 16:
				{
					WAVE16 w16;
					memcpy(&w16,data,2);
					if(w16 < 0)w32f = (WAVE32F)w16 / (WAVE32F)WAVE16_MIN * WAVE32F_MIN;
					else w32f = (WAVE32F)w16 / (WAVE32F)WAVE16_MAX * WAVE32F_MAX;
				}
				break;
#ifdef	__cplusplus
			case 24:
				{
					WAVE24 w24;
					memcpy(&w24,data,3);
					if(w24 < 0)w32f = (WAVE32F)w24 / (WAVE32F)WAVE24_MIN * WAVE32F_MIN;
					else w32f = (WAVE32F)w24 / (WAVE32F)WAVE24_MAX * WAVE32F_MAX;
				}
				break;
#endif
			case 32:
				{
					WAVE32 w32;
					memcpy(&w32,data,4);
					if(w32 < 0)w32f = (WAVE32F)w32 / (WAVE32F)WAVE32_MIN * WAVE32F_MIN;
					else w32f = (WAVE32F)w32 / (WAVE32F)WAVE32_MAX * WAVE32F_MAX;
				}
				break;
			default:
				return 0;


		}
		return w32f;
	}

	//32bit float ->(8/16/24/32)bit
	BOOL	Wave32BitFloatSampleToNBit(WAVE32F indata , void *outdata , int targetbit){
		WAVE8S w8s;
		if(!outdata)return FALSE;
		//WAVE32F_MIN ≦ indata ≦ WAVE32F_MAXの範囲になるようにクリッピング
		indata = max(WAVE32F_MIN , min(WAVE32F_MAX,indata));
		switch(targetbit){
			case 8:
				w8s =(WAVE8S)((indata < 0)?(-1*indata*WAVE8S_MIN):(indata * WAVE8S_MAX));
				*((WAVE8*)outdata)= w8s + 128;
				break;
			case 16:
				*((WAVE16*)outdata) = (WAVE16)((indata < 0)?((-1*indata*WAVE16_MIN)):((indata * WAVE16_MAX)));
				break;
#ifdef	__cplusplus
			case 24:
				*((WAVE24*)outdata) = (WAVE32)((indata < 0)?(-1*indata*WAVE24_MIN):(indata * WAVE24_MAX));
				break;
#endif
			case 32:
				*((WAVE32*)outdata) = (WAVE32)((indata < 0)?(-1*indata*WAVE32_MIN):(indata * WAVE32_MAX));
				break;
			default:
				
				return FALSE;
		}
		return TRUE;
	}

	/*
		データ編集援助用
	*/
	BOOL			WaveNormalize(void *indata  , unsigned __int32 insize, double *outdata , PCMWAVEFORMAT *informat){
		int sampleperbyte , looploop;
		unsigned int pos , outpos;
		if(!indata || !insize || !outdata || !informat)return FALSE;
		sampleperbyte = informat->wf.nBlockAlign;
		if(insize%sampleperbyte)return FALSE;

		for(outpos = pos = 0 ; pos  < insize ; pos+=sampleperbyte , outpos+=informat->wf.nChannels){
			switch(informat->wBitsPerSample){
				case 8:
					{
						WAVE8S w8;
						for(looploop = 0 ; looploop  < informat->wf.nChannels ; looploop++){
							memcpy(&w8,(WAVE8*)((WAVEDN*)indata + pos) + looploop,1);
							w8-=128;
							if(w8 < 0) *(outdata + outpos + looploop) = (double)w8 /(double) WAVE8S_MIN*WAVE32F_MIN;
							else *(outdata + outpos + looploop)= (double)w8 /(double) WAVE8S_MAX*WAVE32F_MAX;
						}
					}
					break;
				case 16:
					{
						WAVE16 w16;
						for(looploop = 0 ; looploop  < informat->wf.nChannels ; looploop++){
							memcpy(&w16,(WAVE16*)((WAVEDN*)indata + pos) + looploop,2);
							if(w16 < 0)*(outdata + outpos + looploop)  = (double)w16 / (double)WAVE16_MIN * WAVE32F_MIN;
							else *(outdata + outpos + looploop)  = (double)w16 / (double)WAVE16_MAX * WAVE32F_MAX;
						}
					}
					break;
				
#ifdef	__cplusplus
				case 24:
					{
						WAVE24 w24;
						for(looploop = 0 ; looploop  < informat->wf.nChannels ; looploop++){
							memcpy(&w24,(WAVE24*)((WAVEDN*)indata + pos) + looploop,3);
							if(w24 < 0)*(outdata + outpos + looploop)  = (double)w24 / (double)WAVE24_MIN * WAVE32F_MIN;
							else *(outdata + outpos + looploop)  = (double)w24 / (double)WAVE24_MAX * WAVE32F_MAX;
						}
					}
					break;				
#endif
				case 32:
					{
						WAVE32 w32;
						if(informat->wf.wFormatTag == WAVE_FORMAT_PCM){
							for(looploop = 0 ; looploop  < informat->wf.nChannels ; looploop++){
								memcpy(&w32,(WAVE32*)((WAVEDN*)indata + pos) + looploop,4);
								if(w32 < 0)*(outdata + outpos + looploop)  = (double)w32 / (double)WAVE32_MIN * WAVE32F_MIN;
								else *(outdata + outpos + looploop)  = (double)w32 / (double)WAVE32_MAX * WAVE32F_MAX;
							}
						}else{
							for(looploop = 0 ; looploop  < informat->wf.nChannels ; looploop++){
								*(outdata + outpos + looploop)  =  *((WAVE32F*)((WAVEDN*)indata + pos) + looploop);
							}
						}
					}
					break;
			}
		}
			
		return TRUE;
	}

	BOOL			WaveUnNormalize(double *indata  ,unsigned __int32 insize ,void *outdata ,PCMWAVEFORMAT  *outformat){
		unsigned int pos;
		double data;
		if(!indata || !insize || !outdata || !outformat)return FALSE;
		if(insize % (sizeof(double) * outformat->wf.nChannels))return FALSE;
		
		for(pos = 0 ; pos < insize ; pos +=8){
			data = min(WAVE32F_MAX,max(*(indata + pos/8),WAVE32F_MIN));
			switch(outformat->wBitsPerSample){
				case 8:
					{
						WAVE8S w8s;
						w8s =(WAVE8S)((data < 0)?(-1*data*WAVE8S_MIN):(data * WAVE8S_MAX));
						*((WAVE8*)outdata + pos/8)= w8s + 128;
					}
					break;
				case 16:
					*((WAVE16*)outdata + pos/8)= (WAVE16)((data < 0)?(-1*data*WAVE16_MIN):(data * WAVE16_MAX));
					break;
				case 24:
					*((WAVE24*)outdata + pos/8)= (WAVE32)((data < 0)?(-1*data*WAVE24_MIN):(data * WAVE24_MAX));
					break;
				case 32:
					if(outformat->wf.wFormatTag == WAVE_FORMAT_PCM){
						*((WAVE32*)outdata + pos/8) = (WAVE32)((data < 0)?(-1*data*WAVE32_MIN):(data * WAVE32_MAX));
					}else{
						*((WAVE32F*)outdata + pos/8) = (WAVE32F)data;
					}
					break;
			}
		}


		return TRUE;
	}

	BOOL			WaveCutChannel(void *indata ,unsigned __int32 insize, void *outL_Channel , void *outR_Channel , int bitrate){
		int sampleperbyte;
		unsigned int pos , outpos;
		if(!indata || !insize || !outL_Channel || !outR_Channel || !bitrate)return FALSE;
		sampleperbyte = bitrate/8 * 2;
		if(insize % sampleperbyte)return FALSE;
		for(outpos = pos = 0 ; pos  < insize ; pos+=sampleperbyte, outpos+=sampleperbyte/2){
			memcpy((WAVEDN*)outL_Channel + outpos , (WAVEDN*)indata + pos , sampleperbyte/2);
			memcpy((WAVEDN*)outR_Channel + outpos , (WAVEDN*)indata + pos + sampleperbyte/2, sampleperbyte/2);
		}
		return TRUE;
	}

	BOOL			WaveMixChannel(void *inL_Channel , void *inR_Channel , unsigned __int32 insize , void *outdata , int bitrate){
		int sampleperbyte;
		unsigned int pos , outpos;
		if(!outdata || !insize || !inL_Channel || !inR_Channel || !bitrate)return FALSE;
		sampleperbyte = bitrate/8;
		if(insize % sampleperbyte)return FALSE;
		for(outpos = pos = 0 ; pos  < insize ; pos+=sampleperbyte , outpos+=sampleperbyte*2){
			memcpy((WAVEDN*)outdata + outpos , (WAVEDN*)inL_Channel + pos , sampleperbyte);
			memcpy((WAVEDN*)outdata + outpos + sampleperbyte , (WAVEDN*)inR_Channel + pos , sampleperbyte);
		}
		return TRUE;
	}

	/*
		ダイアログ系
	*/

	//WAVEファイル選択ダイアログ（開く用)
	BOOL			WaveFileOpenDialog(HWND hwnd , TCHAR *lpszFileName , UINT32 size , TCHAR *lpszDefaultDirectory){
		TCHAR CurrentPath[MAX_PATH+1];
		OPENFILENAME ofn;
		if(!lpszFileName || !size)return FALSE;
		GetCurrentDirectory(MAX_PATH,CurrentPath);
		memset(&ofn,0,sizeof(OPENFILENAME));
		ofn.lStructSize  = sizeof(OPENFILENAME);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = lpszFileName;
		ofn.nMaxFile = size;
		ofn.lpstrFilter = TEXT("WAVE Files(*.wav)\0*.wav\0\0");
		ofn.lpstrTitle = TEXT("開くWAVEファイルを選択してください");
		if(lpszDefaultDirectory) ofn.lpstrInitialDir = lpszDefaultDirectory;
		else ofn.lpstrInitialDir = CurrentPath;
		ofn.Flags=OFN_HIDEREADONLY |  OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		return GetOpenFileName(&ofn);
	}
	
	//WAVEファイル選択ダイアログ（保存用)
	BOOL			WaveFileSaveDialog(HWND hwnd , TCHAR *lpszFileName , UINT32 size , TCHAR *lpszDefaultDirectory){
		TCHAR CurrentPath[MAX_PATH+1];
		OPENFILENAME ofn;
		if(!lpszFileName || !size)return FALSE;
		GetCurrentDirectory(MAX_PATH,CurrentPath);
		memset(&ofn,0,sizeof(OPENFILENAME));
		ofn.lStructSize  = sizeof(OPENFILENAME);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = lpszFileName;
		ofn.nMaxFile = size;
		ofn.lpstrFilter = TEXT("WAVE Files(*.wav)\0*.wav\0\0");
		ofn.lpstrTitle = TEXT("保存先のWAVEファイルを指定してください");
		if(lpszDefaultDirectory) ofn.lpstrInitialDir = lpszDefaultDirectory;
		else ofn.lpstrInitialDir = CurrentPath;
		ofn.Flags=OFN_HIDEREADONLY |  OFN_EXPLORER | OFN_OVERWRITEPROMPT;
		if(GetSaveFileName(&ofn)){
			PathAddExtension(lpszFileName,TEXT(".wav"));
			return TRUE;
		}
		return FALSE;
	}


	/*
		メモリ関係
	*/
	void *				WaveMemoryAlloc(WORD NumberOfChannels , DWORD NumberOfSamples , WORD BitPerSample){
		SIZE_T size;

		size = NumberOfChannels * NumberOfSamples * (BitPerSample/8);

		return HeapAlloc(GetProcessHeap() , HEAP_ZERO_MEMORY , size);
	}

	BOOL				WaveMemoryFree(void *lpData){
		BOOL ret;
		if(!lpData)return FALSE;
		ret = HeapFree(GetProcessHeap() , NULL , lpData);
		return ret;
	}

#endif	/*WAVEFUNC_CPP*/