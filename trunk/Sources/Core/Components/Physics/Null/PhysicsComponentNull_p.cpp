#ifdef EGE_PHYSICS_NULL

#include "Core/Components/Physics/Null/PhysicsComponentNull_p.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsComponentPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsComponentPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsComponentPrivate::PhysicsComponentPrivate(PhysicsComponent* parent, PhysicsManagerPrivate* managerPrivate) : m_d(parent),
                                                                                                                    m_managerPrivate(managerPrivate)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsComponentPrivate::~PhysicsComponentPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::setPosition(const Vector4f& position)
{
  EGE_UNUSED(position);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f PhysicsComponentPrivate::position() const
{
  return d_func()->m_position;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::setOrientation(const Quaternionf& orientation)
{
  EGE_UNUSED(orientation);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Quaternionf PhysicsComponentPrivate::orientation() const
{
  return d_func()->m_orientation;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::applyForce(const Vector4f& force, const Vector4f& worldPos)
{
  EGE_UNUSED(force);
  EGE_UNUSED(worldPos);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::applyLinearImpulse(const Vector4f& impulse, const Vector4f& worldPos)
{
  EGE_UNUSED(impulse);
  EGE_UNUSED(worldPos);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f PhysicsComponentPrivate::force() const
{
  return d_func()->m_force;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::setMass(float32 mass)
{
  EGE_UNUSED(mass);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 PhysicsComponentPrivate::mass() const
{
  return d_func()->m_mass;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::setLinearVelocity(const Vector4f& velocity)
{
  EGE_UNUSED(velocity);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f PhysicsComponentPrivate::linearVelocity() const
{ 
  return d_func()->m_linearVelocity;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsComponentPrivate::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsComponentPrivate::addCircleShape(float32 radius, float32 density, EGEPhysics::CollisionData colissionData)
{
  EGE_UNUSED(radius);
  EGE_UNUSED(density);
  EGE_UNUSED(colissionData);

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsComponentPrivate::addPolygonShape(const DynamicArray<Vector4f>& points, float32 density, EGEPhysics::CollisionData colissionData)
{
  EGE_UNUSED(points);
  EGE_UNUSED(density);
  EGE_UNUSED(colissionData);

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::setScale(const Vector4f& scale)
{
  EGE_UNUSED(scale);

  // unsupported
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f PhysicsComponentPrivate::scale() const
{
  // unsupported, return base value
  return d_func()->m_scale;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsComponentPrivate::isAwake() const
{
  return d_func()->m_awake;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::setAwake(bool set)
{
  EGE_UNUSED(set);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::setAllowSleep(bool set)
{
  EGE_UNUSED(set)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_NULL