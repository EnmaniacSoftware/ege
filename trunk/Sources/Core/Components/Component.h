#ifndef EGE_CORE_COMPONENT_H
#define EGE_CORE_COMPONENT_H

// Components are basically data structures intended to be manipulated from outside. Thus, for performance reasons
// vtable should be non-present within them

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(IComponent, PComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IComponent : public Object
{
  public:

    IComponent(Application* app, u32 uid, const String& name);
    virtual ~IComponent();

    /*! Returns TRUE if component is valid. */
    virtual bool isValid() const = 0;
    /*! Gets component name. */
    const String& name() const { return m_name; }

  private:

    /*! Component name. */
    String m_name;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_COMPONENT_H