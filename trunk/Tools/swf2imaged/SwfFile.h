#ifndef SWFFILE_H
#define SWFFILE_H

#include <QObject>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QFile;
class SwfHeader;
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
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFFILE_H
