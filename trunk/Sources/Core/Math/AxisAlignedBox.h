#ifndef EGE_CORE_AXISALIGNEDBOX_H
#define EGE_CORE_AXISALIGNEDBOX_H

#include <EGE.h>
#include "Core/Math/Vector4.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AxisAlignedBox
{
  public:

    AxisAlignedBox();
	  AxisAlignedBox(const AxisAlignedBox& cBox);
	  AxisAlignedBox(const Vector4f& cMin, const Vector4f& cMax);
	 ~AxisAlignedBox();

    enum Extent
	  {
		  EXTENT_NULL,
		  EXTENT_FINITE,
		  EXTENT_INFINITE
	  };

	  //AxisAlignedBox& operator=( const AxisAlignedBox& cBox );
   // bool             operator==( const AxisAlignedBox& cBox ) const;
   // inline bool      operator!=( const AxisAlignedBox& cBox ) const { return !( *this == cBox ); }

   // // extents related methods
	  //inline void setNull( void ){ m_eExtent = EXTENT_NULL; }                                     // makes it NULL AAB
	  //inline bool isNull( void ) const { return m_eExtent == EXTENT_NULL; }                       // checks if NULL AAB

	  //inline bool isFinite( void ) const { return m_eExtent == EXTENT_FINITE; }                   // checks if finite dimensioned

   // inline void setInfinite( void ){ m_eExtent = EXTENT_INFINITE; }                             // makes it infinite 
	  //inline bool isInfinite( void ) const { return m_eExtent == EXTENT_INFINITE; }               // checkis if infinite AAB

	  void setExtents(const Vector4f& cMin, const Vector4f& cMax);

	  //inline const CVector3* getMinimum( void ) const { return &m_cMinimum; } 	                  // gets minimal extents only
	  //inline void            setMinimum( const CVector3* pcVec )                                  // sets minimal extents only
	  //{
   //   assert( pcVec != NULL && "No required parameter(s)" );

   //   m_eExtent = EXTENT_FINITE;
		 // m_cMinimum = *pcVec;
	  //}

	  //inline void setMinimumX( float x ){ m_cMinimum.x = x; }                                     // sets minimal X-axis dimension only
	  //inline void setMinimumY( float y ){ m_cMinimum.y = y; }                                     // sets minimal Y-axis dimension only
	  //inline void setMinimumZ( float z ){ m_cMinimum.z = z; }                                     // sets minimal Z-axis dimension only

	  //inline const CVector3* getMaximum( void ) const { return &m_cMaximum; } 	                  // gets maximal extents only
	  //inline void            setMaximum( const CVector3* pcVec )                                  // sets maximal extents only
	  //{
   //   assert( pcVec != NULL && "No required parameter(s)" );

   //   m_eExtent = EXTENT_FINITE;
   //   m_cMaximum = *pcVec;
	  //}

	  //inline void setMaximumX( float x ){ m_cMaximum.x = x; }                                     // sets maximal X-axis dimension only
	  //inline void setMaximumY( float y ){ m_cMaximum.y = y; }                                     // sets maximal Y-axis dimension only
	  //inline void setMaximumZ( float z ){ m_cMaximum.z = z; }                                     // sets maximal Z-axis dimension only
	  //
	  //// manipulation methods
   // void move( const CVector3* pcVector );                                                      // moves box by given vector

   // void merge( const AxisAlignedBox& cBox );                                                  // merges boxes
   // void merge( const CVector3* pcPoint );      	                                              // extends the box to encompass the specified point 
   //                                                                                             // (if needed)
   // 
   // // transformation related methods
   // void transform( const CMatrix4* pcMatrix );                                                 // transforms the box according
	  //void transformAffine( const CMatrix4* pcMatrix ); 	                                        // transforms the box according
	  //void transformAffine( const CMatrix3* pcMatrix ); 	                                        // transforms the box according

   // // helper methods
	  //inline CVector3 getCenter( void ) const                                                     // gets center of the box
	  //{
		 // assert( ( m_eExtent == EXTENT_FINITE ) && "Can't get center of a null or infinite AAB" );

		 // return CVector3( ( m_cMaximum.x+m_cMinimum.x )*0.5f, 
   //                    ( m_cMaximum.y+m_cMinimum.y )*0.5f, 
   //                    ( m_cMaximum.z+m_cMinimum.z )*0.5f );
	  //}

	  //inline CVector3 getSize( void ) const                                                       // gets size of the box
	  //{
   //   // get according to extent
		 // switch( m_eExtent )
		 // {
		 //   case EXTENT_NULL:     return CVector3::ZERO;
		 //   case EXTENT_FINITE:   return m_cMaximum-m_cMinimum;
		 //   case EXTENT_INFINITE: return CVector3( Math::POS_INFINITY, Math::POS_INFINITY, Math::POS_INFINITY );
		 // }

   //   assert( false && "Never reached" );
   //   return CVector3::ZERO;
	  //}

	  //inline CVector3 getHalfSize( void ) const                                                   // gets half-size of the box
	  //{
   //   // get according to extent
		 // switch( m_eExtent )
		 // {
		 //   case EXTENT_NULL:     return CVector3::ZERO;
		 //   case EXTENT_FINITE:   return ( m_cMaximum-m_cMinimum )*0.5f;
		 //   case EXTENT_INFINITE: return CVector3( Math::POS_INFINITY, Math::POS_INFINITY, Math::POS_INFINITY);
		 // }

   //   assert( false && "Never reached" );
   //   return CVector3::ZERO;
   // }

	  //// intersection related methods
   // AxisAlignedBox getIntersection( const AxisAlignedBox& cBox ) const;                       // gets common intersection AAB (union)

   // inline bool intersects( const AxisAlignedBox& cBox ) const                                 // checks whether given AAB intersects AAB
   // { 
   //   return Math::Intersects( this, &cBox ); 
   // } 

	  //inline bool intersects( const CSphere* pcSphere ) const                                     // checks whether given sphere intersects AAB
   // { 
   //   return Math::Intersects( pcSphere, this ); 
   // }   
   // 
   // inline bool intersects( const CPlane* pcPlane ) const                                       // checks whether given sphere intersects AAB
   // { 
   //   return Math::Intersects( pcPlane, this ); 
   // }      

	  //// contain related methods
   // inline bool contains( const CVector3* pcPoint ) const                                       // checks whether given point lies within AAB
   // { 
   //   return Math::Contains( this, pcPoint ); 
   // }        

   // inline bool contains( const AxisAlignedBox& cBox ) const                                   // checks whether given AAB lies within AAB
   // { 
   //   return Math::Contains( this, &cBox ); 
   // }     

  private:

    /*! Box minimum dimensions. */
	  Vector4f m_minimum;
    /*! Box max dimensions. */
	  Vector4f m_maximum;
    /*! Box extent. */
    Extent m_extent;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AXISALIGNEDBOX_H 