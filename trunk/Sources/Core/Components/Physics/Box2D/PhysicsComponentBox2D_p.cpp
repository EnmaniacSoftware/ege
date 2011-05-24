#include "Core/Application/Application.h"
#include "EGEPhysics.h"
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsComponentPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsComponentPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps EGE component type to Box2D. */
enum b2BodyType mapType(EGEPhysics::EComponentType type)
{
  switch (type)
  {
    case EGEPhysics::COMPONENT_STATIC: return b2_staticBody;
  }

  return b2_dynamicBody;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsComponentPrivate::PhysicsComponentPrivate(PhysicsComponent* parent, PhysicsManagerPrivate* managerPrivate) : m_d(parent), m_body(NULL), 
                                                                                                                    m_managerPrivate(managerPrivate)
{
  b2BodyDef def;
	def.position.x  = 0;
	def.position.y  = 0;
  def.type        = mapType(parent->type());
  
	m_body = manager()->world()->CreateBody(&def);

  //if (m_body)
  //{
	 // b2CircleShape shape;
	 // shape.m_radius = 0;
  //
  //  m_body->CreateFixture(&shape, 0);
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsComponentPrivate::~PhysicsComponentPrivate()
{
  manager()->world()->DestroyBody(body());
  m_body = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets position vector. */
void PhysicsComponentPrivate::setPosition(const TVector4f& position)
{
  if (isValid())
  {
    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    body()->SetTransform(b2Vec2(position.x * scale, position.y * scale), body()->GetAngle());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns position vector. */
TVector4f PhysicsComponentPrivate::position() const
{
  TVector4f vec;

  if (isValid())
  {
    EGE::float32 scale = manager()->simulationToWorldScaleFactor();

    const b2Vec2& pos = body()->GetPosition();

    vec.x = pos.x * scale;
    vec.y = pos.y * scale;
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
    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    body()->ApplyForce(b2Vec2(force.x * scale, force.y * scale), b2Vec2(worldPos.x * scale, worldPos.y * scale));
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
    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    body()->ApplyLinearImpulse(b2Vec2(impulse.x * scale, impulse.y * scale), b2Vec2(worldPos.x * scale, worldPos.y * scale));
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
    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    body()->SetLinearVelocity(b2Vec2(velocity.x * scale, velocity.y * scale));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns linear velocity vector. */
TVector4f PhysicsComponentPrivate::linearVelocity() const
{ 
  TVector4f vec;

  if (isValid())
  {
    EGE::float32 scale = manager()->simulationToWorldScaleFactor();

    b2Vec2 velocity = body()->GetLinearVelocity();

    vec.x = velocity.x * scale;
    vec.y = velocity.y * scale;
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
