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

    AtlasGroup(const EGE::String& name, const EGE::String& root, const EGE::String& textureImageName, const EGE::String& textureFiltersName, 
               EGE::s32 textureImageSize, EGE::EGEImage::Format imageFormat);
   ~AtlasGroup();

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns group name. */
    inline const EGE::String& name() const { return m_name; }
    /*! Returns image object. */
    inline EGE::PImage image() const { return m_image; }
    /*! Returns root data path. */
    inline const EGE::String& root() const { return m_root; }
    /*! Returns texture image name. */
    inline const EGE::String& textureImageName() const { return m_textureImageName; }
    /*! Returns texture filters name. */
    inline const EGE::String& textureFiltersName() const { return m_textureFiltersName; }
    /* Adds entry to pool. */
    void addEntry(AtlasGroupEntry* entry);
    /*! Returns list of entries. */
    inline EGE::List<AtlasGroupEntry*>& entries() { return m_entries; }
    /*! Returns atlas texture image size (in pixels). */
    inline EGE::s32 textureImageSize() const { return m_textureImageSize; }

  private:

    /*! Name. */
    EGE::String m_name;
    /*! Data root path. */
    EGE::String m_root;
    /*! Texture image name (relative to root). */
    EGE::String m_textureImageName;
    /*! Texture filters name. */
    EGE::String m_textureFiltersName;
    /*! List of group entries. */
    EGE::List<AtlasGroupEntry*> m_entries;
    /*! Atlas image. */
    EGE::PImage m_image;
    /*! Atlas texture image size (in pixels). */
    EGE::s32 m_textureImageSize;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ATLAS_GROUP_H