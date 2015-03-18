#include "Core/Graphics/Font.h"
#include "Core/Graphics/Material.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Font)
EGE_DEFINE_DELETE_OPERATORS(Font)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Font::Font(s32 height, const Map<Char, GlyphData>& glyphData) 
: Object()
, m_height(height)
{
  // copy glyphs data
  for (Map<Char, GlyphData>::ConstIterator it = glyphData.begin(); it != glyphData.end(); ++it)
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
  GlyphData nonExisting;

  const GlyphData& data = m_glyphs.value(c, nonExisting);

  return data.m_textureRect.isNull() ? NULL : &data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector2i Font::metrics(const Text& text) const
{
  Vector2i metrics(0, height());
  for (s32 i = 0; i < text.length(); ++i)
  {
    const GlyphData* data = glyphData(text.at(i));
    if (NULL != data)
    {
      metrics.x += data->m_width;
    }
  }

  return metrics;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END