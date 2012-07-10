#ifndef SWFPARSER_H
#define SWFPARSER_H

#include <QtGui/QApplication>
#include <QString>
#include <QStringList>
#include <QList>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SwfFile;
class ResourceManager;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Swf parser object. */
class SwfParser : public QApplication
{
  Q_OBJECT

  public:

    SwfParser(int argc, char *argv[]);
   ~SwfParser();

    /* Returns resource manager. */
    ResourceManager* resourceManager() const;

  private slots:

    /* Starts processing. */
    void onStart();

  private:

    /* Prints syntax to standard output. */
    void printSyntax() const;
    /* Prints header to standard output. */
    void printHeader() const;

  private:

    /*! Input SWF file names. */
    QStringList m_inputFileNames;
    /*! Output XML file name. */
    QString m_outputFileName;
    /*! Base name for materials and images. */
    QString m_materialImageBaseName;
    /*! List of parsed SWF files. */
    QList<SwfFile*> m_parsedFiles;
    /*! Resource manager. */
    ResourceManager* m_resourceManager;
    /*! Global scale factor. */
    float m_globalScaleFactor;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFPARSER_H
