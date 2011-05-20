#include "EGEPhysics.h"
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsComponentPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsComponentPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsComponentPrivate::PhysicsComponentPrivate(PhysicsComponent* parent, b2World* world) : m_body(NULL), m_world(world)
{
  b2BodyDef def;
	def.position.x  = 0;
	def.position.y  = 0;
  def.type        = b2_dynamicBody;
  
	m_body = m_world->CreateBody(&def);
  if (m_body)
  {
	  b2CircleShape shape;
	  shape.m_radius = 1;
  
    m_body->CreateFixture(&shape, 1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsComponentPrivate::~PhysicsComponentPrivate()
{
  m_world->DestroyBody(body());
  m_body = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets position vector. */
void PhysicsComponentPrivate::setPosition(const TVector4f& position)
{
  if (isValid())
  {
    body()->SetTransform(b2Vec2(position.x, position.y), body()->GetAngle());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns position vector. */
TVector4f PhysicsComponentPrivate::position() const
{
  TVector4f vec;

  if (isValid())
  {
    const b2Vec2& pos = body()->GetPosition();

    vec.x = pos.x;
    vec.y = pos.y;
    vec.z = 0;
    vec.w = 1;
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets orientation quaternion. */
void PhysicsComponentPrivate::setOrientation(const TQuaternionf& orientation)
{
  if (isValid())
  {
    //body()->SetTransform(
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns orientation quaternion. */
TQuaternionf PhysicsComponentPrivate::orientation() const
{
  TQuaternionf orient;

  if (isValid())
  {
  }

  return orient;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Applies a force vector.
* 
*  @param  force    force vector to apply.
*  @param  worldPos world position where force is to be applied. 
*/
void PhysicsComponentPrivate::applyForce(const TVector4f& force, const TVector4f& worldPos)
{
  if (isValid())
  {
    body()->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(worldPos.x, worldPos.y));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Applies an impulse at the giveb point.
* 
*  @param  force    impulse vector to apply.
*  @param  worldPos world position where impulse is to be applied. 
*  @note   This immediatly affects linear velocity.
*/
void PhysicsComponentPrivate::applyLinearImpulse(const TVector4f& impulse, const TVector4f& worldPos)
{
  if (isValid())
  {
    body()->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(worldPos.x, worldPos.y));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns force vector. */
TVector4f PhysicsComponentPrivate::force() const
{
  TVector4f vec;

  if (isValid())
  {
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets mass. */
void PhysicsComponentPrivate::setMass(EGE::float32 mass)
{
  if (isValid())
  {
    b2MassData massData;
    body()->GetMassData(&massData);

    massData.mass = mass;

    body()->SetMassData(&massData);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns mass. */
EGE::float32 PhysicsComponentPrivate::mass() const
{
  EGE::float32 mass = 0;

  if (isValid())
  {
    mass = body()->GetMass();
  }

  return mass;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets linear velocity vector. */
void PhysicsComponentPrivate::setLinearVelocity(const TVector4f& velocity)
{
  if (isValid())
  {
    body()->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns linear velocity vector. */
TVector4f PhysicsComponentPrivate::linearVelocity() const
{ 
  TVector4f vec;

  if (isValid())
  {
    b2Vec2 velocity = body()->GetLinearVelocity();

    vec.x = velocity.x;
    vec.y = velocity.y;
    vec.z = 0;
    vec.w = 1;
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if component is valid. */
bool PhysicsComponentPrivate::isValid() const
{
  return NULL != m_body;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
