#ifndef SWF_PLACEOBJECT2_TAG_H
#define SWF_PLACEOBJECT2_TAG_H

#include "SwfTag.h"
#include "SwfTypes.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing PlaceObject2 SWF tag. */
class SwfPlaceObject2Tag : public SwfTag
{
  public:

    SwfPlaceObject2Tag();
   ~SwfPlaceObject2Tag();

  private:

    /* SwfTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;

  private:

    /*! Depth. */
    quint16 m_depth;
    /*! Character Id. */
    quint16 m_characterId;
    /*! Matrix. */
    Matrix m_matrix;
    /*! Ratio. */
    quint16 m_ratio;
    /*! Clip depth. */
    quint16 m_clipDepth;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_PLACEOBJECT2_TAG_H