#include "SwfHeader.h"
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfHeader::SwfHeader() :  m_version(0),
                          m_fileSize(0),
                          m_fps(0),
                          m_frameCount(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfHeader::~SwfHeader()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads data from file. */
bool SwfHeader::read(SwfDataStream& data)
{
  quint8 signature[3];

  // check signature
  data >> signature[0];
  data >> signature[1];
  data >> signature[2];

  if ((('F' != signature[0]) && 'C' != signature[0]) || ('W' != signature[1]) || ('S' != signature[2]))
  {
    // error!
    qWarning() << "Not a valid SWF file.";
    return false;
  }
  else if ('C' == signature[0])
  {
    // error!
    qWarning() << "Compressed SWF files are not supported.";
    return false;
  }

  // read rest of the header
  data >> m_version;
  data >> m_fileSize;

  // frame size is stored explicitly in x-max and y-max
  QRect rect;
  data >> rect;
  m_frameSize = QSize(rect.right(), rect.bottom());

  m_fps = data.readFP8();
  data >> m_frameCount;

  return QDataStream::Ok == data.status();
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
