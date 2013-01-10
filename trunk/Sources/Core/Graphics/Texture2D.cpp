#include "Core/Graphics/Texture2D.h"
#include "Core/Application/Application.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include <EGEDebug.h>

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Texture2D)
EGE_DEFINE_DELETE_OPERATORS(Texture2D)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D Texture2D::CreateRenderTexture(Application* app, const String& name, s32 width, s32 height, PixelFormat format)
{
  // create empty texture of given size and format
  PTexture2D texture = ege_new Texture2D(app, name);
  if (texture && texture->isValid())
  {
    // TAGE - for the time being set to CLAMP so we non-power of 2 render textures can be created for iOS
    app->graphics()->renderSystem()->setTextureAddressingModeS(EGETexture::AM_CLAMP);
    app->graphics()->renderSystem()->setTextureAddressingModeT(EGETexture::AM_CLAMP);

    // create empty image from which empty texture is to be created
    PImage image = ImageUtils::CreateImage(width, height, format, false, 0, NULL);
    if (NULL == image)
    {
      // error!
      return NULL;
    }

    if (EGE_SUCCESS != texture->p_func()->create(image))
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
Texture2D::Texture2D(Application* app, const String& name) : Object(app, EGE_OBJECT_UID_TEXTURE_2D), 
                                                             m_name(name), 
                                                             m_width(0), 
                                                             m_height(0), 
                                                             m_format(PF_UNKNOWN)
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
bool Texture2D::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
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
EGEResult Texture2D::create(const PDataBuffer& buffer)
{
  if (isValid())
  {
    return p_func()->create(buffer);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Texture2D::create(const PImage& image)
{
  if (isValid())
  {
    return p_func()->create(image);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END