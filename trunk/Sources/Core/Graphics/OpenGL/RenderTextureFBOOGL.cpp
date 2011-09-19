#include "Core/Graphics/OpenGL/RenderTextureFBOOGL.h"
#include <EGEOpenGL.h>
#include <EGETexture.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTextureFBOOGL::RenderTextureFBOOGL(Application* app, const ConfigParams& params, GLenum textureTarget, GLenum faceTarget, GLuint textureId) 
: RenderTarget(app, params), m_textureId(textureId), m_textureTarget(textureTarget), m_faceTarget(faceTarget)
{
  // get defult FBO id
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &m_defaultFBOId);

  // generate new FBO
  glGenFramebuffers(1, &m_frameBufferObjectId);

  if (0 < m_frameBufferObjectId)
  {
    // bind FBO
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_frameBufferObjectId);
    
    // attach texture
    glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, textureTarget, textureId, 0);

    // check status
    GLuint state = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
    if (GL_FRAMEBUFFER_COMPLETE_EXT != state)
    {
      EGE_WARNING("RenderTextureFBOOGL::RenderTextureFBOOGL Failed with error %d!", state);
    }

    // unbind for the time being
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_defaultFBOId);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTextureFBOOGL::~RenderTextureFBOOGL()
{
  if (0 < m_frameBufferObjectId)
  {
    glDeleteFramebuffers(1, &m_frameBufferObjectId);
    m_frameBufferObjectId = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Makes itself current rendering context. */
EGEResult RenderTextureFBOOGL::makeCurrentContext()
{
  // do nothing, this does not change context
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Releases itself from being current rendering context. */
void RenderTextureFBOOGL::releaseCurrentContext()
{
  // do nothing, this does not change context
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Returns target priority. */
RenderTarget::Priority RenderTextureFBOOGL::priority() const
{
  return PRIORITY_RENDER_TEXTURE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Binds render target. */
void RenderTextureFBOOGL::bind()
{
  // bind to FBO
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, frameBufferObjectId());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Unbinds render target. */
void RenderTextureFBOOGL::unbind()
{
  // bind default
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_defaultFBOId);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Returns TRUE if texture flipping is required for this render target. */
bool RenderTextureFBOOGL::requiresTextureFlipping() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
