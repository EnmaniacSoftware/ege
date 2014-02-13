#ifndef CORE_ATTACHABLE_H
#define CORE_ATTACHABLE_H

#include <QUuid>

/** Interface for objects which can have other objects attached/detached to/from itself.
 */

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Attachable
{
  public:

    virtual ~Attachable(){}

    /*! Attaches object of a given ID.
     *  @param  id  Object ID.
     */
   virtual void attachObject(const QUuid& id) = 0;
    /*! Detaches object with a given ID.
     *  @param  id  Object ID.
     */
   virtual void detachObject(const QUuid& id) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Q_DECLARE_INTERFACE(Attachable, "com.littlebeestudios.attachable/1.0")
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // CORE_ATTACHABLE_H
