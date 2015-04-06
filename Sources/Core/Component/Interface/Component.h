#ifndef EGE_CORE_COMPONENTS_COMPONENT_H
#define EGE_CORE_COMPONENTS_COMPONENT_H

/*! Components are basically data structures intended to be manipulated from outside. */

#include "EGE.h"
#include "EGEString.h"
#include "Core/Component/Interface/ComponentHost.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(Component, PComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Component : public Object
                , public ComponentHost
{
  public:

    Component(u32 uid, const String& name);
    virtual ~Component();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Gets component name. */
    const String& name() const;

  private:

    /*! Component name. */
    String m_name;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_COMPONENTS_COMPONENT_H