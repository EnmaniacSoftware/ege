#include "SwfDefineShapeTag.h"
#include "SwfFile.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDefineShapeTag::SwfDefineShapeTag() : SwfTag(),
                                         m_characterId(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfDefineShapeTag::~SwfDefineShapeTag()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SwfTag override. Reads data from file. */
bool SwfDefineShapeTag::read(SwfDataStream& data)
{
  data >> m_characterId;
  data >> m_bounds;

  // read shape (with style)
  m_shape.fillStyles = data.readFillStyleArray(version());
  m_shape.lineStyles = data.readLineStyleArray(version());

  m_shape.numFillBits = data.readBits(4, false);
  m_shape.numLineBits = data.readBits(4, false);

  while (true)
  {
    quint8 type = data.readBits(1, false);
    if (0 == type)
    {
      // non-edge shape record
      // NOTE: possible records:
      //       - ENDSHAPERECORD
      //       - STYLECHANGERECORD
      
      // read 5 control bits
      bool newStateFlag         = (1 == data.readBits(1, false));
      bool lineStyleChangeFlag  = (1 == data.readBits(1, false));
      bool fillStyle1ChangeFlag = (1 == data.readBits(1, false));
      bool fillStyle0ChangeFlag = (1 == data.readBits(1, false));
      bool moveToFlag           = (1 == data.readBits(1, false));

      // check if STYLECHANGERECORD
      if (!newStateFlag && !lineStyleChangeFlag && !fillStyle1ChangeFlag && !fillStyle0ChangeFlag && !moveToFlag)
      {
        // done
        break;
      }

      // process STYLECHANGERECORD
      if (moveToFlag)
      {
        qint8 bits        = data.readBits(5, false);
        qint32 moveDeltaX = data.readBits(bits, true);
        qint32 moveDeltaY = data.readBits(bits, true);

        Q_UNUSED(bits);
        Q_UNUSED(moveDeltaX);
        Q_UNUSED(moveDeltaY);
      }

      if (fillStyle0ChangeFlag)
      {
        quint32 bits = data.readBits(m_shape.numFillBits, false);
        Q_UNUSED(bits);
      }

      if (fillStyle1ChangeFlag)
      {
        quint32 bits = data.readBits(m_shape.numFillBits, false);
        Q_UNUSED(bits);
      }

      if (lineStyleChangeFlag)
      {
        quint32 bits = data.readBits(m_shape.numLineBits, false);
        Q_UNUSED(bits);
      }

      if (newStateFlag)
      {
        // TAGE - does this start new shape ?

        QList<FillStyle> fillStyles = data.readFillStyleArray(version());
        QList<LineStyle> lineStyles = data.readLineStyleArray(version());

        m_shape.numFillBits = data.readBits(4, false);
        m_shape.numLineBits = data.readBits(4, false);

        // add to existing pools
        m_shape.fillStyles << fillStyles;
        m_shape.lineStyles << lineStyles;
      }
    }
    else
    {
      // edge shape record
      // NOTE: possible records:
      //       - STRAIGHTEDGERECORD
      //       - CURVEDEDGERECORD

      // NOTE: There seems to be documentation issue in SWF 10 file specification when describing STRAIGHTEDGERECORD.
      //       Difference definitions are found on page 273 and 138. 
      //       Description on page 138 seems to be correct, though, as long as my test samples are considered.

      bool straightFlag = (1 == data.readBits(1, false));

      quint8 numBits = data.readBits(4, false);

      if (straightFlag)
      {
        // process STRAIGHTEDGERECORD
        bool generalLineFlag = (1 == data.readBits(1, false));
        if (generalLineFlag)
        {
          qint32 deltaX = data.readBits(numBits + 2, true);
          qint32 deltaY = data.readBits(numBits + 2, true);

          Q_UNUSED(deltaX);
          Q_UNUSED(deltaY);
        }
        else
        {
          bool vertLineFlag = (1 == data.readBits(1, false));
          if ( ! vertLineFlag)
          {
            qint32 deltaX = data.readBits(numBits + 2, true);
            Q_UNUSED(deltaX);
          }
          else
          {
            qint32 deltaY = data.readBits(numBits + 2, true);
            Q_UNUSED(deltaY);
          }
        }
      }
      else
      {
        // process CURVEDEDGERECORD
        qint32 controlDeltaX = data.readBits(numBits + 2, true);
        qint32 controlDeltaY = data.readBits(numBits + 2, true);
        qint32 anchorDeltaX  = data.readBits(numBits + 2, true);
        qint32 anchorDeltaY  = data.readBits(numBits + 2, true);

        Q_UNUSED(controlDeltaX);
        Q_UNUSED(controlDeltaY);
        Q_UNUSED(anchorDeltaX);
        Q_UNUSED(anchorDeltaY);
      }
    }

//    data.byteAlign();
  }

  // add to dictionary
  Q_ASSERT( ! file()->dictionary().contains(m_characterId));
  file()->dictionary().insert(m_characterId, this);

  return QDataStream::Ok == data.status();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns character ID. */
quint16 SwfDefineShapeTag::characterId() const
{
  return m_characterId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns shape style data. */
const ShapeWithStyle& SwfDefineShapeTag::shapeStyle() const
{
  return m_shape;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns tag version. */
int SwfDefineShapeTag::version() const
{
  return 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
