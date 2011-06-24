#include "Core/Application/Application.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Physics/PhysicsJointPulley.h"
#include <EGEDebug.h>

#ifdef EGE_PHYSICS_BOX2D
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsJointPulleyBox2D_p.h"
#endif // EGE_PHYSICS_BOX2D

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsJointPulley)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointPulley)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointPulley::PhysicsJointPulley(Application* app, PPhysicsComponent bodyA, PPhysicsComponent bodyB, EGE::float32 maxLengthA, EGE::float32 maxLengthB, 
                                       const Vector4f& groundAnchorA, const Vector4f& groundAnchorB) 
: PhysicsJoint(app, bodyA, bodyB, EGE_OBJECT_UID_PHYSICS_JOINT_PULLEY), m_maxLengthA(maxLengthA), m_maxLengthB(maxLengthB), m_groundAnchorA(groundAnchorA), 
  m_groundAnchorB(groundAnchorB)
{
  m_p = app->physicsManager()->registerJoint(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointPulley::~PhysicsJointPulley()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns length of segment attached to body A. */
EGE::float32 PhysicsJointPulley::lengthA() const
{
  EGE_ASSERT(NULL != p_func());

  if (isValid())
  {
    return p_func()->lengthA();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns length of segment attached to body B. */
EGE::float32 PhysicsJointPulley::lengthB() const
{
  EGE_ASSERT(NULL != p_func());

  if (isValid())
  {
    return p_func()->lengthB();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool PhysicsJointPulley::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
