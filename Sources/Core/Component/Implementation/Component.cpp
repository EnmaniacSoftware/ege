#include "Core/Component/Interface/Component.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Component)
EGE_DEFINE_DELETE_OPERATORS(Component)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Component::Component(Application* app, u32 uid, const String& name) : Object(app, uid)
                                                                    , m_name(name)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Component::~Component()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& Component::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
