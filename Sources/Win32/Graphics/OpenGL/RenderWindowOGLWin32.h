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

    RenderWindowOGLWin32(Engine& engine, const Dictionary& params);
    virtual ~RenderWindowOGLWin32();

    /*! Creates object. */
    EGEResult construct(const Dictionary& params);

  private:

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
    /*! Detects rendering capabilities. */
    void detectCapabilities();
    /*! @see RenderWindow::isAutoRotated. */
    bool isAutoRotated() const override;
    /*! Returns engine object. */
    Engine& engine() const;

  private:

    /*! Window procedure. */
    static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  private:

    /*! Reference to engine. */
    Engine& m_engine;
    /*! Window handle. */
    HWND m_hWnd;
    /*! Window device context. */
    HDC m_hDC;
    /*! Window OpenGL rendering context. */
    HGLRC m_hRC;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_RENDERWINDOWOGL_H