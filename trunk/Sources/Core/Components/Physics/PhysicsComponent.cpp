#include "Core/Application/Application.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Components/Physics/PhysicsComponent.h"
#include "Core/Debug/Debug.h"

#ifdef EGE_PHYSICS_BOX2D
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"
#endif // EGE_PHYSICS_BOX2D

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsComponent)
EGE_DEFINE_DELETE_OPERATORS(PhysicsComponent)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsComponent::PhysicsComponent(Application* app, const EGEString& name, EGEPhysics::EComponentType type) 
: IComponent(app, EGE_OBJECT_UID_PHYSICS_COMPONENT, name), m_type(type), m_position(Vector4f::ZERO), m_linearVelocity(Vector4f::ZERO), 
  m_force(Vector4f::ZERO), m_orientation(Quaternionf::IDENTITY), m_mass(1.0f)
{
  m_manager = app->physicsManager();

  // create private thru manager
  m_p = m_manager->registerComponent(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsComponent::~PhysicsComponent()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets position vector. */
void PhysicsComponent::setPosition(const Vector4f& position)
{
  EGE_ASSERT(isValid(), "No private");
  if (isValid())
  {
    p_func()->setPosition(position);
  }
  
  m_position = position;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns position vector. */
Vector4f PhysicsComponent::position() const
{
  EGE_ASSERT(isValid(), "No private");
  if (isValid())
  {
    return p_func()->position();
  }

  return m_position;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets orientation quaternion. */
void PhysicsComponent::setOrientation(const Quaternionf& orientation)
{
  EGE_ASSERT(isValid(), "No private");
  if (isValid())
  {
    p_func()->setOrientation(orientation);
  }

  m_orientation = orientation;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns orientation quaternion. */
Quaternionf PhysicsComponent::orientation() const
{
  EGE_ASSERT(isValid(), "No private");
  if (isValid())
  {
    return p_func()->orientation();
  }

  return m_orientation;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Applies a force vector.
* 
*  @param  force    force vector to apply.
*  @param  worldPos world position where force is to be applied. 
*/
void PhysicsComponent::applyForce(const Vector4f& force, const Vector4f& worldPos)
{
  EGE_ASSERT(isValid(), "No private");
  if (isValid())
  {
    p_func()->applyForce(force, worldPos);
  }

  m_force += force;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Applies an impulse at the giveb point.
* 
*  @param  force    impulse vector to apply.
*  @param  worldPos world position where impulse is to be applied. 
*  @note   This immediatly affects linear velocity.
*/
void PhysicsComponent::applyLinearImpulse(const Vector4f& impulse, const Vector4f& worldPos)
{
  EGE_ASSERT(isValid(), "No private");
  if (isValid())
  {
    return p_func()->applyLinearImpulse(impulse, worldPos);
  }

  m_linearVelocity += impulse * (1.0f / mass());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns force vector. */
Vector4f PhysicsComponent::force() const
{
  EGE_ASSERT(isValid(), "No private");
  if (isValid())
  {
    return p_func()->force();
  }

  return m_force;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets mass. */
void PhysicsComponent::setMass(EGE::float32 mass)
{
  EGE_ASSERT(isValid(), "No private");
  if (isValid())
  {
    p_func()->setMass(mass);
  }

  if (EGEPhysics::COMPONENT_STATIC == type())
  {
    mass = 0;
  }
  else if (EGEPhysics::COMPONENT_DYNAMIC == type())
  {
    if (0 == mass)
    {
      mass = 1;
    }
  }

  m_mass = mass;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns mass. */
EGE::float32 PhysicsComponent::mass() const
{
  EGE_ASSERT(isValid(), "No private");
  if (isValid())
  {
    return p_func()->mass();
  }

  return m_mass;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets linear velocity vector. */
void PhysicsComponent::setLinearVelocity(const Vector4f& velocity)
{
  EGE_ASSERT(isValid(), "No private");
  if (isValid())
  {
    p_func()->setLinearVelocity(velocity);
  }

  m_linearVelocity = velocity;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns linear velocity vector. */
Vector4f PhysicsComponent::linearVelocity() const
{
  EGE_ASSERT(isValid(), "No private");
  if (isValid())
  {
    return p_func()->linearVelocity();
  }

  return m_linearVelocity;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IComponent override. Returns TRUE if component is valid. */
bool PhysicsComponent::isValid() const
{
  return (NULL != m_p) && p_func()->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
