#ifndef EGE_CORE_TEXTUREIMAGE_H
#define EGE_CORE_TEXTUREIMAGE_H

/** This object represents a subsection of existing texture. Especially useful for texture atlases.
*/

#include <EGE.h>
#include <EGERect.h>
#include <EGETexture.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(TextureImage, PTextureImage)
EGE_DECLARE_SMART_CLASS(Object, PObject)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class TextureImage : public Object
{
  public:

    TextureImage(Application* app, const String& name);
    TextureImage(Application* app, PObject texture, const Rectf& rect);
    TextureImage(Application* app, const PTextureImage& texture, const Rectf& rect);
   ~TextureImage();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;

    /*! Return name. */
    inline const String& name() const { return m_name; }

    /* Sets texture which obejct refers to. */
    void setTexture(const PObject& texture);
    /*! Returns texture object refers to. */
    inline PObject texture() const { return m_texture; }

    /* Sets rectangle within texture this object refers to. 
     * @param rect  Rectangle within texture (in normalized local space).
     */
    void setRect(const Rectf& rect);
    /*! Returns rectangle (in normalized local space) object refers to. */
    inline const Rectf& rect() const { return m_rect; }

    /* Sets environment mode. */
    void setEnvironmentMode(EGETexture::EnvironmentMode mode);
    /*! Returns environment mode. */
    inline EGETexture::EnvironmentMode environmentMode() const { return m_envMode; }

  private:

    /*! Name. */
    String m_name;
    /*! Texture assigned. */
    PObject m_texture;
    /*! Rectangular are of the texture this object refers to (in normalized local space). */
    Rectf m_rect;
    /*! Environment mode. */
    EGETexture::EnvironmentMode m_envMode;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXTUREIMAGE_H
