#include "Core/Graphics/Image/Image.h"
#include "Core/Graphics/Image/ImageHandlerPNG.h"
#include "Core/Graphics/Image/ImageHandlerJPG.h"
#include "Core/Graphics/Image/ImageHandlerPVR.h"
#include "EGEDataBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Image)
EGE_DEFINE_DELETE_OPERATORS(Image)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Image::Image(Application* app) : Object(app, EGE_OBJECT_UID_IMAGE), 
                                 m_format(PF_UNKNOWN), 
                                 m_rowLength(0), 
                                 m_width(0), 
                                 m_height(0),
                                 m_premultiplied(false),
                                 m_rowAlignment(1)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Image::Image(Application* app, s32 width, s32 height, PixelFormat format) : Object(app, EGE_OBJECT_UID_IMAGE),
                                                                            m_premultiplied(false),
                                                                            m_rowAlignment(1)
{
  // allocate empty image
  allocateData(width, height, format);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Image::~Image()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Image::isValid() const
{
  return (0 < m_width) && (0 < m_height) && (PF_UNKNOWN != m_format) && (NULL != m_data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
  // check if PVR
  else if (ImageHandlerPVR::IsValidFormat(file))
  {
    if (-1 != file.seek(0L, EGEFile::SEEK_MODE_BEGIN))
    {
      // load it
      image = ImageHandlerPVR::Load(file, format);
    }
  }

  return image;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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

  // check row alignment
  u32 disalignment = m_rowLength % m_rowAlignment;
  if (0 != disalignment)
  {
    m_rowLength += (m_rowAlignment - disalignment);
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
void Image::setAlphaPremultiply(bool set)
{
  m_premultiplied = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Image::isCompressed() const
{
  switch (m_format)
  {
    case PF_PVRTC_RGB_2BPP:
    case PF_PVRTC_RGB_4BPP:
    case PF_PVRTC_RGBA_2BPP:
    case PF_PVRTC_RGBA_4BPP:
    case PF_DXT1:
    case PF_DXT2:
    case PF_DXT3:
    case PF_DXT4:
    case PF_DXT5:

      return true;
      
    default:
      
      return false;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Image::setRowAlignment(u8 bytes)
{
  // NOTE: should be called on invalid (not loaded yet) images only!
  EGE_ASSERT( ! isValid());

  m_rowAlignment = bytes;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u8 Image::rowAlignment() const
{
  return m_rowAlignment;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END