#ifndef EGE_WIN32_RENDERWINDOWOGL_H
#define EGE_WIN32_RENDERWINDOWOGL_H

#include <EGE.h>
#include <windows.h>
#include "Core/Graphics/Render/RenderWindow.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderWindowOGLWin32 : public RenderWindow
{
  public:

    RenderWindowOGLWin32(Application* app, const Dictionary& params);
    virtual ~RenderWindowOGLWin32();

    /*! Initializes rendering context for worker thread. 
     *  @note This method needs to be called from worker thread.
     */
    void initializeWorkThreadRenderingContext();
    /*! Deinitializes rendering context for worker thread. 
     *  @note This method needs to be called from worker thread.
     */
    void deinitializeWorkThreadRenderingContext();

  private:

    /*! Creates Windows OS OpenGL window. */
    void create(const Dictionary& params);
    /*! Destorys Windows OS OpenGL window. */
    void destroy();
    /*! Returns TRUE if object is valid. */
    bool isValid() const override;
    /*! RenderWindow override. Enables/Disables fullscreen mode. */
    virtual EGEResult enableFullScreen(s32 width, s32 height, bool enable) override;
    /*! RenderTarget override. Returns TRUE if texture flipping is required for this render target. */
		bool requiresTextureFlipping() const override;
    /*! RenderWindow override. Shows frame buffer. */
    void showFrameBuffer() override;
    /*! Selects best pixel format for given parameters. */
    EGEResult setupPixelFormat(const Dictionary& params);

  private:

    /*! Window procedure. */
    static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  private:

    /*! Window handle. */
    HWND m_hWnd;
    /*! Window device context. */
    HDC m_hDC;
    /*! Window OpenGL rendering context. */
    HGLRC m_hRC;
    /*! Work thread OpenGL rendering context. */
    HGLRC m_hRCWorkThread;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_RENDERWINDOWOGL_H