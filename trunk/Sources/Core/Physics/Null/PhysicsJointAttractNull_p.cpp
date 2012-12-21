#ifdef EGE_PHYSICS_NULL

#include "Core/Application/Application.h"
#include "Core/Physics/Null/PhysicsJointAttractNull_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsJointAttractPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointAttractPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointAttractPrivate::PhysicsJointAttractPrivate(PhysicsJointAttract* parent, PhysicsManagerPrivate* managerPrivate) : m_d(parent), 
                                                                                                                             m_managerPrivate(managerPrivate)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointAttractPrivate::~PhysicsJointAttractPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsJointAttractPrivate::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointAttractPrivate::setTarget(const Vector4f& position)
{
  EGE_UNUSED(position);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointAttractPrivate::setDampingRatio(float32 ratio)
{
  EGE_UNUSED(ratio);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PhysicsJointAttractPrivate::dampingRatio() const
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointAttractPrivate::setFrequency(float32 frequencyHz)
{
  EGE_UNUSED(frequencyHz);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PhysicsJointAttractPrivate::frequency() const
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_NULL