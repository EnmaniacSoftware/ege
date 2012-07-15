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

    /* Processes header of a given file. 
       @param fileName  Path to SWF file to process.
       @return  Returns SWF data stream if given file is valid. Otherwise, NULL.
     */
    SwfDataStream* process(const QString &fileName);
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
    /*! SWF data stream. */
    SwfDataStream* m_stream;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFHEADER_H
