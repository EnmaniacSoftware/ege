#include "Core/Graphics/Material.h"
#include "Core/Graphics/OpenGL/MaterialOGL_p.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(MaterialPrivate)
EGE_DEFINE_DELETE_OPERATORS(MaterialPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MaterialPrivate::MaterialPrivate(Material* base) : m_base(base), m_srcBlendFunc(GL_ONE), m_dstBlendFunc(GL_ZERO)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MaterialPrivate::~MaterialPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets source pixel scale function. */
void MaterialPrivate::setSrcBlendFunc(const EGEString& funcName)
{
  // TAGE - seems like Microsoft's implementation only allows this:
  // GL_ZERO, GL_ONE, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA_SATURATE
  //
  // OpenGLES allows this:
  // GL_ZERO, GL_ONE, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA_SATURATE + GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR.

  if ("zero" == funcName)
  {
    m_srcBlendFunc = GL_ZERO;
  }
  else if ("one" == funcName)
  {
    m_srcBlendFunc = GL_ONE;
  }
  else if ("dst-color" == funcName)
  {
    m_srcBlendFunc = GL_DST_COLOR;
  }
  else if ("one-minus-dst-color" == funcName)
  {
    m_srcBlendFunc = GL_ONE_MINUS_DST_COLOR;
  }
  else if ("src-alpha" == funcName)
  {
    m_srcBlendFunc = GL_SRC_ALPHA;
  }
  else if ("one-minus-src-alpha" == funcName)
  {
    m_srcBlendFunc = GL_ONE_MINUS_SRC_ALPHA;
  }
  else if ("dst-alpha" == funcName)
  {
    m_srcBlendFunc = GL_DST_ALPHA;
  }
  else if ("one-minus-dst-alpha" == funcName)
  {
    m_srcBlendFunc = GL_ONE_MINUS_DST_ALPHA;
  }
  else if ("src-alpha-saturate" == funcName)
  {
    m_srcBlendFunc = GL_SRC_ALPHA_SATURATE;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets destination pixel scale function. */
void MaterialPrivate::setDstBlendFunc(const EGEString& funcName)
{
  // TAGE - seems like Microsoft's implementation only allows this:
  // GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA
  // 
  // OpenGLES allows this:
  // GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA + GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR

  if ("zero" == funcName)
  {
    m_dstBlendFunc = GL_ZERO;
  }
  else if ("one" == funcName)
  {
    m_dstBlendFunc = GL_ONE;
  }
  else if ("src-color" == funcName)
  {
    m_dstBlendFunc = GL_SRC_COLOR;
  }
  else if ("one-minus-src-color" == funcName)
  {
    m_dstBlendFunc = GL_ONE_MINUS_SRC_COLOR;
  }
  else if ("src-alpha" == funcName)
  {
    m_dstBlendFunc = GL_SRC_ALPHA;
  }
  else if ("one-minus-src-alpha" == funcName)
  {
    m_dstBlendFunc = GL_ONE_MINUS_SRC_ALPHA;
  }
  else if ("dst-alpha" == funcName)
  {
    m_dstBlendFunc = GL_DST_ALPHA;
  }
  else if ("one-minus-dst-alpha" == funcName)
  {
    m_dstBlendFunc = GL_ONE_MINUS_DST_ALPHA;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
