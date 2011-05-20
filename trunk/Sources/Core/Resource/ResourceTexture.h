#ifndef EGE_CORE_RESOURCETEXTURE_H
#define EGE_CORE_RESOURCETEXTURE_H

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
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
    const EGEString& name() const override;
    /* Initializes resource from XML. 
    * 
    *  \param  path  full path to resource definition file.
    *  \param  tag   xml element with resource definition. 
    */
    EGEResult create(const EGEString& path, const PXmlElement& tag) override;
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
    inline const EGEString& type() const { return m_type; }
    /* Returns TRUE if texture is loaded. */
    inline bool isLoaded() const { return NULL != m_texture; }
    /*! Gets minifying function name. */
    inline const EGEString& minFilter() const { return m_minFilter; }
    /*! Gets magnification function name. */
    inline const EGEString& magFilter() const { return m_magFilter; }
    /*! Gets wrap S coordinate mode name. */
    inline const EGEString& wrapS() const { return m_wrapS; }
    /*! Gets wrap T coordinate mode name. */
    inline const EGEString& wrapT() const { return m_wrapT; }
    /*! Gets path to texture file */
    inline const EGEString& path() const { return m_path; } 

  private:

    /*! Texture name */
    EGEString m_name;
    /*! Texture path */
    EGEString m_path;
    /*! Texture type */
    EGEString m_type;
    /*! Texture minifying function name. */
    EGEString m_minFilter;
    /*! Texture magnification function name. */
    EGEString m_magFilter;
    /*! Texture wrap S coordinate mode name. */
    EGEString m_wrapS;
    /*! Texture wrap T coordinate mode name. */
    EGEString m_wrapT;
    /*! Texture object created from resource. NULL if not created yet. */
    PObject m_texture;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCETEXTURE_H
