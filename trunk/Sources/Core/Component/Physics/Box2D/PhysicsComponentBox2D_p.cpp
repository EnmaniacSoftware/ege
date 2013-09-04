#ifdef EGE_PHYSICS_BOX2D

#include "Core/Application/Application.h"
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include <EGEPhysics.h>
#include <Box2D/Box2D.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsComponentPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsComponentPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps EGE component type to Box2D. */
enum b2BodyType mapType(EGEPhysics::ComponentType type)
{
  switch (type)
  {
    case EGEPhysics::COMPONENT_STATIC:    return b2_staticBody;
    case EGEPhysics::COMPONENT_KINEMATIC: return b2_kinematicBody;
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
  def.allowSleep  = d_func()->m_allowSleep;
  def.awake       = d_func()->m_awake;

	m_body = manager()->world()->CreateBody(&def);

  m_body->SetUserData(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsComponentPrivate::~PhysicsComponentPrivate()
{
  manager()->world()->DestroyBody(body());
  m_body = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::setPosition(const Vector4f& position)
{
  if (isValid())
  {
    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    body()->SetTransform(b2Vec2(position.x * scale, position.y * scale), body()->GetAngle());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f PhysicsComponentPrivate::position() const
{
  Vector4f vec;

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
void PhysicsComponentPrivate::setOrientation(const Quaternionf& orientation)
{
  if (isValid())
  {
    body()->SetTransform(body()->GetPosition(), orientation.angle().radians());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Quaternionf PhysicsComponentPrivate::orientation() const
{
  Quaternionf orient;

  if (isValid())
  {
    // NOTE: we take base orientation Z value into account here as each rotation in 3D space can be represented by 2 opposit quaternions and we want to pick up
    //       right one
    orient.create((0 <= d_func()->m_orientation.z) ? Vector3f::UNIT_Z : Vector3f::NEGATIVE_UNIT_Z, Angle::FromRadians(body()->GetAngle()));
  }

  return orient;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::applyForce(const Vector4f& force, const Vector4f& worldPos)
{
  if (isValid())
  {
    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    body()->ApplyForce(b2Vec2(force.x * scale, force.y * scale), b2Vec2(worldPos.x * scale, worldPos.y * scale));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::applyLinearImpulse(const Vector4f& impulse, const Vector4f& worldPos)
{
  if (isValid())
  {
    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    body()->ApplyLinearImpulse(b2Vec2(impulse.x * scale, impulse.y * scale), b2Vec2(worldPos.x * scale, worldPos.y * scale));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f PhysicsComponentPrivate::force() const
{
  Vector4f vec;

  if (isValid())
  {
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
void PhysicsComponentPrivate::setLinearVelocity(const Vector4f& velocity)
{
  if (isValid())
  {
    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    body()->SetLinearVelocity(b2Vec2(velocity.x * scale, velocity.y * scale));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f PhysicsComponentPrivate::linearVelocity() const
{ 
  Vector4f vec;

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
bool PhysicsComponentPrivate::isValid() const
{
  return NULL != m_body;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsComponentPrivate::addCircleShape(EGE::float32 radius, EGE::float32 density, EGEPhysics::CollisionData colissionData)
{
  if (isValid())
  {
    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    b2CircleShape shape;
	  shape.m_radius = radius * scale;

    b2Filter filter;
		filter.groupIndex   = colissionData.policy;
		filter.categoryBits = colissionData.group;
		filter.maskBits     = colissionData.collideGroup;

    b2Fixture* fixture = m_body->CreateFixture(&shape, density);
    if (fixture)
    { 
      fixture->SetFilterData(filter);
    }

    return NULL != fixture;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsComponentPrivate::addPolygonShape(const DynamicArray<Vector4f>& points, EGE::float32 density, EGEPhysics::CollisionData colissionData)
{
  if (isValid())
  {
    EGE_ASSERT(b2_maxPolygonVertices >= points.size());

    EGE::float32 scale = manager()->worldToSimulationScaleFactor();

    // copy points into Box2D compilant form
    b2Vec2 vertices[b2_maxPolygonVertices];
    for (s32 i = 0; i < (s32) points.size(); ++i)
    {
      vertices[i].Set(points[i].x * scale, points[i].y * scale);
    }

	  b2PolygonShape shape;
	  shape.Set(vertices, (s32) points.size());
  
    b2Filter filter;
		filter.groupIndex   = colissionData.policy;
		filter.categoryBits = colissionData.group;
		filter.maskBits     = colissionData.collideGroup;

    b2Fixture* fixture = m_body->CreateFixture(&shape, density);
    if (fixture)
    { 
      fixture->SetFilterData(filter);
    }

    return NULL != fixture;
  }

  return true;
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
  if (isValid())
  {
    return body()->IsAwake();
  }

  return d_func()->m_awake;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::setAwake(bool set)
{
  if (isValid())
  {
    body()->SetAwake(set);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponentPrivate::setAllowSleep(bool set)
{
  if (isValid())
  {
    body()->SetSleepingAllowed(set);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_BOX2D