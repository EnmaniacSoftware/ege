#ifndef EGE_CORE_RESOURCE_RESOURCELOADER_XML_H
#define EGE_CORE_RESOURCE_RESOURCELOADER_XML_H

/*! Resource loader implementation based on XML.
 */

#include "Core/Resource/Interface/Loader/IResourceLoader.h"
#include "EGEResourceManager.h"
#include "EGEXml.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLoaderXML : public IResourceLoader
{
  public:

    ResourceLoaderXML(Engine& engine);
    virtual ~ResourceLoaderXML();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  private:

    /*! @see IResouceLoader::addResources. */
    EGEResult addResources(const String& filePath) override;

    /*! Processes the resources tag.
     *  @param  filePath  Relative (with respect to resource root directory) path to resouce file.
     *  @param  tag       Element to process. 
     */
    EGEResult processResourcesTag(const String& filePath, const PXmlElement& tag);
    /*! Processes a group tag.
     *  @param  filePath  Relative (with respect to resource root directory) path to resouce file containing the group definition.
     *  @param  tag       Element to process. 
     */
    EGEResult processGroup(const String& filePath, const PXmlElement& tag);
    /*! Processes include tag. 
     *  @param  filePath  Relative (with respect to resource root directory) path to resouce file containing the group definition.
     *  @param  tag       Element to process. 
     */
    EGEResult processInclude(const String& filePath, const PXmlElement& tag);

  private:

    /*! Engine object.. */
    Engine& m_engine;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_RESOURCELOADER_XML_H
