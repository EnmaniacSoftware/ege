#ifndef EGE_IOS_RENDERWINDOWOGL_H
#define EGE_IOS_RENDERWINDOWOGL_H

#include "EGE.h"
#include "EGEOpenGL.h"
#include "Core/Graphics/Render/RenderWindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
@class OGLView;
@class UIWindow;
@class EAGLContext;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderWindowOGLIOS : public RenderWindow
{
  public:

    RenderWindowOGLIOS(Application* app, const Dictionary& params);
    virtual ~RenderWindowOGLIOS();

    /*! Creates object. */
    EGEResult construct(const Dictionary& params);
    /*! Returns iOS view object. */
    OGLView* view() const;
  
  private:

    /*! Destorys Airplay OpenGL window. */
    void destroy();
    /*! Returns TRUE if object is valid. */
    bool isValid() const override;
    /*! RenderWindow override. Makes rendering context calling thread's current rendering context. */
    EGEResult makeCurrentContext() override;
    /*! RenderWindow override. Removes calling thread's current rendering context. */
    void releaseCurrentContext() override;
    /*! RenderWindow override. Enables/Disables fullscreen mode. */
    EGEResult enableFullScreen(s32 width, s32 height, bool enable) override;
    /*! RenderTarget override. Returns TRUE if texture flipping is required for this render target. */
		bool requiresTextureFlipping() const override;
    /*! RenderWindow override. Shows frame buffer. */
    virtual void showFrameBuffer() override;
    /*! Detects rendering capabilities. */
    void detectCapabilities();

  private:

    /*! Orientation change callback. */
    //static int32 OrientationChangeCB(void* systemData, void* data);

  private:

    /*! iOS window view. */
    OGLView* m_view;
    /*! iOS window. */
    UIWindow* m_window;
    /*! Apple EGL context. */
    EAGLContext* m_EAGLContext;
    /*! Main window color buffer id. */
    GLuint m_colorRenderBuffer;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_RENDERWINDOWOGL_H