#ifndef EGE_IOS_RENDERWINDOWOGL_H
#define EGE_IOS_RENDERWINDOWOGL_H

#include "EGE.h"
#include "EGEOpenGL.h"
#include "EGEEvent.h"
#include "EGEDevice.h"
#include "Core/Graphics/Render/RenderWindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
@class OGLView;
@class UIWindow;
@class EAGLContext;
@class ViewController;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderWindowOGLIOS : public RenderWindow
                         , public IEventListener
{
  public:

    RenderWindowOGLIOS(Engine& engine, const Dictionary& params);
    virtual ~RenderWindowOGLIOS();

    /*! Creates object. */
    EGEResult construct(const Dictionary& params);
    /*! Returns iOS view object. */
    OGLView* view() const;
    /*! Returns application window. */
    UIWindow* window() const;
  
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
    /*! @see RenderTarget::isAutoRotated. */
    bool isAutoRotated() const override;
 
    /*! RenderWindow override. Shows frame buffer. */
    virtual void showFrameBuffer() override;
    /*! Detects rendering capabilities. */
    void detectCapabilities();

    /*! Returns engine object. */
    Engine& engine() const;
  
  private:
  
    /*! @see IEventListener::onEventRecieved. */
    void onEventRecieved(PEvent event) override;
    /*! Sets new orientation. */
    void setOrientation(DeviceOrientation orientation);
    /*! Resizes render buffers. */
    void resizeRenderBuffers();
  
  private:

    /*! Reference to engine. */
    Engine& m_engine;
    /*! iOS window view. */
    OGLView* m_view;
    /*! iOS view controller. */
    ViewController* m_viewController;
    /*! iOS window. */
    UIWindow* m_window;
    /*! Apple EGL context. */
    EAGLContext* m_EAGLContext;
    /*! Main window color buffer id. */
    GLuint m_colorBuffer;
    /*! Main window depth buffer id. */
    GLuint m_depthBuffer;
    /*! Main window framebuffer id. */
    GLuint m_frameBuffer;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_RENDERWINDOWOGL_H