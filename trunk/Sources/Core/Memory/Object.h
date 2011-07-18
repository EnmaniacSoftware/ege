#ifndef EGE_CORE_OBJECT_H
#define EGE_CORE_OBJECT_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <EGETypes.h>
#include "Core/ObjectUIDs.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
 
class Application;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef void (*egeObjectDeleteFunc)(void* data);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Object
{
  public:

    Object(Application* app, u32 uid = EGE_OBJECT_UID_GENERIC, egeObjectDeleteFunc deleteFunc = NULL) 
    : m_app(app), m_references(0), m_uid(uid), m_deleteFunc(deleteFunc) {}
    virtual ~Object() {}
    
    /*! Returns object Unique Identifier. */
    inline u32 uid() const { return m_uid; }
    /*! Increases reference count for the object */
    inline void addReference() { ++m_references; }
    /*! Decreses reference count for the object. If no more references are present deallocates object. */
    inline void release() { if (0 < m_references) { if (--m_references == 0) { if (m_deleteFunc) m_deleteFunc(this); else delete this; } } }
    /*! Returns pointer to engine. */
    inline Application* app() const { return m_app; }

  private:

    /*! Pointer to application object. */
    Application* m_app;
    /*! Reference counter. */
    u32 m_references;
    /*! Object UID. */
    u32 m_uid;
    /*! Delete function pointer. If NULL standard delete operator will be used for object deletion. */
    egeObjectDeleteFunc m_deleteFunc;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_OBJECT_H