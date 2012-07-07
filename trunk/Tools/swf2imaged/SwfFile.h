#ifndef SWFFILE_H
#define SWFFILE_H

#include <QObject>
#include <QList>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SwfHeader;
class SwfTag;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief This class represents entire SWF file with all its content. */
class SwfFile : QObject
{
  Q_OBJECT

  public:

    SwfFile(QObject* parent = NULL);
   ~SwfFile();

    /* Process the file with a given name. */
    bool process(const QString& fileName);

  private:

    /*! File header. */
    SwfHeader* m_header;
    /*! List of all valid tags. */
    QList<SwfTag*> m_tags;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFFILE_H
