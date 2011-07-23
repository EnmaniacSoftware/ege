#include "AtlasGroupEntry.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AtlasGroupEntry::AtlasGroupEntry(const String& name, const String& path) : m_name(name), m_path(path)
{
  m_image = ege_new EGE::Image(NULL);
  if (m_image)
  {
    // load image 
    if (EGE::EGE_SUCCESS != m_image->load(m_path, EGE::EGEImage::RGBA_8888))
    {
      // error!
      m_image = NULL;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AtlasGroupEntry::~AtlasGroupEntry()
{
  m_image = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool AtlasGroupEntry::isValid() const 
{ 
  return (NULL != m_image) && (EGE::EGEImage::NONE != m_image->format()) && !m_name.empty() && !m_path.empty(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
