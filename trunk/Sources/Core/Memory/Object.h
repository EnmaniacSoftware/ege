#ifndef EGE_CORE_OBJECT_H
#define EGE_CORE_OBJECT_H

#include "EGETypes.h"
#include "Core/ObjectUIDs.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
 
class Application;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Object
{
  public:

    Object(Application* app, u32 uid = EGE_OBJECT_UID_GENERIC) : m_app(app), m_references(0), m_uid(uid) {}
    virtual ~Object() {}
    
    /*! Returns object Unique Identifier. */
    inline u32 uid() const { return m_uid; }
    /*! Increases reference count for the object */
    inline void addReference() { ++m_references; }
    /*! Decreses reference count for the object. If no more references are present deallocates object. */
    inline void release() { if (--m_references == 0) { delete this; } }
    /*! Returns pointer to engine. */
    inline Application* app() const { return m_app; }

  private:

    /*! Pointer to application object. */
    Application* m_app;
    /*! Reference counter. */
    u32 m_references;
    /*! Object UID. */
    u32 m_uid;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_OBJECT_H