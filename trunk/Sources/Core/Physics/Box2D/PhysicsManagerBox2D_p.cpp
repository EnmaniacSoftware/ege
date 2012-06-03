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
PhysicsManagerPrivate::PhysicsManagerPrivate(PhysicsManager* base, const Dictionary& params) :  m_d(base), 
                                                                                                m_scale(1.0f), 
                                                                                                m_invScale(1.0f)
{
  bool error = false;

  // decompose param list
  m_scale = params.value(EGE_PHYSICS_PARAM_SCALE_FACTOR, "1").toFloat(&error);
  if (!error)
  {
    m_invScale = 1.0f / m_scale;

    m_world = new b2World(b2Vec2(0, 0), true);
    if (m_world)
    {
      m_debugDraw = ege_new DebugDraw(base->app(), this);
  
      if (m_debugDraw)
      {
  	    uint32 flags = b2DebugDraw::e_jointBit | b2DebugDraw::e_shapeBit;
  	    m_debugDraw->SetFlags(flags);
      }

      m_world->SetDebugDraw(m_debugDraw);
    }
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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Returns TRUE if manager is valid. */
bool PhysicsManagerPrivate::isValid() const
{
  return NULL != m_world;
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
  EGE_UNUSED(joint);

  //PhysicsJointDistancePrivate* object = (PhysicsJointDistancePrivate*) joint->GetUserData();

  //// clean it here cause it is going to be deallocated
  //object->m_joint = NULL;

  //// remove from pool
  //d_func()->m_joints.remove(object->d_func());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets gravity. */
void PhysicsManagerPrivate::setGravity(const Vector4f& gravity)
{
  if (isValid())
  {
    world()->SetGravity(b2Vec2(gravity.x * worldToSimulationScaleFactor(), gravity.y * worldToSimulationScaleFactor()));
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

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_BOX2D