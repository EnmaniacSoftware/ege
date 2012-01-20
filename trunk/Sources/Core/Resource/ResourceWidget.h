#ifndef EGE_CORE_RESOURCEWIDGET_H
#define EGE_CORE_RESOURCEWIDGET_H

/**  Widget resource definition class. This object (resource) contains definition of an arbitrary widget.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGEGraphics.h>
#include <EGEMap.h>
#include "Core/UI/Widget.h"
#include "Core/Resource/Resource.h"
#include "Core/Resource/ResourceMaterial.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceWidget, PResourceWidget)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceWidget : public IResource
{
  public:

    virtual ~ResourceWidget();

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

    /* Creates instance of dialog object defined by resource. */
    PWidget createInstance() const;
    /* Set given instance of dialog object to what is defined by resource. */
    EGEResult setInstance(const PWidget& instance) const;

  private:

    ResourceWidget(Application* app, ResourceManager* manager);
    /*! Returns TRUE if material is loaded. */
    inline bool isLoaded() const { return NULL != m_material; }
    /* Processes content area data. */
    EGEResult processContentArea(const PXmlElement& tag);
    /* Processes frame data. */
    EGEResult processFrame(const PXmlElement& tag);

  private:

    /*! Content area data structure. */
    struct ContentAreaData
    {
      Rectf rect;               /*!< Rectangular area of content within the dialog (in local normalized coordinate system). */
      bool vericalScroll;       /*!< TRUE if vertical scroll is required. */
      bool horizontalScroll;    /*!< TRUE if horizontal scroll is required. */
    };

    /*! Frame data structure. */
    struct FrameData
    {
      Recti topLeftRect;          /*!< Top-left rectangle part (in pixels). This also defines texture coordinates. */
      Recti topMiddleRect;        /*!< Top-middle rectangle part (in pixels). This also defines texture coordinates. */
      Recti topRightRect;         /*!< Top-right v (in pixels). This also defines texture coordinates. */
      Recti middleLeftRect;       /*!< Middle-left rectangle part (in pixels). This also defines texture coordinates. */
      Recti fillRect;             /*!< Fill rectangle part (in pixels). This also defines texture coordinates. */
      Recti middleRightRect;      /*!< Middle-right rectangle part (in pixels). This also defines texture coordinates. */
      Recti bottomLeftRect;       /*!< Bottom-left rectangle part (in pixels). This also defines texture coordinates. */
      Recti bottomMiddleRect;     /*!< Bottom-middle rectangle part (in pixels). This also defines texture coordinates. */
      Recti bottomRightRect;      /*!< Bottom-right rectangle part (in pixels). This also defines texture coordinates. */
    };

    typedef Map<String, ContentAreaData> ContentAreaDataMap;

  private:

    /*! Name. */
    String m_name;
    /*! Material name. */
    String m_materialName;
    /*! Type. */
    String m_type;
    /*! Maximal size. */
    Vector2i m_maxSize;
    /*! Frame data. */
    FrameData m_frameData;
    /*! Loaded material. NULL if resource is not loaded yet. */
    PResourceMaterial m_material;
    /*! Map of content areas. */
    ContentAreaDataMap m_contentAreas;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEWIDGET_H
