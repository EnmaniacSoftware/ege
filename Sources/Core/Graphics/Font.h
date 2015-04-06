#ifndef EGE_CORE_GRAPHICS_FONT_H
#define EGE_CORE_GRAPHICS_FONT_H

#include "EGE.h"
#include "EGEText.h"
#include "EGEMap.h"
#include "Core/Resource/Interface/ResourceFont.h"

EGE_NAMESPACE_BEGIN
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class GlyphData;

EGE_PREDECLARE_SMART_CLASS(Material, PMaterial)
EGE_PREDECLARE_SMART_CLASS(Font, PFont)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Font : public Object
{
  public:

    Font(s32 height, const Map<Char, GlyphData>& glyphData);
   ~Font();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns TRUE if material is valid. */
    bool isValid() const;
    /*! Sets material. */
    void setMaterial(PMaterial material);
    /*! Returns material. */
    PMaterial material() const { return m_material; }
    /*! Returns font height (in pixels). */
    s32 height() const { return m_height; }
    /*! Returns pointer to data for a given glyph. NULL if glyph is not defined. */
    const GlyphData* glyphData(Char c) const;
    /*! Returns metrics of the given text. */
    Vector2i metrics(const Text& text) const;

  private:

    /*! Font material. */
    PMaterial m_material;
    /*! Map of glyphs sorted by UTF-16 value. */
    Map<Char, GlyphData> m_glyphs;
    /*! Font screen height. */
    s32 m_height;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_FONT_H
