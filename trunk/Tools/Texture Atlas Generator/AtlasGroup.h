#ifndef ATLAS_GROUP_H
#define ATLAS_GROUP_H

#include <EGEString.h>
#include <EGEImage.h>
#include <EGEList.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AtlasGroupEntry;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AtlasGroup
{
  public:

    AtlasGroup(const EGE::String& name, const EGE::String& path, const EGE::String& image, EGE::s32 atlasImageSize, EGE::EGEImage::Format atlasImageFormat);
   ~AtlasGroup();

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns image object. */
    inline EGE::PImage atlasImage() const { return m_atlasImage; }
    /*! Returns group path to root directory. */
    inline const EGE::String& path() const { return m_path; }
    /*! Returns image path. */
    inline const EGE::String& image() const { return m_image; }
    /*! Returns image name. */
    inline const EGE::String& name() const { return m_name; }
    /* Adds entry to pool. */
    void addEntry(AtlasGroupEntry* entry);
    /*! Returns list of entries. */
    inline EGE::List<AtlasGroupEntry*>& entries() { return m_entries; }

  private:

    /*! Path to theme root dir. */
    EGE::String m_path;
    /*! Image path (relative to theme root). */
    EGE::String m_image;
    /*! Image name. */
    EGE::String m_name;
    /*! List of group entries. */
    EGE::List<AtlasGroupEntry*> m_entries;
    /*! Atlas image. */
    EGE::PImage m_atlasImage;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ATLAS_GROUP_H