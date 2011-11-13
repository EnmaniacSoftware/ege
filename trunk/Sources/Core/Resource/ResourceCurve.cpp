#include "Core/Resource/ResourceCurve.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/Sprite/Sprite.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE

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
ResourceCurve::ResourceCurve(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_CURVE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceCurve::~ResourceCurve()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceCurve::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceCurve(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceCurve::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceCurve::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name  = tag->attribute("name");
  m_type  = MapCurveTypeName(tag->attribute("type").toLower(), EGESpline::TYPE_BEZIER);

  // check if obligatory data is wrong
  if (error || m_name.empty())
  {
    // error!
    EGE_PRINT("ResourceCurve::create - failed for name: %s", m_name.toAscii());
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

      point.position             = child->attribute("pos", "0 0 0").toVector3f(&error);
      point.tangent              = child->attribute("tangent", "0 0 0").toVector3f(&error);
      point.beginTangentOverride = child->hasAttribute("begin-tangent");
      if (point.beginTangentOverride)
      {
        point.beginTangent = child->attribute("begin-tangent", "0 0 0").toVector3f(&error);
      }

      if (error)
      {
        // error!
        EGE_PRINT("ResourceCurve::create - failed for name: %s (2)", m_name.toAscii());
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

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceCurve::load()
{
  // nothing to do
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceCurve::unload() 
{ 
  EGE_PRINT("ResourceCurve::unload - %s", name().toAscii());

  // nothing to do
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of curve object defined by resource. */
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
/*! Set given instance of curve object to what is defined by resource. */
EGEResult ResourceCurve::setInstance(CubicSpline& instance) const
{
  instance = createInstance();
  return instance.isValid() ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
