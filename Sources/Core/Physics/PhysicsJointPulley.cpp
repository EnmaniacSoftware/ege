#include "Core/Application/Application.h"
#include "EGEPhysics.h"
#include "EGEDebug.h"

#ifdef EGE_PHYSICS_BOX2D
  #include "Core/Physics/Box2D/PhysicsJointPulleyBox2D_p.h"
#elif defined EGE_PHYSICS_NULL
  #include "Core/Physics/Null/PhysicsJointPulleyNull_p.h"
#endif // EGE_PHYSICS_BOX2D

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsJointPulley)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointPulley)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointPulley::PhysicsJointPulley(Application* app, PPhysicsComponent bodyA, PPhysicsComponent bodyB, float32 maxLengthA, float32 maxLengthB, 
                                       const Vector4f& groundAnchorA, const Vector4f& groundAnchorB) 
: PhysicsJoint(app, bodyA, bodyB, EGE_OBJECT_UID_PHYSICS_JOINT_PULLEY), 
  m_maxLengthA(maxLengthA), 
  m_maxLengthB(maxLengthB), 
  m_groundAnchorA(groundAnchorA), 
  m_groundAnchorB(groundAnchorB)
{
  m_p = ege_new PhysicsJointPulleyPrivate(this, app->physicsManager()->p_func());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointPulley::~PhysicsJointPulley()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
bool PhysicsJointPulley::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END