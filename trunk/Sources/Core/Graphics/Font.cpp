#include "Core/Graphics/Font.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Font)
EGE_DEFINE_DELETE_OPERATORS(Font)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Font::Font(Application* app, s32 height, const Map<Char, GlyphData>& glyphData) : Object(app), m_height(height)
{
  // copy glyphs data
  for (Map<Char, GlyphData>::const_iterator it = glyphData.begin(); it != glyphData.end(); ++it)
  {
    m_glyphs.insert(it->first, it->second);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Font::~Font()
{
  m_material = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Font::isValid() const
{
  return (NULL != m_material) && m_material->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Font::setMaterial(PMaterial material)
{
  m_material = material;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const GlyphData* Font::glyphData(Char c) const
{
  Map<Char, GlyphData>::const_iterator it = m_glyphs.find(c);
  return (it != m_glyphs.end()) ? &it->second : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector2i Font::metrics(const Text& text) const
{
  Vector2i metrics(0, height());
  for (Text::const_iterator it = text.begin(); it != text.end(); ++it)
  {
    const GlyphData* data = glyphData(*it);
    if (NULL != data)
    {
      metrics.x += data->m_width;
    }
  }

  return metrics;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END