#ifndef EGE_CORE_RESOURCEFONT_H
#define EGE_CORE_RESOURCEFONT_H

/** Font resource definition class. This object (resource) contains definition of bitmap based font.
 *   Upon loading it contains font object to be used in font rendering.
 */

#include "EGE.h"
#include "EGEXml.h"
#include "EGEString.h"
#include "EGEMath.h"
#include "EGEMap.h"
#include "EGERect.h"
#include "Core/Resource/Interface/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

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

    virtual ~ResourceFont();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Engine& engine, ResourceGroup* group);
    /*! Creates instance of resource embedding given font object. This is helper method for manual font adding. */
    static PResource Create(Engine& engine, ResourceGroup* group, const String& name, PFont font);

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PObject& data) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Returns font. */
    PFont font() const { return m_font; }

  private:

    ResourceFont(Engine& engine, ResourceGroup* group);
    /*! Returns material name. */
    const String& materialName() const { return m_materialName; } 
    /*! Returns height of the font (in pixels) */
    s32 height() const { return m_height; }

  private:

    /*! Name. */
    String m_name;
    /*! Material name. */
    String m_materialName;
    /*! Font screen height (in pixels). */
    s32 m_height;
    /*! Map of glyphs sorted by UTF-16 value. */
    Map<Char, GlyphData> m_glyphs;
    /*! Font object. NULL if not created yet. */
    PFont m_font;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEFONT_H
