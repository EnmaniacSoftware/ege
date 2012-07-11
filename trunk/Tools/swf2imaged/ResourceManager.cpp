#include "ResourceManager.h"
#include <QDir>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceManager(const QString& baseName, const QString& outputLocation, float scale, QObject* parent ) : QObject(parent),
                                                                                                                          m_baseName(baseName),
                                                                                                                          m_scale(scale),
                                                                                                                          m_outputLocation(outputLocation)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds image into pool.
    @param image       Image to be added.
    @param characterId SWF character id for an image.
    @return  On success, non-negative unique image identifier assigned to given image. Otherwise, negative.
 */
int ResourceManager::addImage(const QImage& image, quint16 characterId)
{
  // add only if unique
  for (int i = 0; i < m_images.count(); ++i)
  {
    const ImageData& data = m_images[i];

    if (data.image == image)
    {
      // found, return reference
      return i;
    }
  }

  // add to pool
  ImageData data;
  data.image        = image;
  data.characterId  = characterId;
  m_images.append(data);

  return m_images.count() - 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Serializes into EGE XML. */
bool ResourceManager::serialize(QXmlStreamWriter& stream)
{
  for (int i = 0; i < m_images.count(); ++i)
  {
    const ImageData& data = m_images[i];

    stream.writeStartElement("material");

    QString name = generateNameFromCharacterId(data.characterId);

    stream.writeAttribute("name", name);
    stream.writeAttribute("src-blend", "src-alpha");
    stream.writeAttribute("dst-blend", "one-minus-src-alpha");

    stream.writeStartElement("texture");
    stream.writeAttribute("name", name);
    stream.writeEndElement();

    stream.writeEndElement();
  }

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates asset's name from character Id. */
QString ResourceManager::generateNameFromCharacterId(quint16 characterId) const
{
  return m_baseName + QString("-object-%1").arg(characterId);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves assets. */
bool ResourceManager::saveAssets()
{
  // make sure output directory exists
  QDir path;
  if ( ! path.mkpath(m_outputLocation))
  {
    // error!
    qCritical() << "Could not create output directory" << m_outputLocation;
    return false;
  }

  // save assets
  for (int i = 0; i < m_images.count(); ++i)
  {
    const ImageData& data = m_images[i];

    QString fullPath = m_outputLocation + QDir::separator() + generateNameFromCharacterId(data.characterId) + ".png";
    
    // rescale
    QImage image = data.image.scaled(QSize(data.image.width() * m_scale, data.image.height() * m_scale), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (image.isNull())
    {
      // error!
      qCritical() << "Could not rescale image";
      return false;
    }

    // save image
    if ( ! image.save(fullPath, "PNG"))
    {
      // error!
      qCritical() << "Could not save image" << fullPath;
      return false;
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns image references by given character ID. */
QImage ResourceManager::image(quint16 characterId) const
{
  for (int i = 0; i < m_images.count(); ++i)
  {
    const ImageData& data = m_images[i];
    if (data.characterId == characterId)
    {
      // found
      return data.image;
    }
  }

  // not found
  return QImage();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
