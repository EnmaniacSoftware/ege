#ifndef SWFDATASTREAM_H
#define SWFDATASTREAM_H

#include <QDataStream>
#include <QRect>
#include <QColor>
#include <QByteArray>
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
    /* Reads RGB value. */
    QColor readRGB();
    /* Reads RGBA value. */
    QColor readRGBA();
    /* Reads fix point 8.8 value. */
    float readFP8();
    /* Reads fill style array. 
       @param version Version of DefineShape for which loading is supposed to be done.
       @return  Returns list of defined fill styles.
     */
    QList<FillStyle> readFillStyleArray(int version);
    /* Reads line style array. 
       @param version Version of DefineShape for which loading is supposed to be done.
       @return  Returns list of defined line styles.
     */
    QList<LineStyle> readLineStyleArray(int version);

    SwfDataStream& operator>>(qint8 &i);
    SwfDataStream& operator>>(quint8 &i);
    SwfDataStream& operator>>(qint16 &i);
    SwfDataStream& operator>>(quint16 &i);
    SwfDataStream& operator>>(qint32 &i);
    SwfDataStream& operator>>(quint32 &i);
    SwfDataStream& operator>>(qint64 &i);
    SwfDataStream& operator>>(quint64 &i);
    SwfDataStream& operator>>(QRect &i);
    SwfDataStream& operator>>(QByteArray &i);
    SwfDataStream& operator>>(Matrix &i);

  private:

    /* Reads fill style record.
       @param version Version of DefineShape for which loading is supposed to be done.
       @return  Returns fill style.
     */
    FillStyle readFillStyle(int version);
    /* Reads line style record.
       @param version Version of DefineShape for which loading is supposed to be done.
       @return  Returns line style.
     */
    LineStyle readLineStyle(int version);

  private:

    /*! Currently read byte. */
    qint8 m_byte;
    /*! Current bit offset within current byte. */
    int m_bitOffset;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFDATASTREAM_H
