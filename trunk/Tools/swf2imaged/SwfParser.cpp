#include "SwfParser.h"
#include "SwfFile.h"
#include "ResourceManager.h"
#include <QTimer>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDir>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define VERSION 0.11
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfParser::SwfParser(int argc, char *argv[]) : QApplication(argc, argv),
                                               m_resourceManager(NULL)
{
  // process command-line
  QStringList args = QCoreApplication::arguments();
  for (int i = 0; i < args.size(); )
  {
    // check if input image file switch
    if (("--i" == args[i]) && (i + 1 < args.size()))
    {
      m_inputFileName = QDir::fromNativeSeparators(args[i + 1]);
      i += 2;
    }
    else
    {
      // go to next
      ++i;
    }
  }

  // schedule process startup
  QTimer::singleShot(100, this, SLOT(onStart()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfParser::~SwfParser()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts processing. */
void SwfParser::onStart()
{
  // show header
  printHeader();

  // check if required data is missing
  if (m_inputFileName.isEmpty())
  {
    // error!
    printSyntax();
    exit(1);
    return;
  }

  // create resource manager
  m_resourceManager = new ResourceManager(this);
  if (NULL == m_resourceManager)
  {
    // error!
    qCritical() << "Could not create resource manager!";
    exit(1);
    return;
  }

  // process input XML
  if ( ! processInputXML())
  {
    // error!
    exit(1);
    return;
  }

  // done
  exit(0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns resource manager. */
ResourceManager* SwfParser::resourceManager() const
{
  return m_resourceManager;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints syntax to standard output. */
void SwfParser::printSyntax() const
{
  qDebug() << "Usage syntax:";
  qDebug() << "swf2imaged --i <filename>";
  qDebug() << "";
  qDebug() << "--i  Full path to input XML file. ";
  qDebug() << "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints header to standard output. */
void SwfParser::printHeader() const
{
  QString version;
  version.setNum(VERSION, 'f', 2);
  QByteArray versionData = version.toAscii();

  qDebug() << "";
  qDebug() << "SWF To Imaged Animation converter, version" << version.toAscii();
  qDebug() << "Albert Banaszkiewicz, Little Bee Studios Ltd., 2011-2012";
  qDebug() << "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes given input file. */
bool SwfParser::processInputXML()
{
  QFile file(m_inputFileName);
  if ( ! file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    // error!
    qCritical() << "Could not open input XML file:" << m_inputFileName;
    return false;
  }

  QXmlStreamReader input(&file);

  // process children
  while ( ! input.atEnd())
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        if ("animation-group" == input.name())
        {
          if ( ! processAnimationGroupTag(input))
          {
            // error!
            return false;
          }
        }
        else if ("atlas-texture" == input.name())
        {
          if ( ! processAtlasTextureTag(input))
          {
            // error!
            return false;
          }
        }
        break;
    }
  }

  // generate
  if ( ! generateData())
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes animation group XML tag. */
bool SwfParser::processAnimationGroupTag(QXmlStreamReader& input)
{
  bool ok;

  AnimationGroupData group;

  group.name                = input.attributes().value("name").toString();
  group.resourcesBaseName   = input.attributes().value("resources-base-name").toString();
  group.outputLocation      = input.attributes().value("output-dir").toString();
  group.atlasTexture        = input.attributes().value("atlas-texture").toString();
  QString scale             = input.attributes().value("scale").toString();

  // check if required data is missing
  if (group.name.isEmpty())
  {
    // error!
    qCritical() << "Missing data for animation-group tag.";
    return false;
  }

  // check if default values needs to be supplied
  if (scale.isEmpty())
  {
    scale = "1.0";
  }

  // store global animation data
  group.scale = scale.toFloat(&ok);
  if ( ! ok)
  {
    // error!
    qCritical() << "Error converting scale value!";
    return false;
  }

  // process children
  bool done = false;
  while ( ! input.atEnd() && ! done)
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        if ("animation" == input.name())
        {
          if ( ! processAnimationTag(input, group))
          {
            // error!
            return false;
          }
        }
        break;

      case QXmlStreamReader::EndElement:

        if ("animation-group" == input.name())
        {
          // add to pool
          m_animationGroupDataList << group;

          // done
          done = true;
        }
        break;
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Processes animation XML tag. */
bool SwfParser::processAnimationTag(QXmlStreamReader& input, AnimationGroupData& group)
{
  AnimationData data;

  data.name           = input.attributes().value("name").toString();
  data.inputFilePath  = input.attributes().value("path").toString();
  data.parsedFile     = NULL;

  // check if required data is missing
  if (data.inputFilePath.isEmpty())
  {
    // error!
    qCritical() << "Missing data for animation tag.";
    return false;
  }

  // check if default values needs to be supplied
  if (data.name.isEmpty())
  {
    // store file name (without extension)
    data.name = data.inputFilePath.section("/", -1);
    data.name = data.name.section(".", 0, 0);
  }

  // process children
  bool done = false;
  while ( ! input.atEnd() && ! done)
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        if ("sequence" == input.name())
        {
          QString seqName   = input.attributes().value("name").toString();
          QString seqFrames = input.attributes().value("frames").toString();

          if ( ! seqName.isEmpty() && ! seqFrames.isEmpty())
          {
            // check if defined already
            if (data.sequences.contains(seqName))
            {
              qWarning() << "Sequence" << seqName << "already exists for animation" << data.name;
            }
            else
            {
              // add to pool
              data.sequences[seqName] = seqFrames;
            }
          }
          else
          {
            qWarning() << "Sequence declaration error for animation" << data.name;
          }
        }
        break;

      case QXmlStreamReader::EndElement:

        if ("animation" == input.name())
        {
          // add to pool
          group.animationDataList << data;

          // done
          done = true;
        }
        break;
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates output for current data. */
bool SwfParser::generateData()
{
  // go thru all animation groups data
  for (int i = 0; i < m_animationGroupDataList.count(); ++i)
  {
    AnimationGroupData& group = m_animationGroupDataList[i];

    // start resource session for current group
    if ( ! m_resourceManager->beginSession(group.resourcesBaseName, group.scale, group.outputLocation, group.atlasTexture))
    {
      // error!
      qCritical() << "Could not start resource session!";
      return false;
    }

    // go thru all animations within current group
    for (int j = 0; j < group.animationDataList.count(); ++j)
    {
      AnimationData& animData = group.animationDataList[j];

      // create SWF file parser
      animData.parsedFile = new SwfFile(animData.name, group.scale, animData.sequences, this);
      if (NULL == animData.parsedFile)
      {
        // error!
        qCritical() << "Could not create parsed file for animation" << animData.name;
        return false;
      }

      // process it
      if ( ! animData.parsedFile->process(animData.inputFilePath))
      {
        // error!
        return false;
      }
    }
  }

  // serialize
  QString outputXml;
  QXmlStreamWriter output(&outputXml);
  output.setAutoFormatting(true);

  // go thru all animation groups data
  for (int i = 0; i < m_animationGroupDataList.count(); ++i)
  {
    AnimationGroupData& group = m_animationGroupDataList[i];

    // start resource session for current group
    if ( ! m_resourceManager->beginSession(group.resourcesBaseName, group.scale, group.outputLocation, group.atlasTexture))
    {
      // error!
      qCritical() << "Could not start resource session!";
      return false;
    }

    // save assets first
    if ( ! m_resourceManager->saveAssets())
    {
      // error!
      return false;
    }

    output.writeStartElement("resources");
    output.writeStartElement("group");
    output.writeAttribute("name", group.name);

    // resource manager
    if ( ! m_resourceManager->serialize(output))
    {
      // error!
      qCritical() << "Could not serialize resource manager!";
      return false;
    }

    // go thru all animations within current group
    for (int j = 0; j < group.animationDataList.count(); ++j)
    {
      AnimationData& animData = group.animationDataList[j];

      // serialize
      if ( ! animData.parsedFile->serialize(output))
      {
        // error!
        qCritical() << "Could not serialize file!";
        return false;
      }
    }

    output.writeEndElement();
    output.writeEndElement();

    QFile outputFile(group.outputLocation + "/" + group.name + ".xml");
    if ( ! outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      // error!
      qCritical() << "Could not open file for writting" << outputFile.fileName();
      return false;
    }

    QTextStream out(&outputFile);
    out << outputXml;

    outputFile.close();
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes atlas texture XML tag. */
bool SwfParser::processAtlasTextureTag(QXmlStreamReader& input)
{
  QMap<QString, QString> props;

  // name
  QString value = input.attributes().value("name").toString();
  if (value.isEmpty())
  {
    // no name, no texture
    return true;
  }

  props["name"] = value;

  // texture-size
  value = input.attributes().value("texture-size").toString();
  if ( ! value.isEmpty())
  {
    props["texture-size"] = value;
  }

  // root
  value = input.attributes().value("root").toString();
  if ( ! value.isEmpty())
  {
    props["root"] = value;
  }

  // texture-filters
  value = input.attributes().value("texture-filters").toString();
  if ( ! value.isEmpty())
  {
    props["texture-filters"] = value;
  }

  // texture-format
  value = input.attributes().value("texture-format").toString();
  if ( ! value.isEmpty())
  {
    props["texture-format"] = value;
  }

  // texture-image
  value = input.attributes().value("texture-image").toString();
  if ( ! value.isEmpty())
  {
    props["texture-image"] = value;
  }

  // alpha-premultiply
  value = input.attributes().value("alpha-premultiply").toString();
  if ( ! value.isEmpty())
  {
    props["alpha-premultiply"] = value;
  }

  // add to resource manager
  return m_resourceManager->addAtlasTextureDefinition(props);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
