#include "DisplayList.h"
#include "SwfDefineSpriteTag.h"
#include "SwfFile.h"
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DisplayList::DisplayList(SwfFile* file) : m_file(file)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DisplayList::~DisplayList()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Places/Modifies the object at a given depth.
    @param depth        Depth at which the object should be placed/modifed.
    @param characterId  Id of the object. If set to 0, existing object is being modified. Otherwise, object is added.
    @param matrix       Transformation matrix for an object.
    @param color        Color for an object.
 */
void DisplayList::placeObject(quint16 depth, const quint16* characterId, const Matrix* matrix, const ColorTransform* colorTransform)
{
  // add or modify character at given depth
  DisplayList::Data& data = m_displayList[depth];

  // create local display list if necessary
  if (NULL == data.displayList)
  {
    data.displayList = new DisplayList(m_file);
    Q_ASSERT(NULL != data.displayList);
  }

  if (NULL != characterId)
  {
    data.characterId = *characterId;

    // reset sprite data
    data.spriteFrameIndex = 0;
  }

  if (NULL != matrix)
  {
    data.matrix = *matrix;
  }

  if (NULL != colorTransform)
  {
    data.colorTransform = *colorTransform;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes object at a given depth.
    @param depth       Depth at which object is to be removed.
    @param characterId Id of the object to be removed. If set to 0, any character at given depth will be removed.
 */
void DisplayList::removeObject(quint16 depth, quint16 characterId)
{
  // remove given character at given depth
  if (0 != characterId)
  {
    if (m_displayList.contains(depth) && m_displayList[depth].characterId == characterId)
    {
      // remove character at given depth
      m_displayList.remove(depth);
    }
    else
    {
      qWarning() << "Could not remove character" << characterId << "at depth" << depth << "No such character and/or depth!";
    }
  }
  else
  {
    // remove character at given depth
    m_displayList.remove(depth);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Serializes current display list state. */
void DisplayList::serialize(const Matrix& transform, const ColorTransform& colorTransform, QXmlStreamWriter& stream)
{
  for (QMap<quint16, Data>::iterator it = m_displayList.begin(); it != m_displayList.end(); ++it)
  {
  //  const quint16& depth = it.key();
    Data& data = it.value();

    // calculate final transform matrix
    Matrix finalMT = Matrix::Concatenate(transform, data.matrix);

    // calculate final color transform
    ColorTransform finalCT = ColorTransform::Concatenate(colorTransform, data.colorTransform);

    // get tag corresponding to a character to be placed
    QObject* characterTag = m_file->dictionary().value(data.characterId, NULL);
    Q_ASSERT(NULL != characterTag);

    // check if sprite
    SwfDefineSpriteTag* defineSpriteTag = qobject_cast<SwfDefineSpriteTag*>(characterTag);
    if (NULL != defineSpriteTag)
    {
      // update sprite time line
      defineSpriteTag->update(data);

      // serialize sprite display list
      data.displayList->serialize(finalMT, finalCT, stream);
    }
    else
    {
      // normal element
      stream.writeStartElement("action");

      // TAGE - probably should be taken from some object ?!
      QColor color = Qt::white;
      color = finalCT.transform(color);

      stream.writeAttribute("object-id", QString::number(data.characterId));
    //  stream.writeAttribute("queue", QString::number(depth));
      stream.writeAttribute("translate", QString("%1 %2").arg(T2P(finalMT.translateX * m_file->scale())).arg(T2P(finalMT.translateY * m_file->scale())));
      stream.writeAttribute("scale", QString("%1 %2").arg(finalMT.scaleX).arg(finalMT.scaleY));
      stream.writeAttribute("skew", QString("%1 %2").arg(finalMT.rotX).arg(finalMT.rotY));
      stream.writeAttribute("color", QString("%1 %2 %3 %4").arg(color.redF()).arg(color.greenF()).arg(color.blueF()).arg(color.alphaF()));

      stream.writeEndElement();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
