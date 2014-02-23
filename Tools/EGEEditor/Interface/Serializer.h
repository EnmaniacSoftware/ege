#ifndef EDITOR_SERIALIZRER_H
#define EDITOR_SERIALIZRER_H

/*! Interface for objects capable of serialization. 
 */

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ISerializer
{
  public:

    virtual ~ISerializer() {}

    /*! Serializes into given stream. */
    virtual bool serialize(QXmlStreamWriter& stream) const = 0;
    /*! Unserializes from given data stream. */
    virtual bool unserialize(QXmlStreamReader& stream) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EDITOR_SERIALIZRER_H
