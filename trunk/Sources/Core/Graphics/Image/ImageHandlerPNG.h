#ifndef EGE_CORE_IMAGE_HANDLER_PNG_H
#define EGE_CORE_IMAGE_HANDLER_PNG_H

#include <EGEImage.h>

EGE_NAMESPACE_BEGIN
  
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImageHandlerPNG
{
  public:

    /*! Returns TRUE if given buffer contains image data in correct format. */
    static bool IsValidFormat(PObject buffer);
    /*! Loads image from given buffer converting it's pixel format to requested one if possible. 
     *  @param buffer  Buffer containing data to load image from.
     *  @param format  Pixel format loaded image should be converted to.
     *  @return Loaded image on success. NULL otherwise.
     *  @note If requested pixel format is PF_UNKNOWN no conversion is done.
     */
    static PImage Load(PObject buffer, PixelFormat format = PF_UNKNOWN);
    /*! Saves image into a given file with specified pixel format. 
     *  @param image     Image to save.
     *  @param fileName  File name to which the file should be saved.
     *  @param format    Pixel format of the saved image.
     *  @return EGE_SUCCESS on success.
     *  @note If requested pixel format is PF_UNKNOWN image will be saved with current image pixel format.
     */
    static EGEResult Save(PImage image, const String& fileName, PixelFormat format = PF_UNKNOWN);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGE_HANDLER_PNG_H
