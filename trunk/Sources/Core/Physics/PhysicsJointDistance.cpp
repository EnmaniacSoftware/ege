#include "Core/Application/Application.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Physics/PhysicsJointDistance.h"
#include <EGEDebug.h>

#ifdef EGE_PHYSICS_BOX2D
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsJointDistanceBox2D_p.h"
#endif // EGE_PHYSICS_BOX2D

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsJointDistance)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointDistance)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistance::PhysicsJointDistance(Application* app, PPhysicsComponent bodyA, PPhysicsComponent bodyB) 
: PhysicsJoint(app, bodyA, bodyB, EGE_OBJECT_UID_PHYSICS_JOINT_DISTANCE)
{
  m_p = app->physicsManager()->registerJoint(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistance::~PhysicsJointDistance()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets natural length. */
void PhysicsJointDistance::setLength(EGE::float32 length)
{
  EGE_ASSERT(NULL != p_func());

  if (isValid())
  {
    p_func()->setLength(length);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns natural length. */
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
/*! Returns TRUE if object is valid. */
bool PhysicsJointDistance::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets damping ratio.
*
*  @param   ration Damping ratio to set. 0 - no damping, 1 - critical damping.
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointDistance::setDampingRatio(EGE::float32 ratio)
{
  if (isValid())
  {
    p_func()->setDampingRatio(ratio);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current damping ratio. */
EGE::float32 PhysicsJointDistance::dampingRatio() const
{
  if (isValid())
  {
    return p_func()->dampingRatio();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets mass-spring-damper frequency (in Hz). 
*  @note  This describes the speed of response of the system.
*/
void PhysicsJointDistance::setFrequency(EGE::float32 frequencyHz)
{
  if (isValid())
  {
    p_func()->setFrequency(frequencyHz);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current mass-spring-damper frequency (in Hz). */
EGE::float32 PhysicsJointDistance::frequency() const
{
  if (isValid())
  {
    return p_func()->frequency();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
