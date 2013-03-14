#include "Core/Application/Application.h"
#include "EGEPhysics.h"
#include "EGEDebug.h"

#if EGE_PHYSICS_BOX2D
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"
#elif EGE_PHYSICS_NULL
#include "Core/Components/Physics/Null/PhysicsComponentNull_p.h"
#endif // EGE_PHYSICS_BOX2D

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsComponent)
EGE_DEFINE_DELETE_OPERATORS(PhysicsComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructor used to create non-managable component. */
PhysicsComponent::PhysicsComponent() : IComponent(NULL, EGE_OBJECT_UID_PHYSICS_COMPONENT, ""),
                                       m_p(NULL),
                                       m_type(EGEPhysics::COMPONENT_STATIC), 
                                       m_position(Vector4f::ZERO), 
                                       m_linearVelocity(Vector4f::ZERO), 
                                       m_force(Vector4f::ZERO), 
                                       m_orientation(Quaternionf::IDENTITY),
                                       m_transformationMatrix(Matrix4f::IDENTITY),
                                       m_transformationMatrixValid(false),
                                       m_mass(1.0f), 
                                       m_scale(Vector4f::ONE), 
                                       m_awake(true), 
                                       m_allowSleep(true),
                                       m_manager(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructor used to create managable component. */
PhysicsComponent::PhysicsComponent(Application* app, const String& name, EGEPhysics::ComponentType type) 
: IComponent(app, EGE_OBJECT_UID_PHYSICS_COMPONENT, name), 
  m_type(type), 
  m_position(Vector4f::ZERO), 
  m_linearVelocity(Vector4f::ZERO), 
  m_force(Vector4f::ZERO), 
  m_orientation(Quaternionf::IDENTITY),
  m_transformationMatrix(Matrix4f::IDENTITY),
  m_transformationMatrixValid(false),
  m_mass(1.0f), 
  m_scale(Vector4f::ONE), 
  m_awake(true), 
  m_allowSleep(true)
{
  m_manager = app->physicsManager();

  // create private thru manager
  m_p = ege_new PhysicsComponentPrivate(this, app->physicsManager()->p_func());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsComponent::~PhysicsComponent()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponent::setPosition(const Vector4f& position)
{
  EGE_ASSERT(isValid());
  if (p_func())
  {
    p_func()->setPosition(position);
  }
  
  m_position = position;

  // invalidate transformation matrix
  m_transformationMatrixValid = false;

  emit transformationChanged();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f PhysicsComponent::position() const
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return p_func()->position();
  }

  return m_position;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponent::setOrientation(const Quaternionf& orientation)
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->setOrientation(orientation);
  }

  m_orientation = orientation;

  // invalidate transformation matrix
  m_transformationMatrixValid = false;

  emit transformationChanged();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Quaternionf PhysicsComponent::orientation() const
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->orientation();
  }

  return m_orientation;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponent::applyForce(const Vector4f& force, const Vector4f& worldPos)
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->applyForce(force, worldPos);
  }

  m_force += force;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponent::applyLinearImpulse(const Vector4f& impulse, const Vector4f& worldPos)
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->applyLinearImpulse(impulse, worldPos);
  }

  m_linearVelocity += impulse * (1.0f / mass());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f PhysicsComponent::force() const
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->force();
  }

  return m_force;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponent::setMass(EGE::float32 mass)
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->setMass(mass);
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
EGE::float32 PhysicsComponent::mass() const
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->mass();
  }

  return m_mass;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponent::setLinearVelocity(const Vector4f& velocity)
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->setLinearVelocity(velocity);
  }

  m_linearVelocity = velocity;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f PhysicsComponent::linearVelocity() const
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->linearVelocity();
  }

  return m_linearVelocity;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsComponent::isValid() const
{
  return ((NULL != m_manager) && (NULL != m_p) && p_func()->isValid()) || (NULL == m_manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsComponent::addCircleShape(EGE::float32 radius, EGE::float32 density, EGEPhysics::CollisionData colissionData)
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->addCircleShape(radius, density, colissionData);
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsComponent::addPolygonShape(const DynamicArray<Vector4f>& points, EGE::float32 density, EGEPhysics::CollisionData colissionData)
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->addPolygonShape(points, density, colissionData);
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponent::setScale(const Vector4f& scale)
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->setScale(scale);
  }

  m_scale = scale;

  // invalidate transformation matrix
  m_transformationMatrixValid = false;

  emit transformationChanged();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f PhysicsComponent::scale() const
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->scale();
  }

  return m_scale;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PhysicsComponent::isAwake() const
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->isAwake();
  }

  return m_awake;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponent::setAwake(bool set)
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->setAwake(set);
  }

  m_awake = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsComponent::setAllowSleep(bool set)
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->setAllowSleep(set);
  }

  m_allowSleep = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Matrix4f& PhysicsComponent::transformationMatrix() const
{
  if (!m_transformationMatrixValid)
  {
    // update cached matrix
    Math::CreateMatrix(&m_transformationMatrix, &m_position, &m_scale, &m_orientation);

    // reser flag
    m_transformationMatrixValid = true;
  }

  return m_transformationMatrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END