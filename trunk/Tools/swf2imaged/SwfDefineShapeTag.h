#ifndef SWF_DEFINESHAPE_TAG_H
#define SWF_DEFINESHAPE_TAG_H

#include "SwfTag.h"
#include <QRect>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing DefineShape SWF tag. */
class SwfDefineShapeTag : public SwfTag
{
  Q_OBJECT

  public:

    SwfDefineShapeTag();
   ~SwfDefineShapeTag();

  private:

    /* SwfTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;

  private:

    /*! Character Id. */
    quint16 m_characterId;
    /*! Bounds. */
    QRect m_bounds;
    /*! Shape. */
    ShapeWithStyle m_shape;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_DEFINESHAPE_TAG_H