#include <EGEPhysics.h>

#ifdef EGE_PHYSICS_BOX2D
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#elif defined EGE_PHYSICS_NULL
#include "Core/Physics/Null/PhysicsManagerNull_p.h"
#endif // EGE_PHYSICS_BOX2D

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsManager)
EGE_DEFINE_DELETE_OPERATORS(PhysicsManager)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsManager::PhysicsManager(Application* app, const Dictionary& params) : Object(app)
{
  m_p = ege_new PhysicsManagerPrivate(this, params);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsManager::~PhysicsManager()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void PhysicsManager::update(const Time& time)
{
  p_func()->update(time);

  //float32 fTimeInSeconds = time.seconds();

  //// go thru all components
  //for (std::vector<PPhysicsComponent>::iterator iter = m_vpComponents.begin(); iter != m_vpComponents.end(); ++iter)
  //{
  //  PPhysicsComponent& pComponent = *iter;

  //  // calculate acceleration vector
  //  Vector4f cAcceleration = pComponent->force() / pComponent->mass();

  //  // reset forces acting on component
  //  pComponent->setForce(Vector4f::ZERO);

  //  // update velocity
  //  pComponent->setVelocity(pComponent->velocity() + cAcceleration * fTimeInSeconds);

  //  // update position
  //  pComponent->setPosition(pComponent->position() + pComponent->velocity() * fTimeInSeconds);
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if manager is valid. */
bool PhysicsManager::isValid() const
{
  return (NULL != m_p) && p_func()->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets gravity. */
void PhysicsManager::setGravity(const Vector4f& gravity)
{
  if (isValid())
  {
    p_func()->setGravity(gravity);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders data. */
void PhysicsManager::render()
{
  if (isValid())
  {
    p_func()->render();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END