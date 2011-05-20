#ifndef EGE_CORE_VECTOR2_H
#define EGE_CORE_VECTOR2_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class TVector2
{
	public:

    TVector2() : x(0), y(0){}
    TVector2(T x, T y) : x(x), y(y){}
		TVector2(const TVector2& cVector) : x(cVector.x), y(cVector.y){}
   ~TVector2(){}

		//CVector2 operator=( const CVector2& cVector );
		//CVector2 operator+( const CVector2& cVector ) const;
		//CVector2 operator-( const CVector2& cVector ) const;
		//CVector2 operator*( float fNumber ) const;
		//CVector2 operator/( float fNumber ) const;

		//void operator+=( const CVector2& cVector );
		//void operator-=( const CVector2& cVector );

  //  bool operator==( const CVector2& cVector ) const;
  //  bool operator!=( const CVector2& cVector ) const;

  //  // create related methods
    inline void set(T x, T y) { this->x = x; this->y = y; }
 
  //  // magnitude related methods
  //  float getMagnitude( void ) const;                                             // returns length of the vector
  //  float getSquaredMagnitude( void ) const;                                      // returns squared length of the vector

  //  // normalization related methods
  //  CVector2 getNormalized( void );                                               // returns normalized vector
  //  void     normalize( void );                                                   // normalizes the vector

  //  // helper methods
  //  inline float getMax( void ) const { return _MAX( x, y ); }                    // gets max value of its components
  //  inline float getMin( void ) const { return _MIN( x, y ); }                    // gets min value of its components

  //  inline CVector2 getPerpendicular( void ) const { return CVector2( -y, x ); }  // gets perpendicular vector


    T x;
		T y;

    // statics
    //static const CVector2 ZERO;
    //static const CVector2 UNIT_X;
    //static const CVector2 UNIT_Y;
    //static const CVector2 NEGATIVE_UNIT_X;
    //static const CVector2 NEGATIVE_UNIT_Y;
    //static const CVector2 ONE;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VECTOR2_H