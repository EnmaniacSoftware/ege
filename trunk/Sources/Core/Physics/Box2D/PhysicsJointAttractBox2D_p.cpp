#include "Core/Application/Application.h"
#include "Core/Physics/PhysicsJointAttract.h"
#include "Core/Physics/Box2D/PhysicsJointAttractBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsJointAttractPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointAttractPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointAttractPrivate::PhysicsJointAttractPrivate(PhysicsJointAttract* parent, PhysicsManagerPrivate* managerPrivate) : m_d(parent), m_joint(NULL), 
                                                                                                                             m_managerPrivate(managerPrivate)
{
  b2Body* body = d_func()->bodyA()->p_func()->body();

  // create ground body
	b2BodyDef bodyDef;
	m_groundBody = manager()->world()->CreateBody(&bodyDef);
  if (m_groundBody)
  {
    // define joint
    b2MouseJointDef def;
    def.maxForce = 10000.0f;// * body->GetMass();
    def.bodyA = m_groundBody;
    def.bodyB = body;
    def.target = body->GetPosition();
    def.collideConnected = false;
   
    // create joint implementation
    m_joint = (b2MouseJoint*) manager()->world()->CreateJoint(&def);

    //m_joint->SetUserData(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointAttractPrivate::~PhysicsJointAttractPrivate()
{
  if (m_joint)
  {
    manager()->world()->DestroyJoint(m_joint);
    m_joint = NULL;
  }

  if (m_groundBody)
  {
    manager()->world()->DestroyBody(m_groundBody);
    m_groundBody = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool PhysicsJointAttractPrivate::isValid() const
{
  return NULL != m_joint;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets target position to attract to. */
void PhysicsJointAttractPrivate::setTarget(const Vector4f& position)
{
  if (isValid())
  {
    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    m_joint->SetTarget(b2Vec2(position.x * scale, position.y * scale));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets damping ratio.
*
*  @param   ration Damping ratio to set. 0 - no damping, 1 - critical damping.
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointAttractPrivate::setDampingRatio(EGE::float32 ratio)
{
  if (m_joint)
  {
    m_joint->SetDampingRatio(ratio);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current damping ratio. */
EGE::float32 PhysicsJointAttractPrivate::dampingRatio() const
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
void PhysicsJointAttractPrivate::setFrequency(EGE::float32 frequencyHz)
{
  if (m_joint)
  {
    m_joint->SetFrequency(frequencyHz);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current mass-spring-damper frequency (in Hz). */
EGE::float32 PhysicsJointAttractPrivate::frequency() const
{
  if (m_joint)
  {
    return m_joint->GetFrequency();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
