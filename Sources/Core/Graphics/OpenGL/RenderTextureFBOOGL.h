#ifndef EGE_CORE_RENDERTEXTUREFBOOGL_H
#define EGE_CORE_RENDERTEXTUREFBOOGL_H

#include "EGE.h"
#include "EGEOpenGL.h"
#include "Core/Graphics/Render/RenderTarget.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderTextureFBOOGL : public RenderTarget
{
  /* For accessing private data. */
  friend class RenderSystemPrivate;
 
  public:

    RenderTextureFBOOGL(const Dictionary& params, GLenum textureTarget, GLenum faceTarget, GLuint textureId);
    virtual ~RenderTextureFBOOGL();

  private:

    /*! @see RenderTarget::priority. */
    Priority priority() const override;
    /*! @see RenderTarget::bind. */
    void bind() override;
    /*! @see RenderTarget::unbind. */
    void unbind() override;
    /*! @see RenderTarget::requiresTextureFlipping. */
		bool requiresTextureFlipping() const override;
    /*! @see RenderTarget::isAutoRotated. */
    bool isAutoRotated() const override;

    /*! Returns associated texture id. */
    GLuint textureId() const { return m_textureId; }
    /*! Returns FBO id. */
    GLuint frameBufferObjectId() const { return m_frameBufferObjectId; }
    /*! Returns texture target. */
    GLenum textureTarget() const { return m_textureTarget; }
    /*! Returns texture face where data should be rendered to. */
    GLenum faceTarget() const { return m_faceTarget; }

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