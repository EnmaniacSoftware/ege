#ifndef EGE_CORE_RESOURCEWIDGET_H
#define EGE_CORE_RESOURCEWIDGET_H

/**  Widget resource definition class. This object (resource) contains definition of an arbitrary widget.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGEGraphics.h>
#include <EGEList.h>
#include <EGEDictionary.h>
#include "Core/UI/Widget.h"
#include "Core/Resource/Resource.h"
#include "Core/Resource/ResourceMaterial.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceWidget, PResourceWidget)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceWidget : public IResource
{
  public:

    virtual ~ResourceWidget();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceGroup* group);

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PXmlElement& tag) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Creates instance of widget object defined by resource. */
    PWidget createInstance();

  private:

    ResourceWidget(Application* app, ResourceGroup* group);
    /*! Processes child data. */
    EGEResult processChild(const PXmlElement& tag);
    /*! Processes frame data. */
    EGEResult processFrame(const PXmlElement& tag);

  private:

    /*! Child data structure. */
    struct ChildData
    {
      Vector2f offset;          /*!< Child offset (in pixels) within parent. */
      String name;              /*!< Child object name. */
      String widgetName;        /*!< Child widget name. */
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

    typedef List<ChildData> ChildDataList;

  private:

    /*! Name. */
    String m_name;
    /*! Dictionary with defined parameters. */
    Dictionary m_parameters;
    /*! Frame data. */
    FrameData m_frameData;
    /*! List of children. */
    ChildDataList m_children;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEWIDGET_H
