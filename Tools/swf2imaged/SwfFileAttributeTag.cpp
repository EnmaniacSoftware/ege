#include "SwfFileAttributeTag.h"
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfFileAttributeTag::SwfFileAttributeTag() : SwfTag(),
                                             m_useDirectBlit(false),
                                             m_useGpu(false),
                                             m_hasMetadata(false),
                                             m_actionScript(false),
                                             m_useNetwork(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfFileAttributeTag::~SwfFileAttributeTag()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SwfTag override. Reads data from file. */
bool SwfFileAttributeTag::read(SwfDataStream& data)
{
  // read data
  qint32 reserved = data.readBits(1, false);
  Q_ASSERT(0 == reserved);

  m_useDirectBlit  = (1 == data.readBits(1, false));
  m_useGpu         = (1 == data.readBits(1, false));
  m_hasMetadata    = (1 == data.readBits(1, false));
  m_actionScript   = (1 == data.readBits(1, false));
  
  reserved = data.readBits(2, false);
  Q_ASSERT(0 == reserved);

  m_useNetwork = (1 == data.readBits(1, false));

  reserved = data.readBits(24, false);
  Q_ASSERT(0 == reserved);

  return QDataStream::Ok == data.status();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------