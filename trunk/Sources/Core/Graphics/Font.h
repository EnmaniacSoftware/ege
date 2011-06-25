#ifndef EGE_CORE_FONT_H
#define EGE_CORE_FONT_H

#include <EGE.h>
#include <EGEString.h>
#include <EGEMap.h>
#include "Core/Graphics/Material.h"
#include "Core/Resource/ResourceFont.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class GlyphData;

EGE_DECLARE_SMART_CLASS(Font, PFont)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Font : public Object
{
  public:

    Font(Application* app, s32 height, const EGEMap<EGEChar, GlyphData>& glyphData);
   ~Font();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if material is valid. */
    bool isValid() const;
    /* Sets material. */
    void setMaterial(PMaterial material);
    /*! Returns material. */
    inline PMaterial material() const { return m_material; }
    /*! Returns font height (in pixels). */
    inline s32 height() const { return m_height; }
    /* Returns pointer to data for a given glyph. NULL if glyph is not defined. */
    const GlyphData* glyphData(EGEChar c) const;
    /* Returns metrics of the given text. */
    Vector2i metrics(const EGEText& text) const;

  private:

    /*! Font material. */
    PMaterial m_material;
    /*! Map of glyphs sorted by UTF-16 value. */
    EGEMap<EGEChar, GlyphData> m_glyphs;
    /*! Font screen height. */
    s32 m_height;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_FONT_H
