#include "Core/Graphics/TextureImage.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(TextureImage)
EGE_DEFINE_DELETE_OPERATORS(TextureImage)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureImage::TextureImage(Application* app, PObject texture, const Rectf& rect) : Object(app, EGE_OBJECT_UID_TEXTURE_IMAGE), m_texture(texture), m_rect(rect),
                                                                                   m_envMode(EGETexture::EM_MODULATE)
{
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
