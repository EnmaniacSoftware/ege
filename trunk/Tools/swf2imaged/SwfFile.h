#ifndef SWFFILE_H
#define SWFFILE_H

#include <QObject>
#include <QList>
#include <QXmlStreamWriter>
#include <QMap>
#include <QString>
#include "SwfTypes.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SwfHeader;
class SwfTag;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief This class represents entire SWF file with all its content. */
class SwfFile : public QObject
{
  Q_OBJECT

  public:

    SwfFile(const QString& animationName, float scale, const QMap<QString, QString>& sequences, QObject* parent = NULL);
   ~SwfFile();

    /* Process the file with a given name. */
    bool process(const QString& fileName);
    /* Serializes into EGE XML. */
    bool serialize(QXmlStreamWriter& stream);
    /* Returns dictionary. */
    Dictionary& dictionary();

  private:

    /* Serializes objects section into EGE XML. */
    bool serializeObjectsSection(QXmlStreamWriter& stream);
    /* Serializes sequences section into EGE XML. */
    bool serializeSequencesSection(QXmlStreamWriter& stream);
    /* Serializes frames into EGE XML. */
    bool serializeFrames(QXmlStreamWriter& stream);

  private:

    /*! File header. */
    SwfHeader* m_header;
    /*! List of all valid tags. */
    QList<SwfTag*> m_tags;
    /*! Dictionary. */
    Dictionary m_dictionary;
    /*! Scale factor. */
    float m_scale;
    /*! Animation name.   */
    QString m_animationName;
    /*! External sequences to store. */
    QMap<QString, QString> m_sequences;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFFILE_H
