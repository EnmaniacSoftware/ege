#ifndef ATLAS_GROUP_ENTRY_H
#define ATLAS_GROUP_ENTRY_H

#include <EGEString.h>
#include <EGEImage.h>
#include <EGEVector.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AtlasGroupEntry
{
  public:

    AtlasGroupEntry(const EGE::String& name, const EGE::String& path, const EGE::Vector4i& spacing);
   ~AtlasGroupEntry();

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns image object. */
    inline EGE::PImage image() const { return m_image; }
    /*! Returns image path. */
    inline const EGE::String& path() const { return m_path; }
    /*! Returns image name. */
    inline const EGE::String& name() const { return m_name; }
    /*! Returns spacing. */
    inline const EGE::Vector4i& spacing() const { return m_spacing; }

  private:

    /*! Image name. */
    EGE::String m_name;
    /*! Path to image. */
    EGE::String m_path;
    /*! Spacing (left, top, right, bottom). */
    EGE::Vector4i m_spacing;
    /*! Image. */
    EGE::PImage m_image;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ATLAS_GROUP_ENTRY_H