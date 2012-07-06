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
