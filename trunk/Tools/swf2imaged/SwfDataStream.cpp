#include "SwfDataStream.h"
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define BITS_PER_BYTE (8)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//static quint8 l_bitMasks[BITS_PER_BYTE] = { 128, 192, 224, 240, 248, 252, 254, 255 };
static quint8 l_bitMasks[BITS_PER_BYTE] = { 1, 3, 7, 15, 31, 63, 127, 255 };
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream::SwfDataStream(QIODevice* d) : QDataStream(d),
                                             m_bitOffset(BITS_PER_BYTE)
{
  // set proper byte ordering
  setByteOrder(QDataStream::LittleEndian);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads given number of bits. */
qint32 SwfDataStream::readBits(int bitCount, bool signedValue)
{
  qint32 value = 0;
  qint32 bitsLeft = bitCount;

  while (0 < bitsLeft)
  {
    // check if no bit data read yet
    if (BITS_PER_BYTE <= m_bitOffset)
    {
      // read 1 byte
      *this >> m_byte;
      m_bitOffset = 0;
    }

    // check if enough data
    int bitsToRead = 0;
    if (bitsLeft <= (BITS_PER_BYTE - m_bitOffset))
    {
      bitsToRead = bitsLeft;
    }
    else
    {
      // read what is available
      bitsToRead = BITS_PER_BYTE - m_bitOffset;
    }

    // read partial data
    int partial = (m_byte >> (BITS_PER_BYTE - m_bitOffset - bitsToRead) ) & l_bitMasks[bitsToRead - 1];

    // append to final value
    value |= (partial << (bitsLeft - bitsToRead));

    // update state
    m_bitOffset += bitsToRead;
    bitsLeft -= bitsToRead;
  }

  // check if sign expansion needed
  if (signedValue)
  {
		if (value & (1 << (bitCount - 1)))
    {
			value |= -1 << bitCount;
		}
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Align to byte boundary. */
void SwfDataStream::byteAlign()
{
  m_bitOffset = BITS_PER_BYTE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream &SwfDataStream::operator >>(qint8 &i)
{
  byteAlign();

  *reinterpret_cast<QDataStream*>(this) >> i;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream &SwfDataStream::operator >>(quint8 &i)
{
  byteAlign();

  *reinterpret_cast<QDataStream*>(this) >> i;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream &SwfDataStream::operator >>(qint16 &i)
{
  byteAlign();

  *reinterpret_cast<QDataStream*>(this) >> i;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream &SwfDataStream::operator >>(quint16 &i)
{
  byteAlign();

  *reinterpret_cast<QDataStream*>(this) >> i;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream &SwfDataStream::operator >>(qint32 &i)
{
  byteAlign();

  *reinterpret_cast<QDataStream*>(this) >> i;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream &SwfDataStream::operator >>(quint32 &i)
{
  byteAlign();

  *reinterpret_cast<QDataStream*>(this) >> i;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream &SwfDataStream::operator >>(fp8 &i)
{
  byteAlign();

  qint16 val;
  *this >> val;

  i = (fp8) val / 256.0f;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream &SwfDataStream::operator >>(qint64 &i)
{
  byteAlign();

  *reinterpret_cast<QDataStream*>(this) >> i;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream &SwfDataStream::operator >>(quint64 &i)
{
  byteAlign();

  *reinterpret_cast<QDataStream*>(this) >> i;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream &SwfDataStream::operator >>(QRect &i)
{
  byteAlign();

  int bits = readBits(5, false);

  i.setLeft(readBits(bits, true));
  i.setRight(readBits(bits, true));
  i.setTop(readBits(bits, true));
  i.setBottom(readBits(bits, true));

  byteAlign();

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream& SwfDataStream::operator>>(QByteArray &i)
{
  byteAlign();

  int read = readRawData(i.data(), i.size());
  Q_ASSERT(read == i.size());

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream& SwfDataStream::operator>>(Matrix &i)
{
  byteAlign();

  quint8 bits;
  quint32 val1;
  quint32 val2;

  // scale
  bool flag = (1 == readBits(1, false));
  if (flag)
  {
    bits = readBits(5, false);
    val1 = readBits(bits, true);
    val2 = readBits(bits, true);

    i.scaleX = val1 / 65536.0f;
    i.scaleY = val2 / 65536.0f;
  }
  else
  {
    i.scaleX = 0.0f;
    i.scaleY = 0.0f;
  }

  // rotation
  flag = (1 == readBits(1, false));
  if (flag)
  {
    bits = readBits(5, false);
    val1 = readBits(bits, true);
    val2 = readBits(bits, true);

    i.rotX = val1 / 65536.0f;
    i.rotY = val2 / 65536.0f;
  }
  else
  {
    i.rotX = 0.0f;
    i.rotY = 0.0f;
  }

  // translation
  bits = readBits(5, false);
  i.translateX = readBits(bits, true);
  i.translateY = readBits(bits, true);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads RGB value. */
QColor SwfDataStream::readRGB()
{
  byteAlign();

  QColor color;

  quint8 channel;

  *this >> channel;
  color.setRed(channel);

  *this >> channel;
  color.setGreen(channel);

  *this >> channel;
  color.setBlue(channel);

  return color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads RGBA value. */
QColor SwfDataStream::readRGBA()
{
  byteAlign();

  QColor color;

  quint8 channel;

  *this >> channel;
  color.setRed(channel);

  *this >> channel;
  color.setGreen(channel);

  *this >> channel;
  color.setBlue(channel);

  *this >> channel;
  color.setAlpha(channel);

  return color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads fill style array. 
    @param version Version of DefineShape for which loading is supposed to be done.
    @return  Returns list of defined fill styles.
 */
QList<FillStyle> SwfDataStream::readFillStyleArray(int version)
{
  QList<FillStyle> list;

  byteAlign();

  quint16 count = 0;

  // get count
  quint8 byte;
  *this >> byte;

  // check if extended count is present
  if (0xff == byte)
  {
    *this >> count;
  }
  else
  {
    count = byte;
  }

  // load fill styles
  while (0 != count)
  {
    // read single style
    FillStyle style = readFillStyle(version);

    // add to pool
    list.append(style);

    // go to next
    --count;
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads line style array. 
    @param version Version of DefineShape for which loading is supposed to be done.
    @return  Returns list of defined line styles.
 */
QList<LineStyle> SwfDataStream::readLineStyleArray(int version)
{
  QList<LineStyle> list;

  byteAlign();

  quint16 count = 0;

  // get count
  quint8 byte;
  *this >> byte;

  // check if extended count is present
  if (0xff == byte)
  {
    *this >> count;
  }
  else
  {
    count = byte;
  }

  // load fill styles
  while (0 != count)
  {
    // read single style
    LineStyle style = readLineStyle(version);

    // add to pool
    list.append(style);

    // go to next
    --count;
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads fill style record.
    @param version Version of DefineShape for which loading is supposed to be done.
    @return  Returns fill style.
 */
FillStyle SwfDataStream::readFillStyle(int version)
{
  FillStyle style;

  // style type
  *this >> style.type;

  // color
  if (FST_SOLID == style.type)
  {
    if (3 > version)
    {
      style.color = readRGB();
    }
    else
    {
      style.color = readRGBA();
    }
  }

  // gradient matrix
  if ((FST_LINEAR_GRADIENT == style.type) || (FST_RADIAL_GRADIENT == style.type) || (FST_FOCAL_RADIAL_GRADIENT == style.type))
  {
    *this >> style.gradientMatrix;
  }

  // gradient
  if ((FST_LINEAR_GRADIENT == style.type) || (FST_RADIAL_GRADIENT == style.type))
  {
    // TAGE - implement
    Q_ASSERT(false);
  }
  else if (FST_FOCAL_RADIAL_GRADIENT == style.type)
  {
    // TAGE - implement
    Q_ASSERT(false);
  }

  // bitmap ID and matrix
  if ((FST_REPEAT_BITMAP == style.type) || (FST_CLIPPED_BITMAP == style.type) || (FST_NONSMOOTHED_REPEAT_BITMAP == style.type) || 
      (FST_NONSMOOTHED_CLIPPED_BITMAP == style.type))
  {
    *this >> style.bitmapCharacterId;
    *this >> style.bitmapMatrix;
  }

  return style;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads line style record.
    @param version Version of DefineShape for which loading is supposed to be done.
    @return  Returns line style.
 */
LineStyle SwfDataStream::readLineStyle(int version)
{
  LineStyle style;

  Q_ASSERT(false);

  return style;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
