//#include "Core/EGEngine.h"
#include "Core/Graphics/Image.h"
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

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Image)
EGE_DEFINE_DELETE_OPERATORS(Image)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function used to read data from media to PNG allocated memory. */
static void PngReadDataFunc(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead)
{
   if (NULL == png_ptr->io_ptr)
   {
     // error!
     return;
   }

   File& file = *(File*) png_ptr->io_ptr;

   DataBuffer buffer((void*) outBytes, (s64) byteCountToRead);
   
   if (EGE_SUCCESS != file.read(&buffer, buffer.size()))
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
/*! Loads image converting it's format to requested one. */
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
    case STREAM_TYPE_PNG: result = loadPng(file, format); break;
    case STREAM_TYPE_JPG: result = loadJpg(file, format); break;

    default:

      return EGE_ERROR_NOT_SUPPORTED;
  }

  // close file
  file.close();

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads PNG file and converts it into requested format. */
EGEResult Image::loadPng(File& file, EGEImage::Format format)
{
  EGEResult result = EGE_SUCCESS;

	png_structp pngReadStruct;
	png_infop   pngInfoStruct;
	png_bytepp  rowPointers;

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
  png_set_read_fn(pngReadStruct, &file, PngReadDataFunc);

  // setup error handling
  if (setjmp(png_jmpbuf(pngReadStruct)))
  {
    // error!
    png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);
    return EGE_ERROR_IO;
  }

	// read whole image
	png_read_png(pngReadStruct, pngInfoStruct, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

	// get pointers to all rows
	rowPointers = png_get_rows(pngReadStruct, pngInfoStruct);

  // determine pixel format
  if (EGEImage::NONE == format)
  {
    // match it with image pixel format
    switch (pngInfoStruct->channels)
    {
      case 3: format = EGEImage::RGB_888; break;
      case 4: format = EGEImage::RGBA_8888; break;

      default:

        // error!
        png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);
        return EGE_ERROR_NOT_SUPPORTED;
    }
  }

  // allocate internal buffers
  if (EGE_SUCCESS != (result = allocateData(pngInfoStruct->width, pngInfoStruct->height, format)))
  {
    // error!
    png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);
    return result;
  }

  // check if palette is present
  //if (NULL == pngInfoStruct->palette)
  //{
    // allocate memory for palette
    //if ( ( psImageData->psPalette = new SPALETTEENTRY[ pngInfoStruct->num_palette ] ) == NULL )
    //{
    //  // error!
    //  png_destroy_read_struct( &pngReadStruct, &pngInfoStruct, png_infopp_NULL );
    //  fclose( pfFile );
		  //return false;
    //}

    //// store number of palette entries
    //psImageData->uPaletteEntriesNo = pngInfoStruct->num_palette;

    //// copy palette
    //for ( u32 i = 0; i < psImageData->uPaletteEntriesNo; i++ )
    //{
    //  psImageData->psPalette[ i ].byRed   = pngInfoStruct->palette[ i ].red;
    //  psImageData->psPalette[ i ].byGreen = pngInfoStruct->palette[ i ].green;
    //  psImageData->psPalette[ i ].byBlue  = pngInfoStruct->palette[ i ].blue;
    //  psImageData->psPalette[ i ].byAlpha = 255;
    //}
 // }

	// copy data into newly created buffer
	for (u32 row = 0; row < pngInfoStruct->height; row++)
  {
    void* dst = m_data->data(row * m_rowLength);
    void* src = rowPointers[row];

    // copy row
    switch (format)
    {
      case EGEImage::RGBA_8888:

        switch (pngInfoStruct->channels)
        {
          case 4:

            EGE_MEMCPY(dst, src, pngInfoStruct->rowbytes);
            break;

          default:

            // format not supported
            result = EGE_ERROR_NOT_SUPPORTED;
            break;
        }
        break;

      case EGEImage::RGB_888:

        switch (pngInfoStruct->channels)
        {
          case 3:

            EGE_MEMCPY(dst, src, pngInfoStruct->rowbytes);
            break;

          default:

            // format not supported
            result = EGE_ERROR_NOT_SUPPORTED;
            break;
        }
        break;
        
      default:

        // format not supported
        result = EGE_ERROR_NOT_SUPPORTED;
        break;
    }

    // check if error occured
    if (EGE_SUCCESS != result)
    {
      // error!
      png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);
      return result;
    }
  }

	// deallocate all png data 
	png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, png_infopp_NULL);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads JPG file and converts it into requested format. */
EGEResult Image::loadJpg(File& file, EGEImage::Format format)
{
  EGEResult result = EGE_SUCCESS;

	jpeg_decompress_struct cinfo;
	jpeg_error_mgr         jerr;
	
	// have our compression info object point to the error handler address
	cinfo.err = jpeg_std_error(&jerr);
	
	// initialize the decompression object
	jpeg_create_decompress(&cinfo);
	
	// specify the data source
	if (EGE_SUCCESS != (result = jpeg_egefile_src(&cinfo, &file)))
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

  // calculate row length
  m_rowLength = width;
  switch (format)
  {
    case EGEImage::RGBA_8888: m_rowLength *= 4; break;
    case EGEImage::RGB_888:   m_rowLength *= 3; break;

    default:

      // error!
      return EGE_ERROR_NOT_SUPPORTED;
  }

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
/*! Saves image into a given file. */
EGEResult Image::save(const String& fileName)
{
  EGEResult result = EGE_SUCCESS;
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Makes copy of current image in a givem format. */
Image* Image::copy(EGEImage::Format format) const
{
  Image* image = ege_new Image(app(), width(), height(), format);
  if (NULL != image && image->isValid())
  {
    typedef void (*PFNSCANLINEBLT)(void* dst, const void* src, s32 length);
    PFNSCANLINEBLT func = NULL;

    switch (this->format())
    {
      case EGEImage::RGBA_8888:

        if (EGEImage::RGBA_8888 == image->format())
        {
          func = Image::ScanLineBltRGBA8888ToRGBA8888;
          break;
        }
    }

    if (NULL != func)
    {
      // go thru all scanlines
      for (s32 y = 0; y < height(); ++y)
      {
        //func();
      }
    }
  }

  return image;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Performs scan line bit blit from RGBA8888 format onto RGBA8888 format. */
void Image::ScanLineBltRGBA8888ToRGBA8888(void* dst, const void* src, s32 length)
{
  EGE_MEMCPY(dst, src, length);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
