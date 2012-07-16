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
SwfDataStream::SwfDataStream(const QByteArray& a) : QDataStream(a),
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
  Q_UNUSED(read);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDataStream& SwfDataStream::operator>>(Matrix &i)
{
  byteAlign();

  quint8 bits;
  qint32 val1;
  qint32 val2;

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
    i.scaleX = 1.0f;
    i.scaleY = 1.0f;
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
/*! Reads fix point 8.8 value. */
float SwfDataStream::readFP8()
{
  byteAlign();

  qint16 val;
  *this >> val;

  return val / 256.0f;
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
    // NOTE: it is possible that there is the same issue with as for bitmap matrix!!
    *this >> style.gradientMatrix;
  }

  // gradient
  if ((FST_LINEAR_GRADIENT == style.type) || (FST_RADIAL_GRADIENT == style.type))
  {
    style.gradient = readGradient(version);
  }
  else if (FST_FOCAL_RADIAL_GRADIENT == style.type)
  {
    style.focalGradient = readFocalGradient(version);
  }

  // bitmap ID and matrix
  if ((FST_REPEAT_BITMAP == style.type) || (FST_CLIPPED_BITMAP == style.type) || (FST_NONSMOOTHED_REPEAT_BITMAP == style.type) || 
      (FST_NONSMOOTHED_CLIPPED_BITMAP == style.type))
  {
    Matrix matrix;

    *this >> style.bitmapCharacterId;
    *this >> matrix;

		// Invert the rotation part.
		float	det = matrix.scaleX * matrix.scaleY - matrix.rotX * matrix.rotY;
		if (0.0f == det)
		{
			// Not invertible.
		  Q_ASSERT("Invertible!"); // castano: this happens sometimes! (ie. sample6.swf)

			// Arbitrary fallback.
			style.bitmapMatrix.scaleX     = 1.0f;
			style.bitmapMatrix.scaleY     = 1.0f;
			style.bitmapMatrix.rotX       = 0.0f;
			style.bitmapMatrix.rotY       = 0.0f;
			style.bitmapMatrix.translateX = -matrix.translateX;
			style.bitmapMatrix.translateY = -matrix.translateY;
		}
		else
		{
			float	inv_det = 1.0f / det;
      
      style.bitmapMatrix.scaleX = matrix.scaleY * inv_det;
      style.bitmapMatrix.scaleY = matrix.scaleX * inv_det;
      style.bitmapMatrix.rotX   = -matrix.rotY * inv_det;
      style.bitmapMatrix.rotY   = -matrix.rotX * inv_det;
			//style.bitmapMatrix.translateX = -(style.bitmapMatrix.scaleX * matrix.translateX + style.bitmapMatrix.rotX   * matrix.translateY);
			//style.bitmapMatrix.translateY = -(style.bitmapMatrix.rotY   * matrix.translateX + style.bitmapMatrix.scaleY * matrix.translateY);
      style.bitmapMatrix.translateX = matrix.translateX;
      style.bitmapMatrix.translateY = matrix.translateY;

      style.bitmapMatrix.scaleX *= 20.0f;
      style.bitmapMatrix.scaleY *= 20.0f;

		}

    style.bitmapMatrix = matrix;

    // NOTE: it seems for FillStyle matrix scale (and rotation ?) is in twips ?!
    style.bitmapMatrix.scaleX *= 0.05f;
    style.bitmapMatrix.scaleY *= 0.05f;
    style.bitmapMatrix.rotX *= 0.05f;
    style.bitmapMatrix.rotY *= 0.05f;
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

  *this >> style.width;

  if (3 > version)
  {
    style.color = readRGB();
  }
  else
  {
    style.color = readRGBA();
  }

  return style;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads gradient record. 
    @param version Version of DefineShape for which loading is supposed to be done.
    @return  Returns gradient record read.
 */
GradientRecord SwfDataStream::readGradientRecord(int version)
{
  GradientRecord data;

  *this >> data.ratio;

  if (3 > version)
  {
    data.color = readRGB();
  }
  else
  {
    data.color = readRGBA();
  }

  return data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads gradient. 
    @param version Version of DefineShape for which loading is supposed to be done.
    @return  Returns gradient read.
 */
Gradient SwfDataStream::readGradient(int version)
{
  Gradient gradient;

  byteAlign();

  gradient.spreadMode        = readBits(2, false);
  gradient.interpolationMode = readBits(2, false);
  
  quint32 count = readBits(4, false);
  for (quint32 i = 0; i < count; ++i)
  {
    gradient.gradientPoints << readGradientRecord(version);
  }

  return gradient;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads focal gradient. 
    @param version Version of DefineShape for which loading is supposed to be done.
    @return  Returns focal gradient read.
 */
FocalGradient SwfDataStream::readFocalGradient(int version)
{
  FocalGradient gradient;

  byteAlign();

  gradient.spreadMode        = readBits(2, false);
  gradient.interpolationMode = readBits(2, false);
  
  quint32 count = readBits(4, false);
  for (quint32 i = 0; i < count; ++i)
  {
    gradient.gradientPoints << readGradientRecord(version);
  }

  gradient.focalPoint = readFP8();
  return gradient;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
