#include "SwfDefineBitsTag.h"
#include "SwfFile.h"
#include "ResourceManager.h"
#include <QImage>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDefineBitsTag::SwfDefineBitsTag() : SwfTag(),
                                       m_characterId(0),
                                       m_imageId(-1)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDefineBitsTag::~SwfDefineBitsTag()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SwfTag override. Reads data from file. */
bool SwfDefineBitsTag::read(SwfDataStream& data)
{
  data >> m_characterId;

  // read data (all remaining)
  m_imageData.resize(length() - sizeof (m_characterId));
  data >> m_imageData;

  // create image from pixel data
  QImage image = QImage::fromData(m_imageData);
  if (image.isNull())
  {
    // error!
    qCritical() << Q_FUNC_INFO << "Could not create image from data! Character Id" << m_characterId;
    return false;
  }

  // add to dictionary
  Q_ASSERT( ! file()->dictionary().contains(m_characterId));
  file()->dictionary().insert(m_characterId, this);

  // add image to resource manager
  m_imageId = resourceManager()->addImage(image);
  if (0 > m_imageId)
  {
    // error!
    return false;
  }

  //image.save(QString("jpeg3-%1-%2.png").arg(m_characterId).arg(m_imageId), "PNG");

  return QDataStream::Ok == data.status();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns character ID. */
quint16 SwfDefineBitsTag::characterId() const
{
  return m_characterId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns image ID. */
int SwfDefineBitsTag::imageId() const
{
  return m_imageId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
