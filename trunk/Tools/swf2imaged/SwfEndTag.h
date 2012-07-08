#ifndef SWF_END_TAG_H
#define SWF_END_TAG_H

#include "SwfTag.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing End SWF tag. */
class SwfEndTag : public SwfTag
{
  public:

    SwfEndTag();
   ~SwfEndTag();

  private:

    /* SwfTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_END_TAG_H