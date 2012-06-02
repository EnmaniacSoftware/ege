#ifdef EGE_PHYSICS_NULL

#include <EGEPhysics.h>
#include <EGEDebug.h>

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
/*! Sets position vector. */
void PhysicsComponentPrivate::setPosition(const Vector4f& position)
{
  EGE_UNUSED(position);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns position vector. */
Vector4f PhysicsComponentPrivate::position() const
{
  return d_func()->m_position;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets orientation quaternion. */
void PhysicsComponentPrivate::setOrientation(const Quaternionf& orientation)
{
  EGE_UNUSED(orientation);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns orientation quaternion. */
Quaternionf PhysicsComponentPrivate::orientation() const
{
  return d_func()->m_orientation;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Applies a force vector.
* 
*  @param  force    force vector to apply.
*  @param  worldPos world position where force is to be applied. 
*/
void PhysicsComponentPrivate::applyForce(const Vector4f& force, const Vector4f& worldPos)
{
  EGE_UNUSED(force);
  EGE_UNUSED(worldPos);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Applies an impulse at the giveb point.
* 
*  @param  force    impulse vector to apply.
*  @param  worldPos world position where impulse is to be applied. 
*  @note   This immediatly affects linear velocity.
*/
void PhysicsComponentPrivate::applyLinearImpulse(const Vector4f& impulse, const Vector4f& worldPos)
{
  EGE_UNUSED(impulse);
  EGE_UNUSED(worldPos);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns force vector. */
Vector4f PhysicsComponentPrivate::force() const
{
  return d_func()->m_force;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets mass. */
void PhysicsComponentPrivate::setMass(float32 mass)
{
  EGE_UNUSED(mass);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns mass. */
float32 PhysicsComponentPrivate::mass() const
{
  return d_func()->m_mass;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets linear velocity vector. */
void PhysicsComponentPrivate::setLinearVelocity(const Vector4f& velocity)
{
  EGE_UNUSED(velocity);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns linear velocity vector. */
Vector4f PhysicsComponentPrivate::linearVelocity() const
{ 
  return d_func()->m_linearVelocity;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if component is valid. */
bool PhysicsComponentPrivate::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds circular shape. */
bool PhysicsComponentPrivate::addCircleShape(float32 radius, float32 density, EGEPhysics::CollisionData colissionData)
{
  EGE_UNUSED(radius);
  EGE_UNUSED(density);
  EGE_UNUSED(colissionData);

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Adds polygonal shape. 
*  @param points  vertices of polygon shape.
*  @param density shape density. Affects mass.
*/
bool PhysicsComponentPrivate::addPolygonShape(const DynamicArray<Vector4f>& points, float32 density, EGEPhysics::CollisionData colissionData)
{
  EGE_UNUSED(points);
  EGE_UNUSED(density);
  EGE_UNUSED(colissionData);

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets scale vector. */
void PhysicsComponentPrivate::setScale(const Vector4f& scale)
{
  EGE_UNUSED(scale);

  // unsupported
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns scale vector. */
Vector4f PhysicsComponentPrivate::scale() const
{
  // unsupported, return base value
  return d_func()->m_scale;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if component is 'awaken'. Awaken components are the ones processed by solver. */
bool PhysicsComponentPrivate::isAwake() const
{
  return d_func()->m_awake;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets 'awake' state. */
void PhysicsComponentPrivate::setAwake(bool set)
{
  EGE_UNUSED(set);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets whether object is allowed to sleep. */
void PhysicsComponentPrivate::setAllowSleep(bool set)
{
  EGE_UNUSED(set)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_NULL