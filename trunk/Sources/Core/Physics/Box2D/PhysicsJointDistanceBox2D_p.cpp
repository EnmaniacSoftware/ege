#ifdef EGE_PHYSICS_BOX2D

#include "Core/Application/Application.h"
#include "Core/Physics/PhysicsJointDistance.h"
#include "Core/Physics/Box2D/PhysicsJointDistanceBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"
#include <Box2D/Box2D.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsJointDistancePrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointDistancePrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistancePrivate::PhysicsJointDistancePrivate(PhysicsJointDistance* parent, PhysicsManagerPrivate* managerPrivate) : m_d(parent), 
                                                                                                                                m_joint(NULL), 
                                                                                                                                m_managerPrivate(managerPrivate)
{
  b2Body* bodyA = d_func()->bodyA()->p_func()->body();
  b2Body* bodyB = d_func()->bodyB()->p_func()->body();

  // define joint
	b2DistanceJointDef def;
	def.frequencyHz      = 4;
	def.dampingRatio     = 1;
	def.collideConnected = false;
	def.Initialize(bodyA, bodyB, bodyA->GetPosition(), bodyB->GetPosition());

  // create joint implementation
  m_joint = (b2DistanceJoint*) manager()->world()->CreateJoint(&def);

  m_joint->SetUserData(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistancePrivate::~PhysicsJointDistancePrivate()
{
  if (m_joint)
  {
    manager()->world()->DestroyJoint(m_joint);
    m_joint = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets natural length. */
void PhysicsJointDistancePrivate::setLength(EGE::float32 length)
{
  if (m_joint)
  {
    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    m_joint->SetLength(length * scale);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns natural length. */
float32 PhysicsJointDistancePrivate::length() const
{
  if (m_joint)
  {
    float32 scale = manager()->simulationToWorldScaleFactor();

    return m_joint->GetLength() * scale;
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool PhysicsJointDistancePrivate::isValid() const
{
  return NULL != m_joint;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets damping ratio.
*
*  @param   ration Damping ratio to set. 0 - no damping, 1 - critical damping.
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointDistancePrivate::setDampingRatio(float32 ratio)
{
  if (m_joint)
  {
    m_joint->SetDampingRatio(ratio);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current damping ratio. */
float32 PhysicsJointDistancePrivate::dampingRatio() const
{
  if (m_joint)
  {
    return m_joint->GetDampingRatio();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets mass-spring-damper frequency (in Hz). 
*  @note  This describes the speed of response of the system.
*/
void PhysicsJointDistancePrivate::setFrequency(float32 frequencyHz)
{
  if (m_joint)
  {
    m_joint->SetFrequency(frequencyHz);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current mass-spring-damper frequency (in Hz). */
float32 PhysicsJointDistancePrivate::frequency() const
{
  if (m_joint)
  {
    return m_joint->GetFrequency();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_BOX2D