#ifndef EGE_CORE_RESOURCETEXTURE_H
#define EGE_CORE_RESOURCETEXTURE_H

/** Class defines texture resource (all properties). From its content actual instance of texture is created.
    Texture resource can define any type of texture i.e. 2D, 3D or CUBE MAP.
*/

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
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

   ~ResourceTexture();

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

    /*! Gets instance of texture object defined by resource. */
    inline PObject texture() const { return m_texture; }

  private:

    ResourceTexture(Application* app, ResourceManager* manager);
    /* Creates 2D texture. */
    EGEResult create2D();
    /*! Gets texture type. */
    inline const String& type() const { return m_type; }
    /* Returns TRUE if texture is loaded. */
    inline bool isLoaded() const { return NULL != m_texture; }
    /*! Gets minifying function name. */
    inline const String& minFilter() const { return m_minFilter; }
    /*! Gets magnification function name. */
    inline const String& magFilter() const { return m_magFilter; }
    /*! Gets texture addressing mode name for S coordinate. */
    inline const String& adressingModeS() const { return m_addressingModeS; }
    /*! Gets texture addressing mode name for T coordinate. */
    inline const String& adressingModeT() const { return m_addressingModeT; }
    /*! Gets path to texture file */
    inline const String& path() const { return m_path; } 

  private:

    /*! Texture name. */
    String m_name;
    /*! Texture path. */
    String m_path;
    /*! Texture type. */
    String m_type;
    /*! Texture minifying function name. */
    String m_minFilter;
    /*! Texture magnification function name. */
    String m_magFilter;
    /*! Texture addressing mode name for S coordinate. */
    String m_addressingModeS;
    /*! Texture addressing mode name for T coordinate. */
    String m_addressingModeT;
    /*! Texture object created from resource. NULL if not created yet. */
    PObject m_texture;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCETEXTURE_H
