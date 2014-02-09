#include "Core/Math/Interface/Splines/CubicSpline.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static Matrix4f BezierMatrix( -1.0f,  3.0f, -3.0f, 1.0f,        // column 0
                               1.0f,  0.0f,  0.0f, 0.0f,        // column 1
                               3.0f, -6.0f,  3.0f, 0.0f,        // column 2
                              -3.0f,  3.0f,  0.0f, 0.0f);       // column 3

static Matrix4f HermiteMatrix( 2.0f, -3.0f, 0.0f, 1.0f,     // column 0
                              -2.0f,  3.0f, 0.0f, 0.0f,     // column 1
                               1.0f, -2.0f, 1.0f, 0.0f,     // column 2
                               1.0f, -1.0f, 0.0f, 0.0f);    // column 3

static Matrix4f CatMullRomMatrix(-0.5f,  1.0f, -0.5f, 0.0f,     // column 0
                                  1.5f, -2.5f,  0.0f, 1.0f,     // column 1
                                 -1.5f,  2.0f,  0.5f, 0.0f,     // column 2
                                  0.5f, -0.5f,  0.0f, 0.0f);    // column 3

static Matrix4f BSplineMatrix(-1.0f / 6.0f,  0.5f, -0.5f, 1.0f / 6.0f,      // column 0
                               0.5f,        -1.0f,  0.0f, 4.0f / 6.0f,      // column 1
                              -0.5f,         0.5f,  0.5f, 1.0f / 6.0f,      // column 2
                               1.0f / 6.0f,  0.0f,  0.0f, 0.0f);            // column 3
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CubicSpline::CubicSpline(CubicSplineType type) : m_type(ENone)
                                               , m_length(0)
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
      case EBSpline:  m_matrix = BSplineMatrix; break;
        
      default:
        
        EGE_ASSERT_X(false, "Unknown spline type");
        break;
    }

    // TAGE - update length etc
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CurveSegment& CubicSpline::addPoint(const Vector4f& point, const Vector4f& tangent)
{
  // check if first segment
  if (m_segments.empty())
  {
    // create new segment
    CurveSegment segment;

    segment.setBegin(point);
    segment.setBeginTangent(tangent);

    // add it into pool
    m_segments.push_back(segment);
  }
  else
  {
    // get last segment
    CurveSegment& segment = m_segments.back();

    // check if end point is not defined yet
    if (Vector4f::ZERO == segment.end())
    {
      // define end point
      segment.setEndTangent(tangent);
      segment.setEnd(point);

      ege_connect(&segment, pointChanged, this, CubicSpline::segmentPointChanged);
      
      // force length recalulation
      calculateSegmentLength(segment);

      // update total length
      m_length += segment.length();
      return segment;
    }

    // new segmnet is necessary
    CurveSegment newSegment;

    // new segment starts where last ends
    newSegment.setBegin(segment.end());
    newSegment.setBeginTangent(segment.endTangent());

    // and ends with given point
    newSegment.setEndTangent(tangent);
    newSegment.setEnd(point);

    ege_connect(&newSegment, pointChanged, this, CubicSpline::segmentPointChanged);

    // force length recalulation
    calculateSegmentLength(newSegment);

    // update total length
    m_length += newSegment.length();

    // add it into pool
    m_segments.push_back(newSegment);
  }

  return m_segments.back();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CubicSpline::value(Vector4f& pos, float32 t) const
{
  const CurveSegment* segment = NULL;

  // get segment and distance to it
  float32 distanceToSegment = 0;
  segment = this->segment(t, distanceToSegment);
  if (NULL == segment)
  {
    // do nothing
    return;
  }

  // make sure value is valid
  t = Math::Bound(t, 0.0f, 1.0f);

  // convert into [0-length] space
  t *= length();

  // convert into segment [0-1] interval
  if (0 < segment->length())
  {
    t = (t - distanceToSegment) / segment->length();
  }
  else
  {
    t = 0;
  }

  // precalculations  
  float32 t2 = t * t;
  float32 t3 = t2 * t;

  //float h1 = m_matrix.data[0] * t3 + m_matrix.data[1] * t2 + m_matrix.data[2] * t + m_matrix.data[3];
  //float h2 = m_matrix.data[4] * t3 + m_matrix.data[5] * t2 + m_matrix.data[6] * t + m_matrix.data[7];
  //float h3 = m_matrix.data[8] * t3 + m_matrix.data[9] * t2 + m_matrix.data[10] * t + m_matrix.data[11];
  //float h4 = m_matrix.data[12] * t3 + m_matrix.data[13] * t2 + m_matrix.data[14] * t + m_matrix.data[15];

  pos = (m_matrix.data[0] * t3 + m_matrix.data[1] * t2 + m_matrix.data[2] * t + m_matrix.data[3]) * segment->begin() +
        (m_matrix.data[4] * t3 + m_matrix.data[5] * t2 + m_matrix.data[6] * t + m_matrix.data[7]) * segment->end() +
        (m_matrix.data[8] * t3 + m_matrix.data[9] * t2 + m_matrix.data[10] * t + m_matrix.data[11]) * segment->beginTangent() +
        (m_matrix.data[12] * t3 + m_matrix.data[13] * t2 + m_matrix.data[14] * t + m_matrix.data[15]) * segment->endTangent();

  pos.w = 1;
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
    posB.w = 1.0f;

    length += posB.length();
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
  t = Math::Bound(t, 0.0f, 1.0f);

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
float32 CubicSpline::length() const 
{ 
  return m_length; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
