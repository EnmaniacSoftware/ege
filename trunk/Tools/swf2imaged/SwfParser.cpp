#include "SwfParser.h"
#include "SwfFile.h"
#include "ResourceManager.h"
#include <QTimer>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDebug>

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
      m_inputFileNames.append(args[i + 1]);
      i += 2;
    }
    // check if output file switch
    else if (("--o" == args[i]) && (i + 1 < args.size()))
    {
      m_outputFileName = args[i + 1];
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
  // create resource manager
  m_resourceManager = new ResourceManager(this);
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
    SwfFile* swfFile = new SwfFile(this);

    // process it
    if (swfFile->process((inputFileName)))
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
