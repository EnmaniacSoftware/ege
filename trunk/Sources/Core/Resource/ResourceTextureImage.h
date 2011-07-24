#ifndef EGE_CORE_RESOURCESUBTEXTURE_H
#define EGE_CORE_RESOURCESUBTEXTURE_H

/** Texture image resource definition class. This object (resource) contains definition of rectangular sub-part of 2D texture object.
 *  Textures images refer to Texture objects. This is especially handy for atlasing.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include "Core/Resource/Resource.h"
#include "Core/Graphics/TextureImage.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceTextureImage, PResourceTextureImage)
EGE_DECLARE_SMART_CLASS(TextureImage, PTextureImage)
EGE_DECLARE_SMART_CLASS(Object, PObject)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceTextureImage : public IResource
{
  public:

    virtual ~ResourceTextureImage();

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

    /*! Returns instance of texture image object defined by resource. */
    inline PTextureImage textureImage() const { return m_textureImage; }
    /*! Returns referred texture name. */
    inline const String& textureName() const { return m_textureName; }

  private:

    ResourceTextureImage(Application* app, ResourceManager* manager);
    /* Returns TRUE if texture image is loaded. */
    inline bool isLoaded() const { return NULL != m_textureImage; }
    /*! Returns rectangular area referred by this object. */
    inline const String& rect() const { return m_rect; }

  private:

    /*! Texture image name. */
    String m_name;
    /*! Name of refered texture. */
    String m_textureName;
    /*! Rectangle area refered by this object. */
    String m_rect;
    /*! Texture image object created from resource. NULL if not created yet. */
    PTextureImage m_textureImage;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCESUBTEXTURE_H
