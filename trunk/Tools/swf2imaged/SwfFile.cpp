#include "SwfFile.h"
#include "SwfHeader.h"
#include "SwfTag.h"
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
  qDeleteAll(m_tags);
  m_tags.clear();
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

  // create and read header
  m_header = new SwfHeader();
  if ( ! m_header->read(input))
  {
    // error!
    return false;
  }

  // read rest of the file (tags)
  do
  {
    // process tag
    SwfTag* tag = SwfTag::ProcessTag(input);
    if (tag)
    {
      // add to pool
      m_tags.append(tag);
    }

  } while ((QDataStream::Ok == input.status()) && !input.atEnd());

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------