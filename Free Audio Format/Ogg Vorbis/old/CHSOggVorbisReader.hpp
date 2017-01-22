#pragma once

#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>
#include <Windows.h>
#include <Shlwapi.h>


#pragma comment(lib,"Shlwapi.lib")

enum struct EHSOggVorbisFileOpenError {
	WithoutOpenProcess = 0,
	ParameterInvalid,
	HasAnotherFileOpened,
	Success,
	OpenError,
	NotFoundFile,
	OVE_READ ,
	OVE_NOTVORBIS ,
	OVE_VERSION ,
	OVE_BADHEADER ,
	OVE_FAULT
};


template <typename T> class CHSOggVorbisReaderBaseTemplate {

protected:
	HANDLE hFile;
	OggVorbis_File oggFile;
	EHSOggVorbisFileOpenError LastOpenError;
	vorbis_info info;
	vorbis_comment comment;
	WAVEFORMATEX wfex;
protected:
	void ClassInit ( T* lpszFilePath ) {
		hFile = NULL;
		LastOpenError = EHSOggVorbisFileOpenError::WithoutOpenProcess;
		if ( lpszFilePath != nullptr ) this->Open ( lpszFilePath );
	}

	void SetLastOpenErrorFromVorbisOpenError ( int er ) {
		switch ( er ) {
			case 0:
				LastOpenError = EHSOggVorbisFileOpenError::Success;
				break;
			case OV_EREAD:
				LastOpenError = EHSOggVorbisFileOpenError::OVE_READ;
				break;
			case OV_ENOTVORBIS:
				LastOpenError = EHSOggVorbisFileOpenError::OVE_NOTVORBIS;
				break;
			case OV_EVERSION:
				LastOpenError = EHSOggVorbisFileOpenError::OVE_VERSION;
				break;
			case OV_EBADHEADER:
				LastOpenError = EHSOggVorbisFileOpenError::OVE_BADHEADER;
				break;
			case OV_EFAULT:
				LastOpenError = EHSOggVorbisFileOpenError::OVE_FAULT;
				break;
		}
	}

	static size_t Raw_Read ( void *ptr , size_t size , size_t nmemb , void *datasource ){
		return ( ( CHSOggVorbisReaderBaseTemplate* ) datasource )->Inner_Raw_Read(ptr , size , nmemb);
	}
	static int Raw_Seek ( void *datasource , ogg_int64_t offset , int whence ) {
		return ( ( CHSOggVorbisReaderBaseTemplate* ) datasource )->Inner_Raw_Seek(offset , whence);
	}
	static int Raw_Close ( void *datasource ) {
		return ( ( CHSOggVorbisReaderBaseTemplate* ) datasource )->Inner_Raw_Close();
	}
	static long Raw_Tell ( void *datasource ) {
		return ( ( CHSOggVorbisReaderBaseTemplate* ) datasource )->Inner_Raw_Tell ( );
	}
	
	size_t Inner_Raw_Read ( void *ptr , size_t size , size_t nmemb ) {

		if ( ( size == 0 ) || ( nmemb == 0 ) ) return 0;

	//	printf ( "Inner_Raw_Read\n" );
		DWORD outSize;
		if ( ReadFile ( this->hFile , ptr , size*nmemb , &outSize , NULL ) ) {
			return outSize / size;
		}
		return 0;
	}
	int Inner_Raw_Seek ( ogg_int64_t offset , int whence ) {
		int base;
		//printf ( "Inner_Raw_Seek\n" );

		switch ( whence ) {
			case SEEK_SET:
				base = FILE_BEGIN;
				break;
			case SEEK_CUR:
				base = FILE_CURRENT;
				break;
			case SEEK_END:
				base = FILE_END;
				break;
			default:
				return -1;
		}

		LARGE_INTEGER li;

		li.QuadPart = offset;

		if ( SetFilePointerEx ( this->hFile , li , nullptr , base ) == FALSE ) {
			return -1;
		}

		return 0;
	}
	int Inner_Raw_Close (void ) {
		//printf ( "Inner_Raw_Close\n" );

		if ( CloseHandle ( this->hFile ) == TRUE ) {
			this->hFile = NULL;
			return 0;
		}
		return EOF;
	}
	long Inner_Raw_Tell ( void ) {
		LARGE_INTEGER zero , li;
		zero.QuadPart = 0;
		//printf ( "Inner_Raw_Tell\n" );

		if ( SetFilePointerEx ( this->hFile , zero , &li , FILE_CURRENT ) ) {
			return ( long ) li.QuadPart;
		}
		
		return -1;
	}


	bool OnAfterOpenEvent ( void ){

		ov_callbacks cb;

		cb.read_func = this->Raw_Read;
		cb.seek_func = this->Raw_Seek;
		cb.tell_func = this->Raw_Tell;
		cb.close_func = this->Raw_Close;

		int ret = ov_open_callbacks ( (void*)this , &this->oggFile , NULL , 0 , cb );

		this->SetLastOpenErrorFromVorbisOpenError ( ret );

		if ( ret != 0 ) {
			this->Inner_Raw_Close ( );
			return false;
		}

		this->info = *ov_info ( &this->oggFile , -1 );
		this->comment = *ov_comment ( &this->oggFile , -1 );
		this->MakeWaveFormatExStruct ( );
		return true;
	}

	void MakeWaveFormatExStruct ( void ) {
		wfex.cbSize = 0;
		wfex.nChannels = this->info.channels;
		wfex.nSamplesPerSec = this->info.rate;
		wfex.wBitsPerSample = 16;
		wfex.wFormatTag = WAVE_FORMAT_PCM;
		wfex.nBlockAlign = wfex.wBitsPerSample / 8 * wfex.nChannels;
		wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;
	}

public:

	virtual bool Open ( T* lpszFilePath ) = 0;
	
	bool Close ( void ){
		if ( this->IsOpened ( ) ) {
			if ( ov_clear ( &this->oggFile ) == 0 ) return true;
		}
		return false;
	}

	bool IsOpened ( void ) const{
		return ( hFile != NULL ) ? true : false;
	}

	EHSOggVorbisFileOpenError GetLastOpenError ( void ) const{
		return this->LastOpenError;
	}

	vorbis_info GetVorbisInfo ( void ) const {
		return this->info;
	}

	vorbis_comment GetVorbisComment ( void ) const {
		return this->comment;
	}

	WAVEFORMATEX GetWaveFormatEx ( void ) const {
		return this->wfex;
	}



};

