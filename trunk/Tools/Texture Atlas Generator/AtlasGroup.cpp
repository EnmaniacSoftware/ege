#include "AtlasGroup.h"
#include "AtlasGroupEntry.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AtlasGroup::AtlasGroup(const String& name, const String& path, const String& image, s32 atlasImageSize, EGEImage::Format atlasImageFormat) 
: m_name(name), m_path(path), m_image(image)
{
  m_atlasImage = ege_new EGE::Image(NULL, atlasImageSize, atlasImageSize, atlasImageFormat);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AtlasGroup::~AtlasGroup()
{
  for (List<AtlasGroupEntry*>::iterator it = m_entries.begin(); it != m_entries.end();)
  {
    EGE_DELETE(*it);

    m_entries.erase(it++);
  }

  m_atlasImage = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool AtlasGroup::isValid() const 
{ 
  return (NULL != m_atlasImage) && (EGE::EGEImage::NONE != m_atlasImage->format()) && !m_path.empty() && !m_name.empty() && !m_image.empty(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds entry to pool. */
void AtlasGroup::addEntry(AtlasGroupEntry* entry)
{
  m_entries.push_back(entry);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
