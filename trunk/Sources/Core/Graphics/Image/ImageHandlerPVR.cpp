#include "Core/Graphics/Image/ImageHandlerPVR.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct PVRHeader
{
	u32 version;			    /*!< Version. */
	u32 flags;				    /*!< */
	u64 pixelFormat;			/*!< Pixel format. */
	u32 colorSpace;		    /*!< */
	u32 channelType;			/*!< */
	u32 height;	          /*!< Height of input surface. */
	u32 width;			      /*!< Width of input surface. */
	u32 depth;			      /*!< */
	u32 numSurfaces;			/*!< */
	u32 numFaces;			    /*!< */
	u32 mipMapCount;		  /*!< Number of mip-maps. */
  u32 metaDataSize;     /*!< Meta-date size. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ImageHandlerPVR::IsValidFormat(PObject buffer)
{
  // sanity check
  if (NULL == buffer)
  {
    // error!
    return false;
  }

  if (EGE_OBJECT_UID_FILE == buffer->uid())
  {
    PFile file = buffer;

    if (-1 != file->seek(0L, EGEFile::SEEK_MODE_BEGIN))
    {
      u32 version;
      *file >> version;

      if (0x03525650 == version)
      {
        // it is
        return true;
      }
    }
  }
  else if (EGE_OBJECT_UID_DATA_BUFFER == buffer->uid())
  {
    PDataBuffer data = buffer;

    data->setReadOffset(0);
    data->setByteOrdering(LITTLE_ENDIAN);

    u32 version;
    *data >> version;

    if (0x03525650 == version)
    {
      // it is
      return true;
    }
  }
  else
  {
    EGE_ASSERT(false && "Unsupported source type.");
    return NULL;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PImage ImageHandlerPVR::Load(PObject buffer, PixelFormat format)
{
  EGEResult result = EGE_SUCCESS;

  // no pixel format conversion possible
  if (PF_UNKNOWN != format)
  {
    // error!
    return NULL;
  }

  PVRHeader header;
  s32 rowLength = 0;

  // allocate pixel buffer
  PDataBuffer pixelData = ege_new DataBuffer();
  if (NULL == pixelData)
  {
    // error!
    return NULL;
  }

  // set customg stream read function
  if (EGE_OBJECT_UID_FILE == buffer->uid())
  {
    PFile file = buffer;

    *file >> header.version;
    *file >> header.flags;
    *file >> header.pixelFormat;
    *file >> header.colorSpace;
    *file >> header.channelType;
    *file >> header.height;
    *file >> header.width;
    *file >> header.depth;
    *file >> header.numSurfaces;
    *file >> header.numFaces;
    *file >> header.mipMapCount;
    *file >> header.metaDataSize;

    // skip meta data
    if (-1 == file->seek(header.metaDataSize, EGEFile::SEEK_MODE_CURRENT))
    {
      // error!
      return NULL;
    }

    // NOTE: Use 52 instead of sizeof (PVRHeader) to make sure no alignment is being applied (ie can be 8 - byte alignment which screws everything)
    s64 pixelDataSize = file->size() - 52 - header.metaDataSize;

    // allocate buffer for pixel data
    if (EGE_SUCCESS != pixelData->setSize(pixelDataSize))
    {
      // error!
      return NULL;
    }

    // read in pixel data
    if (pixelDataSize != file->read(pixelData, pixelDataSize))
    {
      // error!
      return NULL;
    }
  }
  else if (EGE_OBJECT_UID_DATA_BUFFER == buffer->uid())
  {
    PDataBuffer data = buffer;
    data->setByteOrdering(LITTLE_ENDIAN);

    *data >> header.version;
    *data >> header.flags;
    *data >> header.pixelFormat;
    *data >> header.colorSpace;
    *data >> header.channelType;
    *data >> header.height;
    *data >> header.width;
    *data >> header.depth;
    *data >> header.numSurfaces;
    *data >> header.numFaces;
    *data >> header.mipMapCount;
    *data >> header.metaDataSize;

    // skip meta data
    data->setReadOffset(data->readOffset() + header.metaDataSize);

    // NOTE: Use 52 instead of sizeof (PVRHeader) to make sure no alignment is being applied (ie can be 8 - byte alignment which screws everything)
    s64 pixelDataSize = data->size() - 52 - header.metaDataSize;

    // allocate buffer for pixel data
    if (EGE_SUCCESS != pixelData->setSize(pixelDataSize))
    {
      // error!
      return NULL;
    }

    // read in pixel data
    if (pixelDataSize != pixelData->write(data, pixelDataSize))
    {
      // error!
      return NULL;
    }
  }
  else
  {
    EGE_ASSERT(false && "Unsupported source type.");
    return NULL;
  }

  // detrmine pixel format
  // NOTE: there are 2 cases here:
  //       - when high 32 bits are 0 -> lower 32 bits contain enumarated value describing the pixel format
  //       - when high 32 are set -> high 32 bits describe number of bits per channel (each byte), lower 32 bits describe channels and channel order
  //         For instance:
  //         0x 01 05 05 05 | 61 62 67 72 -> 1555 ABGR

  u32 pixelFormatHigh = header.pixelFormat >> 32;
  u32 pixelFormatLow  = (header.pixelFormat & 0x00000000ffffffffL);
  if (0 == pixelFormatHigh)
  {
    switch (pixelFormatLow)
    {
      case 0:   format = PF_PVRTC_RGB_2BPP; break;
      case 1:   format = PF_PVRTC_RGBA_2BPP; break;
      case 2:   format = PF_PVRTC_RGB_4BPP; break;
      case 3:   format = PF_PVRTC_RGBA_4BPP; break;
      case 7:   format = PF_DXT1; break;
      case 8:   format = PF_DXT2; break;
      case 9:   format = PF_DXT3; break;
      case 10:  format = PF_DXT4; break;
      case 11:  format = PF_DXT5; break;
    }
  }
  else
  {
    const u32 rgba    = 'r' | ('g' << 8) | ('b' << 16) | ('a' << 24);
    const u32 rgb     = 'r' | ('g' << 8) | ('b' << 16) | (0 << 24);
    const u32 pf5551  = 0x01050505;
    const u32 pf4444  = 0x04040404;

    // RGBA5551
    if ((rgba == pixelFormatLow) && (pf5551 == pixelFormatHigh))
    {
      format = PF_RGBA_5551;

      rowLength = header.width * 2;

      // align row length to 4 bytes
      if (rowLength & 0x3)
      {
        rowLength = (rowLength & 0xFFFFFFFC);
        rowLength += 4;
      }
    }
    // RGBA4444
    if ((rgba == pixelFormatLow) && (pf4444 == pixelFormatHigh))
    {
      format = PF_RGBA_4444;

      rowLength = header.width * 2;

      // align row length to 4 bytes
      if (rowLength & 0x3)
      {
        rowLength = (rowLength & 0xFFFFFFFC);
        rowLength += 4;
      }
    }
    else
    {
      // Unsupported format
      EGE_ASSERT(false && "Unsupported format");
      return NULL;
    }
  }

  EGE_ASSERT(1 == header.numFaces);
  EGE_ASSERT(1 == header.numSurfaces);
  EGE_ASSERT(1 == header.mipMapCount);

  bool premultiplied = (0x2 == (header.flags & 0x2));

  // create image
  return ImageUtils::CreateImage(header.width, header.height, format, premultiplied, rowLength, pixelData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImageHandlerPVR::Save(PImage image, const String& fileName, PixelFormat format)
{
  EGE_UNUSED(image);
  EGE_UNUSED(fileName);
  EGE_UNUSED(format);

  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END