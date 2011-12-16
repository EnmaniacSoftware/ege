#include "Core/Graphics/OpenGL/RenderTextureCopyOGL.h"
#include <EGETexture.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTextureCopyOGL::RenderTextureCopyOGL(Application* app, const Dictionary& params, GLenum textureTarget, GLenum faceTarget, GLuint textureId) 
: RenderTarget(app, params), 
  m_textureId(textureId), 
  m_textureTarget(textureTarget), 
  m_faceTarget(faceTarget)
{
  // decompose param list
  Dictionary::const_iterator iterWidth  = params.find(EGE_RENDER_TARGET_PARAM_WIDTH);
  Dictionary::const_iterator iterHeight = params.find(EGE_RENDER_TARGET_PARAM_HEIGHT);

  // set physical size to logical one
  m_physicalWidth  = (iterWidth != params.end()) ? iterWidth->second.toInt() : 0;
  m_physicalHeight = (iterHeight != params.end()) ? iterHeight->second.toInt() : 0;
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
  if (GL_NO_ERROR != glGetError())
  {
    EGE_PRINT("RenderTextureCopyOGL::unbind - could not bind texture");
  }
  
  switch (m_textureTarget)
  {
    case GL_TEXTURE_2D:
    //case GL_TEXTURE_CUBE_MAP:

      glCopyTexSubImage2D(faceTarget(), 0, 0, 0, 0, 0, width(), height());
      if (GL_NO_ERROR != glGetError())
      {
        EGE_PRINT("RenderTextureCopyOGL::unbind - glCopyTexSubImage2D failed");
      }
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
