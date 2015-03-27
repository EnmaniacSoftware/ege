#ifndef EGE_CORE_OBJECT_H
#define EGE_CORE_OBJECT_H

#include "EGETypes.h"
#include "EGEAtomicInt.h"
#include "Core/ObjectUIDs.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef void (*egeObjectDeleteFunc)(void* data);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Object
{
  public:

    Object(u32 uid = EGE_OBJECT_UID_GENERIC, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~Object();
    
    /*! Returns object Unique Identifier. */
    u32 uid() const;
    /*! Increases reference count for the object */
    void addReference();
    /*! Decreses reference count for the object. If no more references are present deallocates object. */
    void release();
    /*! Returns reference count. */
    s32 referenceCount();

  private:

    /*! Reference counter. */
    AtomicInt m_references;
    /*! Object UID. */
    u32 m_uid;
    /*! Delete function pointer. If NULL standard delete operator will be used for object deletion. */
    egeObjectDeleteFunc m_deleteFunc;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Object::Object(u32 uid, egeObjectDeleteFunc deleteFunc) 
: m_references(0)
, m_uid(uid)
, m_deleteFunc(deleteFunc) 
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Object::~Object() 
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline u32 Object::uid() const 
{ 
  return m_uid; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void Object::addReference() 
{ 
  m_references.increment();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void Object::release() 
{ 
  if (0 == m_references.decrement()) 
  { 
    if (NULL != m_deleteFunc) 
    {
      m_deleteFunc(this); 
    }
    else 
    {
      delete this; 
    } 
  } 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline s32 Object::referenceCount()
{
  return m_references.load();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_OBJECT_H