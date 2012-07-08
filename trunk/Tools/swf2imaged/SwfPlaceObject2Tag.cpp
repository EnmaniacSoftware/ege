#include "SwfPlaceObject2Tag.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfPlaceObject2Tag::SwfPlaceObject2Tag() : SwfTag(),
                                           m_depth(0),
                                           m_characterId(0),
                                           m_clipDepth(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfPlaceObject2Tag::~SwfPlaceObject2Tag()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SwfTag override. Reads data from file. */
bool SwfPlaceObject2Tag::read(SwfDataStream& data)
{
  bool hasClipActions     = (1 == data.readBits(1, false));
  bool hasClipDepth       = (1 == data.readBits(1, false));
  bool hasName            = (1 == data.readBits(1, false));
  bool hasRatio           = (1 == data.readBits(1, false));
  bool hasColorTransform  = (1 == data.readBits(1, false));
  bool hasMatrix          = (1 == data.readBits(1, false));
  bool hasCharacter       = (1 == data.readBits(1, false));
  bool move               = (1 == data.readBits(1, false));

  Q_UNUSED(move);

  data >> m_depth;

  if (hasCharacter)
  {
    data >> m_characterId;
  }

  if (hasMatrix)
  {
    data >> m_matrix;
  }

  if (hasColorTransform)
  {
    Q_ASSERT("Implement");
  }

  if (hasRatio)
  {
    data >> m_ratio;
  }

  if (hasName)
  {
    Q_ASSERT("Implement");
  }

  if (hasClipDepth)
  {
    data >> m_clipDepth;
  }

  if (hasClipActions)
  {
    Q_ASSERT("Implement");
  }

  return QDataStream::Ok == data.status();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------