#include "Core/Application/Application.h"
#include "Core/Physics/PhysicsJointAttract.h"
#include "Core/Physics/Box2D/PhysicsJointAttractBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsJointAttractPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointAttractPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointAttractPrivate::PhysicsJointAttractPrivate(PhysicsJointAttract* parent, b2World* world) : m_d(parent), m_joint(NULL), m_world(world)
{
  b2Body* body = d_func()->bodyA()->p_func()->body();

  // create ground body
	b2BodyDef bodyDef;
	m_groundBody = m_world->CreateBody(&bodyDef);
  if (m_groundBody)
  {
    // define joint
    b2MouseJointDef def;

    def.maxForce = 1000000.0f;
    def.bodyA = m_groundBody;
    def.bodyB = body;
    def.frequencyHz = 1;
    def.dampingRatio = 1;
    def.target = body->GetPosition();

    // create joint implementation
    m_joint = (b2MouseJoint*) m_world->CreateJoint(&def);

    m_joint->SetUserData(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointAttractPrivate::~PhysicsJointAttractPrivate()
{
  if (m_joint)
  {
    m_world->DestroyJoint(m_joint);
    m_joint = NULL;
  }

  if (m_groundBody)
  {
    m_world->DestroyBody(m_groundBody);
    m_groundBody = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool PhysicsJointAttractPrivate::isValid() const
{
  return NULL != m_joint;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets target position to attract to. */
void PhysicsJointAttractPrivate::setTarget(const TVector4f& position)
{
  if (isValid())
  {
    m_joint->SetTarget(b2Vec2(position.x, position.y));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
