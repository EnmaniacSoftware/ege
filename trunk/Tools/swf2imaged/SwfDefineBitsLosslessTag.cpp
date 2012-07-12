#include "SwfDefineBitsLosslessTag.h"
#include "SwfFile.h"
#include "ResourceManager.h"
#include <QImage>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDefineBitsLosslessTag::SwfDefineBitsLosslessTag() : SwfTag(),
                                                       m_characterId(0),
                                                       m_imageId(-1)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDefineBitsLosslessTag::~SwfDefineBitsLosslessTag()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SwfTag override. Reads data from file. */
bool SwfDefineBitsLosslessTag::read(SwfDataStream& data)
{
  data >> m_characterId;

  quint8 type;
  data >> type;

  quint16 width;
  quint16 height;
  data >> width;
  data >> height;

  // check if 8-bit format
  if (3 == type)
  {
    // unsupported
    qCritical() << "8bit format is unsupported!";
    return false;
  }

  // check if 16-bit format
  if (4 == type)
  {
    // unsupported
    qCritical() << "16bit format is unsupported!";
    return false;
  }

  // read compressed data (all remaining)
  m_imageData.resize(length() - sizeof (m_characterId) - sizeof (type) - sizeof(width) - sizeof (height));
  data >> m_imageData;
          
  // prepend pixel data block with uncompressed size so qUncompress can be used
  // NOTE: image rows need to be 4-bytes aligned
  int imageUncompressedSize = width * height;

  m_imageData.prepend((imageUncompressedSize & 0x000000ff));
  m_imageData.prepend((imageUncompressedSize & 0x0000ff00) >> 8);
  m_imageData.prepend((imageUncompressedSize & 0x00ff0000) >> 16);
  m_imageData.prepend((imageUncompressedSize & 0xff000000) >> 24);

  // decompress image
  m_imageData = qUncompress(m_imageData);

  // create empty ARGB image
  QImage image = QImage(width, height, QImage::Format_ARGB32);

  // TAGE - optimize

  // fill in pixel data
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      image.setPixel(x, y, qRgba(m_imageData[4 * (x + y * width) + 1], m_imageData[4 * (x + y * width) + 2], 
                                 m_imageData[4 * (x + y * width) + 3], m_imageData[4 * (x + y * width) + 0]));
    }
  }

  // add to dictionary
  Q_ASSERT( ! file()->dictionary().contains(m_characterId));
  file()->dictionary().insert(m_characterId, this);

  // add image to resource manager
  m_imageId = resourceManager()->addImage(image, m_characterId);
  if (0 > m_imageId)
  {
    // error!
    return false;
  }

  //image.save(QString("jpeg3-%1-%2.png").arg(m_characterId).arg(m_imageId), "PNG");

  return QDataStream::Ok == data.status();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
