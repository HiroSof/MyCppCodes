#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "CHSOggVorbisReader.hpp"

template<typename T=char> struct THSOggVorbisCommentDataTemplate {
	std::basic_string<T> Name;
	std::basic_string<T> BaseName;
	std::basic_string<T> Value;
};

using THSOggVorbisCommentDataA = THSOggVorbisCommentDataTemplate<char>;
using THSOggVorbisCommentDataW = THSOggVorbisCommentDataTemplate<wchar_t>;

template<typename T=char> class CHSOggVorbisCommentReaderBaseTemplate {


public:
	using TStdString = std::basic_string<T>;
	using TCommentData = THSOggVorbisCommentDataTemplate<T>;
	using TStdMapArray = std::unordered_map<TStdString , TStdString>;
	using TStdVectorArray = std::vector<TCommentData>;
protected:

	static bool IsUTF8 ( char *lpText , int length ) {
		int size = length;
		if ( lpText == nullptr )return false;
		if ( size == -1 ) size = static_cast< int >( strlen ( lpText ) );
		int utf8_subsize = 0;
		bool utf8_subcheck = true;
		for ( int i = 0; i < size; i++ ) {

			unsigned char c = lpText [ i ];

			//0xF0`0xFD‚È‚çUTF-8
			if ( ( c >= 0xF0 ) && ( c <= 0xFD ) ) return true;

			//0x80`0x9F‚È‚çShift-JISã‚ÅŽŸ‚ÌƒoƒCƒg‚Æ‡‚í‚¹‚Ä‚Ì‘SŠp•¶Žš
			if ( ( c >= 0x80 ) && ( c <= 0x9F ) ) return false;

			//0xA0`0xC1‚ÍShift-JIS‚Ì”¼ŠpƒJƒ^ƒJƒi
			if ( ( c >= 0xA0 ) && ( c <= 0xC1 ) ) return false;

			utf8_subsize = 0;

			if ( ( c >= 0xC2 ) && ( c <= 0xDF ) ) utf8_subsize = 1;
			if ( ( c >= 0xE0 ) && ( c <= 0xEF ) ) utf8_subsize = 2;

			if ( utf8_subsize > 0 ) {
				utf8_subcheck = true;
				for ( int j = 1; ( j <= utf8_subsize ) && ( ( i + j ) < size ); j++ ) {
					unsigned char v = lpText [ i + j ];
					if( ( ( v >= 0x80 ) && ( v <= 0xBF ) ) == false ) {
						utf8_subcheck = false;
						break;
					}
				}
				if ( utf8_subcheck ) return true;
			}
		}
		return false;
	}


	virtual TStdString ToLower ( TStdString base ) = 0;

protected:
	TStdMapArray CommentMapArray;
	TStdVectorArray CommentVectorArray;
	TStdString Vender;
	virtual TStdString ToStdString ( char *pCommentString , int length ) = 0;
	virtual void AddCommentData ( TStdString Comment , int index ) = 0;


public:

	template <typename U> void From ( CHSOggVorbisReaderBaseTemplate<U>  &Reader ) {
		this->From ( Reader.GetVorbisComment ( ) );
	}

	void From ( vorbis_comment vc ) {
		CommentMapArray.clear ( );
		CommentVectorArray.resize ( vc.comments );
		Vender = this->ToStdString ( vc.vendor , -1 );
		TStdString str;
		for ( int i = 0; i < vc.comments; i++ ) {
			str = this->ToStdString ( vc.user_comments [ i ] , vc.comment_lengths [ i ] );
			this->AddCommentData ( str , i);
		}
	}


	TCommentData operator[](unsigned int index ) {
		if ( index >= CommentVectorArray.size ( ) ) {
			TCommentData cd;
			cd.Name = "";
			cd.BaseName = "";
			cd.Value = "";
			return cd;
		}
		return CommentVectorArray.at ( index );
	}

	TStdString operator()( TStdString Name ) {
		auto it = CommentMapArray.find ( this->ToLower ( Name ) );
		if ( it == CommentMapArray.end ( ) ) {
			TStdString s;
			s.clear ( );
			return s;
		}
		return it->second;
	}

	TStdString GetVendor ( void ) {
		return  this->Vender;
	}

	TStdMapArray GetMapArray ( void ) {
		return this->CommentMapArray;
	}

	TStdVectorArray GetVectorArray ( void ) {
		return this->CommentVectorArray;
	}

	size_t Count ( ) {
		return CommentVectorArray.size ( );
	}



};

using CHSOggVorbisCommentReaderBaseA = CHSOggVorbisCommentReaderBaseTemplate<char>;
using CHSOggVorbisCommentReaderBaseW = CHSOggVorbisCommentReaderBaseTemplate<wchar_t>;


class CHSOggVorbisCommentReaderA : public CHSOggVorbisCommentReaderBaseA {

private:

