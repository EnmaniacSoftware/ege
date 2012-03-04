#ifndef SERIALIZRER_H
#define SERIALIZRER_H

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Interface for objects capable of serialization. */
class ISerializer
{
  public:

    /*! Serializes into given stream. */
    virtual bool serialize(QXmlStreamWriter& stream) const = 0;
    /*! Unserializes from given data stream. */
    virtual bool unserialize(const QXmlStreamReader& stream) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SERIALIZRER_H