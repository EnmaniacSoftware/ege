#include "Core/Application/Application.h"
#include "Core/Physics/PhysicsJointPulley.h"
#include "Core/Physics/Box2D/PhysicsJointPulleyBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsJointPulleyPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointPulleyPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointPulleyPrivate::PhysicsJointPulleyPrivate(PhysicsJointPulley* parent, PhysicsManagerPrivate* managerPrivate) : m_d(parent), m_joint(NULL), 
                                                                                                                          m_managerPrivate(managerPrivate)
{
  EGE::float32 scale = manager()->worldToSimulationScaleFactor();

  b2Body* bodyA = d_func()->bodyA()->p_func()->body();
  b2Body* bodyB = d_func()->bodyB()->p_func()->body();

  // define joint
	b2PulleyJointDef def;
	def.collideConnected = true;
  def.maxLengthA = d_func()->m_maxLengthA * scale;
  def.maxLengthB = d_func()->m_maxLengthB * scale;

	def.Initialize(bodyA, bodyB, b2Vec2(d_func()->m_groundAnchorA.x * scale, d_func()->m_groundAnchorA.y * scale), 
                 b2Vec2(d_func()->m_groundAnchorB.x * scale, d_func()->m_groundAnchorB.y * scale), bodyA->GetPosition(), bodyB->GetPosition(), 1);

  // create joint implementation
  m_joint = (b2PulleyJoint*) manager()->world()->CreateJoint(&def);

  m_joint->SetUserData(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointPulleyPrivate::~PhysicsJointPulleyPrivate()
{
  if (m_joint)
  {
    manager()->world()->DestroyJoint(m_joint);
    m_joint = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns length of segment attached to body A. */
EGE::float32 PhysicsJointPulleyPrivate::lengthA() const
{
  if (m_joint)
  {
    EGE::float32 scale = manager()->simulationToWorldScaleFactor();

    return m_joint->GetLength1() * scale;
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns length of segment attached to body B. */
EGE::float32 PhysicsJointPulleyPrivate::lengthB() const
{
  if (m_joint)
  {
    EGE::float32 scale = manager()->simulationToWorldScaleFactor();

    return m_joint->GetLength2() * scale;
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool PhysicsJointPulleyPrivate::isValid() const
{
  return NULL != m_joint;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
