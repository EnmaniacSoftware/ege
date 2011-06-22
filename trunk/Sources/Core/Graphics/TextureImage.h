#ifndef EGE_CORE_TEXTUREIMAGE_H
#define EGE_CORE_TEXTUREIMAGE_H

/** This object represents a subsection of existing texture. Especially useful for texture atlases.
*/

#include <EGE.h>
#include <EGERect.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(TextureImage, PTextureImage)
EGE_DECLARE_SMART_CLASS(Object, PObject)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class TextureImage : public Object
{
  public:

    TextureImage(Application* app, PObject texture, const Rectf& rect);
   ~TextureImage();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns texture object refers to. */
    inline PObject texture() const { return m_texture; }
    /*! Returns rectangle (in texture coords) object refers to. */
    inline const Rectf& rectangle() const { return m_rect; }

  private:

    /*! Texture assigned. */
    PObject m_texture;
    /*! Rectangular are of the texture this object refers to (in texture coords). */
    Rectf m_rect;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXTUREIMAGE_H
