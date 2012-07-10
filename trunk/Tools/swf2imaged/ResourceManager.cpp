#include "ResourceManager.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceManager(QObject* parent ) : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds image into pool. */
int ResourceManager::addImage(const QImage& image)
{
  // add only if unique
  for (int i = 0; i < m_images.count(); ++i)
  {
    const QImage& img = m_images[i];

    if (img == image)
    {
      // found, return reference
      return i;
    }
  }

  // add to pool
  m_images.append(image);  
  return m_images.count() - 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Serializes into EGE XML. */
bool ResourceManager::serialize(QXmlStreamWriter& stream)
{
  for (int i = 0; i < m_images.count(); ++i)
  {
    stream.writeStartElement("material");

    stream.writeAttribute("name", "some-name-a");
    stream.writeAttribute("src-blend", "src-alpha");
    stream.writeAttribute("dst-blend", "one-minus-src-alpha");

    stream.writeStartElement("texture");
    stream.writeAttribute("name", "some-texture-name");
    stream.writeEndElement();

    stream.writeEndElement();
  }

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
