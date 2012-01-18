#ifndef EGE_CORE_RESOURCEDIALOG_H
#define EGE_CORE_RESOURCEDIALOG_H

/**  Dialog resource definition class. This object (resource) contains definition of dialog widget.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGEGraphics.h>
#include <EGEMap.h>
#include "Core/UI/Dialog.h"
#include "Core/Resource/Resource.h"
#include "Core/Resource/ResourceMaterial.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceDialog, PResourceDialog)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceDialog : public IResource
{
  public:

    virtual ~ResourceDialog();

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
    PDialog createInstance() const;
    /* Set given instance of dialog object to what is defined by resource. */
    EGEResult setInstance(const PDialog& instance) const;

  private:

    ResourceDialog(Application* app, ResourceManager* manager);
    /*! Returns TRUE if material is loaded. */
    inline bool isLoaded() const { return NULL != m_material; }
    /* Adds content area. */
    EGEResult addContentArea(const PXmlElement& tag);

  private:

    /*! Content area data structure. */
    struct ContentAreaData
    {
      Rectf rect;               /*!< Rectangular area of content within the dialog (in local normalized coordinate system). */
    };

    typedef Map<String, ContentAreaData> ContentAreaDataMap;

  private:

    /*! Name. */
    String m_name;
    /*! Material name. */
    String m_materialName;
    /*! Top-left rect with (in pixels). This is relative to dialog texture. */
    Recti m_topLeftRect;
    /*! Top-middle rect with (in pixels). This is relative to dialog texture. */
    Recti m_topMiddleRect;
    /*! Top-right rect with (in pixels). This is relative to dialog texture. */
    Recti m_topRightRect;
    /*! Middle-left rect with (in pixels). This is relative to dialog texture. */
    Recti m_middleLeftRect;
    /*! Fill rect with (in pixels). This is relative to dialog texture. */
    Recti m_fillRect;
    /*! Middle-right rect with (in pixels). This is relative to dialog texture. */
    Recti m_middleRightRect;
    /*! Bottom-left rect with (in pixels). This is relative to dialog texture. */
    Recti m_bottomLeftRect;
    /*! Bottom-middle rect with (in pixels). This is relative to dialog texture. */
    Recti m_bottomMiddleRect;
    /*! Bottom-right rect with (in pixels). This is relative to dialog texture. */
    Recti m_bottomRightRect;
    /*! Maximal size. */
    Vector2i m_maxSize;
    /*! Loaded material. NULL if resource is not loaded yet. */
    PResourceMaterial m_material;
    /*! Map of content areas. */
    ContentAreaDataMap m_contentAreas;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEDIALOG_H
