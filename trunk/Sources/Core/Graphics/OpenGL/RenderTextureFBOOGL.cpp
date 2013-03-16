#include "Core/Graphics/OpenGL/RenderTextureFBOOGL.h"
#include <EGEOpenGL.h>
#include <EGETexture.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTextureFBOOGL::RenderTextureFBOOGL(Application* app, const Dictionary& params, GLenum textureTarget, GLenum faceTarget, GLuint textureId) 
: RenderTarget(app, params), 
  m_textureId(textureId), 
  m_textureTarget(textureTarget), 
  m_faceTarget(faceTarget)
{
  // get defult FBO id
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_defaultFBOId);

  // generate new FBO
  glGenFramebuffers(1, &m_frameBufferObjectId);

  if (0 < m_frameBufferObjectId)
  {
    // bind FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObjectId);
    
    // attach texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureTarget, textureId, 0);

    // check status
    GLuint state = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (GL_FRAMEBUFFER_COMPLETE != state)
    {
      egeWarning() << "RenderTextureFBOOGL::RenderTextureFBOOGL Failed with error" << state;
    }

    // unbind for the time being
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBOId);
  }

  // decompose param list
  Dictionary::const_iterator iterWidth  = params.find(EGE_RENDER_TARGET_PARAM_WIDTH);
  Dictionary::const_iterator iterHeight = params.find(EGE_RENDER_TARGET_PARAM_HEIGHT);

  // set physical size to logical one
  m_physicalWidth  = (iterWidth != params.end()) ? iterWidth->second.toInt() : 0;
  m_physicalHeight = (iterHeight != params.end()) ? iterHeight->second.toInt() : 0;

  egeDebug() << "Creating FBO:" << m_physicalWidth << m_physicalHeight << m_defaultFBOId << m_frameBufferObjectId;
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
RenderTarget::Priority RenderTextureFBOOGL::priority() const
{
  return PRIORITY_RENDER_TEXTURE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderTextureFBOOGL::bind()
{
  // bind to FBO
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectId());
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderTextureFBOOGL::unbind()
{
  // bind default
  glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBOId);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderTextureFBOOGL::requiresTextureFlipping() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END