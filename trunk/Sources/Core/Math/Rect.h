#ifndef EGE_CORE_RECT_H
#define EGE_CORE_RECT_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class TRect
{
  public:
  
    TRect(T x = 0, T y = 0, T w = 0, T h = 0);
    TRect(const TRect<T>& rect);

    inline TRect<T>& operator=(const TRect<T>& rect);

    inline bool operator==(const TRect<T>& rect) const;
    inline bool operator!=(const TRect<T>& rect) const;

    /* Returns TRUE if rectangle is empty (has no dimensions). */
    inline bool isEmpty() const;
    /* Returns coordinate of right edge. */
    inline T right() const;
    /* Returns coordinate of left edge. */
    inline T left() const;
    /* Returns coordinate of top edge. */
    inline T top() const;
    /* Returns coordinate of bottom edge. */
    inline T bottom() const;

  public:

    /*! X coordinate (aka left edge). */
    T x;
    /*! Y coordinate (aka top edge). */
    T y;
    /*! Width. */
    T width;
    /*! Height. */
    T height;
};

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
TRect<T>& TRect<T>::operator=(const TRect<T>& rect)
{
  x      = rect.x;
  y      = rect.y;
  width  = rect.width;
  height = rect.height;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TRect<T>::operator==(const TRect<T>& rect) const
{
  return x == rect.x && y == rect.y && width == rect.width && height == rect.height;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool TRect<T>::operator!=(const TRect<T>& rect) const
{
  return !(*this == rect);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if rectangle is empty (has no dimensions). */
template <typename T>
bool TRect<T>::isEmpty() const
{
  return width == 0 && height == 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns coordinate of right edge. */
template <typename T>
T TRect<T>::right() const
{
  return x + width;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns coordinate of bottom edge. */
template <typename T>
T TRect<T>::bottom() const
{
  return y + height;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns coordinate of left edge. */
template <typename T>
T TRect<T>::left() const
{
  return x;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns coordinate of top edge. */
template <typename T>
T TRect<T>::top() const
{
  return y;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RECT_H