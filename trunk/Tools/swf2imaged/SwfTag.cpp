#include "SwfTag.h"
#include "SwfFileAttributeTag.h"
#include "SwfSetBackgroundColorTag.h"
#include "SwfDefineBitsJpeg3Tag.h"
#include "SwfDefineShapeTag.h"
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define TAGID_DEFINE_SHAPE          (2)
#define TAGID_SET_BACKGROUND_COLOR  (9)
#define TAGID_DEFINE_BITS_JPEG3     (35)
#define TAGID_FILE_ATTRIBUTE        (69)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfTag::SwfTag()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfTag::~SwfTag()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates SWFTag of a proper type. */
SwfTag* SwfTag::ProcessTag(SwfDataStream& data)
{
  SwfTag* tag = NULL;

  // read tag and length (2-byte)
  quint16 idAndLength;
  data >> idAndLength;

  // decompose tag ID (upper 10-bits)
  quint16 tagId = (idAndLength & 0xFFC0) >> 6;

  // decompose tag length (lower 6-bits)
  quint32 tagLength = idAndLength & 0x003F;

  // check if long tag
  if (0x3f == tagLength)
  {
    // read real length
    data >> tagLength;
  }

  // create tag according to ID
  switch (tagId)
  {
    case TAGID_DEFINE_SHAPE:          tag = new SwfDefineShapeTag(); break;
    case TAGID_SET_BACKGROUND_COLOR:  tag = new SwfSetBackgroundColorTag(); break;
    case TAGID_DEFINE_BITS_JPEG3:     tag = new SwfDefineBitsJpeg3Tag(); break;
    case TAGID_FILE_ATTRIBUTE:        tag = new SwfFileAttributeTag(); break;

    default:

      qDebug() << "Skipping tag" << tagId << "length" << tagLength;

      // skip
      if (tagLength != data.skipRawData(tagLength))
      {
        // error!
        return NULL;
      }
      break;
  }

  // check if any tag created
  if (tag)
  {
    // set data
    tag->m_id     = tagId;
    tag->m_length = tagLength;
    
    // read tag data
    if (!tag->read(data))
    {
      // error!
      delete tag;
      tag = NULL;
    }
  }

  return tag;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------