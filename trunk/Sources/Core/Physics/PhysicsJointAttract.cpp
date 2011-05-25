#include "Core/Application/Application.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Debug/Debug.h"
#include "Core/Physics/PhysicsJointAttract.h"

#ifdef EGE_PHYSICS_BOX2D
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsJointAttractBox2D_p.h"
#endif // EGE_PHYSICS_BOX2D

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsJointAttract)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointAttract)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointAttract::PhysicsJointAttract(Application* app, PPhysicsComponent body) : PhysicsJoint(app, body, NULL, EGE_OBJECT_UID_PHYSICS_JOINT_ATTRACT)
{
  m_p = app->physicsManager()->registerJoint(this);
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