	TStdString ToLower ( TStdString base ) {
		TStdString out = base;
		std::transform ( out.cbegin ( ) , out.cend ( ) , out.begin ( ) , tolower );
		return out;
	}
	TStdString ToStdString ( char *pCommentString , int length ) {
		TStdString str;
		size_t len;

		if ( pCommentString != nullptr ) {
			len = ( length >= 0 ) ? length : strlen ( pCommentString );

			if ( len != 0 ) {
				if ( this->IsUTF8 ( pCommentString , len ) == false ) {
					str = pCommentString;
				} else {
					wchar_t *lpwideStr;
					char *lpmbcsStr;
					char *pInput = pCommentString;
					int InputSize = ( int ) len;

					//UTF8¨Unicode
					int wcharsize = MultiByteToWideChar ( CP_UTF8 , 0 , pInput , InputSize , nullptr , 0 );
					lpwideStr = new wchar_t [ wcharsize + 1 ];
					*( lpwideStr + wcharsize ) = '\0';
					MultiByteToWideChar ( CP_UTF8 , 0 , pInput , InputSize , lpwideStr , wcharsize );


					//Unicode->MBCS(Shift-JIS)
					int mbcs_size = WideCharToMultiByte ( CP_ACP , 0 , lpwideStr , -1 , nullptr , 0 , nullptr , nullptr );
					lpmbcsStr = new char [ mbcs_size + 1 ];
					*( lpmbcsStr + mbcs_size ) = '\0';
					WideCharToMultiByte ( CP_ACP , 0 , lpwideStr , -1 , lpmbcsStr , mbcs_size , nullptr , nullptr );
					delete [ ]lpwideStr;

					
					str = lpmbcsStr;
					delete [ ]lpmbcsStr;
				}

			}
		}

		return str;
	}

	void AddCommentData ( TStdString Comment , int index) {
		size_t pos = Comment.find ( "=" );
		TCommentData Data;
		Data.BaseName = Comment.substr ( 0 , pos );
		Data.Name = this->ToLower ( Data.BaseName );
		Data.Value = Comment.substr ( pos + 1 );

	//	printf ( "Add(A)[%d] \"%s\" : \"%s\"\n" , Data.Value.length ( ) ,Data.Name.c_str ( ) , Data.Value.substr(0,100).c_str() );
		this->CommentMapArray [ Data.Name ] = Data.Value;
		this->CommentVectorArray [ index ] = Data;
	}
public:
	CHSOggVorbisCommentReaderA ( ) {

	}

	template <typename U> CHSOggVorbisCommentReaderA ( CHSOggVorbisReaderBaseTemplate<U>  &Reader ) {
		this->From ( Reader );
	}
	CHSOggVorbisCommentReaderA ( vorbis_comment vc ) {
		this->From ( vc );
	}

};


class CHSOggVorbisCommentReaderW : public CHSOggVorbisCommentReaderBaseW {
private:

	TStdString ToLower ( TStdString base ) {
		TStdString out = base;
		std::transform ( out.cbegin ( ) , out.cend ( ) , out.begin ( ) , towlower );
		return out;
	}

	TStdString ToStdString ( char *pCommentString , int length ) {
		TStdString str;
		size_t len;

		if ( pCommentString != nullptr ) {
			len = ( length >= 0 ) ? length : strlen ( pCommentString );
			wchar_t *lpwideStr;
			char *pInput = pCommentString;
			int InputSize = ( int ) len;

			if ( len != 0 ) {
				if ( this->IsUTF8 ( pCommentString , len ) == false ) {
					//MBCS(Shift-JIS)¨Unicode
					int wcharsize = MultiByteToWideChar ( CP_ACP , 0 , pInput , InputSize , nullptr , 0 );
					lpwideStr = new wchar_t [ wcharsize + 1 ];
					*( lpwideStr + wcharsize ) = '\0';
					MultiByteToWideChar ( CP_ACP , 0 , pInput , InputSize , lpwideStr , wcharsize );
				} else {
					//UTF8¨Unicode
					int wcharsize = MultiByteToWideChar ( CP_UTF8 , 0 , pInput , InputSize , nullptr , 0 );
					lpwideStr = new wchar_t [ wcharsize + 1 ];
					*( lpwideStr + wcharsize ) = '\0';
					MultiByteToWideChar ( CP_UTF8 , 0 , pInput , InputSize , lpwideStr , wcharsize );
				}

				str = lpwideStr;
				delete [ ]lpwideStr;
			}
		}
		return str;
	}


	void AddCommentData ( TStdString Comment , int index ) {
		size_t pos = Comment.find ( L"=" );
		TCommentData Data;
		Data.BaseName = Comment.substr ( 0 , pos );
		Data.Name = this->ToLower ( Data.BaseName );
		Data.Value = Comment.substr ( pos + 1 );

		//wprintf ( L"Add(W)[%d] \"%s\" : \"%s\"\n" , Data.Value.length(),  Data.Name.c_str ( )  , Data.Value.c_str ( ) );
	
		this->CommentMapArray [ Data.Name ] = Data.Value;
		this->CommentVectorArray [ index ] = Data;
	}


public:

	CHSOggVorbisCommentReaderW ( ) {

	}

	template <typename U> CHSOggVorbisCommentReaderW ( CHSOggVorbisReaderBaseTemplate<U>  &Reader ) {
		this->From ( Reader );
	}

	CHSOggVorbisCommentReaderW ( vorbis_comment vc ) {
		this->From ( vc );
	}

};
