#include "EGEEngine.h"
#include "EGEPhysics.h"
#include "EGEDebug.h"
#include "Core/Physics/PhysicsManager.h"

#ifdef EGE_PHYSICS_BOX2D
  #include "Core/Physics/Box2D/PhysicsJointAttractBox2D_p.h"
#elif defined EGE_PHYSICS_NULL
  #include "Core/Physics/Null/PhysicsJointAttractNull_p.h"
#endif // EGE_PHYSICS_BOX2D

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsJointAttract)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointAttract)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointAttract::PhysicsJointAttract(Engine& engine, PPhysicsComponent body) : PhysicsJoint(body, NULL, EGE_OBJECT_UID_PHYSICS_JOINT_ATTRACT)
{
  m_p = ege_new PhysicsJointAttractPrivate(this, dynamic_cast<PhysicsManager*>(engine.physicsManager())->p_func());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointAttract::~PhysicsJointAttract()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsJointAttract::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointAttract::setTarget(const Vector4f& position)
{
  if (isValid())
  {
    p_func()->setTarget(position);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointAttract::setDampingRatio(EGE::float32 ratio)
{
  if (isValid())
  {
    p_func()->setDampingRatio(ratio);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PhysicsJointAttract::dampingRatio() const
{
  if (isValid())
  {
    return p_func()->dampingRatio();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointAttract::setFrequency(EGE::float32 frequencyHz)
{
  if (isValid())
  {
    p_func()->setFrequency(frequencyHz);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PhysicsJointAttract::frequency() const
{
  if (isValid())
  {
    return p_func()->frequency();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END