#ifndef SWF_TAG_H
#define SWF_TAG_H

#include "SwfDataStream.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing base of SWF tag. */
class SwfTag
{
  public:

    SwfTag();
    virtual ~SwfTag();

  public:

    /* Creates SWFTag of a proper type. */
    static SwfTag* ProcessTag(SwfDataStream& data);

  public:

    /*! Reads data from file. */
    virtual bool read(SwfDataStream& data) = 0;
    /*! Returns ID. */
    inline quint16 id() const { return m_id; }
    /*! Returns length (in bytes). */
    inline quint32 length() const { return m_length; }

  private:

    /*! Tag ID. */
    quint16 m_id;
    /*! Tag length (in bytes). */
    quint32 m_length;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_TAG_H