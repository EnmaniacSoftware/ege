#ifndef EGE_CORE_CURVE_SEGMENT_H
#define EGE_CORE_CURVE_SEGMENT_H

/** This class represents curve segment. It does not do anything except of being a data holder used by spline classes.
 */

#include "EGEVector.h"
#include "EGESignal.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CurveSegment
{
  public:

    CurveSegment();
   ~CurveSegment();

  public signals:

    /*! Signal emitted when one of the segments points is changed. 
     *  @param segment Segment for which point has changed.
     */
    Signal1<CurveSegment&> pointChanged; 
    
  public:

    /*! Returns begin point. */
    const Vector4f& begin() const { return m_begin; }
    /*! Sets begin point. */
    void setBegin(const Vector4f& point);
    /*! Returns begin point tangent vector. */
    const Vector4f& beginTangent() const { return m_beginTangent; }
    /*! Sets begin point tangent vector. */
    void setBeginTangent(const Vector4f& tangent);
    /*! Returns end point. */
    const Vector4f& end() const { return m_end; }
    /*! Sets end point. */
    void setEnd(const Vector4f& point);
    /*! Returns end point tangent vector. */
    const Vector4f& endTangent() const { return m_endTangent; }
    /*! Sets end point tangent vector. */
    void setEndTangent(const Vector4f& tangent);
    /*! Returns segment length. */
    float32 length() const { return m_length; }
    /*! Sets length. */
    void setLength(float32 length);

  private:

    /*! Begin point of segment. */
    Vector4f m_begin;
    /*! End point of segment. */
    Vector4f m_end;
    /*! Begin point tangent vector. */
    Vector4f m_beginTangent;
    /*! End point tangent vector. */
    Vector4f m_endTangent;
    /*! Segment length. */
    float32 m_length;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CURVE_SEGMENT_H
