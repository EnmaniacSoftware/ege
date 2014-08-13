#include "Core/Graphics/OpenGL/RenderTextureCopyOGL.h"
#include "EGEOpenGL.h"
#include "EGETexture.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTextureCopyOGL::RenderTextureCopyOGL(const Dictionary& params, GLenum textureTarget, GLenum faceTarget, GLuint textureId) 
: RenderTarget(params)
, m_textureId(textureId)
, m_textureTarget(textureTarget)
, m_faceTarget(faceTarget)
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
RenderTarget::Priority RenderTextureCopyOGL::priority() const
{
  return PRIORITY_RENDER_TEXTURE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderTextureCopyOGL::bind()
{
  // do nothing, no extra work is required
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderTextureCopyOGL::unbind()
{
  glBindTexture(textureTarget(), textureId());
  OGL_CHECK();
  
  switch (m_textureTarget)
  {
    case GL_TEXTURE_2D:
    //case GL_TEXTURE_CUBE_MAP:

      glCopyTexSubImage2D(faceTarget(), 0, 0, 0, 0, 0, width(), height());
      break;
  }

  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderTextureCopyOGL::requiresTextureFlipping() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderTextureCopyOGL::isAutoRotated() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END