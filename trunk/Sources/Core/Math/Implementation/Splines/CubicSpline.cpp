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
  EGE_ASSERT(0 == (points.size() % 4));

  bool result = false;

  // check if multiple of 4 points
  if (0 == (points.size() % 4))
  {
    // process all points
    for (List<Vector3f>::const_iterator it = points.begin(); it != points.end(); )
    {
      CurveSegment segment;

      segment.setBegin(Vector4f(it->x, it->y, it->z));
      ++it;
      segment.setBeginTangent(Vector4f(it->x, it->y, it->z));
      ++it;
      segment.setEndTangent(Vector4f(it->x, it->y, it->z));
      ++it;
      segment.setEnd(Vector4f(it->x, it->y, it->z));
      ++it;

      //ege_connect(&segment, pointChanged, this, CubicSpline::segmentPointChanged);
      
      // force length recalulation
      calculateSegmentLength(segment);

      // update total length
      m_length += segment.length();

      // add it into pool
      m_segments.push_back(segment);
    }

    // done
    result = true;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector3f CubicSpline::value(float32 parameter) const
{
  const CurveSegment* segment = NULL;

  // get segment and distance to it
  float32 distanceToSegment = 0;
  segment = this->segment(parameter, distanceToSegment);
  if (NULL == segment)
  {
    // do nothing
    return Vector3f::ZERO;
  }

  // make sure value is valid
  parameter = Math::Clamp(parameter, 0.0f, 1.0f);

  // convert into [0-length] space
  parameter *= length();

  // convert into segment [0-1] interval
  if (0 < segment->length())
  {
    parameter = (parameter - distanceToSegment) / segment->length();
  }
  else
  {
    parameter = 0;
  }

  // precalculations  
  const float32 t2 = parameter * parameter;
  const float32 t3 = t2 * parameter;

  //float h1 = m_matrix.data[0] * t3 + m_matrix.data[1] * t2 + m_matrix.data[2] * t + m_matrix.data[3];
  //float h2 = m_matrix.data[4] * t3 + m_matrix.data[5] * t2 + m_matrix.data[6] * t + m_matrix.data[7];
  //float h3 = m_matrix.data[8] * t3 + m_matrix.data[9] * t2 + m_matrix.data[10] * t + m_matrix.data[11];
  //float h4 = m_matrix.data[12] * t3 + m_matrix.data[13] * t2 + m_matrix.data[14] * t + m_matrix.data[15];

  Vector4f pos = (m_matrix.data[0] * t3 + m_matrix.data[1] * t2 + m_matrix.data[2] * parameter + m_matrix.data[3]) * segment->begin() +
        (m_matrix.data[4] * t3 + m_matrix.data[5] * t2 + m_matrix.data[6] * parameter + m_matrix.data[7]) * segment->end() +
        (m_matrix.data[8] * t3 + m_matrix.data[9] * t2 + m_matrix.data[10] * parameter + m_matrix.data[11]) * segment->beginTangent() +
        (m_matrix.data[12] * t3 + m_matrix.data[13] * t2 + m_matrix.data[14] * parameter + m_matrix.data[15]) * segment->endTangent();

  pos.w = 1;

  return Vector3f(pos.x, pos.y, pos.z);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CubicSpline::calculateSegmentLength(CurveSegment& segment)
{
  const float32 step = 0.01f;

  float32 length = 0;
  for (float32 t = 0; t < 1.0f; )
  {
    // precalculations
    float32 t2 = t * t;
    float32 t3 = t2 * t;

    Vector4f posA = (m_matrix.data[0] * t3 + m_matrix.data[1] * t2 + m_matrix.data[2] * t + m_matrix.data[3]) * segment.begin() +
                    (m_matrix.data[4] * t3 + m_matrix.data[5] * t2 + m_matrix.data[6] * t + m_matrix.data[7]) * segment.end() +
                    (m_matrix.data[8] * t3 + m_matrix.data[9] * t2 + m_matrix.data[10] * t + m_matrix.data[11]) * segment.beginTangent() +
                    (m_matrix.data[12] * t3 + m_matrix.data[13] * t2 + m_matrix.data[14] * t + m_matrix.data[15]) * segment.endTangent();

    // precalculations
    t += step;
    t = Math::Min(t, 1.0f);

    t2 = t * t;
    t3 = t2 * t;

    Vector4f posB = (m_matrix.data[0] * t3 + m_matrix.data[1] * t2 + m_matrix.data[2] * t + m_matrix.data[3]) * segment.begin() +
                    (m_matrix.data[4] * t3 + m_matrix.data[5] * t2 + m_matrix.data[6] * t + m_matrix.data[7]) * segment.end() +
                    (m_matrix.data[8] * t3 + m_matrix.data[9] * t2 + m_matrix.data[10] * t + m_matrix.data[11]) * segment.beginTangent() +
                    (m_matrix.data[12] * t3 + m_matrix.data[13] * t2 + m_matrix.data[14] * t + m_matrix.data[15]) * segment.endTangent();


    posB -= posA;
    
    Vector3f diff(posB.x, posB.y, posB.z);

    length += diff.length();
  }

  segment.setLength(length);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CubicSpline::segmentPointChanged(CurveSegment& segment)
{
  // substract old value
  m_length -= segment.length();

  // recalculate segment length
  calculateSegmentLength(segment);

  // apply new value
  m_length += segment.length();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const CurveSegment* CubicSpline::segment(float32 t, float32& distanceToSegment) const
{
  const CurveSegment* segment = NULL;

  // reset
  distanceToSegment = 0;

  // make sure value is valid
  t = Math::Clamp(t, 0.0f, 1.0f);

  // convert into [0-length] space
  t *= length();

  // go thru all segments
  for (SegmentArray::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it) 
  {
    const CurveSegment& currentSegment = *it;

    // check if wrong segment
    // NOTE: if this is last segment we assume it is the one no matter what
    if ((t > currentSegment.length()) && ((it + 1) != m_segments.end()))
    {
      // go to next
      t -= currentSegment.length();

      // update distance to segment
      distanceToSegment += currentSegment.length();
    }
    else
    {
      // found
      segment = &currentSegment;
      break;
    }
  }

  return segment;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
