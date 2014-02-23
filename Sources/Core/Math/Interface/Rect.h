#ifndef EGE_CORE_MATH_RECT_H
#define EGE_CORE_MATH_RECT_H

/** Rectangle object. Rectangle is defined by two sets of numbers: x-y coordinate of top-left corner and width-height.
 *  Rectangle is EMPTY if width and/or height equals 0.
 *  Rectangle is INVALID (NULL) if width and/or height is negative.
 *
 *  Rectangle originating at point (X,Y) and being W wide and H high, will occpy the following space:
 *  - [X, W - 1] along X axis
 *  - [Y, H - 1] along Y axis
 */

#include "EGETypes.h"
#include "EGEVector2.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TRect
{
  public:
  
    TRect(T x = 0, T y = 0, T w = 0, T h = 0);
    TRect(const TRect<T>& rect);
    
  operators:

    TRect<T>& operator  = (const TRect<T>& other);
    bool      operator == (const TRect<T>& other) const;
    bool      operator != (const TRect<T>& other) const;

  public:

    /*! Returns TRUE if rectangle is empty (has no dimensions). */
    bool isEmpty() const;
    /*! Returns TRUE if rectangle is invalid (negative dimenions). */
    bool isNull() const;
    /*! Returns coordinate of right edge. 
     *  @note This point still belongs to rectangle.
     */
    T right() const;
    /*! Returns coordinate of left edge. 
     *  @note This point still belongs to rectangle.
     */
    T left() const;
    /*! Returns coordinate of top edge. 
     *  @note This point still belongs to rectangle.
     */
    T top() const;
    /*! Returns coordinate of bottom edge. 
     *  @note This point still belongs to rectangle.
     */
    T bottom() const;
    /*! Returns TRUE if given point is within rectangle. 
     *  @param  x Point X coordinate.
     *  @param  y Point Y coordinate.
     *  @return Returns TRUE if point lies within rectangle.
     */
    bool contains(T x, T y) const;
    /*! Returns TRUE if given point is within rectangle. 
     *  @param  point Point to test.
     *  @return Returns TRUE if point lies within rectangle.
     */
    bool contains(const TVector2<T>& point) const;
    /*! Returns TRUE if given rectangle is within current one. 
     *  @param  other Rectangle to test.
     *  @return Returns TRUE if given rectangle lies completely within current one.
     */
    bool contains(const TRect<T>& other) const;
    /*! Calculates intersection rectangle between current and given rectangle. 
     *  @param  other Rectangle to intersect with.
     *  @return Intersection rectangle. In case of no intersection, returned rectangle may be either NULL or EMPTY.
     */
    TRect<T> intersect(const TRect<T>& other) const;
    /*! Calculates union rectangle between current and given rectangle. 
     *  @param  other Rectangle to unite with.
     *  @return United rectangle.
     */
    TRect<T> unite(const TRect<T>& other) const;
    /*! Calculates combined rectangle between current and given rectangle.
     *  @param  rect  Rectangle to combine self with.
     *  @note Combined rectangle is the a new rectangle in local space of current one. This is useful to calculate part of current rectangle defined by other.
     */
    TRect<T> combine(const TRect<T>& other) const;

  public:

    /*! X coordinate (aka left edge). */
    T x;
    /*! Y coordinate (aka top edge). */
    T y;
    /*! Width. */
    T width;
    /*! Height. */
    T height;

  public:

    static const TRect<T> INVALID;
    static const TRect<T> UNIT;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const TRect<T> TRect<T>::INVALID = TRect<T>(0, 0, -1, -1);
template <typename T>
const TRect<T> TRect<T>::UNIT = TRect<T>(0, 0, 1, 1);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TRect<T>::TRect(T x, T y, T w, T h) : x(x)
                                    , y(y)
                                    , width(w)
                                    , height(h)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TRect<T>::TRect(const TRect<T>& other) : x(other.x)
                                       , y(other.y)
                                       , width(other.width)
                                       , height(other.height) 
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TRect<T>& TRect<T>::operator=(const TRect<T>& other)
{
  x      = other.x;
  y      = other.y;
  width  = other.width;
  height = other.height;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TRect<T>::operator==(const TRect<T>& other) const
{
  return (x == other.x) && (y == other.y) && (width == other.width) && (height == other.height);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TRect<T>::operator!=(const TRect<T>& other) const
{
  return !(*this == other);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TRect<T>::isEmpty() const
{
  return (0 == width) || (0 == height);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TRect<T>::right() const
{
  return x + width - 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TRect<T>::bottom() const
{
  return y + height - 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TRect<T>::left() const
{
  return x;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T TRect<T>::top() const
{
  return y;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TRect<T>::isNull() const
{
  return (0 > width) || (0 > height);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline bool TRect<T>::contains(T x, T y) const
{
  return (x >= left()) && (y >= top()) && (x <= right()) && (y <= bottom());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TRect<T>::contains(const TVector2<T>& point) const
{
  return contains(point.x, point.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TRect<T> TRect<T>::intersect(const TRect<T>& other) const
{
  TRect<T> out;

  out.x       = (x > other.x) ? x : other.x;
  out.y       = (y > other.y) ? y : other.y;
  out.width   = ((right() < other.right()) ? (right() - out.x) : (other.right() - out.x)) + 1;
  out.height  = ((bottom() < other.bottom()) ? (bottom() - out.y) : (other.bottom() - out.y)) + 1;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TRect<T> TRect<T>::unite(const TRect<T>& other) const
{
  TRect<T> out;

  out.x       = (x < other.x) ? x : other.x;
  out.y       = (y < other.y) ? y : other.y;
  out.width   = ((right() > other.right()) ? (right() - out.x) : (other.right() - out.x)) + 1;
  out.height  = ((bottom() > other.bottom()) ? (bottom() - out.y) : (other.bottom() - out.y)) + 1;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TRect<T> TRect<T>::combine(const TRect<T>& other) const
{
  TRect<T> out;

  out.x       = x + width * other.x;
  out.y       = y + height * other.y;
  out.width   = width * other.width;
  out.height  = height * other.height;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TRect<T>::contains(const TRect<T>& other) const
{
  // sanity checks
  if (other.isNull() || other.isEmpty() || isNull() || isEmpty())
  {
    return false;
  }

  // check if entirely inside
  if ((other.x >= x) && (other.y >= y) && (other.bottom() <= bottom()) && (other.right() <= right()))
  {
    return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Debug operator << (Debug debug, const TRect<T>& obj)
{
  debug.nospace() << "Rect(" << obj.x << "," << obj.y << " - " << obj.width << "," << obj.height << ")";
  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_RECT_H