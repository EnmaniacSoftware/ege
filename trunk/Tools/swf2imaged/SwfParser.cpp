#include "SwfParser.h"
#include "SwfFile.h"
#include <QTimer>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfParser::SwfParser(int argc, char *argv[]) : QApplication(argc, argv)
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

  // done
  exit(0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------