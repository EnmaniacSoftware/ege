#ifndef EGE_CORE_RENDERWINDOW_H
#define EGE_CORE_RENDERWINDOW_H

#include "EGE.h"
#include "Core/Graphics/Render/RenderTarget.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(RenderWindow, PRenderWindow)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderWindow : public RenderTarget
{
  public:

    RenderWindow(Application* app, const ConfigParams& params);
    virtual ~RenderWindow();

    /*! Returns window title. */
    inline const EGEString title() const { return m_title; }
    /*! Shows frame buffer. */
    virtual void showFrameBuffer() = 0;
    /*! Returns window width (in pixels). This value is affected by landscape mode. */
    inline s32 width() const { return m_width; }
    /*! Returns window height (in pixels). This value is affected by landscape mode. */
    inline s32 height() const { return m_height; }

  private:

    /* RenderTarget override. Returns target priority. */
    virtual Priority priority() const override;

  protected:

    /* Enables/Disables fullscreen mode. */
    virtual EGEResult enableFullScreen(s32 width, s32 height, bool enable) = 0;
    
  protected:

    /*! TRUE if fullscreen mode is enabled. */
    bool m_fullScreen;
    /*! Window width (in pixels). This value is affected by landscape mode. */
    s32 m_width;
    /*! Window height (in pixels). This value is affected by landscape mode. */
    s32 m_height;
    /*! Title. */
    EGEString m_title;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERWINDOW_H