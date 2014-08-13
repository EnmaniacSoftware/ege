#include "EGEEngine.h"
#include "EGEPhysics.h"
#include "EGEDebug.h"

#ifdef EGE_PHYSICS_BOX2D
  #include "Core/Physics/Box2D/PhysicsJointDistanceBox2D_p.h"
#elif defined EGE_PHYSICS_NULL
  #include "Core/Physics/Null/PhysicsJointDistanceNull_p.h"
#endif // EGE_PHYSICS_BOX2D

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsJointDistance)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointDistance)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistance::PhysicsJointDistance(Engine& engine, PPhysicsComponent bodyA, PPhysicsComponent bodyB) 
: PhysicsJoint(bodyA, bodyB, EGE_OBJECT_UID_PHYSICS_JOINT_DISTANCE)
{
  m_p = ege_new PhysicsJointDistancePrivate(this, engine.physicsManager()->p_func());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistance::~PhysicsJointDistance()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointDistance::setLength(EGE::float32 length)
{
  EGE_ASSERT(NULL != p_func());

  if (isValid())
  {
    p_func()->setLength(length);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::float32 PhysicsJointDistance::length() const
{
  EGE_ASSERT(NULL != p_func());

  if (isValid())
  {
    return p_func()->length();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsJointDistance::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointDistance::setDampingRatio(EGE::float32 ratio)
{
  if (isValid())
  {
    p_func()->setDampingRatio(ratio);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PhysicsJointDistance::dampingRatio() const
{
  if (isValid())
  {
    return p_func()->dampingRatio();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointDistance::setFrequency(EGE::float32 frequencyHz)
{
  if (isValid())
  {
    p_func()->setFrequency(frequencyHz);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PhysicsJointDistance::frequency() const
{
  if (isValid())
  {
    return p_func()->frequency();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END