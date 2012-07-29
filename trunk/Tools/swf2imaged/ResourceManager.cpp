#include "ResourceManager.h"
#include <QDir>
#include <QXmlStreamWriter>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager::ResourceManager(QObject* parent ) : QObject(parent),
                                                     m_currentSession(NULL)
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
  Q_ASSERT(NULL != m_currentSession);

  // add only if unique
  for (int i = 0; i < m_images.count(); ++i)
  {
    if (m_images[i] == image)
    {
      // found

      // bound image id with current session
      if ( ! m_currentSession->imageList.contains(i))
      {
        m_currentSession->imageList << i;
      }

      // return reference id
      return i;
    }
  }

  // add to pool
  m_images.append(image);

  // bound image id with current session
  m_currentSession->imageList << m_images.count() - 1;

  return m_images.count() - 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Serializes into EGE XML. */
bool ResourceManager::serialize(QXmlStreamWriter& stream)
{
  Q_ASSERT(NULL != m_currentSession);

  for (int i = 0; i < m_currentSession->imageList.count(); ++i)
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
QString ResourceManager::generateNameFromImageId(int id) const
{
  Q_ASSERT(NULL != m_currentSession);

  return QString::number(m_currentSession->scale).remove(".") + "-" + m_currentSession->baseName + QString("-object-%1").arg(id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves assets. */
bool ResourceManager::saveAssets()
{
  Q_ASSERT(NULL != m_currentSession);

  // make sure output directory exists
  QDir path;
  if ( ! path.mkpath(m_currentSession->outputLocation))
  {
    // error!
    qCritical() << "Could not create output directory" << m_currentSession->outputLocation;
    return false;
  }

  // save assets
  for (int i = 0; i < m_currentSession->imageList.count(); ++i)
  {
    QString fullPath = m_currentSession->outputLocation + QDir::separator() + generateNameFromImageId(i) + ".png";
    
    // rescale
    QImage image = m_images[m_currentSession->imageList[i]].scaled(QSize(m_images[i].width() * m_currentSession->scale, 
                                                                         m_images[i].height() * m_currentSession->scale), Qt::KeepAspectRatio, 
                                                                         Qt::SmoothTransformation);
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

  // generate atlas texture XML if necessary
  return generateAtlasTextureXML();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns image references by given id. */
QImage ResourceManager::image(int id) const
{
  return m_images.value(id, QImage());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds atlas texture definition. */
bool ResourceManager::addAtlasTextureDefinition(const QMap<QString, QString>& properties)
{
  // check if invalid definition
  if ( ! properties.contains("name"))
  {
    // error!
    qCritical() << "Invalid atlas texture definition";
    return false;
  }

  // check if such atlas texture exists already
  foreach (const AtlasTextureData& texture, m_atlasTexturesList)
  {
    if (texture.properties["name"] == properties["name"])
    {
      // warning!
      qWarning() << "Atla texture" << properties["name"] << "already defined!";
      return true;
    }
  }

  // add to pool
  AtlasTextureData texture;
  texture.properties = properties;

  m_atlasTexturesList << texture;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Begins new resource session. */
bool ResourceManager::beginSession(const QString& baseName, float scale, const QString& outputLocation, const QString& atlasTexture)
{
  SessionData session;

  session.baseName        = baseName;
  session.outputLocation  = outputLocation;
  session.atlasTexture    = atlasTexture;
  session.scale           = scale;

  // check if the same as one of the existing sessions
  for (int i = 0; i < m_sessionList.count(); ++i)
  {
    SessionData& s = m_sessionList[i];

    if ((s.baseName == session.baseName) && (s.outputLocation == session.outputLocation) && (s.scale == session.scale) && 
        (s.atlasTexture == session.atlasTexture))
    {
      // found, make it current
      m_currentSession = &s;
      return true;
    }
  }

  // add new session
  m_sessionList << session;

  // make it current;
  m_currentSession = &m_sessionList.last();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates atlas texture XML. */
bool ResourceManager::generateAtlasTextureXML()
{
  Q_ASSERT(NULL != m_currentSession);

  // check if atlas texture XML not necessary
  if (m_currentSession->atlasTexture.isEmpty())
  {
    // done
    return true;
  }

  // find proper atlast texture data
  const AtlasTextureData* atlasTextureData = NULL;
  foreach (const AtlasTextureData& data, m_atlasTexturesList)
  {
    if (data.properties["name"] == m_currentSession->atlasTexture)
    {
      atlasTextureData = &data;
      break;
    }
  }

  if (NULL == atlasTextureData)
  {
    // error!
    qCritical() << "Could not find atlas texture definition" << m_currentSession->atlasTexture;
    return false;
  }

  QString outputXml;
  QXmlStreamWriter output(&outputXml);
  output.setAutoFormatting(true);

  output.writeStartElement("resources");
  
  output.writeStartElement("atlas-group");
  for (QMap<QString, QString>::const_iterator it = atlasTextureData->properties.begin(); it != atlasTextureData->properties.end(); ++it)
  {
    output.writeAttribute(it.key(), it.value());
  }

  for (int i = 0; i < m_currentSession->imageList.count(); ++i)
  {
    QString fullPath = m_currentSession->outputLocation + "/" + generateNameFromImageId(i) + ".png";

    output.writeStartElement("image");
    output.writeAttribute("name", generateNameFromImageId(i));
    output.writeAttribute("path", QDir::fromNativeSeparators(fullPath));
    output.writeAttribute("spacing", "1 1 1 1");
    output.writeEndElement();
  }

  output.writeEndElement();
  
  output.writeEndElement();

  QFile outputFile(m_currentSession->outputLocation + "/atlas-" + m_currentSession->atlasTexture + ".xml");
  if ( ! outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    // error!
    qCritical() << "Could not open file for writting" << outputFile.fileName();
    return false;
  }

  QTextStream out(&outputFile);
  out << outputXml;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns image index within current session from global image id. */
int ResourceManager::imageIndex(int id) const
{
  Q_ASSERT(NULL != m_currentSession);
  
  for (int i = 0; m_currentSession->imageList.count(); ++i)
  {
    if (m_currentSession->imageList[i] == id)
    {
      // found
      return i;
    }
  }

  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
