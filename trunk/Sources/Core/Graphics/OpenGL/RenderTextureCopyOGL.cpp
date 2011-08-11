#include "Core/Graphics/OpenGL/RenderTextureCopyOGL.h"
#include <EGETexture.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTextureCopyOGL::RenderTextureCopyOGL(Application* app, const ConfigParams& params, GLenum textureTarget, GLenum faceTarget, GLuint textureId) 
: RenderTarget(app, params), m_textureId(textureId), m_textureTarget(textureTarget), m_faceTarget(faceTarget)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTextureCopyOGL::~RenderTextureCopyOGL()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Makes itself current rendering context. */
EGEResult RenderTextureCopyOGL::makeCurrentContext()
{
  // do nothing, this does not change context
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Releases itself from being current rendering context. */
void RenderTextureCopyOGL::releaseCurrentContext()
{
  // do nothing, this does not change context
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Returns target priority. */
RenderTarget::Priority RenderTextureCopyOGL::priority() const
{
  return PRIORITY_RENDER_TEXTURE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Binds render target. */
void RenderTextureCopyOGL::bind()
{
  // do nothing, no extra work is required
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Unbinds render target. */
void RenderTextureCopyOGL::unbind()
{
  glBindTexture(textureTarget(), textureId());
  
  switch (m_textureTarget)
  {
    case GL_TEXTURE_2D:
    //case GL_TEXTURE_CUBE_MAP:

      glCopyTexSubImage2D(faceTarget(), 0, 0, 0, 0, 0, width(), height()); 
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Returns TRUE if texture flipping is required for this render target. */
bool RenderTextureCopyOGL::requiresTextureFlipping() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
