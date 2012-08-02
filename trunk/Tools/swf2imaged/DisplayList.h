#ifndef DISPLAYLIST_H
#define DISPLAYLIST_H

#include <QMap>
#include <QXmlStreamWriter>
#include "SwfTypes.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SwfFile;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Display list object. */
class DisplayList
{
  public:

    DisplayList(SwfFile* file);
   ~DisplayList();

  public:

    /*! Display data struct. */
    struct Data
    {
      Data() : characterId(0), color(Qt::white), spriteFrameIndex(0), displayList(NULL) {}
     ~Data() { if (NULL != displayList) delete displayList; displayList = NULL; }

      quint16 characterId;        /*!< Character id. */
      Matrix matrix;              /*!< Transformation matrix. */
      QColor color;               /*!< Color. */
      int spriteFrameIndex;       /*!< Current sprite frame index. Only used if current character is sprite. */
      DisplayList* displayList;   /*!< Pointer to local display list if any. Used by sprites. */
    };

  public:

    /* Places/Modifies the object at a given depth.
       @param depth        Depth at which the object should be placed/modifed.
       @param characterId  Id of the object. NULL if none.
       @param matrix       Transformation matrix for an object. NULL if none.
       @param color        Color transformation for an object. NULL if none.
     */
    void placeObject(quint16 depth, const quint16* characterId, const Matrix* matrix, const ColorTransform* colorTransform);
    /* Removes object at a given depth.
       @param depth       Depth at which object is to be removed.
       @param characterId Id of the object to be removed. If set to 0, any character at given depth will be removed.
     */
    void removeObject(quint16 depth, quint16 characterId = 0);
    /* Serializes current display list state. */
    void serialize(const Matrix& transform, QXmlStreamWriter& stream);

  private:

    /*! Current display list accessed by depth. */
    QMap<quint16, Data> m_displayList;
    /*! Owner SWF file. */
    SwfFile* m_file;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // DISPLAYLIST_H
