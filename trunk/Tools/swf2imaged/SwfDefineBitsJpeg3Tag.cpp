#include "SwfDefineBitsJpeg3Tag.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDefineBitsJpeg3Tag::SwfDefineBitsJpeg3Tag() : SwfTag(),
                                                 m_characterId(0),
                                                 m_alphaOffset(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDefineBitsJpeg3Tag::~SwfDefineBitsJpeg3Tag()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SwfTag override. Reads data from file. */
bool SwfDefineBitsJpeg3Tag::read(SwfDataStream& data)
{
  data >> m_characterId;
  data >> m_alphaOffset;

  // read JPEG data
  m_imageData.resize(m_alphaOffset);
  data >> m_imageData;

  // read alpha data (all remaining)
  m_alphaData.resize(length() - sizeof (m_characterId) - sizeof (m_alphaOffset) - m_alphaOffset);
  data >> m_alphaData;

  // create image from pixel data
  QImage image = QImage::fromData(m_imageData);

  // prepend alpha data block with uncompressed size so qUncompress can be used
  // NOTE: alpha block format is 8-bit per pixel
  int alphaBlockUncompressedSize = image.width() * image.height();

  m_alphaData.prepend((alphaBlockUncompressedSize & 0x000000ff));
  m_alphaData.prepend((alphaBlockUncompressedSize & 0x0000ff00) >> 8);
  m_alphaData.prepend((alphaBlockUncompressedSize & 0x00ff0000) >> 16);
  m_alphaData.prepend((alphaBlockUncompressedSize & 0xff000000) >> 24);

  // decompress alpha channel
  m_alphaData = qUncompress(m_alphaData);

  // create ARGBA image from pixel data image
  m_image = image.convertToFormat(QImage::Format_ARGB32);
      
  // TAGE - optimize

  // add alpha channel
  for (int y = 0; y < m_image.height(); ++y)
  {
    for (int x = 0; x < m_image.width(); ++x)
    {
      QRgb color = m_image.pixel(x, y);
      m_image.setPixel(x, y, qRgba(qRed(color), qGreen(color), qBlue(color), m_alphaData[x + y * m_image.width()]));
    }
  }

  //m_image.save("jpeg3.png", "png");

  return QDataStream::Ok == data.status();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------