#ifndef EGE_CORE_CUBIC_SPLINE_H
#define EGE_CORE_CUBIC_SPLINE_H

#include <EGEDynamicArray.h>
#include <EGEVector.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class CubicSpline
{
  public:

    enum Type
    {
      TYPE_BEZIER,
      TYPE_HERMITE,
      TYPE_CARDINAL,
      TYPE_BSPLINE
    };

  public:

    CubicSpline(Type type = TYPE_CARDINAL);
   ~CubicSpline();

    /*! Returns spline type. */
    inline Type type() const { return m_type; }
    /* Sets spline type. */
    void setType(Type type);
    /* Adds control point.
     * @param point Control point to be added.
     * @return Returns added control point index.
     */
    s32 addControlPoint(const Vector4f& point);

  private:

    /*! Spline type. */
    Type m_type;
    /*! Control points. */
    DynamicArray<Vector4f> m_controlPoints;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CUBIC_SPLINE_H
