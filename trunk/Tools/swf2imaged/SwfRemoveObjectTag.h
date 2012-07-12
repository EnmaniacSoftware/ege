#ifndef SWF_REMOVEOBJECT_TAG_H
#define SWF_REMOVEOBJECT_TAG_H

#include "SwfTag.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing RemoveObject SWF tag. */
class SwfRemoveObjectTag : public SwfTag
{
  Q_OBJECT

  public:

    SwfRemoveObjectTag();
   ~SwfRemoveObjectTag();

    /* Returns character ID. */
    quint16 characterId() const;

  private:

    /* SwfTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;

  protected:

    /*! Character Id. */
    quint16 m_characterId;
    /*! Depth. */
    quint16 m_depth;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_REMOVEOBJECT_TAG_H
