#ifndef SWFFILE_H
#define SWFFILE_H

#include <QObject>
#include <QList>
#include <QXmlStreamWriter>
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

    SwfFile(QObject* parent = NULL);
   ~SwfFile();

    /* Process the file with a given name. */
    bool process(const QString& fileName);
    /* Serializes into EGE XML. */
    bool serialize(QXmlStreamWriter& stream);
    /* Returns dictionary. */
    Dictionary& dictionary();

  private:

    /*! File header. */
    SwfHeader* m_header;
    /*! List of all valid tags. */
    QList<SwfTag*> m_tags;
    /*! Dictionary. */
    Dictionary m_dictionary;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFFILE_H
