#include "Core/Overlay/TextOverlay.h"
#include "Core/Graphics/Render/Renderer.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(TextOverlay)
EGE_DEFINE_DELETE_OPERATORS(TextOverlay)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextOverlay::TextOverlay(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) : Overlay(app, name, EGE_OBJECT_UID_OVERLAY_TEXT, deleteFunc), 
                                                                                                 m_alignmentOffset(Vector4f::ZERO),
                                                                                                 m_textDataValid(false),
                                                                                                 m_renderableCharactersCount(0)
{
  initialize();

  ege_connect(physics(), transformationChanged, this, TextOverlay::transformationChanged);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextOverlay::~TextOverlay()
{
  m_font = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets text. */
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
/*! Sets font. */
void TextOverlay::setFont(PFont font)
{
  if ((font != m_font) && font->isValid())
  {
    // store font
    m_font = font;

    // clone font material so we can locally change it without propagation
    m_renderData->setMaterial(m_font->material()->clone());

    // invalidate text data
    m_textDataValid = false;

    // invalidate render data
    invalidate();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates render data. */
void TextOverlay::updateRenderData()
{
  // cache font height
  static const float32 height = static_cast<float32>(font()->height());

  // update vertex data
  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, m_renderableCharactersCount * 6));
  if (data)
  {
    // get initial position
    Vector4f pos = Vector4f::ZERO;

    float32 spacing = 0.0f;

    // go thru all lines of text
    for (TextLineDataList::const_iterator it = m_textLines.begin(); it != m_textLines.end(); ++it)
    {
      const TextLineData& lineData = *it;

      // go thru all characters in current line
      for (Text::const_iterator itChar = lineData.start; itChar != lineData.end; ++itChar)
      {
        // get current glyph texture coords
        const GlyphData* glyphData = font()->glyphData(*itChar);
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
      pos.x = 0.0f;
      pos.y += height;
    }

    // update alignment offset
    transformationChanged();
  }

  m_renderData->vertexBuffer()->unlock();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Overlay override. Sets alignment. */
void TextOverlay::setAlignment(Alignment align)
{
  if (align != alignment())
  {
    // force alignment update
    transformationChanged();

    // call base class
    Overlay::setAlignment(align);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when physics data has been updated. */
void TextOverlay::transformationChanged()
{
  // update alignment offset
  Vector2f size(m_textSize.x * physics()->scale().x, m_textSize.y * physics()->scale().y);

  if (alignment() != 0)
  {
    int a= 1;
  }

  m_alignmentOffset.x = 0;
  m_alignmentOffset.y = 0;
  Math::AlignXY(&m_alignmentOffset, &size, ALIGN_TOP_LEFT, alignment());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns text size (in pixels). */
Vector2f TextOverlay::textSize()
{
  // check if text data is invalid
  if (!m_textDataValid)
  {
    // update text data
    updateTextData();

    // validate text data
    m_textDataValid = true;
  }

  return m_textSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Overlay override. Initializes object. */
void TextOverlay::initialize()
{
  // call base class
  Overlay::initialize();

  m_renderData  = RenderObjectFactory::CreateQuadXY(app(), "overlay-" + name(), Vector4f::ZERO, Vector2f::ONE, ALIGN_TOP_LEFT, 
                                                    EGEVertexBuffer::ST_V2_T2, EGEGraphics::RP_MAIN_OVERLAY, EGEGraphics::RPT_TRIANGLES,
                                                    EGEVertexBuffer::UT_DYNAMIC_WRITE_DONT_CARE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Update text data. */
void TextOverlay::updateTextData()
{
  static const float32 spacing = 0.0f;

  // clear data
  m_textLines.clear();
  m_textSize.set(0, 0);
  m_renderableCharactersCount = 0;

  PFont currentFont = font();

  // go thru all characters
  TextLineData lineData;
  lineData.start = m_text.begin();
  lineData.end   = m_text.end();
  lineData.width = 0.0f;
  for (Text::const_iterator it = m_text.begin(); it != m_text.end(); ++it)
  {
    Char c = *it;

    // check if EOL found
    if ('\n' == c)
    {
      // store end
      lineData.end = it;

      // add to pool
      m_textLines.push_back(lineData);

      // update text width
      m_textSize.x = Math::Max(m_textSize.x, lineData.width);

      // prepare data for next line
      lineData.start = it + 1;
      lineData.end   = m_text.end();
      lineData.width = 0.0f;
      continue;
    }

    // get current glyph data
    const GlyphData* glyphData = currentFont->glyphData(*it);
    if (glyphData)
    {
      lineData.width += static_cast<float32>(glyphData->m_width) + spacing;
    }
    else
    {
      EGE_WARNING("Undefined character found: %C", *it);
    }

    // update renderable characters counter
    ++m_renderableCharactersCount;
  }

  // check if any final line to be added
  if (lineData.start != m_text.end())
  {
    // add to pool
    m_textLines.push_back(lineData);

    // update text width
    m_textSize.x = Math::Max(m_textSize.x, lineData.width);
  }

  // set text total height
  m_textSize.y = static_cast<float32>(m_textLines.size() * currentFont->height());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Overlay override. Renders overlay. */
void TextOverlay::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (visible())
  {
    // check if text data needs updating
    if (!m_textDataValid)
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

    Matrix4f worldMatrix;
    Quaternionf orientation = physics()->orientation();
    Vector4f position = physics()->position() - m_alignmentOffset;
    Vector4f scale = physics()->scale();
    Math::CreateMatrix(&worldMatrix, &position, &scale, &orientation);

    renderer->addForRendering(m_renderData, transform * worldMatrix);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------