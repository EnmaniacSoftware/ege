#ifndef SWF_SETBACKGROUNDCOLOR_TAG_H
#define SWF_SETBACKGROUNDCOLOR_TAG_H

#include "SwfTag.h"
#include <QColor>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing SetBackgroundColor tag. */
class SwfSetBackgroundColorTag : public SwfTag
{
  public:

    SwfSetBackgroundColorTag();
   ~SwfSetBackgroundColorTag();

  private:

    /* SwfTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;

  private:

    /*! Color. */
    QColor m_color;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_SETBACKGROUNDCOLOR_TAG_H