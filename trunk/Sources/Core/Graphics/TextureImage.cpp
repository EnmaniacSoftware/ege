#include "Core/Graphics/TextureImage.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(TextureImage)
EGE_DEFINE_DELETE_OPERATORS(TextureImage)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureImage::TextureImage(Application* app) : Object(app, EGE_OBJECT_UID_TEXTURE_IMAGE), m_envMode(EGETexture::EM_MODULATE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureImage::TextureImage(Application* app, PObject texture, const Rectf& rect) : Object(app, EGE_OBJECT_UID_TEXTURE_IMAGE), m_texture(texture), m_rect(rect),
                                                                                   m_envMode(EGETexture::EM_MODULATE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureImage::TextureImage(Application* app, const PTextureImage& texture, const Rectf& rect) : Object(app, EGE_OBJECT_UID_TEXTURE_IMAGE), 
                                                                                                m_texture(texture->m_texture), m_envMode(texture->m_envMode)
{
  // NOTE: rect is in local space of this object, combine these for final local space coords
  m_rect.x      = texture->m_rect.x + rect.x * texture->m_rect.width;
  m_rect.y      = texture->m_rect.y + rect.y * texture->m_rect.height;
  m_rect.width  = texture->m_rect.width * rect.width;
  m_rect.height = texture->m_rect.height * rect.height;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureImage::~TextureImage()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool TextureImage::isValid() const
{
  return (NULL != m_texture) && !m_rect.isEmpty() && !m_rect.isNull();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets environment mode. */
void TextureImage::setEnvironmentMode(EGETexture::EnvironmentMode mode)
{
  m_envMode = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets texture which obejct refers to. */
void TextureImage::setTexture(const PObject& texture)
{
  m_texture = texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets rectangle within texture this object refers to. 
 * @param rect  Rectangle within texture (in normalized local space).
 */
void TextureImage::setRect(const Rectf& rect)
{
  m_rect = rect;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
