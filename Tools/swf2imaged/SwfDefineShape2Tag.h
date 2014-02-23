#ifndef SWF_DEFINESHAPE2_TAG_H
#define SWF_DEFINESHAPE2_TAG_H

#include "SwfDefineShapeTag.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing DefineShape2 SWF tag. */
class SwfDefineShape2Tag : public SwfDefineShapeTag
{
  Q_OBJECT

  public:

    SwfDefineShape2Tag();
   ~SwfDefineShape2Tag();

  private:

    /* SwfDefineShapeTag override. Returns tag version. */
    int version() const override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_DEFINESHAPE2_TAG_H
