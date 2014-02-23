#ifdef EGE_PHYSICS_BOX2D

#include "Core/Application/Application.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"
#include "Core/Physics/Box2D/DebugDrawBox2D.h"
#include <Box2D/Box2D.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsManagerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsManagerPrivate::PhysicsManagerPrivate(PhysicsManager* base) : m_d(base), 
                                                                     m_scale(1.0f), 
                                                                     m_invScale(1.0f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsManagerPrivate::~PhysicsManagerPrivate()
{
  EGE_DELETE(m_world);
  EGE_DELETE(m_debugDraw);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PhysicsManagerPrivate::construct(const Dictionary& params)
{
  bool error = false;

  // decompose param list
  m_scale = params.value(EGE_PHYSICS_PARAM_SCALE_FACTOR, "1").toFloat(&error);
  if ( ! error)
  {
    m_invScale = 1.0f / m_scale;
  }

  // create world
  m_world = new b2World(b2Vec2(0, 0), true);
  if (NULL == m_world)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create debug drawer
  m_debugDraw = ege_new DebugDraw(base->app(), this);
  if (NULL == m_debugDraw)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  uint32 flags = b2DebugDraw::e_jointBit | b2DebugDraw::e_shapeBit;
  m_debugDraw->SetFlags(flags);

  m_world->SetDebugDraw(m_debugDraw);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsManagerPrivate::update(const Time& time)
{
  m_world->Step(time.seconds(), 8, 3);
  m_world->ClearForces();

  // go thru all bodies and emit tranformation changes
  // NOTE: this is due no way we can detect only the bodies which has been really changed in Box2D :-/
  b2Body* node = m_world->GetBodyList();
  while (node)
  {
    b2Body* b = node;
    node = node->GetNext();

    PhysicsComponentPrivate* component = (PhysicsComponentPrivate*) b->GetUserData();
    if (component)
    {
      emit component->d_func()->transformationChanged();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsManagerPrivate::SayGoodbye(b2Fixture* fixture)
{
  EGE_UNUSED(fixture);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsManagerPrivate::SayGoodbye(b2Joint* joint)
{
  EGE_UNUSED(joint);

  //PhysicsJointDistancePrivate* object = (PhysicsJointDistancePrivate*) joint->GetUserData();

  //// clean it here cause it is going to be deallocated
  //object->m_joint = NULL;

  //// remove from pool
  //d_func()->m_joints.remove(object->d_func());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsManagerPrivate::setGravity(const Vector4f& gravity)
{
  world()->SetGravity(b2Vec2(gravity.x * worldToSimulationScaleFactor(), gravity.y * worldToSimulationScaleFactor()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsManagerPrivate::render()
{
  m_world->DrawDebugData();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_BOX2D