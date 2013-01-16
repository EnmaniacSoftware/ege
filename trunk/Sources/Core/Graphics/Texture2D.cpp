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
Texture2D::Texture2D(Application* app, const String& name) : Object(app, EGE_OBJECT_UID_TEXTURE_2D), 
                                                             m_name(name), 
                                                             m_width(0), 
                                                             m_height(0), 
                                                             m_format(PF_UNKNOWN)
{
  egeDebug() << "Creating texture" << name;

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