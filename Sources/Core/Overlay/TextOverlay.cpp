#include "Core/Overlay/TextOverlay.h"
#include "EGERenderer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KTextOverlayDebugName = "EGETextOverlay";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(TextOverlay)
EGE_DEFINE_DELETE_OPERATORS(TextOverlay)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextOverlay::TextOverlay(Engine& engine, const String& name, egeObjectDeleteFunc deleteFunc) 
: Overlay(engine, name, EGE_OBJECT_UID_OVERLAY_TEXT, deleteFunc)
, m_textDataValid(false)
, m_renderableCharactersCount(0)
, m_textAlignment(ALIGN_TOP_LEFT)
{
  initialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextOverlay::~TextOverlay()
{
  m_font = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextOverlay::setText(const Text& text)
{
  if (m_text != text)
  {
    // store new text
    m_text = text;

    // invalidate text data
    m_textDataValid = false;

    // invalidate render data
    invalidate();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextOverlay::setFont(PFont font)
{
  if (font != m_font)
  {
    // store font
    m_font = font;

    // clone font material so we can locally change it without propagation
    m_renderData->setMaterial(m_font ? m_font->material()->clone() : NULL);

    // invalidate text data
    m_textDataValid = false;

    // invalidate render data
    invalidate();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextOverlay::updateRenderData()
{
  // update vertex data
  if (m_renderData->vertexBuffer()->setSize(m_renderableCharactersCount * 6))
  {
    // cache font height
    const float32 height = static_cast<float32>(font()->height());

    float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, m_renderableCharactersCount * 6));
    if (NULL != data)
    {
      float32 spacing = 0.0f;

      Vector2f pos(0, 0);

      // go thru all lines of text
      float32 startPosX = pos.x;
      for (TextLineDataList::const_iterator it = m_textLines.begin(); it != m_textLines.end(); ++it)
      {
        const TextLineData& lineData = *it;

        // apply text alignment
        // NOTE: text is aligned with respect to overall size.
        if (m_textAlignment & ALIGN_RIGHT)
        {
          pos.x += (size().x - lineData.width);
        }
        else if (m_textAlignment & ALIGN_HCENTER)
        {
          pos.x += Math::Floor((size().x - lineData.width) * 0.5f);
        }

        // go thru all characters in current line
        for (s32 i = lineData.start; i < lineData.end; ++i)
        {
          // get current glyph texture coords
          const Char c = m_text.at(i);
          const GlyphData* glyphData = font()->glyphData(c);
          if (glyphData)
          {
            float32 width = static_cast<float32>(glyphData->m_width);

            // Glyph quad looks like follows:
            //
            //   (0,3)  (5)
            //    *------*
            //    |\     |
            //    | \Tri2|
            //    |  \   |
            //    |   \  |
            //    |    \ |
            //    |Tri1 \|
            //    |      |
            //    *------*
            //   (1)   (2,4)

            *data++ = pos.x;
            *data++ = pos.y;
            *data++ = glyphData->m_textureRect.x;
            *data++ = glyphData->m_textureRect.y;

            *data++ = pos.x;
            *data++ = pos.y + height;
            *data++ = glyphData->m_textureRect.x;
            *data++ = glyphData->m_textureRect.y + glyphData->m_textureRect.height;

            *data++ = pos.x + width;
            *data++ = pos.y + height;
            *data++ = glyphData->m_textureRect.x + glyphData->m_textureRect.width;
            *data++ = glyphData->m_textureRect.y + glyphData->m_textureRect.height;

            *data++ = pos.x;
            *data++ = pos.y;
            *data++ = glyphData->m_textureRect.x;
            *data++ = glyphData->m_textureRect.y;

            *data++ = pos.x + width;
            *data++ = pos.y + height;
            *data++ = glyphData->m_textureRect.x + glyphData->m_textureRect.width;
            *data++ = glyphData->m_textureRect.y + glyphData->m_textureRect.height;

            *data++ = pos.x + width;
            *data++ = pos.y;
            *data++ = glyphData->m_textureRect.x + glyphData->m_textureRect.width;
            *data++ = glyphData->m_textureRect.y;

            pos.x += (width + spacing);
          }
        }

        // next line of text
        pos.y += height;
        pos.x = startPosX;
      }

      m_renderData->vertexBuffer()->unlock(data - 1);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Vector2f& TextOverlay::size() const
{
  // check if text data is invalid
  if ( ! m_textDataValid)
  {
    // NOTE: nasty, but find it less obscure than mutabling everything around

    // update text data
    const_cast<TextOverlay*>(this)->updateTextData();

    // validate text data
    const_cast<TextOverlay*>(this)->m_textDataValid = true;
  }

  return Overlay::size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextOverlay::initialize()
{
  // call base class
  Overlay::initialize();

  // create vertex declaration
  VertexDeclaration declaration;
  if (declaration.addElement(NVertexBuffer::VES_POSITION_XY) && declaration.addElement(NVertexBuffer::VES_TEXTURE_UV))
  {
    // create render buffer
    m_renderData  = ege_new RenderComponent(engine(), "overlay-" + name(), declaration, EGEGraphics::RP_MAIN_OVERLAY, EGEGraphics::RPT_TRIANGLES,
                                            NVertexBuffer::UT_DYNAMIC_WRITE_DONT_CARE);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextOverlay::updateTextData()
{
  const float32 spacing = 0.0f;

  Vector2f textSize = Vector2f::ZERO;

  // clear data
  m_textLines.clear();
  m_renderableCharactersCount = 0;

  PFont currentFont = font();

  // go thru all characters
  TextLineData lineData;
  lineData.start = 0;
  lineData.end   = m_text.length();
  lineData.width = 0.0f;
  for (s32 i = 0; i < m_text.length(); ++i)
  {
    Char c = m_text.at(i);

    // check if EOL found
    if ('\n' == c)
    {
      // store end
      lineData.end = i;

      // add to pool
      m_textLines.push_back(lineData);

      // update text width
      textSize.x = Math::Max(textSize.x, lineData.width);

      // prepare data for next line
      lineData.start = i + 1;
      lineData.end   = m_text.length();
      lineData.width = 0.0f;
      continue;
    }

    // get current glyph data
    const GlyphData* glyphData = currentFont->glyphData(c);
    if (glyphData)
    {
      lineData.width += static_cast<float32>(glyphData->m_width) + spacing;
    }
    else
    {
      egeWarning(KTextOverlayDebugName) << "Undefined character found:" << (s32) c;
      continue;
    }

    // update renderable characters counter
    ++m_renderableCharactersCount;
  }

  // check if any final line to be added
  if (lineData.start != m_text.length())
  {
    // add to pool
    m_textLines.push_back(lineData);

    // update text width
    textSize.x = Math::Max(textSize.x, lineData.width);
  }

  // set text total height
  textSize.y = static_cast<float32>(m_textLines.size() * currentFont->height());

  // update overlay size
  setSize(textSize);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextOverlay::addForRendering(IRenderer* renderer, const Matrix4f& transform)
{
  if (visible())
  {
    // check if text data needs updating
    if ( ! m_textDataValid)
    {
      // update text data
      updateTextData();

      // invalidate render data
      invalidate();

      // validate text data
      m_textDataValid = true;
    }

    // check if render data is invalid
    if (isUpdateNeeded())
    {
      // update render data
      updateRenderData();

      // validate render data
      validate();
    }

    Vector2f pos(physics()->position().x, physics()->position().y);
    Vector2f finalSize(size().x, size().y);
    finalSize.x *= physics()->scale().x;
    finalSize.y *= physics()->scale().y;
    pos = Math::Align(pos, finalSize, ALIGN_TOP_LEFT, alignment());

    Matrix4f matrix = Math::CreateMatrix(Vector4f(pos.x, pos.y, physics()->position().z), physics()->scale(), Quaternionf::IDENTITY);

    renderer->addForRendering(m_renderData, transform * matrix);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextOverlay::setTextAlignment(Alignment alignment)
{
  m_textAlignment = alignment;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END