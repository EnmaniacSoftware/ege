#include "EGEPhysics.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsJointDistanceBox2D_p.h"
#include "Core/Physics/Box2D/DebugDrawBox2D.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PhysicsManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsManagerPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsManagerPrivate::PhysicsManagerPrivate(PhysicsManager* base) : m_d(base)
{
  m_world = new b2World(b2Vec2(0, 0), true);
  if (m_world)
  {
    m_debugDraw = ege_new DebugDraw(base->app());
  
    if (m_debugDraw)
    {
  	  uint32 flags = b2DebugDraw::e_jointBit;
  	  m_debugDraw->SetFlags(flags);
    }

    m_world->SetDebugDraw(m_debugDraw);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsManagerPrivate::~PhysicsManagerPrivate()
{
  EGE_DELETE(m_world);
  EGE_DELETE(m_debugDraw);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void PhysicsManagerPrivate::update(const Time& time)
{
  if (isValid())
  {
    m_world->Step(1.0f / 60.0f, 8, 3);
    m_world->ClearForces();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Returns TRUE if manager is valid. */
bool PhysicsManagerPrivate::isValid() const
{
  return NULL != m_world;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Registers component. This is supposed to be called by PhysicsComponent only.
*
*  @param   component component object for which registration is to be performed.
*  @return  on successful registration, private implementation is returned. Otherwise, NULL is returned. 
*/
PhysicsComponentPrivate* PhysicsManagerPrivate::registerComponent(PhysicsComponent* component)
{
  PhysicsComponentPrivate* object = ege_new PhysicsComponentPrivate(component, world());
  if (!object->isValid())
  {
    EGE_DELETE(object);
  }

  return  object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Registers distance joint. This is supposed to be called by PhysicsJointDistance only.
*
*  @param   joint joint object for which registration is to be performed.
*  @return  on successful registration, private implementation is returned. Otherwise, NULL is returned. 
*/
PhysicsJointDistancePrivate* PhysicsManagerPrivate::registerJoint(PhysicsJointDistance* joint)
{
  PhysicsJointDistancePrivate* object = ege_new PhysicsJointDistancePrivate(joint, world());
  if (!object->isValid())
  {
    EGE_DELETE(object);
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! b2DestructionListener override. Box2D fixture is about to be destroyed. */
void PhysicsManagerPrivate::SayGoodbye(b2Fixture* fixture)
{
  EGE_UNUSED(fixture);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! b2DestructionListener override. Box2D joint is about to be destroyed. */
void PhysicsManagerPrivate::SayGoodbye(b2Joint* joint)
{
  PhysicsJointDistancePrivate* object = (PhysicsJointDistancePrivate*) joint->GetUserData();

  // clean it here cause it is going to be deallocated
  object->m_joint = NULL;

  // remove from pool
  d_func()->m_joints.remove(object->d_func());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets gravity. */
void PhysicsManagerPrivate::setGravity(const TVector4f& gravity)
{
  if (isValid())
  {
    world()->SetGravity(b2Vec2(gravity.x, gravity.y));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders data. */
void PhysicsManagerPrivate::render()
{
  if (m_world)
  {
    m_world->DrawDebugData();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
