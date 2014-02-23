#include "SwfDefineSpriteTag.h"
#include "SwfFile.h"
#include "SwfRemoveObject2Tag.h"
#include "SwfRemoveObjectTag.h"
#include "SwfPlaceObject2Tag.h"
#include "DisplayList.h"
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDefineSpriteTag::SwfDefineSpriteTag() : SwfTag(),
                                           m_characterId(0),
                                           m_frameCount(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDefineSpriteTag::~SwfDefineSpriteTag()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SwfTag override. Reads data from file. */
bool SwfDefineSpriteTag::read(SwfDataStream& data)
{
  data >> m_characterId;
  data >> m_frameCount;

  //qDebug() << "SPRITE" << m_characterId << m_frameCount;

  // process control tags
  // NOTE: last tag is always EndTag
  SwfTag* tag;
  do
  {
    // process tag
    tag = SwfTag::ProcessTag(data, file());
    if (NULL != tag)
    {
      // add to pool
      m_tags.append(tag);
    }

  } while(SWF_TAG_ID_END != tag->id());

  // add to dictionary
  Q_ASSERT( ! file()->dictionary().contains(m_characterId));
  file()->dictionary().insert(m_characterId, this);

  return QDataStream::Ok == data.status();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns the list of tags for a given frame index. */
QList<SwfTag*> SwfDefineSpriteTag::frameTags(quint16 frameIndex) const
{
  Q_ASSERT(frameIndex < m_frameCount);

  QList<SwfTag*> list;

  // go thru all tags and try to serialize given frame index
  foreach (SwfTag* tag, m_tags)
  {
    // check if tag belongs to desired frame
    if (0 == frameIndex)
    {
      // added it list
      list.append(tag);
    }

    // check if end of frame
    if (SWF_TAG_ID_SHOW_FRAME == tag->id())
    {
      --frameIndex;
    }
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns character ID. */
quint16 SwfDefineSpriteTag::characterId() const
{
  return m_characterId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates display data. */
void SwfDefineSpriteTag::update(DisplayList::Data& displayData)
{
  // check if anything to be updated
  if (displayData.spriteFrameIndex < m_frameCount)
  {
    QList<SwfTag*> tags = frameTags(displayData.spriteFrameIndex);

    // go thru all tags
    foreach (const SwfTag* tag, tags)
    {
      if (SWF_TAG_ID_PLACE_OBJECT_2 == tag->id())
      {
        const SwfPlaceObject2Tag* placeTag = qobject_cast<const SwfPlaceObject2Tag*>(tag);

        quint16 characterId = placeTag->characterId();

        displayData.displayList->placeObject(placeTag->depth(), placeTag->hasCharacterId() ? &characterId : NULL,
                                             placeTag->hasMatrix() ? &placeTag->matrix() : NULL,
                                             placeTag->hasColorTransformation() ? &placeTag->colorTransformation() : NULL);
      }
      else if (SWF_TAG_ID_REMOVE_OBJECT == tag->id())
      {
        const SwfRemoveObjectTag* removeTag = qobject_cast<const SwfRemoveObjectTag*>(tag);

        displayData.displayList->removeObject(removeTag->depth(), removeTag->characterId());
      }
      else if (SWF_TAG_ID_REMOVE_OBJECT_2 == tag->id())
      {
        const SwfRemoveObject2Tag* removeTag = qobject_cast<const SwfRemoveObject2Tag*>(tag);

        // remove character at given depth
        displayData.displayList->removeObject(removeTag->depth());
      }
      else if (SWF_TAG_ID_SHOW_FRAME == tag->id())
      {
        // done, go to next frame
        ++displayData.spriteFrameIndex;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

