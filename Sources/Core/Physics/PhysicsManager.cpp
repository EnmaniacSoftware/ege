#include "EGEPhysics.h"

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
PhysicsManager::PhysicsManager(const Dictionary& params) 
: m_params(params)
{
  // create private implementation
  m_p = ege_new PhysicsManagerPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsManager::~PhysicsManager()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PhysicsManager::construct()
{
  EGEResult result;

  if (EGE_SUCCESS != (result = m_p->construct(m_params)))
  {
    // error!
    return result;
  }

  return EngineModule::construct();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
void PhysicsManager::setGravity(const Vector4f& gravity)
{
  p_func()->setGravity(gravity);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsManager::render()
{
  p_func()->render();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 PhysicsManager::uid() const
{
  return EGE_OBJECT_UID_PHYSICS_MANAGER_MODULE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END