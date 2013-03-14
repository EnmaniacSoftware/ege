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

    RenderWindow(Application* app, const Dictionary& params);
    virtual ~RenderWindow();

    /*! Returns window title. */
    const String title() const { return m_title; }
    /*! Shows frame buffer. */
    virtual void showFrameBuffer() = 0;
    
  private:

    /*! RenderTarget override. Returns target priority. */
    Priority priority() const override;
    /*! RenderTarget override. Binds render target. */
    void bind() override;
    /*! RenderTarget override. Unbinds render target. */
    void unbind() override;

  protected:

    /*! Enables/Disables fullscreen mode. */
    virtual EGEResult enableFullScreen(s32 width, s32 height, bool enable) = 0;
    
  protected:

    /*! TRUE if fullscreen mode is enabled. */
    bool m_fullScreen;
    /*! Title. */
    String m_title;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERWINDOW_H