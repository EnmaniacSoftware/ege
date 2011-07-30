#include "AtlasGroup.h"
#include "AtlasGroupEntry.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AtlasGroup::AtlasGroup(const String& name, const String& root, const String& imageName, s32 imageSize, EGEImage::Format imageFormat) 
: m_name(name), m_root(root), m_imageName(imageName), m_imageSize(imageSize)
{
  m_image = ege_new EGE::Image(NULL, imageSize, imageSize, imageFormat);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AtlasGroup::~AtlasGroup()
{
  for (List<AtlasGroupEntry*>::iterator it = m_entries.begin(); it != m_entries.end();)
  {
    EGE_DELETE(*it);

    m_entries.erase(it++);
  }

  m_image = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool AtlasGroup::isValid() const 
{ 
  return (NULL != m_image) && (EGE::EGEImage::NONE != m_image->format()) && !m_root.empty() && !m_name.empty() && !m_imageName.empty(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds entry to pool. */
void AtlasGroup::addEntry(AtlasGroupEntry* entry)
{
  m_entries.push_back(entry);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
