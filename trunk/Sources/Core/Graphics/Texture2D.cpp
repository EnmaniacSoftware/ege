#include "Core/Graphics/Texture2D.h"
#include "Core/Application/Application.h"
#include "Core/Graphics/Graphics.h"
#include <EGEDebug.h>

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Texture2D)
EGE_DEFINE_DELETE_OPERATORS(Texture2D)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates render texture. */
PTexture2D Texture2D::CreateRenderTexture(Application* app, const String& name, s32 width, s32 height, EGEImage::Format format)
{
  // create empty texture of given size and format
  PTexture2D texture = ege_new Texture2D(app, name);
  if (texture && texture->isValid())
  {
    // set data
    texture->m_width  = width;
    texture->m_height = height;
    texture->m_format = format;

    if (EGE_SUCCESS != texture->p_func()->create())
    {
      // error!
      return NULL;
    }

    // create render target
    if (EGE_SUCCESS == texture->p_func()->createRenderTarget())
    {
      // add into render targets
      app->graphics()->registerRenderTarget(texture->renderTarget());
    }
    else
    {
      // error!
      texture = NULL;
    }
  }

  return texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Texture2D::Texture2D(Application* app, const String& name) : Object(app, EGE_OBJECT_UID_TEXTURE_2D), m_name(name), m_minFilter(EGETexture::BILINEAR), 
                                                             m_magFilter(EGETexture::BILINEAR), m_addressingModeS(EGETexture::AM_REPEAT), 
                                                             m_addressingModeT(EGETexture::AM_REPEAT), m_width(0), m_height(0), m_format(EGEImage::NONE)
{
  m_p = ege_new Texture2DPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Texture2D::~Texture2D()
{
  EGE_DELETE(m_p);

  // remove render target
  if (m_target)
  {
    app()->graphics()->removeRenderTarget(m_target->name());
  }

  m_target = NULL;
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
