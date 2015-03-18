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
  m_physicalWidth  = params.value(EGE_RENDER_TARGET_PARAM_WIDTH, "0").toInt();
  m_physicalHeight = params.value(EGE_RENDER_TARGET_PARAM_HEIGHT, "0").toInt();
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