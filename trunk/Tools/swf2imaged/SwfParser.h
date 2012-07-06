#ifndef SWFPARSER_H
#define SWFPARSER_H

#include <QtGui/QApplication>
#include <QString>
#include <QStringList>
#include <QList>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SwfFile;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SwfParser : public QApplication
{
  Q_OBJECT

  public:

    SwfParser(int argc, char *argv[]);
   ~SwfParser();

  private:


  private slots:

    /* Starts processing. */
    void onStart();

  private:

    /*! Input SWF file names. */
    QStringList m_inputFileNames;
    /*! Output XML file name. */
    QString m_outputFileName;
    /*! List of parsed SWF files. */
    QList<SwfFile*> m_parsedFiles;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFPARSER_H
