#ifndef EGE_CORE_CUBIC_SPLINE_H
#define EGE_CORE_CUBIC_SPLINE_H

/** This class represents cubic spline. Each type of spline is constructed from 4 basis functions. These functions are blended (added) together to form final
 *  location on curve. Each column in matrix represents cubic polynomial for base function:
 *
 *  | f00 f10 f20 f30 |
 *  | f01 f11 f21 f31 |
 *  | f02 f12 f22 f32 |
 *  | f03 f13 f23 f33 |
 *
 *  where:
 *  f0x are the components of first base function polynomial applied to segment start point.
 *  f1x are the components of second base function polynomial applied to segment end point.
 *  f2x are the components of third base function polynomial applied to segment first control point.
 *  f3x are the components of fourth base function polynomial applied to segment second control point.
 */

#include <EGEDynamicArray.h>
#include <EGEList.h>
#include <EGEVector.h>
#include <EGEMatrix.h>
#include "Core/Math/Splines/CurveSegment.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class CubicSpline
{
  public:

    /*! Type of spline. */
    enum Type
    {
      TYPE_NONE,
      TYPE_BEZIER,
      TYPE_HERMITE,
      TYPE_CARDINAL,
      TYPE_BSPLINE
    };

  public:

    CubicSpline(Type type = TYPE_CARDINAL);
   ~CubicSpline();

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns spline type. */
    inline Type type() const { return m_type; }
    /* Sets spline type. */
    void setType(Type type);
    /* Adds point to spline.
     * @param point   Point to be added.
     * @param tangent Tangent vector for added point.
     * @return Retrurns curve segment point belogns to.
     */
    CurveSegment& addPoint(const Vector4f& point, const Vector4f& tangent);
    /* Calculates value on spline at given position
     * @param pos Calculated position on spline at given position.
     * @param t   Parametrized distance on a spline at which calculations are done. Typicially in [0-1] interval.
     */
    void value(Vector4f& pos, float32 t) const;
    /*! Returns spline length. */
    inline float32 length() const { return m_length; }

  private:

    /* Calculates segement length. */
    void calculateSegmentLength(CurveSegment& segment);

  private slots:

    /* Called when one of the segments point has been changed. */
    void segmentPointChanged(CurveSegment& segment);

  private:

    /*! Spline type. */
    Type m_type;
    /*! List of all spline segments. */
    DynamicArray<CurveSegment> m_segments;
    /*! Matrix of basis functions for currently selected spline type. */
    Matrix4f m_matrix;
    /*! Spline length. */
    float32 m_length;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CUBIC_SPLINE_H
