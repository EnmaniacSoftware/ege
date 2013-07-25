#ifndef EGE_CORE_TEXTUREIMAGE_H
#define EGE_CORE_TEXTUREIMAGE_H

/** This object represents a subsection of existing 2D texture. Especially useful for texture atlases.
*/

#include "EGE.h"
#include "EGERect.h"
#include "EGETexture.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(TextureImage, PTextureImage)
EGE_DECLARE_SMART_CLASS(Texture2D, PTexture2D)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TextureImage : public Object
{
  public:

    explicit TextureImage(Application* app);
    TextureImage(const PTexture2D& texture, Rectf rect = Rectf(0, 0, 1.0f, 1.0f));
    TextureImage(const PTextureImage& texture, const Rectf& rect);
   ~TextureImage();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
    
    /*! Returns TRUE if object is valid. */
    bool isValid() const;

    /*! Return name. */
    const String& name() const;
    /*! Sets name. */
    void setName(const String& name);

    /*! Sets texture which obejct refers to. */
    void setTexture(const PTexture2D& texture);
    /*! Returns texture object refers to. */
    PTexture2D texture() const;

    /*! Sets rectangle within texture this object refers to. 
     *  @param rect  Rectangle within texture (in normalized local space).
     */
    void setRect(const Rectf& rect);
    /*! Returns rectangle (in normalized local space) object refers to. */
    const Rectf& rect() const;

    /*! Sets environment mode. */
    void setEnvironmentMode(EGETexture::EnvironmentMode mode);
    /*! Returns environment mode. */
    EGETexture::EnvironmentMode environmentMode() const;

    /*! Sets rotation angle. */
    void setRotationAngle(const Angle& angle);
    /*! Returns rotation angle. */
    const Angle& rotationAngle() const;

    /*! Makes a copy of given texture image. */
    void copy(const TextureImage* other);

    /*! Returns width (in texels). */
    s32 width() const;
    /*! Returns height (in texels). */
    s32 height() const;

  private:

    /*! Name. */
    String m_name;
    /*! Texture assigned. */
    PTexture2D m_texture;
    /*! Rectangular are of the texture this object refers to (in normalized local space). */
    Rectf m_rect;
    /*! Environment mode. */
    EGETexture::EnvironmentMode m_envMode;
    /*! Rotation angle. */
    Angle m_rotationAngle;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXTUREIMAGE_H
