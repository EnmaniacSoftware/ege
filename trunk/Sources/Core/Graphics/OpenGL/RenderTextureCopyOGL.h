#ifndef EGE_CORE_RENDERTEXTURECOPYOGL_H
#define EGE_CORE_RENDERTEXTURECOPYOGL_H

#include "EGE.h"
#include "EGEOpenGL.h"
#include "Core/Graphics/Render/RenderTarget.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderTextureCopyOGL : public RenderTarget
{
  /* For accessing private data. */
  friend class RenderSystemPrivate;

  public:

    virtual ~RenderTextureCopyOGL();

  private:

    /*! @see RenderTarget::priority. */
    Priority priority() const override;
    /*! @see RenderTarget::bind. */
    void bind() override;
    /*! @see RenderTarget::unbind. */
    void unbind() override;
    /*! @see RenderTarget::requiresTextureFlipping. */
		bool requiresTextureFlipping() const override;

    /*! Returns associated texture id. */
    GLuint textureId() const { return m_textureId; }
    /*! Returns texture target. */
    GLenum textureTarget() const { return m_textureTarget; }
    /*! Returns texture face where data should be rendered to. */
    GLenum faceTarget() const { return m_faceTarget; }

  protected:

    /*! Constructing only via RenderSystem. */
    RenderTextureCopyOGL(Application* app, const Dictionary& params, GLenum textureTarget, GLenum faceTarget, GLuint textureId);

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