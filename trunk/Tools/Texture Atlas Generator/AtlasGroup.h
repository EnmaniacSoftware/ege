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

    AtlasGroup(const EGE::String& name, const EGE::String& root, const EGE::String& imageName, EGE::s32 imageSize, EGE::EGEImage::Format imageFormat);
   ~AtlasGroup();

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns group name. */
    inline const EGE::String& name() const { return m_name; }
    /*! Returns image object. */
    inline EGE::PImage image() const { return m_image; }
    /*! Returns root data path. */
    inline const EGE::String& root() const { return m_root; }
    /*! Returns image name. */
    inline const EGE::String& imageName() const { return m_imageName; }
    /* Adds entry to pool. */
    void addEntry(AtlasGroupEntry* entry);
    /*! Returns list of entries. */
    inline EGE::List<AtlasGroupEntry*>& entries() { return m_entries; }
    /*! Returns atlas image size (in pixels). */
    inline EGE::s32 imageSize() const { return m_imageSize; }

  private:

    /*! Name. */
    EGE::String m_name;
    /*! Data root path. */
    EGE::String m_root;
    /*! Image name (relative to root). */
    EGE::String m_imageName;
    /*! List of group entries. */
    EGE::List<AtlasGroupEntry*> m_entries;
    /*! Atlas image. */
    EGE::PImage m_image;
    /*! Atlas image size (in pixels). */
    EGE::s32 m_imageSize;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ATLAS_GROUP_H