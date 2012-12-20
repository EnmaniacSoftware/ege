#ifndef EGE_CORE_TEXTOVERLAY_H
#define EGE_CORE_TEXTOVERLAY_H

#include <EGETime.h>
#include <EGEString.h>
#include <EGEList.h>
#include "Core/Overlay/Overlay.h"
#include "Core/Graphics/Font.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(TextOverlay, PTextOverlay)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief TextOverlay is an overlay containing raw text. Text dimensions will define final size of overlay. */
class TextOverlay : public Overlay
{
  public: 

    TextOverlay(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~TextOverlay();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Sets text. */
    void setText(const Text& text);
    /*! Returns text. */
    const Text& text() const { return m_text; }
    /*! Sets font. */
    void setFont(PFont font);
    /*! Returns font. */
    PFont font() const { return m_font; }
    /*! Sets text alignment. */
    void setTextAlignment(Alignment alignment);
    /*! Returns text alignment. */
    Alignment textAlignment() const { return m_textAlignment; }

    /*! @see Overlay::addForRendering. */
    void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;
    /*! @see Overlay::size. */
    const Vector2f& size() const override;

  private:

    /*! @see Overlay::initialize. */
    void initialize() override;
    /*! Updates render data. */
    void updateRenderData();
    /*! Update text data. */
    void updateTextData();

  private:

    /*! Single text line data structure. */
    struct TextLineData
    {
      Text::const_iterator start;         /*!< Iterator to first character in line. */
      Text::const_iterator end;           /*!< Iterator to one after last character in line. */

      float32 width;                      /*!< Line width (in pixels). */
    };

    typedef List<TextLineData> TextLineDataList;

  private:

    /*! Overlay text. */
    Text m_text;
    /*! Font. */
    PFont m_font;
    /*! List of text lines. */
    TextLineDataList m_textLines;
    /*! Text data validity flag. */
    bool m_textDataValid;
    /*! Renderable characters count. */
    u32 m_renderableCharactersCount;
    /*! Text (content) alignment. */
    Alignment m_textAlignment;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXTOVERLAY_H
