#ifndef SWF_SHOWFRAME_TAG_H
#define SWF_SHOWFRAME_TAG_H

#include "SwfTag.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing ShowFrame SWF tag. */
class SwfShowFrameTag : public SwfTag
{
  Q_OBJECT

  public:

    SwfShowFrameTag();
   ~SwfShowFrameTag();

  private:

    /* SwfTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_SHOWFRAME_TAG_H