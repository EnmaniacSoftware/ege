#include "Core/Overlay/TextOverlay.h"
#include "Core/Graphics/Render/Renderer.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(TextOverlay)
EGE_DEFINE_DELETE_OPERATORS(TextOverlay)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextOverlay::TextOverlay(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) 
: Overlay(app, name, EGEGraphics::RPT_TRIANGLES, EGE_OBJECT_UID_OVERLAY_TEXT, deleteFunc), m_alignmentOffset(Vector4f::ZERO)
{
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
    m_text = text;
    invalidate();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Overlay override. Updates overlay. */
void TextOverlay::update(const Time& time)
{
  // call base class
  Overlay::update(time);

  if (visible())
  {
    // check if update is needed
    if (isUpdateNeeded())
    {
      // update render data
      updateRenderData();

      // validate
      validate();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets font. */
void TextOverlay::setFont(PFont font)
{
  if (font != m_font && font->isValid())
  {
    m_font = font;
    invalidate();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates render data. */
void TextOverlay::updateRenderData()
{
  Color color = Color::WHITE;

  // set material
  renderData()->setMaterial(m_font->material());

  // allocate vertex buffer of required size
  float32* data = (float32*) renderData()->vertexBuffer()->lock(0, text().length() * 6);
  if (data)
  {
    // get initial position
    Vector4f pos = Vector4f::ZERO;

    float32 spacing = 0.0f;

    // reset text size
    m_textSize.set(0, static_cast<float32>(font()->height()));

    // go thru all characters
    for (Text::const_iterator it = text().begin(); it != text().end(); ++it)
    {
      // get current glyph texture coords
      const GlyphData* glyphData = font()->glyphData(*it);
      if (glyphData)
      {
        float32 width  = static_cast<float32>(glyphData->m_width);
        float32 height = static_cast<float32>(font()->height());

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
        *data++ = 0;
        *data++ = glyphData->m_textureRect.x;
        *data++ = glyphData->m_textureRect.y;
        *data++ = color.red;
        *data++ = color.green;
        *data++ = color.blue;
        *data++ = color.alpha;

        *data++ = pos.x;
        *data++ = pos.y + height;
        *data++ = 0;
        *data++ = glyphData->m_textureRect.x;
        *data++ = glyphData->m_textureRect.y + glyphData->m_textureRect.height;
        *data++ = color.red;
        *data++ = color.green;
        *data++ = color.blue;
        *data++ = color.alpha;

        *data++ = pos.x + width;
        *data++ = pos.y + height;
        *data++ = 0;
        *data++ = glyphData->m_textureRect.x + glyphData->m_textureRect.width;
        *data++ = glyphData->m_textureRect.y + glyphData->m_textureRect.height;
        *data++ = color.red;
        *data++ = color.green;
        *data++ = color.blue;
        *data++ = color.alpha;

        *data++ = pos.x;
        *data++ = pos.y;
        *data++ = 0;
        *data++ = glyphData->m_textureRect.x;
        *data++ = glyphData->m_textureRect.y;
        *data++ = color.red;
        *data++ = color.green;
        *data++ = color.blue;
        *data++ = color.alpha;

        *data++ = pos.x + width;
        *data++ = pos.y + height;
        *data++ = 0;
        *data++ = glyphData->m_textureRect.x + glyphData->m_textureRect.width;
        *data++ = glyphData->m_textureRect.y + glyphData->m_textureRect.height;
        *data++ = color.red;
        *data++ = color.green;
        *data++ = color.blue;
        *data++ = color.alpha;

        *data++ = pos.x + width;
        *data++ = pos.y;
        *data++ = 0;
        *data++ = glyphData->m_textureRect.x + glyphData->m_textureRect.width;
        *data++ = glyphData->m_textureRect.y;
        *data++ = color.red;
        *data++ = color.green;
        *data++ = color.blue;
        *data++ = color.alpha;

        pos.x += (width + spacing);

        // update total width
        m_textSize.x += (width + spacing);
      }
    }

    // update alignment offset
    transformationChanged();
  }

  renderData()->vertexBuffer()->unlock();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Overlay override. Renders element. */
void TextOverlay::render(const Viewport* viewport, Renderer* renderer)
{
  if (visible())
  {
    PRenderComponent renderComponent = this->renderData();

    Matrix4f worldMatrix;
    if (NULL != physics())
    {
      Quaternionf orientation = physics()->orientation();
      Vector4f position = physics()->position() - m_alignmentOffset;
      Vector4f scale = physics()->scale();

      Math::CreateMatrix(&worldMatrix, &position, &scale, &orientation);
    }
    else
    {
      worldMatrix = Matrix4f::IDENTITY;
    }

    renderer->addForRendering(worldMatrix, renderComponent);
  }
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
  Math::AlignXY(&m_alignmentOffset, &size, EGEAlignment::ALIGN_TOP_LEFT, alignment());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
