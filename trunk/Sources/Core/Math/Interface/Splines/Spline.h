#ifndef EGE_CORE_MATH_SPLINES_SPLINE_H
#define EGE_CORE_MATH_SPLINES_SPLINE_H

/** This is a base class for various types of splines. In fact, it is not only meant to be for a regular splines but also for series of such forming 
 *  i.e. spline paths.
 */

#include "EGEList.h"
#include "EGEVector3.h"
#include "EGECachedObject.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Spline
{
  public:

    Spline();
    virtual ~Spline();

  public:

    /*! Adds spline points.
     *  @param  points  Points to be added.
     *  @return TRUE on success. Otherwise FALSE.
     */
    virtual bool addPoints(const List<Vector3f>& points) = 0;
    /*! Calculates value on spline at given position.
     *  @param parameter  Parametrized distance on a spline at which calculations are done. Typicially in [0-1] interval.
     *  @return Calculated position on spline at given position.
     */
    virtual Vector3f value(float32 parameter) const = 0;
    /*! Returns spline length. */
    virtual float32 length() const = 0;

  protected:

    /*! Spline length. */
    mutable CachedFloat32 m_length;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_SPLINES_SPLINE_H
