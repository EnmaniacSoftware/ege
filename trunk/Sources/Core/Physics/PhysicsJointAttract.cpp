#include "Core/Application/Application.h"
#include <EGEPhysics.h>
#include <EGEDebug.h>

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
PhysicsJointAttract::PhysicsJointAttract(Application* app, PPhysicsComponent body) : PhysicsJoint(app, body, NULL, EGE_OBJECT_UID_PHYSICS_JOINT_ATTRACT)
{
  m_p = ege_new PhysicsJointAttractPrivate(this, app->physicsManager()->p_func());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointAttract::~PhysicsJointAttract()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool PhysicsJointAttract::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets target position to attract to. */
void PhysicsJointAttract::setTarget(const Vector4f& position)
{
  if (isValid())
  {
    p_func()->setTarget(position);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets damping ratio.
*
*  @param   ration Damping ratio to set. 0 - no damping, 1 - critical damping.
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointAttract::setDampingRatio(EGE::float32 ratio)
{
  if (isValid())
  {
    p_func()->setDampingRatio(ratio);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current damping ratio. */
EGE::float32 PhysicsJointAttract::dampingRatio() const
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
void PhysicsJointAttract::setFrequency(EGE::float32 frequencyHz)
{
  if (isValid())
  {
    p_func()->setFrequency(frequencyHz);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current mass-spring-damper frequency (in Hz). */
EGE::float32 PhysicsJointAttract::frequency() const
{
  if (isValid())
  {
    return p_func()->frequency();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END