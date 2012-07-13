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
    @param image  Image to be added.
    @return  On success, non-negative unique image identifier assigned to given image. Otherwise, negative.
 */
int ResourceManager::addImage(const QImage& image)
{
  // add only if unique
  for (int i = 0; i < m_images.count(); ++i)
  {
    if (m_images[i] == image)
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

    QString name = generateNameFromImageId(i);

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
/*! Generates asset's name from image Id. */
QString ResourceManager::generateNameFromImageId(quint16 id) const
{
  return m_baseName + QString("-object-%1").arg(id);
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
    QString fullPath = m_outputLocation + QDir::separator() + generateNameFromImageId(i) + ".png";
    
    // rescale
    QImage image = m_images[i].scaled(QSize(m_images[i].width() * m_scale, m_images[i].height() * m_scale), Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
/*! Returns image references by given id. */
QImage ResourceManager::image(int id) const
{
  return m_images.value(id, QImage());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
