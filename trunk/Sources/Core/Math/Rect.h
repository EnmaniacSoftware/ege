#ifndef EGE_CORE_RECT_H
#define EGE_CORE_RECT_H

/** Rectangle object. Rectangle is defined by two sets of numbers: x-y coordinate of top-left corner and width-height.
 *  Rectangle is EMPTY if width and/or height equals 0.
 *  Rectangle is INVALID (NULL) if width and/or height is negative.
 */

#include "EGETypes.h"
#include "EGEVector.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class TRect
{
  public:
  
    TRect(T x = 0, T y = 0, T w = 0, T h = 0);
    TRect(const TRect<T>& rect);

    TRect<T>& operator =(const TRect<T>& rect);
    bool      operator==(const TRect<T>& rect) const;
    bool      operator!=(const TRect<T>& rect) const;

    /*! Returns TRUE if rectangle is empty (has no dimensions). */
    bool isEmpty() const;
    /*! Returns TRUE if rectangle is invalid (negative dimenions). */
    bool isNull() const;
    /*! Returns coordinate of right edge. */
    T right() const;
    /*! Returns coordinate of left edge. */
    T left() const;
    /*! Returns coordinate of top edge. */
    T top() const;
    /*! Returns coordinate of bottom edge. */
    T bottom() const;
    /*! Returns TRUE if given point is within rectangle. */
    bool contains(T x, T y) const;
    /*! Returns TRUE if given point is within rectangle. */
    bool contains(const TVector2<T>& point) const;
    /*! Returns TRUE if given rectangle is within current one. */
    bool contains(const TRect<T>& rect) const;
    /*! Calculates intersection rectangle between current and given rectangle. If there is no intersection resulting rectangle is nulled. */
    TRect<T> intersect(const TRect<T>& rect) const;
    /*! Calculates union rectangle between current and given rectangle. */
    TRect<T> unite(const TRect<T>& rect) const;
    /*! Calculates combined rectangle between current and given rectangle.
     *  @param  rect  Rectangle to combine self with.
     *  @note Combined rectangle is the a new rectangle in local space of current one. This is useful to calculate part of current rectangle defined by other.
     */
    TRect<T> combine(const TRect<T>& rect) const;

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
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const TRect<T> TRect<T>::INVALID = TRect<T>(0, 0, -1, -1);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TRect<T>::TRect(T x, T y, T w, T h ) : x(x), y(y), width(w), height(h)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
TRect<T>::TRect(const TRect<T>& rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height) 
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TRect<T>& TRect<T>::operator=(const TRect<T>& rect)
{
  x      = rect.x;
  y      = rect.y;
  width  = rect.width;
  height = rect.height;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline bool TRect<T>::operator==(const TRect<T>& rect) const
{
  return x == rect.x && y == rect.y && width == rect.width && height == rect.height;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline bool TRect<T>::operator!=(const TRect<T>& rect) const
{
  return !(*this == rect);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline bool TRect<T>::isEmpty() const
{
  return (0 == width) || (0 == height);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline T TRect<T>::right() const
{
  return x + width - 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline T TRect<T>::bottom() const
{
  return y + height - 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline T TRect<T>::left() const
{
  return x;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline T TRect<T>::top() const
{
  return y;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline bool TRect<T>::isNull() const
{
  return (0 > width) || (0 > height);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline bool TRect<T>::contains(T x, T y) const
{
  return (x >= left()) && (y >= top()) && (x < right()) && (y < bottom());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline bool TRect<T>::contains(const TVector2<T>& point) const
{
  return contains(point.x, point.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TRect<T> TRect<T>::intersect(const TRect<T>& rect) const
{
  TRect<T> out;

  out.x       = (x > rect.x) ? x : rect.x;
  out.y       = (y > rect.y) ? y : rect.y;
  out.width   = ((right() < rect.right()) ? (right() - out.x) : (rect.right() - out.x)) + 1;
  out.height  = ((bottom() < rect.bottom()) ? (bottom() - out.y) : (rect.bottom() - out.y)) + 1;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TRect<T> TRect<T>::unite(const TRect<T>& rect) const
{
  TRect<T> out;

  out.x       = (x < rect.x) ? x : rect.x;
  out.y       = (y < rect.y) ? y : rect.y;
  out.width   = ((right() > rect.right()) ? (right() - out.x) : (rect.right() - out.x)) + 1;
  out.height  = ((bottom() > rect.bottom()) ? (bottom() - out.y) : (rect.bottom() - out.y)) + 1;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline TRect<T> TRect<T>::combine(const TRect<T>& rect) const
{
  TRect<T> out;

  out.x       = x + width * rect.x;
  out.y       = y + height * rect.y;
  out.width   = width * rect.width;
  out.height  = height * rect.height;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline bool TRect<T>::contains(const TRect<T>& rect) const
{
  // sanity checks
  if (rect.isNull() || rect.isEmpty() || isNull() || isEmpty())
  {
    return false;
  }

  // check if entirely inside
  if ((rect.x >= x) && (rect.y >= y) && (rect.bottom() <= bottom()) && (rect.right() <= rect.right()))
  {
    return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline Debug operator << (Debug debug, const TRect<T>& obj)
{
  debug.nospace() << "Rect(" << obj.x << "," << obj.y << " - " << obj.width << "," << obj.height << ")";
  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RECT_H