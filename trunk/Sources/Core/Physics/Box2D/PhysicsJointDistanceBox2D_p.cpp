#include "Core/Application/Application.h"
#include "Core/Physics/PhysicsJointDistance.h"
#include "Core/Physics/Box2D/PhysicsJointDistanceBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsJointDistancePrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointDistancePrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistancePrivate::PhysicsJointDistancePrivate(PhysicsJointDistance* parent, PhysicsManagerPrivate* managerPrivate) : m_d(parent), m_joint(NULL), 
                                                                                                                                m_managerPrivate(managerPrivate)
{
  b2Body* bodyA = d_func()->bodyA()->p_func()->body();
  b2Body* bodyB = d_func()->bodyB()->p_func()->body();

  // define joint
	b2DistanceJointDef def;
	def.frequencyHz      = 4;
	def.dampingRatio     = 1;
	def.collideConnected = true;
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
EGE::float32 PhysicsJointDistancePrivate::length() const
{
  if (m_joint)
  {
    EGE::float32 scale = manager()->simulationToWorldScaleFactor();

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
