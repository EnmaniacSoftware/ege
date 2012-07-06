#ifndef SWFDATASTREAM_H
#define SWFDATASTREAM_H

#include <QDataStream>
#include <QRect>
#include "SwfTypes.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QIODevice;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SwfDataStream : public QDataStream
{
  public:

    SwfDataStream(QIODevice* d);

    /* Reads given number of bits. */
    qint32 readBits(int bitCount, bool signedValue);
    /* Align to byte boundary. */
    void byteAlign();

    SwfDataStream& operator>>(qint8 &i);
    SwfDataStream& operator>>(quint8 &i);
    SwfDataStream& operator>>(qint16 &i);
    SwfDataStream& operator>>(quint16 &i);
    SwfDataStream& operator>>(qint32 &i);
    SwfDataStream& operator>>(quint32 &i);
    SwfDataStream& operator>>(fp8 &i);
    SwfDataStream& operator>>(qint64 &i);
    SwfDataStream& operator>>(quint64 &i);
    SwfDataStream& operator>>(QRect &i);

  private:

    /*! Currently read byte. */
    qint8 m_byte;
    /*! Current bit offset within current byte. */
    int m_bitOffset;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFDATASTREAM_H
