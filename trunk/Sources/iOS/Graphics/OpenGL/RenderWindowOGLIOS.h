#ifndef EGE_IOS_RENDERWINDOWOGL_H
#define EGE_IOS_RENDERWINDOWOGL_H

#include <EGE.h>
#include <GLES/egl.h>
#include "Core/Graphics/Render/RenderWindow.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderWindowOGLIOS : public RenderWindow
{
  public:

    RenderWindowOGLIOS(Application* app, const Dictionary& params);
    virtual RenderWindowOGLIOS();

    /*! Creates object. */
    EGEResult construct(const Dictionary& params);

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

  private:

    /*! Orientation change callback. */
    static int32 OrientationChangeCB(void* systemData, void* data);

  private:

    /*! OpenGLES display device. */
    EGLDisplay m_eglDisplay;
    /*! OpengGLES context. */
    EGLContext m_eglContext;
    /*! OpenGLES display surface. */
    EGLSurface m_eglSurface;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_RENDERWINDOWOGL_H