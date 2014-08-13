#include "EGEPhysics.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsJoint)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJoint)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJoint::PhysicsJoint(PPhysicsComponent bodyA, PPhysicsComponent bodyB, u32 uid) : Object(uid)
                                                                                      , m_bodyA(bodyA)
                                                                                      , m_bodyB(bodyB)
{
  // associate joint with bodies
  if (NULL != m_bodyA)
  {
    m_bodyA->m_joints.push_back(this);
  }

  if (NULL != m_bodyB)
  {
    m_bodyB->m_joints.push_back(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJoint::~PhysicsJoint()
{
  if (NULL != m_bodyA)
  {
    m_bodyA->m_joints.remove(this);
    m_bodyA = NULL;
  }

  if (NULL != m_bodyB)
  {
    m_bodyB->m_joints.remove(this);
    m_bodyB = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END