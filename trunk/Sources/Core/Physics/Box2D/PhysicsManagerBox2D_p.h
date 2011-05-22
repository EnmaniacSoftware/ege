#ifndef EGE_CORE_PHYSICSMANAGERBOX2D_H
#define EGE_CORE_PHYSICSMANAGERBOX2D_H

#include "EGE.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Physics/PhysicsJointDistance.h"
#include "Core/Components/Physics/PhysicsComponent.h"
#include "Core/Timer/Time.h"
#include "Box2D/Box2d.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
EGE_DECLARE_SMART_CLASS(PhysicsJointDistance, PPhysicsJointDistance)

class PhysicsManager;
class DebugDraw;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsManagerPrivate : public b2DestructionListener
{
  public:

    PhysicsManagerPrivate(PhysicsManager* base);
   ~PhysicsManagerPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(PhysicsManager)

    /* Returns TRUE if manager is valid. */
    bool isValid() const;
    /* Updates manager. */
    void update(const Time& time);
    /* Renders data. */
    void render();
    /* Registers component. This is supposed to be called by PhysicsComponent only.
    *
    *  @param   component component object for which registration is to be performed.
    *  @return  on successful registration, private implementation is returned. Otherwise, NULL is returned. 
    */
    PhysicsComponentPrivate* registerComponent(PhysicsComponent* component);
    /* Registers distance joint. This is supposed to be called by PhysicsJointDistance only.
    *
    *  @param   joint joint object for which registration is to be performed.
    *  @return  on successful registration, private implementation is returned. Otherwise, NULL is returned. 
    */
    PhysicsJointDistancePrivate* registerJoint(PhysicsJointDistance* joint);
    /* Sets gravity. */
    void setGravity(const TVector4f& gravity);

  private:

    /*! Returns Box2D world. */
    inline b2World* world() const { return m_world; }
    /* b2DestructionListener override. Box2D fixture is about to be destroyed. */
  	void SayGoodbye(b2Fixture* fixture) override;
    /* b2DestructionListener override. Box2D joint is about to be destroyed. */
  	void SayGoodbye(b2Joint* joint) override;

  private:

    /*! Box2D world. */
    b2World* m_world;
    /*! Debug draw for Box2D entities. */
    DebugDraw* m_debugDraw;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSMANAGERODE_H