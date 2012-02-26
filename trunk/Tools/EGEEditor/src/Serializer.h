#ifndef SERIALIZRER_H
#define SERIALIZRER_H

#include <QString>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Interface for objects capable of serialization. */
class ISerializer
{
  public:

    /*! Serializes into given buffer. */
    virtual QString serialize() const = 0;
    /*! Unserializes from given data buffer. */
    virtual bool unserialize(const QString& data) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SERIALIZRER_H