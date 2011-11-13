#ifndef EGE_CORE_RESOURCECURVE_H
#define EGE_CORE_RESOURCECURVE_H

/** Curve resource definition class. This object (resource) contains definition of curve object.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGETime.h>
#include <EGESpline.h>
#include <EGEList.h>
#include <EGEVector.h>
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceCurve, PResourceCurve)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceCurve : public IResource
{
  public:

    virtual ~ResourceCurve();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceManager* manager);

    /* IResource override. Returns name of resource. */
    const String& name() const override;
    /* Initializes resource from XML. 
    * 
    *  \param  path  full path to resource definition file.
    *  \param  tag   xml element with resource definition. 
    */
    EGEResult create(const String& path, const PXmlElement& tag) override;
    /* IResource override. Loads resource. */
    EGEResult load() override;
    /* IResource override. Unloads resource. */
    void unload() override;

    /* Creates instance of curve object defined by resource. */
    CubicSpline createInstance() const;
    /* Set given instance of curve object to what is defined by resource. */
    EGEResult setInstance(CubicSpline& instance) const;

  private:

    /*! Point data info structure. */
    struct PointData
    {
      Vector3f position;          /*!< Position. */
      Vector3f tangent;           /*!< Tangent. */
      Vector3f beginTangent;      /*!< Begin tangent override. */
      bool beginTangentOverride;  /*!< TRUE if begin tangent is defined and should be overwritten. */
    };

  private:

    ResourceCurve(Application* app, ResourceManager* manager);
    /* Returns TRUE if object is loaded. */
    inline bool isLoaded() const { return true; }
    /*! Returns curve type. */
    inline EGESpline::Type type() const { return m_type; }
    
  private:

    typedef List<PointData> PointDataList;

  private:

    /*! Name. */
    String m_name;
    /*! Curve type. */
    EGESpline::Type m_type;
    /*! List of curve points. */
    PointDataList m_points;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCECURVE_H
