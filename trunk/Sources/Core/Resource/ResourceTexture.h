#ifndef EGE_CORE_RESOURCETEXTURE_H
#define EGE_CORE_RESOURCETEXTURE_H

/** Texture resource definition class. This object (resource) contains definition of texture object.
 *  Texture resource can define any type of texture i.e. 2D, 3D or CUBE MAP. Upon loading it contains texture object to be used in rendering.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGETexture.h>
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceTexture, PResourceTexture)
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceTexture : public IResource
{
  public:

    virtual ~ResourceTexture();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceManager* manager);
    /* Creates instance of resource embedding given texture object. This is helper method for manual texture adding. */
    static PResource Create(Application* app, ResourceManager* manager, const String& name, PObject texture);

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
    /* IResource override. Returns TRUE if object is loaded. */
    bool isLoaded() const override;

    /*! Gets instance of texture object defined by resource. */
    inline PObject texture() const { return m_texture; }

  private:

    ResourceTexture(Application* app, ResourceManager* manager);
    /* Creates 2D texture. */
    EGEResult create2D();
    /*! Gets texture type. */
    inline const String& type() const { return m_type; }
    /*! Gets minifying function. */
    inline EGETexture::Filter minFilter() const { return m_minFilter; }
    /*! Gets magnification function. */
    inline EGETexture::Filter magFilter() const { return m_magFilter; }
    /*! Gets texture addressing mode for S coordinate. */
    inline EGETexture::AddressingMode adressingModeS() const { return m_addressingModeS; }
    /*! Gets texture addressing mode for T coordinate. */
    inline EGETexture::AddressingMode adressingModeT() const { return m_addressingModeT; }
    /*! Gets path to texture file */
    inline const String& path() const { return m_path; } 

  private:

    /*! Texture name. */
    String m_name;
    /*! Texture path. */
    String m_path;
    /*! Texture type. */
    String m_type;
    /*! Texture minifying function. */
    EGETexture::Filter m_minFilter;
    /*! Texture magnification function. */
    EGETexture::Filter m_magFilter;
    /*! Texture addressing mode for S coordinate. */
    EGETexture::AddressingMode m_addressingModeS;
    /*! Texture addressing mode for T coordinate. */
    EGETexture::AddressingMode m_addressingModeT;
    /*! Texture object created from resource. NULL if not created yet. */
    PObject m_texture;
    /*! Rotation angle. */
    Angle m_rotation;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCETEXTURE_H
