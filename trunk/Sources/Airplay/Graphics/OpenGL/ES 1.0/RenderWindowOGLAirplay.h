#ifndef EGE_AIRPLAY_RENDERWINDOWOGL_H
#define EGE_AIRPLAY_RENDERWINDOWOGL_H

#include "EGE.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "GLES/egl.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderWindowOGLAirplay : public RenderWindow
{
  public:

    RenderWindowOGLAirplay(Application* app, const ConfigParams& params);
    virtual ~RenderWindowOGLAirplay();

  private:

    /* Creates Airplay OpenGL window. */
    void create(const ConfigParams& params);
    /* Destorys Airplay OpenGL window. */
    void destroy();
    /* Returns TRUE if object is valid. */
    virtual bool isValid() const override;
    /* RenderWindow override. Makes rendering context calling thread's current rendering context. */
    virtual EGEResult makeCurrentContext() override;
    /* RenderWindow override. Removes calling thread's current rendering context. */
    virtual void releaseCurrentContext() override;
    /* RenderWindow override. Enables/Disables fullscreen mode. */
    virtual EGEResult enableFullScreen(s32 width, s32 height, bool enable) override;
    /* RenderWindow override. Shows frame buffer. */
    virtual void showFrameBuffer() override;

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

#endif // EGE_AIRPLAY_RENDERWINDOWOGL_H