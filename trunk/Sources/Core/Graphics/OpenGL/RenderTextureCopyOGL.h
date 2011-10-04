#ifndef EGE_CORE_RENDERTEXTURECOPYOGL_H
#define EGE_CORE_RENDERTEXTURECOPYOGL_H

#include <EGE.h>
#include <EGEOpenGL.h>
#include "Core/Graphics/Render/RenderTarget.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderTextureCopyOGL : public RenderTarget
{
  public:

    RenderTextureCopyOGL(Application* app, const Dictionary& params, GLenum textureTarget, GLenum faceTarget, GLuint textureId);
    virtual ~RenderTextureCopyOGL();

  private:

    /* RenderTarget override. Returns target priority. */
    Priority priority() const override;
    /* RenderTarget override. Makes itself current rendering context. */
    EGEResult makeCurrentContext() override;
    /* RenderTarget override. Releases itself from being current rendering context. */
    void releaseCurrentContext() override;
    /* RenderTarget override. Binds render target. */
    void bind() override;
    /* RenderTarget override. Unbinds render target. */
    void unbind() override;
    /* RenderTarget override. Returns TRUE if texture flipping is required for this render target. */
		bool requiresTextureFlipping() const override;

    /*! Returns associated texture id. */
    inline GLuint textureId() const { return m_textureId; }
    /*! Returns texture target. */
    inline GLenum textureTarget() const { return m_textureTarget; }
    /*! Returns texture face where data should be rendered to. */
    inline GLenum faceTarget() const { return m_faceTarget; }

  private:

    /*! Texture id associated with render target. */
    GLuint m_textureId;
    /*! Texture type associated with render target. */
    GLenum m_textureTarget;
    /*! Texture face type data should be rendered to. */
    GLenum m_faceTarget;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERTEXTURECOPYOGL_H