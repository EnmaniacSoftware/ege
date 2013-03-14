#ifndef EGE_CORE_TEXTUREIMAGE_H
#define EGE_CORE_TEXTUREIMAGE_H

/** This object represents a subsection of existing texture. Especially useful for texture atlases.
*/

#include "EGE.h"
#include "EGERect.h"
#include "EGETexture.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(TextureImage, PTextureImage)
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TextureImage : public Object
{
  public:

    TextureImage(Application* app);
    TextureImage(Application* app, PObject texture, const Rectf& rect);
    TextureImage(Application* app, const PTextureImage& texture, const Rectf& rect);
   ~TextureImage();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns TRUE if object is valid. */
    bool isValid() const;

    /*! Return name. */
    const String& name() const { return m_name; }
    /*! Sets name. */
    void setName(const String& name);

    /*! Sets texture which obejct refers to. */
    void setTexture(const PObject& texture);
    /*! Returns texture object refers to. */
    PObject texture() const { return m_texture; }

    /*! Sets rectangle within texture this object refers to. 
     *  @param rect  Rectangle within texture (in normalized local space).
     */
    void setRect(const Rectf& rect);
    /*! Returns rectangle (in normalized local space) object refers to. */
    const Rectf& rect() const { return m_rect; }

    /*! Sets environment mode. */
    void setEnvironmentMode(EGETexture::EnvironmentMode mode);
    /*! Returns environment mode. */
    EGETexture::EnvironmentMode environmentMode() const { return m_envMode; }

    /*! Sets rotation angle. */
    void setRotationAngle(const Angle& angle);
    /*! Returns rotation angle. */
    const Angle& rotationAngle() const { return m_rotationAngle; }

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
    PObject m_texture;
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
