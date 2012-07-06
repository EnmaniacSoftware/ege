#include "SwfFile.h"
#include "SwfHeader.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfFile::SwfFile(QObject* parent) : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfFile::~SwfFile()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Process the file with a given name. */
bool SwfFile::process(const QString &fileName)
{
  // open input file
  QFile file(fileName);
  if ( ! file.open(QIODevice::ReadOnly))
  {
    // error!
    qCritical() << "Unable to open file" << fileName;
    return false;
  }

  // prepare data steam
  SwfDataStream input(&file);

  // set proper byte ordering
  input.setByteOrder(QDataStream::LittleEndian);

  // create and read header
  m_header = new SwfHeader();
  if ( ! m_header->read(input))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------