using CHSOggVorbisReaderBaseA = CHSOggVorbisReaderBaseTemplate<char>;
using CHSOggVorbisReaderBaseW = CHSOggVorbisReaderBaseTemplate<wchar_t>;


class CHSOggVorbisReaderA : public CHSOggVorbisReaderBaseA {

private:
	using T = char;
public:

	CHSOggVorbisReaderA ( ) {
		this->ClassInit (nullptr);
	}
	
	CHSOggVorbisReaderA ( T* lpszFilePath ) {
		this->ClassInit ( lpszFilePath );
	}

	~CHSOggVorbisReaderA ( ){
		if ( this->IsOpened ( ) ) {
			this->Close ( );
		}
	}



	bool Open (T* lpszFilePath ) {

		if ( lpszFilePath == nullptr ) {
			this->LastOpenError = EHSOggVorbisFileOpenError::ParameterInvalid;
			return false;
		}

		if ( this->IsOpened ( ) ) {
			this->LastOpenError = EHSOggVorbisFileOpenError::HasAnotherFileOpened;
			return false;
		}

		if ( PathFileExistsA ( lpszFilePath ) == FALSE ) {
			this->LastOpenError = EHSOggVorbisFileOpenError::NotFoundFile;
			return false;
		}

		HANDLE hNextFile;

		hNextFile = CreateFileA ( lpszFilePath , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
		if ( hNextFile == INVALID_HANDLE_VALUE ) {
			this->LastOpenError = EHSOggVorbisFileOpenError::OpenError;
			return false;
		}
		
		this->hFile = hNextFile;

		return this->OnAfterOpenEvent ();
	}



};

class CHSOggVorbisReaderW : public CHSOggVorbisReaderBaseW {
private:
	using T = wchar_t;
public:

	CHSOggVorbisReaderW ( ) {
		this->ClassInit ( nullptr );
	}

	CHSOggVorbisReaderW ( T* lpszFilePath ) {
		this->ClassInit ( lpszFilePath );
	}

	~CHSOggVorbisReaderW ( ) {
		if ( this->IsOpened ( ) ) {
			this->Close ( );
		}
	}


	bool Open ( T* lpszFilePath ) {

		if ( lpszFilePath == nullptr ) {
			this->LastOpenError = EHSOggVorbisFileOpenError::ParameterInvalid;
			return false;
		}

		if ( this->IsOpened ( ) ) {
			this->LastOpenError = EHSOggVorbisFileOpenError::HasAnotherFileOpened;
			return false;
		}

		if ( PathFileExistsW ( lpszFilePath ) == FALSE ) {
			this->LastOpenError = EHSOggVorbisFileOpenError::NotFoundFile;
			return false;
		}

		HANDLE hNextFile;

		hNextFile = CreateFileW ( lpszFilePath , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
		if ( hNextFile == INVALID_HANDLE_VALUE ) {
			this->LastOpenError = EHSOggVorbisFileOpenError::OpenError;
			return false;
		}

		this->hFile = hNextFile;

		return this->OnAfterOpenEvent ( );
	}

};


