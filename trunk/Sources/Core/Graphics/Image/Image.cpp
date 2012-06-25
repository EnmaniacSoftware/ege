#include "Core/Graphics/Image/Image.h"
#include "Core/Data/DataBuffer.h"
#include "Core/Graphics/Image/ImageHandlerPNG.h"
#include "Core/Graphics/Image/ImageHandlerJPG.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Image)
EGE_DEFINE_DELETE_OPERATORS(Image)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Image::Image(Application* app) : Object(app, EGE_OBJECT_UID_IMAGE), 
                                 m_format(PF_UNKNOWN), 
                                 m_rowLength(0), 
                                 m_width(0), 
                                 m_height(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Image::Image(Application* app, s32 width, s32 height, PixelFormat format) : Object(app, EGE_OBJECT_UID_IMAGE)
{
  // allocate empty image
  allocateData(width, height, format);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Image::~Image()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Image::isValid() const
{
  return (0 < m_width) && (0 < m_height) && (PF_UNKNOWN != m_format) && (NULL != m_data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads image from given file converting it's pixel format to requested one if possible. 
    @param fileName  File name to load image from.
    @param format    Pixel format loaded image should be converted to.
    @return Loaded image on success. NULL otherwise.
    @note If requested pixel format is PF_UNKNOWN no conversion is done.
 */
PImage Image::Load(const String& fileName, PixelFormat format)
{
  PImage image;

  File file(fileName);
  if (EGE_SUCCESS != file.open(EGEFile::MODE_READ_ONLY))
  {
    // error!
    return NULL;
  }

  // check if JPG
  if (ImageHandlerJPG::IsValidFormat(file))
  {
    if (-1 != file.seek(0L, EGEFile::SEEK_MODE_BEGIN))
    {
      // load it
      image = ImageHandlerJPG::Load(file, format);
    }
  }
  // check if PNG
  else if (ImageHandlerPNG::IsValidFormat(file))
  {
    if (-1 != file.seek(0L, EGEFile::SEEK_MODE_BEGIN))
    {
      // load it
      image = ImageHandlerPNG::Load(file, format);
    }
  }

  return image;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads image from given buffer converting it's pixel format to requested one if possible. 
    @param buffer  Buffer containing data to load image from.
    @param format  Pixel format loaded image should be converted to.
    @return Loaded image on success. NULL otherwise.
    @note If requested pixel format is PF_UNKNOWN no conversion is done.
 */
PImage Image::Load(const PDataBuffer& buffer, PixelFormat format)
{
  PImage image;

  // check if JPG
  if (ImageHandlerJPG::IsValidFormat(buffer))
  {
    buffer->setReadOffset(0);

    // load it
    image = ImageHandlerJPG::Load(buffer, format);
  }
  // check if PNG
  else if (ImageHandlerPNG::IsValidFormat(buffer))
  {
    buffer->setReadOffset(0);

    // load it
    image = ImageHandlerPNG::Load(buffer, format);
  }

  return image;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves image into a given file with specified pixel format. 
    @param image     Image to save.
    @param fileName  File name to which the file should be saved.
    @param format    Pixel format of the saved image.
    @return EGE_SUCCESS on success.
    @note If requested pixel format is PF_UNKNOWN image will be saved with current image pixel format.
 */
EGEResult Image::Save(PImage image, const String& fileName, PixelFormat format)
{
  EGEResult result = EGE_SUCCESS;

  // sanity check
  if ((NULL == image) || !image->isValid())
  {
    // error!
    return EGE_ERROR;
  }

  // determine pixel format
  if (PF_UNKNOWN == format)
  {
    format = image->format();
  }

  // check if JPG requested
  if (fileName.endsWith(".jpg") || fileName.endsWith(".jpeg"))
  {
    result = ImageHandlerJPG::Save(image, fileName, format);
  }
  else
  {
    // by default, save in PNG format
    result = ImageHandlerPNG::Save(image, fileName, format);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Allocated internal data buffer to be able to hold image of a given size and format. */
EGEResult Image::allocateData(s32 width, s32 height, PixelFormat format)
{
  EGEResult result = EGE_SUCCESS;

  // create buffer
  m_data = ege_new DataBuffer();
  if (NULL == m_data)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // get pixel size
  u32 pixelSize = ImageUtils::PixelSize(format);
  if (0 == pixelSize)
  {
    // error!
    return EGE_ERROR_NOT_SUPPORTED;
  }

  // calculate row length
  m_rowLength = width * pixelSize;

  // align row length to 4 bytes
  if (m_rowLength & 0x3)
  {
    m_rowLength = (m_rowLength & 0xFFFFFFFC);
    m_rowLength += 4;
  }

  // allocate buffer
  if (EGE_SUCCESS != (result = m_data->setSize(m_rowLength * height)))
  {
    // error!
    return result;
  }

  // store format
  m_format = format;

  // store dimensions
  m_width  = width;
  m_height = height;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END