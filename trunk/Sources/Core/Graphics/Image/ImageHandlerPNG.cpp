#include "Core/Graphics/Image/ImageHandlerPNG.h"
#include <EGEDebug.h>

extern "C"
{
  #include "png.h"
}

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function used to read data from media to PNG allocated memory. */
static void PngReadDataFromFileFunc(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead)
{
   if (NULL == png_ptr->io_ptr)
   {
     // error!
     return;
   }

   // retrieve file data is read from
   File& source = *(File*) png_ptr->io_ptr;

   // wrap PNG library supplied storage for convinience
   DataBuffer buffer((void*) outBytes, (s64) byteCountToRead);
   
   // read data from file into buffer
   if (buffer.size() != source.read(buffer, buffer.size()))
   {
     // error!
     return;
   }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function used to read data from buffer to PNG allocated memory. */
static void PngReadDataFromBufferFunc(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead)
{
   if (NULL == png_ptr->io_ptr)
   {
     // error!
     return;
   }

   // retrieve buffer data is read from
   DataBuffer& source = *(DataBuffer*) png_ptr->io_ptr;
 
   // read requested data into wrapped buffer
   if (byteCountToRead != source.read(outBytes, byteCountToRead))
   {
     // error!
     return;
   }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function used to write PNG data into file. */
static void PngWriteDataFunc(png_structp png_ptr, png_bytep data, png_size_t length)
{
   if (NULL == png_ptr->io_ptr)
   {
     // error!
     return;
   }

   File& file = *(File*) png_ptr->io_ptr;

   DataBuffer buffer((void*) data, (s64) length);
   
   if (buffer.size() != file.write(buffer, buffer.size()))
   {
     // error!
     return;
   }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ImageHandlerPNG::IsValidFormat(PObject buffer)
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
      u8 header[8];
      *file >> header[0];
      *file >> header[1];
      *file >> header[2];
      *file >> header[3];
      *file >> header[4];
      *file >> header[5];
      *file >> header[6];
      *file >> header[7];

      // check header
      if (0 == png_sig_cmp(header, 0, 8))
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

    // check header
    if (0 == png_sig_cmp(reinterpret_cast<u8*>(data->data()), 0, 8))
    {
      // it is
      return true;
    }
  }
  else
  {
    EGE_ASSERT(false && "Unsupported buffer type.");
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PImage ImageHandlerPNG::Load(PObject buffer, PixelFormat format)
{
	png_structp pngReadStruct;
	png_infop   pngInfoStruct;

	// allocate memory for read structure
	if (NULL == (pngReadStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp) NULL, NULL, NULL))) 
	{
    // error!
		return NULL;
	}

	// allocate memory for info structure
  if (NULL == (pngInfoStruct = png_create_info_struct(pngReadStruct)))
	{
    // error!
    png_destroy_read_struct(&pngReadStruct, (png_infopp) NULL, png_infopp_NULL);
		return NULL;
  }

  // set customg stream read function
  if (EGE_OBJECT_UID_FILE == buffer->uid())
  {
    png_set_read_fn(pngReadStruct, buffer, PngReadDataFromFileFunc);
  }
  else if (EGE_OBJECT_UID_DATA_BUFFER == buffer->uid())
  {
    png_set_read_fn(pngReadStruct, buffer, PngReadDataFromBufferFunc);
  }
  else
  {
    EGE_ASSERT(false && "Unsupported source type.");
    return NULL;
  }

  // setup error handling
  if (setjmp(png_jmpbuf(pngReadStruct)))
  {
    // error!
    png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);
    return NULL;
  }

  // read chunk data
  png_read_info(pngReadStruct, pngInfoStruct);

  // determine pixel format
  if (PF_UNKNOWN == format)
  {
    // match it with image pixel format
    switch (pngInfoStruct->channels)
    {
      case 3: 
        
        // check if any transparent color is present
        if (0 < pngInfoStruct->num_trans)
        {
          // we want expand it to RGBA then
          format = PF_RGBA_8888;
        }
        else
        {
          // ok it is safe to keep RGB here
          format = PF_RGB_888;
        }
        break;
        
      case 4: 
        
        format = PF_RGBA_8888; 
        break;

      default:

        // error!
        png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);
        return NULL;
    }
  }
  else
  {
    // check if any fillers needs to be added to match required format
    if (PF_RGBA_8888 == format && (3 == pngInfoStruct->channels) && (0 == pngInfoStruct->num_trans))
    {
      // add opacity alpha channel to form RGBA from RGB
      png_set_filler(pngReadStruct, 0xff, PNG_FILLER_AFTER);
    }

    // TAGE - needs to verify that png_set_strip_alpha does correct job
    // check if alpha channel should not be taken into account
    if (PF_RGB_888 == format && ((4 == pngInfoStruct->channels) || (0 < pngInfoStruct->num_trans)))
    {
      png_set_strip_alpha(pngReadStruct);
    }
  }

  // allocate image
  PImage image = ege_new Image(NULL, pngInfoStruct->width, pngInfoStruct->height, format);
  if ((NULL == image) || !image->isValid())
  {
    // error!
    png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);
    return NULL;
  }

  // expand color data
  png_set_expand(pngReadStruct);

  // decompress row by row
  u8* row = reinterpret_cast<u8*>(image->data()->data(0));
  for (u32 y = 0; y < pngInfoStruct->height; ++y)
  {
    // read row
    png_read_row(pngReadStruct, row, NULL);

    row += image->rowLength();
  }

  // finialize reading
  png_read_end(pngReadStruct, pngInfoStruct);

	// deallocate all png data 
	png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);

  return image;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImageHandlerPNG::Save(PImage image, const String& fileName, PixelFormat format)
{
  EGEResult result;

  png_structp pngWriteStruct;
	png_infop   pngInfoStruct;
	png_bytepp  rowPointers;

  // sanity check
  if ((NULL == image) || !image->isValid())
  {
    // error!
    return EGE_ERROR;
  }

  // open file for writing
  File file(fileName);
  if (EGE_SUCCESS != (result = file.open(EGEFile::MODE_WRITE_ONLY)))
  {
    // error!
    return result;
  }

	// allocate memory for read structure
	if (NULL == (pngWriteStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp) NULL, NULL, NULL))) 
	{
    // error!
		return EGE_ERROR_NO_MEMORY;
	}

	// allocate memory for info structure
  if (NULL == (pngInfoStruct = png_create_info_struct(pngWriteStruct)))
	{
    // error!
    png_destroy_read_struct(&pngWriteStruct, (png_infopp) NULL, png_infopp_NULL);
		return EGE_ERROR_NO_MEMORY;
  }

  // set custom stream read function
  png_set_write_fn(pngWriteStruct, &file, PngWriteDataFunc, NULL);

  // setup error handling
  if (setjmp(png_jmpbuf(pngWriteStruct)))
  {
    // error!
    png_destroy_read_struct(&pngWriteStruct, &pngInfoStruct, png_infopp_NULL);
    return EGE_ERROR_IO;
  }

  // determine parameters according to requested format
  s32 bitDepth = 0;
  s32 colorType = 0;
  switch (format)
  {
    case PF_RGBA_8888:

      colorType = PNG_COLOR_TYPE_RGB_ALPHA;
      bitDepth = 8;
      break;

    case PF_RGB_888:

      colorType = PNG_COLOR_TYPE_RGB;
      bitDepth = 8;
      break;

    default:

      // error!
      EGE_ASSERT(false && "Format not supported!");
      png_destroy_read_struct(&pngWriteStruct, &pngInfoStruct, png_infopp_NULL);
      return EGE_ERROR_IO;
  }

  // set attributes
  png_set_IHDR(pngWriteStruct, pngInfoStruct, image->width(), image->height(), bitDepth, colorType, PNG_INTERLACE_NONE, 
               PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  // allocate memory for row pointers
  rowPointers = (png_byte**) png_malloc(pngWriteStruct, image->height() * sizeof (png_byte*));

  // assign row pointers
  for (s32 y = 0; y < image->height(); ++y)
  {
    rowPointers[y] = (png_byte*) image->data()->data(y * image->rowLength());
  }

  // supply row pointer to library
  png_set_rows(pngWriteStruct, pngInfoStruct, rowPointers);

  // write png
  png_write_png(pngWriteStruct, pngInfoStruct, PNG_TRANSFORM_IDENTITY, NULL);

  // clean up
  png_free(pngWriteStruct, rowPointers);

	// deallocate all png data 
	png_destroy_write_struct(&pngWriteStruct, &pngInfoStruct);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END