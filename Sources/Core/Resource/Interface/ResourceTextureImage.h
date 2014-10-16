#ifndef EGE_CORE_RESOURCESUBTEXTURE_H
#define EGE_CORE_RESOURCESUBTEXTURE_H

/** Texture image resource definition class. This object (resource) contains definition of rectangular sub-part of 2D texture object.
 *  Textures images refer to Texture objects. This is especially handy for atlasing.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGERect.h"
#include "EGEAngle.h"
#include "Core/Resource/Interface/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

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

    /*! Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Engine& engine, ResourceGroup* group);

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PXmlElement& tag) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Creates instance of texture image object defined by resource. 
     *  @note  Loads resource if it is not loaded yet.
     */
    PTextureImage createInstance();
    /*! Set given instance of texture image object to what is defined by resource. 
     *  @note  Loads resource if it is not loaded yet.
     */
    EGEResult setInstance(const PTextureImage& instance);

  private:

    ResourceTextureImage(Engine& engine, ResourceGroup* group);

  private:

    /*! Texture image name. */
    String m_name;
    /*! Name of refered texture. */
    String m_textureName;
    /*! Rectangle area refered by this object. */
    Rectf m_rect;
    /*! Rotation angle. */
    Angle m_rotationAngle;
	  /*! Texture object. NULL if not loaded yet. */
	  PObject m_texture;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCESUBTEXTURE_H
