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

    AtlasGroup(const EGE::String& name, const EGE::String& imagePath, EGE::s32 atlasImageSize, EGE::EGEImage::Format atlasImageFormat);
   ~AtlasGroup();

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns image object. */
    inline EGE::PImage atlasImage() const { return m_atlasImage; }
    /*! Returns image path. */
    inline const EGE::String& imagePath() const { return m_imagePath; }
    /*! Returns image name. */
    inline const EGE::String& name() const { return m_name; }
    /* Adds entry to pool. */
    void addEntry(AtlasGroupEntry* entry);
    /*! Returns list of entries. */
    inline EGE::List<AtlasGroupEntry*>& entries() { return m_entries; }

  private:

    /*! Path to image. */
    EGE::String m_imagePath;
    /*! Image name. */
    EGE::String m_name;
    /*! List of group entries. */
    EGE::List<AtlasGroupEntry*> m_entries;
    /*! Atlas image. */
    EGE::PImage m_atlasImage;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ATLAS_GROUP_H