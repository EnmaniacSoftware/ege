#ifndef EGE_CORE_RENDERTEXTUREFBOOGL_H
#define EGE_CORE_RENDERTEXTUREFBOOGL_H

#include <EGE.h>
#include <EGEOpenGL.h>
#include "Core/Graphics/Render/RenderTarget.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderTextureFBOOGL : public RenderTarget
{
  public:

    RenderTextureFBOOGL(Application* app, const ConfigParams& params, GLenum textureTarget, GLenum faceTarget, GLuint textureId);
    virtual ~RenderTextureFBOOGL();

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
    /*! Returns FBO id. */
    inline GLuint frameBufferObjectId() const { return m_frameBufferObjectId; }
    /*! Returns texture target. */
    inline GLenum textureTarget() const { return m_textureTarget; }
    /*! Returns texture face where data should be rendered to. */
    inline GLenum faceTarget() const { return m_faceTarget; }

  private:

    /*! Frame buffer object id. */
    GLuint m_frameBufferObjectId;
    /*! Texture id associated with render target. */
    GLuint m_textureId;
    /*! Texture type associated with render target. */
    GLenum m_textureTarget;
    /*! Texture face type data should be rendered to. */
    GLenum m_faceTarget;
    /*! Default frame buffer object id. */
    GLint m_defaultFBOId;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERTEXTUREFBOOGL_H