#include "SwfParser.h"
#include "SwfFile.h"
#include "ResourceManager.h"
#include <QTimer>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDir>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define VERSION 0.1
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfParser::SwfParser(int argc, char *argv[]) : QApplication(argc, argv),
                                               m_resourceManager(NULL),
                                               m_globalScaleFactor(1.0f)
{
  // process command-line
  QStringList args = QCoreApplication::arguments();
  for (int i = 0; i < args.size(); )
  {
    // check if input image file switch
    if (("--i" == args[i]) && (i + 1 < args.size()))
    {
      m_inputFileNames.append(QDir::fromNativeSeparators(args[i + 1]));
      i += 2;
    }
    // check if output file switch
    else if (("--o" == args[i]) && (i + 1 < args.size()))
    {
      m_outputFileName = QDir::fromNativeSeparators(args[i + 1]);
      i += 2;
    }
    // check if material/image base name switch
    else if (("--base-name" == args[i]) && (i + 1 < args.size()))
    {
      m_materialImageBaseName = args[i + 1];
      i += 2;
    }
    // check if global scale factor switch
    else if (("--scale" == args[i]) && (i + 1 < args.size()))
    {
      m_globalScaleFactor = args[i + 1].toFloat();
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
  if (m_materialImageBaseName.isEmpty() || m_outputFileName.isEmpty())
  {
    // error!
    printSyntax();
    exit(1);
    return;
  }

  // retrieve output dir
  QString outputDir = m_outputFileName.section("/", 0, -2);

  // create resource manager
  m_resourceManager = new ResourceManager(m_materialImageBaseName, outputDir, m_globalScaleFactor, this);
  if (NULL == m_resourceManager)
  {
    // error!
    qCritical() << "Could not create resource manager!";
    exit(3);
    return;
  }

  // go thru all input files
  foreach(const QString& inputFileName, m_inputFileNames)
  {
    // create SWF file parser
    SwfFile* swfFile = new SwfFile(m_globalScaleFactor, this);

    // process it
    if (swfFile->process(inputFileName))
    {
      // add to queue
      m_parsedFiles.append(swfFile);
    }
  }

  // serialize
  QString outputXml;
  QXmlStreamWriter output(&outputXml);
  output.setAutoFormatting(true);

  // resource manager
  if ( ! m_resourceManager->serialize(output))
  {
    // error!
    qCritical() << "Could not serialize resource manager!";
    exit(1);
    return;
  }

  foreach (SwfFile* file, m_parsedFiles)
  {
    // serialize
    if ( ! file->serialize(output))
    {
      // error!
      qCritical() << "Could not serialize file!";
      exit(1);
      return;
    }
  }

  // save
  if ( ! m_resourceManager->saveAssets())
  {
    // error!
    exit(2);
    return;
  }

  QFile outputFile(m_outputFileName);
  if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    // error!
    qCritical() << "Could not open file for writting" << m_outputFileName;
    exit(2);
    return;
  }

  QTextStream out(&outputFile);
  out << outputXml;

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
  qDebug() << "swf2imaged --i <filename> --o <filename> --base-name <string> [--scale <value>]";
  qDebug() << "";
  qDebug() << "--i          Full path to input SWF file. It is allowed to specify multiple input files.";
  qDebug() << "--o          Full path to output XML file.";
  qDebug() << "--base-name  Base name for auto-generated material and image names.";
  qDebug() << "--scale      [Optional] Global scale factor. Default 1.0.";
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
