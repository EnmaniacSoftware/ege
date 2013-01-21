#include "Core/Resource/ResourceCurve.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/SpriteAnimation/SpriteAnimation.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGEXml.h>
#include <EGEResources.h>
#include <EGEVector.h>
#include <EGEStringUtils.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceCurve)
EGE_DEFINE_DELETE_OPERATORS(ResourceCurve)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_POINT "point"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping literal curve type name into numeric value. */
static EGESpline::Type MapCurveTypeName(const String& name, EGESpline::Type defaultValue)
{
  if ("bezier" == name)
  {
    return EGESpline::TYPE_BEZIER;
  }
  else if ("bspline" == name)
  {
    return EGESpline::TYPE_BSPLINE;
  }
  else if ("cardinal" == name)
  {
    return EGESpline::TYPE_CARDINAL;
  }
  else if ("hermite" == name)
  {
    return EGESpline::TYPE_HERMITE;
  }

  return defaultValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceCurve::ResourceCurve(Application* app, ResourceGroup* group) : IResource(app, group, RESOURCE_NAME_CURVE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceCurve::~ResourceCurve()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceCurve::Create(Application* app, ResourceGroup* group)
{
  return ege_new ResourceCurve(app, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceCurve::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceCurve::create(const String& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name  = tag->attribute("name");
  m_type  = MapCurveTypeName(tag->attribute("type").toLower(), EGESpline::TYPE_BEZIER);

  // check if obligatory data is wrong
  if (error || m_name.empty())
  {
    // error!
    egeWarning() << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all sub node
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (NODE_POINT == child->name())
    {
      PointData point;

      point.position             = StringUtils::ToVector3f(child->attribute("pos", "0 0 0"), &error);
      point.tangent              = StringUtils::ToVector3f(child->attribute("tangent", "0 0 0"), &error);
      point.beginTangentOverride = child->hasAttribute("begin-tangent");
      if (point.beginTangentOverride)
      {
        point.beginTangent = StringUtils::ToVector3f(child->attribute("begin-tangent", "0 0 0"), &error);
      }

      if (error)
      {
        // error!
        egeWarning() << "Failed for name (2):" << m_name;
        result = EGE_ERROR_BAD_PARAM;
        break;
      }

      // add into list
      m_points.push_back(point);
    }

    // check if failed
    if (EGE_SUCCESS != result)
    {
      // error, done!
      break;
    }

    // go to next child
    child = child->nextChild();
  }

  // check if success
  if (EGE_SUCCESS == result)
  {
    // set state
    m_state = STATE_UNLOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceCurve::load()
{
  // set flag
  m_state = STATE_LOADED;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceCurve::unload() 
{ 
  // unset flag
  m_state = STATE_UNLOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CubicSpline ResourceCurve::createInstance() const
{
  CubicSpline spline(type());

  // add points
  for (PointDataList::const_iterator it = m_points.begin(); it != m_points.end(); ++it)
  {
    const PointData& point = *it;

    Vector4f pos(point.position.x, point.position.y, point.position.z);
    Vector4f tangent(point.tangent.x, point.tangent.y, point.tangent.z);

    CurveSegment& segment = spline.addPoint(pos, tangent);
    if (point.beginTangentOverride)
    {
      Vector4f beginTangent(point.beginTangent.x, point.beginTangent.y, point.beginTangent.z);
      segment.setBeginTangent(beginTangent);
    }
  }

  return spline;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceCurve::setInstance(CubicSpline& instance) const
{
  instance = createInstance();
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END