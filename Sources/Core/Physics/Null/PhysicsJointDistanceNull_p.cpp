#ifdef EGE_PHYSICS_NULL

#include "Core/Physics/Null/PhysicsJointDistanceNull_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsJointDistancePrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointDistancePrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistancePrivate::PhysicsJointDistancePrivate(PhysicsJointDistance* parent, PhysicsManagerPrivate* managerPrivate) : m_d(parent),  
                                                                                                                                m_managerPrivate(managerPrivate)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistancePrivate::~PhysicsJointDistancePrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointDistancePrivate::setLength(float32 length)
{
  EGE_UNUSED(length);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PhysicsJointDistancePrivate::length() const
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsJointDistancePrivate::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointDistancePrivate::setDampingRatio(float32 ratio)
{
  EGE_UNUSED(ratio);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PhysicsJointDistancePrivate::dampingRatio() const
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointDistancePrivate::setFrequency(float32 frequencyHz)
{
  EGE_UNUSED(frequencyHz);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PhysicsJointDistancePrivate::frequency() const
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_NULL