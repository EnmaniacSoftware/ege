#ifndef EGE_CORE_IMAGE_H
#define EGE_CORE_IMAGE_H

#include "EGEFile.h"
#include "EGEImage.h"
#include "EGEString.h"
#include "EGEDataBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(Image, PImage)
EGE_PREDECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
EGE_PREDECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Image : public Object
{
  friend class ImageUtils;

  public:

    Image();
    Image(s32 width, s32 height, PixelFormat format);
   ~Image();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Loads image from given file converting it's pixel format to requested one if possible. 
     *  @param fileName  File name to load image from.
     *  @param format    Pixel format loaded image should be converted to.
     *  @return Loaded image on success. NULL otherwise.
     *  @note If requested pixel format is PF_UNKNOWN no conversion is done.
     */
    static PImage Load(const String& fileName, PixelFormat format = PF_UNKNOWN);
    /*! Loads image from given buffer converting it's pixel format to requested one if possible. 
     *  @param buffer  Buffer containing data to load image from.
     *  @param format  Pixel format loaded image should be converted to.
     *  @return Loaded image on success. NULL otherwise.
     *  @note If requested pixel format is PF_UNKNOWN no conversion is done.
     */
    static PImage Load(const PDataBuffer& buffer, PixelFormat format = PF_UNKNOWN);
    /*! Saves image into a given file with specified pixel format. 
     *  @param image     Image to save.
     *  @param fileName  File name to which the file should be saved.
     *  @param format    Pixel format of the saved image.
     *  @return EGE_SUCCESS on success.
     *  @note If requested pixel format is PF_UNKNOWN image will be saved with current image pixel format.
     */
    static EGEResult Save(PImage image, const String& fileName, PixelFormat format = PF_UNKNOWN);

  public:

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Gets image width (in pixels) */
    s32 width() const { return m_width; }
    /*! Gets image height (in pixels) */
    s32 height() const { return m_height; }
    /*! Gets image format */
    PixelFormat format() const { return m_format; }
    /*! Gets image pixel data buffer */
    PDataBuffer data() const { return m_data; }
    /*! Returns TRUE if image contains alpha channel. */
    bool hasAlpha() const { return PF_RGBA_8888 == m_format; }
    /*! Returns row length (in bytes). */
    u32 rowLength() const { return m_rowLength; }
    /*! Sets alpha premultiply flag. */
    void setAlphaPremultiply(bool set);
    /*! Returns TRUE if image is alpha premultiplied. */
    bool isAlphaPremultiplied() const { return m_premultiplied; }
    /*! Returns TRUE if image is compressed. */
    bool isCompressed() const;
    /*! Sets row alignment value. */
    void setRowAlignment(u8 bytes);
    /*! Returns row alignment value. */
    u8 rowAlignment() const;

  private:

    /*! Allocated internal data buffer to be able to hold image of a given size and format. */
    EGEResult allocateData(s32 width, s32 height, PixelFormat format);

  private:

    /*! Image pixel format. */
    PixelFormat m_format;
    /*! Pixel data. */
    PDataBuffer m_data;
    /*! Row length (in bytes) */
    u32 m_rowLength;
    /*! Image width (in pixels) */
    s32 m_width;
    /*! Image height (in pixels) */
    s32 m_height;
    /*! Alpha pre-multiplied flag. */
    bool m_premultiplied;
    /*! Row alignment (in bytes). 
     *  @note: Each row will be align to this boundary.
     */
    u8 m_rowAlignment;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGE_H