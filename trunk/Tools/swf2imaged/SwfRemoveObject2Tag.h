#ifndef SWF_REMOVEOBJECT2_TAG_H
#define SWF_REMOVEOBJECT2_TAG_H

#include "SwfRemoveObjectTag.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing RemoveObject2 SWF tag. */
class SwfRemoveObject2Tag : public SwfRemoveObjectTag
{
  Q_OBJECT

  public:

    SwfRemoveObject2Tag();
   ~SwfRemoveObject2Tag();

  private:

    /* SwfRemoveObjectTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_REMOVEOBJECT2_TAG_H
