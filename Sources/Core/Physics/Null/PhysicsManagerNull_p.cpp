#ifdef EGE_PHYSICS_NULL

#include "Core/Application/Application.h"
#include "Core/Physics/Null/PhysicsManagerNull_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsManagerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsManagerPrivate::PhysicsManagerPrivate(PhysicsManager* base) : m_d(base), 
                                                                     m_scale(1.0f), 
                                                                     m_invScale(1.0f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsManagerPrivate::~PhysicsManagerPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PhysicsManagerPrivate::construct(const Dictionary& params)
{
  bool error = false;

  // decompose param list
  m_scale = params.value(EGE_PHYSICS_PARAM_SCALE_FACTOR, "1").toFloat(&error);
  if ( ! error)
  {
    m_invScale = 1.0f / m_scale;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsManagerPrivate::update(const Time& time)
{
  EGE_UNUSED(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsManagerPrivate::setGravity(const Vector4f& gravity)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsManagerPrivate::render()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_NULL