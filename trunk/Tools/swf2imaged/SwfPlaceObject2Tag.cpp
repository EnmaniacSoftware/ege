#include "SwfPlaceObject2Tag.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfPlaceObject2Tag::SwfPlaceObject2Tag() : SwfTag(),
                                           m_depth(0),
                                           m_characterId(0),
                                           m_clipDepth(0),
                                           m_hasClipActions(false),
                                           m_hasClipDepth(false),
                                           m_hasName(false),
                                           m_hasRatio(false),
                                           m_hasColorTransform(false),
                                           m_hasMatrix(false),
                                           m_hasCharacter(false),
                                           m_move(false)
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
  m_hasClipActions     = (1 == data.readBits(1, false));
  m_hasClipDepth       = (1 == data.readBits(1, false));
  m_hasName            = (1 == data.readBits(1, false));
  m_hasRatio           = (1 == data.readBits(1, false));
  m_hasColorTransform  = (1 == data.readBits(1, false));
  m_hasMatrix          = (1 == data.readBits(1, false));
  m_hasCharacter       = (1 == data.readBits(1, false));
  m_move               = (1 == data.readBits(1, false));

  data >> m_depth;

  if (m_hasCharacter)
  {
    data >> m_characterId;
  }

  if (m_hasMatrix)
  {
    data >> m_matrix;
  }

  if (m_hasColorTransform)
  {
    Q_ASSERT("Implement");
  }

  if (m_hasRatio)
  {
    data >> m_ratio;
  }

  if (m_hasName)
  {
    Q_ASSERT("Implement");
  }

  if (m_hasClipDepth)
  {
    data >> m_clipDepth;
  }

  if (m_hasClipActions)
  {
    Q_ASSERT("Implement");
  }

  return QDataStream::Ok == data.status();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns depth character should be placed on. */
quint16 SwfPlaceObject2Tag::depth() const
{
  return m_depth;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns character id. */
quint16 SwfPlaceObject2Tag::characterId() const
{
  return m_characterId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns transformation matrix. */
const Matrix& SwfPlaceObject2Tag::matrix() const
{
  return m_matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if transformation matrix is defined. */
bool SwfPlaceObject2Tag::hasMatrix() const
{
  return m_hasMatrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if transformation matrix is defined. */
bool SwfPlaceObject2Tag::hasColorTransformation() const
{
  return m_hasColorTransform;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if character ID is defined. */
bool SwfPlaceObject2Tag::hasCharacterId() const
{
  return m_hasCharacter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if move flag is defined. */
bool SwfPlaceObject2Tag::shallMove() const
{
  return m_move;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
