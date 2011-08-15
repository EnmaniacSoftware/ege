#include "Core/Graphics/Image/Image.h"
#include "Core/Data/DataBuffer.h"
extern "C"
{
#include "png.h"
}

// for JpegLib
#define HAVE_INT32
extern "C"
{
#include "jpeglib.h"
}

#include "Core/Graphics/JpegDataSrcFile.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Image)
EGE_DEFINE_DELETE_OPERATORS(Image)

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
Image::Image(Application* app) : Object(app, EGE_OBJECT_UID_IMAGE), m_format(EGEImage::NONE), m_rowLength(0), m_width(0), m_height(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Image::Image(Application* app, s32 width, s32 height, EGEImage::Format format) : Object(app, EGE_OBJECT_UID_IMAGE)
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
  return (0 < m_width) && (0 < m_height) && (EGEImage::NONE != m_format) && (NULL != m_data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Determines file stream type. */
Image::StreamType Image::determineStreamType(File& file) const
{
  StreamType type = STREAM_TYPE_UNKNOWN;

  // check if JPEG
  if (-1 != file.seek(0L, EGEFile::SEEK_MODE_BEGIN))
  {
    u32 id;
    file >> id;

    // check if ID is correct
	  id <<= 8;
	  if (0xffd8ff00 == id)
    {
      // found
		  type = STREAM_TYPE_JPG;
    }
  }

  // check if PNG
  if (STREAM_TYPE_UNKNOWN == type)
  {
    if (-1 != file.seek(0L, EGEFile::SEEK_MODE_BEGIN))
    {
      u8 header[8];
      file >> header[0];
      file >> header[1];
      file >> header[2];
      file >> header[3];
      file >> header[4];
      file >> header[5];
      file >> header[6];
      file >> header[7];

      // check header
      if (0 == png_sig_cmp(header, 0, 8))
      {
        // found
        type = STREAM_TYPE_PNG;
      }
    }
  }

  // rewind
  file.seek(0L, EGEFile::SEEK_MODE_BEGIN);

  return type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Determines file stream type. */
Image::StreamType Image::determineStreamType(const PDataBuffer& data) const
{
  StreamType type = STREAM_TYPE_UNKNOWN;

  u32 id;

  // check if JPEG
  data->setReadOffset(0);
  *data >> id;

  // check if ID is correct
	id <<= 8;
	if (0xffd8ff00 == id)
  {
    // found
		type = STREAM_TYPE_JPG;
  }

  // check if PNG
  if (STREAM_TYPE_UNKNOWN == type)
  {
    data->setReadOffset(0);

    // check header
    if (0 == png_sig_cmp(reinterpret_cast<u8*>(data->data()), 0, 8))
    {
      // found
      type = STREAM_TYPE_PNG;
    }
  }

  // rewind
  data->setReadOffset(0);

  return type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads image from file converting it's format to requested one. */
EGEResult Image::load(const String& fileName, EGEImage::Format format)
{
  EGEResult result;

  File file(fileName);
  if (EGE_SUCCESS != (result = file.open(EGEFile::MODE_READ_ONLY)))
  {
    // error!
    return result;
  }

  // determine file stream type
  StreamType type = determineStreamType(file);
  switch (type)
  {
    case STREAM_TYPE_PNG: result = decompressPng(file, format); break;
    case STREAM_TYPE_JPG: result = decompressJpg(file, format); break;

    default:

      return EGE_ERROR_NOT_SUPPORTED;
  }

  // close file
  file.close();

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates image from buffer converting it's format to requested one. */
EGEResult Image::create(const PDataBuffer& buffer, EGEImage::Format format)
{
  EGEResult result;

  // determine file stream type
  StreamType type = determineStreamType(buffer);
  switch (type)
  {
    case STREAM_TYPE_PNG: result = decompressPng(buffer, format); break;
    case STREAM_TYPE_JPG: result = decompressJpg(buffer, format); break;

    default:

      return EGE_ERROR_NOT_SUPPORTED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Decompresses PNG data from file and converts it into requested format. */
EGEResult Image::decompressPng(PObject source, EGEImage::Format format)
{
  EGEResult result = EGE_SUCCESS;

	png_structp pngReadStruct;
	png_infop   pngInfoStruct;

	// allocate memory for read structure
	if (NULL == (pngReadStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp) NULL, NULL, NULL))) 
	{
    // error!
		return EGE_ERROR_NO_MEMORY;
	}

	// allocate memory for info structure
  if (NULL == (pngInfoStruct = png_create_info_struct(pngReadStruct)))
	{
    // error!
    png_destroy_read_struct(&pngReadStruct, (png_infopp) NULL, png_infopp_NULL);
		return EGE_ERROR_NO_MEMORY;
  }

  // set customg stream read function
  if (EGE_OBJECT_UID_FILE == source->uid())
  {
    png_set_read_fn(pngReadStruct, source, PngReadDataFromFileFunc);
  }
  else if (EGE_OBJECT_UID_DATA_BUFFER == source->uid())
  {
    png_set_read_fn(pngReadStruct, source, PngReadDataFromBufferFunc);
  }
  else
  {
    EGE_ASSERT("Unsupported source type.");
    return EGE_ERROR_NOT_SUPPORTED;
  }

  // setup error handling
  if (setjmp(png_jmpbuf(pngReadStruct)))
  {
    // error!
    png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);
    return EGE_ERROR_IO;
  }

  // read chunk data
  png_read_info(pngReadStruct, pngInfoStruct);

  // determine pixel format
  if (EGEImage::NONE == format)
  {
    // match it with image pixel format
    switch (pngInfoStruct->channels)
    {
      case 3: 
        
        // check if any transparent color is present
        if (0 < pngInfoStruct->num_trans)
        {
          // we want expand it to RGBA then
          format = EGEImage::RGBA_8888;
        }
        else
        {
          // ok it is safe to keep RGB here
          format = EGEImage::RGB_888;
        }
        break;
        
      case 4: 
        
        format = EGEImage::RGBA_8888; 
        break;

      default:

        // error!
        png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);
        return EGE_ERROR_NOT_SUPPORTED;
    }
  }
  else
  {
    // check if any fillers needs to be added to match required format
    if (EGEImage::RGBA_8888 == format && (3 == pngInfoStruct->channels) && (0 == pngInfoStruct->num_trans))
    {
      // add opacity alpha channel to form RGBA from RGB
      png_set_filler(pngReadStruct, 0xff, PNG_FILLER_AFTER);
    }

    // TAGE - needs to verify that png_set_strip_alpha does correct job
    // check if alpha channel should not be taken into account
    if (EGEImage::RGB_888 == format && ((4 == pngInfoStruct->channels) || (0 < pngInfoStruct->num_trans)))
    {
      png_set_strip_alpha(pngReadStruct);
    }
  }

  // allocate internal buffers
  if (EGE_SUCCESS != (result = allocateData(pngInfoStruct->width, pngInfoStruct->height, format)))
  {
    // error!
    png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);
    return result;
  }

  // expand color data
  png_set_expand(pngReadStruct);

  // decompress row by row
  u8* row = reinterpret_cast<u8*>(data()->data(0));
  for (u32 y = 0; y < pngInfoStruct->height; ++y)
  {
    // read row
    png_read_row(pngReadStruct, row, NULL);

    row += m_rowLength;
  }

  // finialize reading
  png_read_end(pngReadStruct, pngInfoStruct);

	// deallocate all png data 
	png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Decompresses JPG data from given source and converts it into requested format. */
EGEResult Image::decompressJpg(PObject source, EGEImage::Format format)
{
  EGEResult result = EGE_SUCCESS;

	jpeg_decompress_struct cinfo;
	jpeg_error_mgr         jerr;
	
	// have our compression info object point to the error handler address
	cinfo.err = jpeg_std_error(&jerr);
	
	// initialize the decompression object
	jpeg_create_decompress(&cinfo);
	
	// specify the data source
	if (EGE_SUCCESS != (result = jpeg_ege_src(&cinfo, source)))
  {
    // error!
    return result;
  }
	
  // read in the header of the jpeg file
	jpeg_read_header(&cinfo, true);

	// start to decompress the jpeg file with our compression info
	jpeg_start_decompress(&cinfo);
	
  // determine pixel format
  if (EGEImage::NONE == format)
  {
    // match it with image pixel format
    switch (cinfo.num_components)
    {
      case 3: format = EGEImage::RGB_888; break;
      case 4: format = EGEImage::RGBA_8888; break;

      default:

        // error!
		    jpeg_finish_decompress(&cinfo);
		    jpeg_destroy_decompress(&cinfo);
        return EGE_ERROR_NOT_SUPPORTED;
    }
  }

  // allocate internal buffers
  if (EGE_SUCCESS != (result = allocateData(cinfo.image_width, cinfo.image_height, format)))
  {
    // error!
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
    return result;
  }

	// now comes the juice of our work, here we extract all the pixel data
	u32 rowsRead = 0;
	while (cinfo.output_scanline < cinfo.output_height) 
	{
    void* dst = m_data->data(rowsRead * m_rowLength);

		// read in the current row of pixels and increase the rowsRead count
		rowsRead += jpeg_read_scanlines(&cinfo, reinterpret_cast<u8**>(&dst), 1);
	}
	
	// finish decompressing the data
	jpeg_finish_decompress(&cinfo);

	// release all the stored memory for reading and decoding the jpeg
	jpeg_destroy_decompress(&cinfo);

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Allocated internal data buffer to be able to hold image of a given size and format. */
EGEResult Image::allocateData(s32 width, s32 height, EGEImage::Format format)
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
/*! Saves image into a given file with specified format. */
EGEResult Image::save(const String& fileName, EGEImage::Format format)
{
  EGEResult result = EGE_SUCCESS;

  File file(fileName);
  if (EGE_SUCCESS != (result = file.open(EGEFile::MODE_WRITE_ONLY)))
  {
    // error!
    return result;
  }

  // determine file stream type
  StreamType type = STREAM_TYPE_PNG;
  if (fileName.endsWith(".jpg") || fileName.endsWith(".jpeg"))
  {
    type = STREAM_TYPE_JPG;
  }

  // determine pixel format
  if (EGEImage::NONE == format)
  {
    format = this->format();
  }

  // save according to stream
  switch (type)
  {
    case STREAM_TYPE_PNG: result = savePng(file, format); break;
    case STREAM_TYPE_JPG: result = saveJpg(file, format); break;

    default:

      return EGE_ERROR_NOT_SUPPORTED;
  }

  // close file
  file.close();

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves PNG file and converts it into requested format. */
EGEResult Image::savePng(File& file, EGEImage::Format format)
{
	png_structp pngWriteStruct;
	png_infop   pngInfoStruct;
	png_bytepp  rowPointers;

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
    case EGEImage::RGBA_8888:

      colorType = PNG_COLOR_TYPE_RGB_ALPHA;
      bitDepth = 8;
      break;

    case EGEImage::RGB_888:

      colorType = PNG_COLOR_TYPE_RGB;
      bitDepth = 8;
      break;

    default:

      // error!
      EGE_ASSERT("Format not supported!");
      png_destroy_read_struct(&pngWriteStruct, &pngInfoStruct, png_infopp_NULL);
      return EGE_ERROR_IO;
  }

  // set attributes
  png_set_IHDR(pngWriteStruct, pngInfoStruct, width(), height(), bitDepth, colorType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  // allocate memory for row pointers
  rowPointers = (png_byte**) png_malloc(pngWriteStruct, height() * sizeof (png_byte*));

  // assign row pointers
  for (s32 y = 0; y < height(); ++y)
  {
    rowPointers[y] = (png_byte*) data()->data(y * m_rowLength);
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
/*! Saves JPG file and converts it into requested format. */
EGEResult Image::saveJpg(File& file, EGEImage::Format format)
{
  EGE_UNUSED(file);
  EGE_UNUSED(format);

  EGE_ASSERT("Implement");

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
