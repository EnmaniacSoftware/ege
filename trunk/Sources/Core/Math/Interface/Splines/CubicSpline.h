#ifndef EGE_CORE_MATH_CUBICSPLINE_H
#define EGE_CORE_MATH_CUBICSPLINE_H

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

#include "EGEDynamicArray.h"
#include "EGEVector4.h"
#include "EGEMatrix.h"
#include "Core/Math/Interface/Splines/Spline.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available cubic spline types. */
enum CubicSplineType
{
  ENone,
  EBezier,
  EHermite,
  ECardinal
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CubicSpline : public Spline
{
  public:

    CubicSpline(CubicSplineType type = ECardinal);
   ~CubicSpline();

  operators:

    CubicSpline& operator = (const CubicSpline& other);

  public:

    /*! Returns spline type. */
    CubicSplineType type() const;
    /*! Sets spline type. */
    void setType(CubicSplineType type);

    /*! @see Spline::addPoints. 
     *  @note Points should be given in quadruples: [begin, tangent-1, tangent-2, end]
     */
    bool addPoints(const List<Vector3f>& points) override;
    /*! @see Spline::value. */
    Vector3f value(float32 parameter) const override;

  private:

    /*! Calculates segement length. */
    void calculateSegmentLength(CurveSegment& segment);
    const CurveSegment* segment(float32 t, float32& distanceToSegment) const;

  private slots:

    /*! Called when one of the segments point has been changed. */
    void segmentPointChanged(CurveSegment& segment);

  private:

    /*! Spline type. */
    CubicSplineType m_type;
    /*! Matrix of basis functions for currently selected spline type. */
    Matrix4f m_matrix;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_CUBICSPLINE_H
