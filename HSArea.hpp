#ifndef __HSAREA__
#define __HSAREA__


#define HSAREA_PROPERTY(GetFunc , SetFunc , Type , Name) __declspec(property(get = GetFunc , put = SetFunc)) Type Name
#define HSAREA_PROPERTY_R(GetFunc , Type , Name) __declspec(property(get = GetFunc)) Type Name

enum EHSAreaType {
	EHSAreaType_Rectangle = 0,
	EHSAreaType_Circle
};



template <typename T> struct THSPoint {
	T x;
	T y;
};

template <typename T> struct THSize {
	T Width;
	T Height;
};

template <typename T> struct THSArea {
	T left;
	T right;
	T top;
	T bottom;
};



template <typename T> class CHSArea : public THSArea<T>{
private:
	EHSAreaType _InnnerAreaType;

public:

	CHSArea () {
		_InnnerAreaType = EHSAreaType_Rectangle;
		left = right = top = bottom = 0; 
	}

	void Set ( T left , T top , T right , T bottom ) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
	void Set ( THSArea<T> tArea ) {
		this->left = tArea.left;
		this->top = tArea.top;
		this->right = tArea.right;
		this->bottom = tArea.bottom;
	}



	EHSAreaType SetAreaType ( EHSAreaType Type ) {
		_InnnerAreaType = Type;
		return Type;
	}

	EHSAreaType GetAreaType ( void ) {
		return _InnnerAreaType;
		
	}

	//__declspec ( property( get = GetAreaType , put = SetAreaType ) ) EHSAreaType AreaType;
	HSAREA_PROPERTY ( GetAreaType , SetAreaType , EHSAreaType , AreaType );

	THSPoint<T> GetCenter ( void ) {
		THSPoint<T> pt;
		pt.x = ( left + right ) / 2;
		pt.y = ( top + bottom ) / 2;
		return pt;
	}

	void SetCenter ( T x , T y ) {
		THSPoint<T> c = this->GetCenter ();
		this->MoveOffset ( x - c.x , y - c.y );
	}
	void SetCenter ( THSPoint<T> ptCenter ) {
		THSPoint<T> c = this->GetCenter ();
		this->MoveOffset ( ptCenter.x - c.x , ptCenter.y - c.y );
	}

	THSize<T> GetSize (void) {
		THSize<T> s;
		s.Width = right - left;
		s.Height = bottom - top;
		return s;
	}


	void SetSize ( T Width , T Height ) {
		this->right = this->left + Width;
		this->bottom = this->top + Height;
	}
	void SetSize ( THSize<T> size ) {
		this->SetSize ( size.Width , size.Height );
	}






	THSize<T> GetPixelSize ( void ) {
		THSize<T> s = this->GetSize ();
		s.Width++;
		s.Height++;
		return s;
	}

	void SetPixelSize ( T Width , T Height  ) {
		this->SetSize ( Width - 1, Height  - 1);
	}
	void SetPixelSize ( THSize<T> size ) {
		this->SetPixelSize ( size.Width , size.Height );
	}

	T GetCircleRadius () {
		THSize<T> s = this->GetSize ();
		T m = ( s.Width < s.Height ) ? s.Width : s.Height;
		return m / 2;
	}

	void SetCircleRadius ( T radius ) {

		THSPoint<T> pt = this->GetCenter ();

		this->left = pt.x - radius;
		this->top = pt.y - radius;
		this->right = pt.x + radius;
		this->bottom = pt.y + radius;

	}


	void MoveTo ( T left , T top ){
		T offsetX = left - this->left;
		T offsetY = top - this->top;
		this->right += offsetX;
		this->bottom += offsetY;

		this->left = left;
		this->top = top;

	}

	void MoveOffset ( T OffsetX , T OffsetY ) {
		this->left += OffsetX;
		this->right += OffsetX;
		this->top += OffsetY;
		this->bottom += OffsetY;

	}

	bool HitTest ( T x , T y ) {
		THSize<T> size;
		T r2;
		switch ( _InnnerAreaType ) {
			case EHSAreaType_Rectangle:
				if ( ( x >= left ) && ( x <= right ) ) {
					if ( ( y >= top ) && ( y <= bottom ) ) {
						return true;
					}
				}
				break;
			case EHSAreaType_Circle:
				size = this->GetSize ();
				if ( size.Width < size.Height ) r2 = size.Width * size.Width;
				else r2 = size.Height * size.Height;

				T xside = this->left + this->right - 2 * x;
				T yside = this->top + this->bottom - 2 * y;
				if ( r2 >= ( xside * xside + yside * yside ) ) {
					return true;
				}
				break;
		}

		return false;
	}
	bool HitTest ( THSPoint<T> pt ) {
		return this->HitTest ( pt.x , pt.y );
	}

	bool HitTest ( CHSArea<T> Area ) {
		THSize<T> size;
		if ( Area.AreaType == _InnnerAreaType ) {
			switch ( _InnnerAreaType ) {
				case EHSAreaType_Rectangle:
					if ( ( this->right >= Area.left ) && ( this->left <= Area.right ) ) {
						if ( ( this->bottom >= Area.top ) && ( this->top <= Area.bottom ) ) {
							return true;
						}
					}
					break;
				case EHSAreaType_Circle:
					T r1 , r2;
					THSPoint<T> pt1 , pt2;
					size = this->GetSize ();
					r1 = ( size.Width < size.Height ) ? size.Width : size.Height;

					size = Area.GetSize ();
					r2 = ( size.Width < size.Height ) ? size.Width : size.Height;


					pt1 = this->GetCenter ();
					pt2 = Area.GetCenter ();

					T lhs = ( r1 + r2 ) * ( r1 + r2 ) / 4;

					T rhs1 = ( pt2.x - pt1.x ) * ( pt2.x - pt1.x );
					T rhs2 = ( pt2.y - pt1.y ) * ( pt2.y - pt1.y );

					if ( lhs >= ( rhs1 + rhs2 ) ) {
						return true;
					}
					break;
			}
		} else {

			CHSArea *pCircleSide , *pRectangleSide;
			if ( this->_InnnerAreaType == EHSAreaType_Rectangle ) {
				pRectangleSide = this;
				pCircleSide = &Area;
			} else {
				pRectangleSide = &Area;
				pCircleSide = this;
			}

			THSPoint<T> CircleCenter = pCircleSide->GetCenter ();
			THSize<T> CircleSize = pCircleSide->GetSize ();


			T Radius = ( CircleSize.Width < CircleSize.Height ) ? CircleSize.Width  / 2: CircleSize.Height / 2;



			//í∑ï˚å`ÇÃí∏ì_Ç™â~Ç…ê⁄êGÇµÇƒÇ¢Ç»Ç¢Ç©ÇämîF

			//ç∂è„
			if ( pCircleSide->HitTest ( pRectangleSide->left , pRectangleSide->top ) ) return true;
			
			//âEè„
			if ( pCircleSide->HitTest ( pRectangleSide->right , pRectangleSide->top ) ) return true;
			
			//âEâ∫
			if ( pCircleSide->HitTest ( pRectangleSide->right , pRectangleSide->bottom ) ) return true;
			
			//ç∂â∫
			if ( pCircleSide->HitTest ( pRectangleSide->left , pRectangleSide->bottom ) ) return true;


			//â~Ç™í∑ï˚å`ÇÃì‡ïîÇ…Ç†ÇÈÇ©
			if ( pRectangleSide->HitTest ( CircleCenter ) ) return true;

			
			//ï”Ç∆â~Ç™ê⁄êGÇµÇƒÇ»Ç¢Ç©(å„ì˙é¿ëï)

		}
		return false;
	}

	bool HitTest ( THSArea<T> Area , EHSAreaType Type ) {
		CHSArea<T> ca;
		ca.Set ( Area );
		ca.AreaType = Type;
		return this->HitTest(ca);
	}

	/*
	//å„ì˙é¿ëïó\íË
	bool HitTestLine ( T x1 , T y1 , T x2 , T y2 ) {
		switch ( _InnnerAreaType ) {
			case EHSAreaType_Rectangle:

				break;
			case EHSAreaType_Circle:
				break;
		}



		return false;
	}
	*/
};


#endif