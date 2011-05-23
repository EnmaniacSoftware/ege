#include "EGEPhysics.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsJoint)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJoint)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJoint::PhysicsJoint(Application* app, PPhysicsComponent bodyA, PPhysicsComponent bodyB, u32 uid) : Object(app, uid), m_bodyA(bodyA), m_bodyB(bodyB)
{
  // associate joint with bodies
  if (m_bodyA)
  {
    m_bodyA->m_joints.push_back(this);
  }

  if (m_bodyB)
  {
    m_bodyB->m_joints.push_back(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJoint::~PhysicsJoint()
{
  if (m_bodyA)
  {
    m_bodyA->m_joints.remove(this);
    m_bodyA = NULL;
  }

  if (m_bodyB)
  {
    m_bodyB->m_joints.remove(this);
    m_bodyB = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
