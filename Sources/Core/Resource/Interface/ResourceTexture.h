#ifndef EGE_CORE_RESOURCETEXTURE_H
#define EGE_CORE_RESOURCETEXTURE_H

/** Texture resource definition class. This object (resource) contains definition of texture object.
 *  Texture resource can define any type of texture i.e. 2D, 3D or CUBE MAP. Upon loading it contains texture object to be used in rendering.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGETexture.h"
#include "Core/Resource/Interface/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_PREDECLARE_SMART_CLASS(ResourceTexture, PResourceTexture)
EGE_PREDECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceTexture : public IResource
{
  public:

    ResourceTexture(Engine& engine, ResourceGroup* group);
    virtual ~ResourceTexture();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates instance of resource embedding given texture object. This is helper method for manual texture adding. */
    static PResource Create(Engine& engine, ResourceGroup* group, const String& name, PObject texture);

  public:

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PObject& data) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Gets instance of texture object defined by resource. */
    PObject texture() const;

  private:

    /*! Creates 2D texture. */
    EGEResult create2D();
    /*! Gets texture type. */
    const String& type() const;
    /*! Gets minifying function. */
    TextureFilter minFilter() const;
    /*! Gets magnification function. */
    TextureFilter magFilter() const;
    /*! Gets texture addressing mode for S coordinate. */
    TextureAddressingMode adressingModeS() const;
    /*! Gets texture addressing mode for T coordinate. */
    TextureAddressingMode adressingModeT() const;
    /*! Creates and loads texture data from XML text. */
    EGEResult loadTextureData(const PXmlElement& tag);
    /*! Returns mip-mapping flag. */
    bool mipmap() const;

  private slots:

    /*! Slot called when texture request is done. */
    void onRequestComplete(PObject object);

  private:

    /*! Texture name. */
    String m_name;
    /*! Texture type. */
    String m_type;
    /*! Data type. Can be empty if actual file is referred. */
    String m_dataType;
    /*! Data buffer. 
     *  @note Only available if m_dataType is set.
     */
    PDataBuffer m_data;
    /*! Texture minifying function. */
    TextureFilter m_minFilter;
    /*! Texture magnification function. */
    TextureFilter m_magFilter;
    /*! Texture addressing mode for S coordinate. */
    TextureAddressingMode m_addressingModeS;
    /*! Texture addressing mode for T coordinate. */
    TextureAddressingMode m_addressingModeT;
    /*! Texture object created from resource. NULL if not created yet. */
    PObject m_texture;
    /*! Rotation angle. */
    Angle m_rotation;
    /*! Mipmap flag. */
    bool m_mipmap;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCETEXTURE_H
