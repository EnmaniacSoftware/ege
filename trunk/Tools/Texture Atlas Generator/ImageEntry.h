#ifndef IMAGE_ENTRY_H
#define IMAGE_ENTRY_H

#include <EGEString.h>
#include <EGEImage.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ImageEntry
{
  public:

    ImageEntry(const EGE::String& path, const EGE::String& group) : m_path(path), m_group(group)
    {
      m_image = ege_new EGE::Image(NULL);
      if (m_image)
      {
        m_image->load(m_path, EGE::EGEImage::RGBA_8888);
      }
    }

   ~ImageEntry()
    {
      m_image = NULL;
    }

    /*! Returns TRUE if object is valid. */
    bool isValid() const { return (NULL != m_image) && (EGE::EGEImage::NONE != m_image->format()); }
    /*! Returns image object. */
    EGE::PImage image() const { return m_image; }
    /*! Returns group name. */
    const EGE::String& group() const { return m_group; }
    /*! Returns image path. */
    const EGE::String& path() const { return m_path; }

  private:

    /*! Path to image. */
    EGE::String m_path;
    /*! Atlast group name image should belong to. */
    EGE::String m_group;
    /*! Image. */
    EGE::PImage m_image;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // IMAGE_ENTRY_H