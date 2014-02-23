#ifndef SWFPARSER_H
#define SWFPARSER_H

#include <QtGui/QApplication>
#include <QString>
#include <QMap>
#include <QList>
#include <QXmlStreamReader>

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

    /*! Animation data struct. */
    struct AnimationData
    {
      QString name;                                     /*!< Animation name. */
      QString inputFilePath;                            /*!< Path to input animation file (ie. SWF). */
      SwfFile* parsedFile;                              /*!< Parsed input animation file data. NULL if not parsed yet. */
      QMap<QString, QString> sequences;                 /*!< List of sequences to generate. */
    };

    typedef QList<AnimationData> AnimationDataList;

    /*! Animation group data struct. */
    struct AnimationGroupData
    {
      float scale;                                      /*!< Scale factor. */
      QString resourcesBaseName;                        /*!< Base name for resources. */
      QString name;                                     /*!< Animation group name. */
      QString outputLocation;                           /*!< Output location. */
      QString atlasTexture;                             /*!< Atlas texture name for assets. Empty if no atlas should be generated for assets. */

      AnimationDataList animationDataList;              /*!< List of defined animations. */
    };

    typedef QList<AnimationGroupData> AnimationGroupDataList;

  private:

    /* Prints syntax to standard output. */
    void printSyntax() const;
    /* Prints header to standard output. */
    void printHeader() const;
    /* Processes given input file. */
    bool processInputXML();
    /* Processes animation group XML tag. */
    bool processAnimationGroupTag(QXmlStreamReader& input);
    /* Processes animation XML tag. */
    bool processAnimationTag(QXmlStreamReader& input, AnimationGroupData& group);
    /* Processes atlas texture XML tag. */
    bool processAtlasTextureTag(QXmlStreamReader& input);
    /* Generates output for current data. */
    bool generateData();

  private:

    /*! Input file name. */
    QString m_inputFileName;
    /*! Resource manager. */
    ResourceManager* m_resourceManager;
    /*! List of all defined animations groups. */
    AnimationGroupDataList m_animationGroupDataList;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFPARSER_H
