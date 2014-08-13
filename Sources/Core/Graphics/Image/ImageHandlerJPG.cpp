#include "Core/Graphics/Image/ImageHandlerJPG.h"

#define HAVE_INT32
extern "C"
{
  #include "jpeglib.h"
}

#include "Core/Graphics/JpegDataSrcFile.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ImageHandlerJPG::IsValidFormat(PObject buffer)
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
      u32 id;
      *file >> id;

      // check if ID is correct
	    id <<= 8;
	    if (0xffd8ff00 == id)
      {
        // it is
        return true;
      }
    }
  }
  else if (EGE_OBJECT_UID_DATA_BUFFER == buffer->uid())
  {
    PDataBuffer data = buffer;

    u32 id;

    data->setReadOffset(0);

    *data >> id;

    // check if ID is correct
	  id <<= 8;
	  if (0xffd8ff00 == id)
    {
      // it is
		  return true;;
    }
  }
  else
  {
    EGE_ASSERT_X(false, "Unsupported buffer type.");
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PImage ImageHandlerJPG::Load(PObject buffer, PixelFormat format)
{
  EGEResult result = EGE_SUCCESS;

	jpeg_decompress_struct cinfo;
	jpeg_error_mgr         jerr;
	
	// have our compression info object point to the error handler address
	cinfo.err = jpeg_std_error(&jerr);
	
	// initialize the decompression object
	jpeg_create_decompress(&cinfo);
	
	// specify the data source
	if (EGE_SUCCESS != (result = jpeg_ege_src(&cinfo, buffer)))
  {
    // error!
    return NULL;
  }
	
  // read in the header of the jpeg file
	jpeg_read_header(&cinfo, true);

	// start to decompress the jpeg file with our compression info
	jpeg_start_decompress(&cinfo);
	
  // determine pixel format
  if (PF_UNKNOWN == format)
  {
    // match it with image pixel format
    switch (cinfo.num_components)
    {
      case 3: format = PF_RGB_888; break;
      case 4: format = PF_RGBA_8888; break;

      default:

        // error!
		    jpeg_finish_decompress(&cinfo);
		    jpeg_destroy_decompress(&cinfo);
        return NULL;
    }
  }

  // allocate image
  PImage image = ege_new Image(cinfo.image_width, cinfo.image_height, format);
  if ((NULL == image) || !image->isValid())
  {
    // error!
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
    return NULL;
  }

	// now comes the juice of our work, here we extract all the pixel data
	u32 rowsRead = 0;
	while (cinfo.output_scanline < cinfo.output_height) 
	{
    void* dst = image->data()->data(rowsRead * image->rowLength());

		// read in the current row of pixels and increase the rowsRead count
		rowsRead += jpeg_read_scanlines(&cinfo, reinterpret_cast<u8**>(&dst), 1);
	}
	
	// finish decompressing the data
	jpeg_finish_decompress(&cinfo);

	// release all the stored memory for reading and decoding the jpeg
	jpeg_destroy_decompress(&cinfo);

  return image;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImageHandlerJPG::Save(PImage image, const String& fileName, PixelFormat format)
{
  EGE_UNUSED(image);
  EGE_UNUSED(fileName);
  EGE_UNUSED(format);

  EGE_ASSERT_X(false, "Implement");

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END