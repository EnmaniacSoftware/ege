#include "SwfHeader.h"
#include <QFile>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfHeader::SwfHeader() :  m_version(0),
                          m_fileSize(0),
                          m_fps(0),
                          m_frameCount(0),
                          m_stream(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfHeader::~SwfHeader()
{
  if (m_stream)
  {
    delete m_stream;
    m_stream = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes header of a given file. 
    @param fileName  Path to SWF file to process.
    @return  Returns SWF data stream if given file is valid. Otherwise, NULL.
 */
SwfDataStream* SwfHeader::process(const QString &fileName)
{
  quint8 signature[3];

  // open input file
  QFile file(fileName);
  if ( ! file.open(QIODevice::ReadOnly))
  {
    // error!
    qCritical() << "Unable to open file" << fileName;
    return NULL;
  }

  // prepare data steam
  m_stream = new SwfDataStream(&file);
  if (NULL == m_stream)
  {
    // error!
    return NULL;
  }

  // check signature
  *m_stream >> signature[0];
  *m_stream >> signature[1];
  *m_stream >> signature[2];

  bool compressed = false;

  if ((('F' != signature[0]) && 'C' != signature[0]) || ('W' != signature[1]) || ('S' != signature[2]))
  {
    // error!
    qWarning() << "Not a valid SWF file.";
    return false;
  }
  else if ('C' == signature[0])
  {
    compressed = true;    
  }

  // read rest of the header
  *m_stream >> m_version;
  *m_stream >> m_fileSize;

  // check if compressed
  if (compressed)
  {    
    // decompress
    QByteArray compressedData = m_stream->device()->readAll();

    compressedData.prepend((m_fileSize & 0x000000ff));
    compressedData.prepend((m_fileSize & 0x0000ff00) >> 8);
    compressedData.prepend((m_fileSize & 0x00ff0000) >> 16);
    compressedData.prepend((m_fileSize & 0xff000000) >> 24);

    compressedData = qUncompress(compressedData);

    // delete old stream
    delete m_stream;

    // create new stream
    m_stream = new SwfDataStream(compressedData);
    if (NULL == m_stream)
    {
      // error!
      return NULL;
    }
  }

  // frame size is stored explicitly in x-max and y-max
  QRect rect;
  *m_stream >> rect;
  m_frameSize = QSize(rect.right(), rect.bottom());

  m_fps = m_stream->readFP8();
  *m_stream >> m_frameCount;

  // check if error
  if (QDataStream::Ok != m_stream->status())
  {
    // error!
    return NULL;
  }

  return m_stream;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns FPS. */
float SwfHeader::fps() const
{
  return m_fps;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns frame size. */
const QSize& SwfHeader::frameSize() const
{
  return m_frameSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns frame count. */
quint16 SwfHeader::frameCount() const
{
  return m_frameCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
