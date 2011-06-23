#ifndef EGE_CORE_RESOURCEFONT_H
#define EGE_CORE_RESOURCEFONT_H

#include <EGE.h>
#include <EGEXml.h>
#include <EGEString.h>
#include <EGEMath.h>
#include <EGEMap.h>
#include "Core/Resource/Resource.h"
#include "Core/Resource/ResourceMaterial.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceFont, PResourceFont)
EGE_DECLARE_SMART_CLASS(Font, PFont)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class GlyphData
{
  public:

    /*! Coordinates on texture of the glyph. */
    Rectf m_textureRect;
    /*! Screen width (in pixels) of glyph. */
    s32 m_width;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceFont : public IResource
{
  public:

    ~ResourceFont();

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
    /*! Returns font. */
    inline PFont font() const { return m_font; }

  private:

    ResourceFont(Application* app, ResourceManager* manager);
    /* Returns TRUE if resource is loaded. */
    bool isLoaded() const;
    /*! Returns material name. */
    inline const EGEString& materialName() const { return m_materialName; } 
    /*! Returns height of the font (in pixels) */
    inline s32 height() const { return m_height; }

  private:

    /*! Name. */
    EGEString m_name;
    /*! Material name. */
    EGEString m_materialName;
    /*! Font screen height (in pixels). */
    s32 m_height;
    /*! Map of glyphs sorted by UTF-16 value. */
    EGEMap<EGEChar, GlyphData> m_glyphs;
    /*! Font object. NULL if not created yet. */
    PFont m_font;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEFONT_H
