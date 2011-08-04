#include "CurveSegment.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CurveSegment::CurveSegment() : m_begin(Vector4f::ZERO), m_end(Vector4f::ZERO), m_beginTangent(Vector4f::ZERO), m_endTangent(Vector4f::ZERO), m_length(0.0f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CurveSegment::~CurveSegment()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Returns TRUE if object is valid. */
bool CurveSegment::isValid() const
{
  return (Vector4f::ZERO == begin() || Vector4f::ZERO == end() || Vector4f::ZERO == beginTangent() || Vector4f::ZERO == endTangent()) ? false : true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets begin point. */
void CurveSegment::setBegin(const Vector4f& point)
{
  if (m_begin != point)
  {
    m_begin = point;

    // emit
    emit pointChanged(*this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets begin point tangent vector. */
void CurveSegment::setBeginTangent(const Vector4f& tangent)
{
  if (m_beginTangent != tangent)
  {
    m_beginTangent = tangent;

    // emit
    emit pointChanged(*this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets end point. */
void CurveSegment::setEnd(const Vector4f& point)
{
  if (m_end != point)
  {
    m_end = point;

    // emit
    emit pointChanged(*this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets end point tangent vector. */
void CurveSegment::setEndTangent(const Vector4f& tangent)
{
  if (m_endTangent != tangent)
  {
    m_endTangent = tangent;

    // emit
    emit pointChanged(*this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets length. */
void CurveSegment::setLength(float32 length)
{
  m_length = length;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
