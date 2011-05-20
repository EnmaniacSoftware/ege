#ifndef EGE_CORE_IMAGE_H
#define EGE_CORE_IMAGE_H

#include "EGE.h"
#include "EGEFile.h"
#include "EGEImage.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Image, PImage)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Image : public Object
{
  public:

    Image(Application* app);
    virtual ~Image();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Loads image converting it's format to requested one. */
    EGEResult load(const EGEString& fileName, EGEImage::Format format = EGEImage::NONE);
    /*! Gets image width (in pixels) */
    inline u32 width() const { return m_width; }
    /*! Gets image height (in pixels) */
    inline u32 height() const { return m_height; }
    /*! Gets image format */
    inline EGEImage::Format format() const { return m_format; }
    /*! Gets image pixel data buffer */
    inline PDataBuffer data() const { return m_data; }
    /*! Returns TRUE if image contains alpha channel. */
    inline bool hasAlpha() const { return EGEImage::RGBA_8888 == m_format; }

  protected:

    enum StreamType
    {
      STREAM_TYPE_UNKNOWN = -1,
      STREAM_TYPE_PNG,
      STREAM_TYPE_JPG
    };

    /* Determines file stream type. */
    StreamType determineStreamType(File& file) const;

  private:

    /* Loads PNG file and converts it into requested format. */
    EGEResult loadPng(File& file, EGEImage::Format format);
    /* Loads JPG file and converts it into requested format. */
    EGEResult loadJpg(File& file, EGEImage::Format format);
    /* Allocated internal data buffer to be able to hold image of a given size and format. */
    EGEResult allocateData(u32 width, u32 height, EGEImage::Format format);

  private:

    /*! Image pixel format. */
    EGEImage::Format m_format;
    /*! Pixel data. */
    PDataBuffer m_data;
    /*! Row length (in bytes) */
    u32 m_rowLength;
    /*! Image width (in pixels) */
    u32 m_width;
    /*! Image height (in pixels) */
    u32 m_height;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGE_H