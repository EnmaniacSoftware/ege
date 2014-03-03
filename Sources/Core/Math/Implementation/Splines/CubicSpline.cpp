#include "Core/Math/Interface/Splines/CubicSpline.h"
#include "EGEMath.h"
#include "EGEVector3.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static Matrix4f BezierMatrix(-1.0f,  3.0f, -3.0f, 1.0f,                 // column 0 (start)
                              1.0f,  0.0f,  0.0f, 0.0f,                 // column 1 (end)
                              3.0f, -6.0f,  3.0f, 0.0f,                 // column 2 (start tangent)
                             -3.0f,  3.0f,  0.0f, 0.0f);                // column 3 (end tangent)

static Matrix4f HermiteMatrix( 2.0f, -3.0f, 0.0f, 1.0f,                 // column 0 (start)
                              -2.0f,  3.0f, 0.0f, 0.0f,                 // column 1 (end)
                               1.0f, -2.0f, 1.0f, 0.0f,                 // column 2 (start tangent)
                               1.0f, -1.0f, 0.0f, 0.0f);                // column 3 (end tangent)

static Matrix4f CatMullRomMatrix(-0.5f,  1.0f, -0.5f, 0.0f,             // column 0 (start)
                                  1.5f, -2.5f,  0.0f, 1.0f,             // column 1 (end)
                                 -1.5f,  2.0f,  0.5f, 0.0f,             // column 2 (start tangent)
                                  0.5f, -0.5f,  0.0f, 0.0f);            // column 3 (end tangent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CubicSpline::CubicSpline(CubicSplineType type) : Spline()
                                               , m_type(ENone)
{
  setType(type);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CubicSpline::~CubicSpline()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CubicSpline& CubicSpline::operator = (const CubicSpline& other)
{
  // clean up
  m_segments.clear();

  // copy
  m_segments.copy(other.m_segments);
  m_type   = other.m_type;
  m_matrix = other.m_matrix;
  m_length = other.m_length;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CubicSplineType CubicSpline::type() const
{
  return m_type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CubicSpline::setType(CubicSplineType type)
{
  if (m_type != type)
  {
    m_type = type;

    switch (type)
    {
      case EBezier:   m_matrix = BezierMatrix; break;
      case EHermite:  m_matrix = HermiteMatrix; break;
      case ECardinal: m_matrix = CatMullRomMatrix; break;
        
      default:
        
        EGE_ASSERT_X(false, "Unknown spline type");
        break;
    }

    // TAGE - update length etc
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CubicSpline::addPoints(const List<Vector3f>& points)
{
  bool result = false;

  const s32 KFirstSegmentPointCount = 4;
  const s32 KNextSegmentPointCount  = 3;
 
  // check if proper number of points is given
  // NOTE: few different scenarios are possible here:
  //       - if currently no segment is given, new list should contain at least 4 points (for first segment) and/or 3 points for every next segment
  //       - if at least one segment is already specified, new list should contain 3 or mulitple of 3 points for another segment(s)
  if ((m_segments.empty() && ((KFirstSegmentPointCount == points.size()) || (0 == (points.size() - KFirstSegmentPointCount) % KNextSegmentPointCount))) ||
      ( ! m_segments.empty() && (0 == (points.size() % KNextSegmentPointCount))))
  {
    // add all points to spline
    for (List<Vector3f>::const_iterator it = points.begin(); it != points.end(); )
    {
      SegmentData segment;

      // check if first segment
      if (m_segments.empty())
      {
        segment.begin     = *it++;
        segment.control1  = *it++;
        segment.control2  = *it++;
        segment.end       = *it++;
      }
      else
      {
        segment.begin     = m_segments.last().end;
        segment.control1  = *it++;
        segment.control2  = *it++;
        segment.end       = *it++;
      }

      // add segment to pool
      m_segments.push_back(segment);
    }

    // invalidate length
    m_length.invalidate();

    // done
    result = true;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector3f CubicSpline::value(float32 parameter) const
{
  Vector3f out = Vector3f::ZERO;

  // make sure value is valid
  parameter = Math::Clamp(parameter, 0.0f, 1.0f);

  // convert into [0-length] space
  parameter *= length();

  // go thru all segments and find the one which contains point at given location
  for (DynamicArray<SegmentData>::const_iterator it = m_segments.begin(); (it != m_segments.end() && (0 <= parameter)); ++it)
  {
    const SegmentData& segment = *it;

    // check if position within current segment
    if (parameter <= segment.length)
    {
      // re-map to [0-1] interval of current segement
      parameter = Math::Clamp(parameter / segment.length, 0.0f, 1.0f);

      // get value within segment
      out = value(parameter, segment);

      // done
      parameter = -1.0f;
    }
    else
    {
      // update remaining distance
      parameter -= segment.length;
    }
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CubicSpline::calculateSegmentLength(const SegmentData& segment) const
{
  const float32 step = 0.01f;

  float32 length = 0;
  for (float32 t = 0; t < 1.0f; t += step)
  {
    const Vector3f posA = value(t, segment);
    const Vector3f posB = value(Math::Min(t + step, 1.0f), segment);

    length += (posB - posA).length();
  }

  // update length
  segment.length = length;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector3f CubicSpline::value(float32 parameter, const SegmentData& segment) const
{
  const float32 t2 = parameter * parameter;
  const float32 t3 = t2 * parameter;

  return (m_matrix.data[0]  * t3 + m_matrix.data[1]  * t2 + m_matrix.data[2]  * parameter + m_matrix.data[3]) * segment.begin +
         (m_matrix.data[4]  * t3 + m_matrix.data[5]  * t2 + m_matrix.data[6]  * parameter + m_matrix.data[7]) * segment.end +
         (m_matrix.data[8]  * t3 + m_matrix.data[9]  * t2 + m_matrix.data[10] * parameter + m_matrix.data[11]) * segment.control1 +
         (m_matrix.data[12] * t3 + m_matrix.data[13] * t2 + m_matrix.data[14] * parameter + m_matrix.data[15]) * segment.control2;  
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 CubicSpline::length() const
{
  // check if invalid
  if ( ! m_length.isValid())
  {
    float32 length = 0;

    // go thru all segments and sum up their lengths
    for (SegmentArray::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
    {
      const SegmentData& segment = *it;

      // check if segment data length needs update
      if ( ! segment.length.isValid())
      {
        // recalculate segment length
        calculateSegmentLength(segment);
      }

      // update total spline length
      length += segment.length.data();
    }

    // store new spline length
    m_length = length;
  }

  return m_length;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------