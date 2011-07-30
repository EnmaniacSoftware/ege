#include "Core/Graphics/Texture2D.h"
#include <EGEDebug.h>

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Texture2D)
EGE_DEFINE_DELETE_OPERATORS(Texture2D)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Texture2D::Texture2D(Application* app, EGETexture::Filter minFilter, EGETexture::Filter magFilter, EGETexture::AddressingMode modeS, 
                     EGETexture::AddressingMode modeT) 
: Object(app, EGE_OBJECT_UID_TEXTURE_2D), m_minFilter(minFilter), m_magFilter(magFilter), m_addressingModeS(modeS), m_addressingModeT(modeT)
{
  m_p = ege_new Texture2DPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Texture2D::~Texture2D()
{
  //EGE_PRINT("Texture2D::~Texture2D - 
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Texture2D::isValid() const
{
  return NULL != m_p;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Texture2D::create(const String& path)
{
  if (isValid())
  {
    return p_func()->create(path);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets minifying function filter. */
void Texture2D::setMinFilter(EGETexture::Filter filter)
{
  m_minFilter = filter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets magnification function filter. */
void Texture2D::setMagFilter(EGETexture::Filter filter)
{
  m_magFilter = filter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets wrap parameter for S coordinate. */
void Texture2D::setTextureAddressingModeS(EGETexture::AddressingMode mode)
{
  m_addressingModeS = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets wrap parameter for T coordinate. */
void Texture2D::setTextureAddressingModeT(EGETexture::AddressingMode mode)
{
  m_addressingModeT = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
