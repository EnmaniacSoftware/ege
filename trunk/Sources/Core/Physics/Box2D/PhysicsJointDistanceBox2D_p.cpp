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
PhysicsJointDistancePrivate::PhysicsJointDistancePrivate(PhysicsJointDistance* parent, b2World* world) : m_d(parent), m_joint(NULL), m_world(world)
{
  // define joint
	b2DistanceJointDef def;

	def.frequencyHz      = 4;
	def.dampingRatio     = 1;
	def.collideConnected = true;

  b2Body* bodyA = d_func()->bodyA()->p_func()->body();
  b2Body* bodyB = d_func()->bodyB()->p_func()->body();

	def.Initialize(bodyA, bodyB, bodyA->GetPosition(), bodyB->GetPosition());
  //def.bodyB = m_droplet[m_nodeDroplets[nodeId][i]], m_droplet[m_nodeDroplets[nodeId][(i + 1) % 6]], m_droplet[m_nodeDroplets[nodeId][i]]->GetPosition(), m_droplet[m_nodeDroplets[nodeId][(i + 1) % 6]]->GetPosition());
  //def.bodyA = d_func()->bodyA()->p_func()->body();
  //def.bodyB = d_func()->bodyB()->p_func()->body();
  //def.localAnchorA = def.bodyA->GetLocalPoint(def.bodyA->GetPosition());
  //def.localAnchorB = def.bodyB->GetLocalPoint(def.bodyB->GetPosition());

  //def.length = 400;//d_func()->length();

  // create joint implementation
  m_joint = (b2DistanceJoint*) m_world->CreateJoint(&def);

  m_joint->SetUserData(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistancePrivate::~PhysicsJointDistancePrivate()
{
  if (m_joint)
  {
    m_world->DestroyJoint(m_joint);
    m_joint = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets natural length. */
void PhysicsJointDistancePrivate::setLength(EGE::float32 length)
{
  if (m_joint)
  {
    m_joint->SetLength(length);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns natural length. */
EGE::float32 PhysicsJointDistancePrivate::length() const
{
  return (NULL != m_joint) ? m_joint->GetLength() : 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool PhysicsJointDistancePrivate::isValid() const
{
  return NULL != m_joint;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
