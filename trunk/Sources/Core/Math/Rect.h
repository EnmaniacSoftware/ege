#ifndef EGE_CORE_RECT_H
#define EGE_CORE_RECT_H

#include <EGETypes.h>

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
    /* Returns TRUE if rectangle is invalid (negative dimenions). */
    inline bool isNull() const;
    /* Returns coordinate of right edge. */
    inline T right() const;
    /* Returns coordinate of left edge. */
    inline T left() const;
    /* Returns coordinate of top edge. */
    inline T top() const;
    /* Returns coordinate of bottom edge. */
    inline T bottom() const;
    /* Returns TRUE if given point is within rectangle. */
    inline bool contains(T x, T y) const;
    /* Calculates intersection rectangle between current and given rectangle. */
    inline TRect<T> intersect(const TRect<T>& rect) const;
    /* Calculates union rectangle between current and given rectangle. */
    inline TRect<T> unite(const TRect<T>& rect) const;

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
  return (0 == width) || (0 == height);
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
/*! Returns TRUE if rectangle is invalid (negative dimensions). */
template <typename T>
bool TRect<T>::isNull() const
{
  return (0 > width) || (0 > height);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given point is within rectangle. */
template <typename T>
bool TRect<T>::contains(T x, T y) const
{
  return (x >= left()) && (y >= top()) && (x < right()) && (y < bottom());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates intersection rectangle between current and given rectangle. */
template <typename T>
TRect<T> TRect<T>::intersect(const TRect<T>& rect) const
{
  TRect<T> out;

  out.x       = (x > rect.x) ? x : rect.x;
  out.y       = (y > rect.y) ? y : rect.y;
  out.width   = (right() < rect.right()) ? (right() - out.x) : (rect.right() - out.x);
  out.height  = (bottom() < rect.bottom()) ? (bottom() - out.y) : (rect.bottom() - out.y);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates union rectangle between current and given rectangle. */
template <typename T>
TRect<T> TRect<T>::unite(const TRect<T>& rect) const
{
  TRect<T> out;

  out.x       = (x < rect.x) ? x : rect.x;
  out.y       = (y < rect.y) ? y : rect.y;
  out.width   = (right() > rect.right()) ? (right() - out.x) : (rect.right() - out.x);
  out.height  = (bottom() > rect.bottom()) ? (bottom() - out.y) : (rect.bottom() - out.y);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RECT_H