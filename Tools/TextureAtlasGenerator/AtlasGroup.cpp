#include "AtlasGroup.h"
#include "AtlasGroupEntry.h"
#include <QColor>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AtlasGroup::AtlasGroup(const QString& name, const QString& root, const QString& textureImageName, const QString& textureFormatName, 
                       const QString& textureFiltersName, const QSize& textureImageSize, bool alphaPremultiply) 
: m_name(name), 
  m_root(root), 
  m_textureImageName(textureImageName), 
  m_textureFormatName(textureFormatName),
  m_textureFiltersName(textureFiltersName), 
  m_textureImageSize(textureImageSize),
  m_alphaPremultiply(alphaPremultiply)
{
  m_image = QImage(textureImageSize.width(), textureImageSize.height(), QImage::Format_ARGB32);
  clearImage();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AtlasGroup::~AtlasGroup()
{
  for (QList<AtlasGroupEntry*>::iterator it = m_entries.begin(); it != m_entries.end();)
  {
    AtlasGroupEntry* entry = *it;

    if (NULL != entry)
    {
      delete entry;
    }

    m_entries.erase(it++);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool AtlasGroup::isValid() const 
{ 
  return !m_image.isNull() && !m_root.isEmpty() && !m_name.isEmpty() && !m_textureImageName.isEmpty() && !m_textureFiltersName.isEmpty() &&
         !m_textureFormatName.isEmpty(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds entry to pool. */
void AtlasGroup::addEntry(AtlasGroupEntry* entry)
{
  m_entries.push_back(entry);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears image. */
void AtlasGroup::clearImage()
{
  m_image.fill(QColor(0, 0, 0, 0));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
