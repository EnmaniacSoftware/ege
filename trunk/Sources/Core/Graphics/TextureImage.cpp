#include "Core/Graphics/TextureImage.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(TextureImage)
EGE_DEFINE_DELETE_OPERATORS(TextureImage)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureImage::TextureImage(Application* app) : Object(app, EGE_OBJECT_UID_TEXTURE_IMAGE), 
                                               m_envMode(EGETexture::EM_MODULATE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureImage::TextureImage(const PTexture2D& texture, Rectf rect) : Object(texture->app(), EGE_OBJECT_UID_TEXTURE_IMAGE), 
                                                                    m_texture(texture),
                                                                    m_name(texture->name()),
                                                                    m_rect(rect),
                                                                    m_envMode(EGETexture::EM_MODULATE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureImage::TextureImage(const PTextureImage& texture, const Rectf& rect) : Object(texture->app(), EGE_OBJECT_UID_TEXTURE_IMAGE), 
                                                                              m_name(texture->name()), 
                                                                              m_texture(texture->m_texture), 
                                                                              m_envMode(texture->m_envMode)
{
  // NOTE: rect is in local space of this object, combine these for final local space coords
  m_rect = texture->m_rect.combine(rect);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureImage::~TextureImage()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TextureImage::isValid() const
{
  return (NULL != m_texture) && ! m_rect.isEmpty() && ! m_rect.isNull();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextureImage::setEnvironmentMode(EGETexture::EnvironmentMode mode)
{
  m_envMode = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGETexture::EnvironmentMode TextureImage::environmentMode() const 
{ 
  return m_envMode; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextureImage::setTexture(const PTexture2D& texture)
{
  m_texture = texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D TextureImage::texture() const 
{ 
  return m_texture; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextureImage::setRect(const Rectf& rect)
{
  m_rect = rect;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Rectf& TextureImage::rect() const 
{ 
  return m_rect; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextureImage::setName(const String& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& TextureImage::name() const 
{ 
  return m_name; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextureImage::copy(const TextureImage* other)
{
  EGE_ASSERT(other);

  m_name          = other->name();
  m_rect          = other->m_rect;
  m_texture       = other->m_texture;
  m_envMode       = other->m_envMode;
  m_rotationAngle = other->m_rotationAngle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 TextureImage::width() const
{
  s32 width = 0;

  if (m_texture)
  {
    switch (m_texture->uid())
    {
      case EGE_OBJECT_UID_TEXTURE_2D:

        width = static_cast<s32>(ege_cast<Texture2D*>(m_texture)->width() * m_rect.width);
        break;
    }
  }

  return width;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 TextureImage::height() const
{
  s32 height = 0;

  if (m_texture)
  {
    switch (m_texture->uid())
    {
      case EGE_OBJECT_UID_TEXTURE_2D:

        height = static_cast<s32>(ege_cast<Texture2D*>(m_texture)->height() * m_rect.height);
        break;
    }
  }

  return height;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextureImage::setRotationAngle(const Angle& angle)
{
  m_rotationAngle = angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Angle& TextureImage::rotationAngle() const 
{ 
  return m_rotationAngle; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END