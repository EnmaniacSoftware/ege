#ifndef SWFHEADER_H
#define SWFHEADER_H

#include <QRect>
#include <QFile>
#include "SwfDataStream.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SwfHeader
{
  public:

    SwfHeader();
   ~SwfHeader();

    /* Reads data from file. */
    bool read(SwfDataStream& data);
    /* Returns FPS. */
    float fps() const;
    /* Returns frame size. */
    const QSize& frameSize() const;
    /* Returns frame count. */
    quint16 frameCount() const;
    
  private:

    /*! Version. */
    quint8 m_version;
    /*! File size (in bytes). */
    quint32 m_fileSize;
    /*! Frame size (in twips). */
    QSize m_frameSize;
    /*! Frame rate. */
    float m_fps;
    /*! Frame count. */
    quint16 m_frameCount;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFHEADER_H
