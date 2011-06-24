#ifndef EGE_CORE_PHYSICSMANAGERBOX2D_H
#define EGE_CORE_PHYSICSMANAGERBOX2D_H

#include <EGE.h>
#include <EGETime.h>
#include <Box2D/Box2d.h>
#include "Core/Physics/PhysicsManager.h"
#include "Core/Physics/PhysicsJointDistance.h"
#include "Core/Physics/PhysicsJointAttract.h"
#include "Core/Physics/PhysicsJointPulley.h"
#include "Core/Components/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
EGE_DECLARE_SMART_CLASS(PhysicsJointDistance, PPhysicsJointDistance)
EGE_DECLARE_SMART_CLASS(PhysicsJointAttract, PPhysicsJointAttract)
EGE_DECLARE_SMART_CLASS(PhysicsJointPulley, PPhysicsJointPulley)

class PhysicsManager;
class DebugDraw;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsManagerPrivate : public b2DestructionListener
{
  public:

    PhysicsManagerPrivate(PhysicsManager* base, const ConfigParams& params);
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
    /* Registers attract joint. This is supposed to be called by PhysicsJointAttract only.
    *
    *  @param   joint joint object for which registration is to be performed.
    *  @return  on successful registration, private implementation is returned. Otherwise, NULL is returned. 
    */
    PhysicsJointAttractPrivate* registerJoint(PhysicsJointAttract* joint);
    /* Registers pulley joint. This is supposed to be called by PhysicsJointPulley only.
    *
    *  @param   joint joint object for which registration is to be performed.
    *  @return  on successful registration, private implementation is returned. Otherwise, NULL is returned. 
    */
    PhysicsJointPulleyPrivate* registerJoint(PhysicsJointPulley* joint);
    /* Sets gravity. */
    void setGravity(const Vector4f& gravity);
    /*! Returns Box2D world. */
    inline b2World* world() const { return m_world; }
    /*! Returns world to simulation world scale coefficient. */
    inline float32 worldToSimulationScaleFactor() const { return m_scale; }
    /*! Returns simulation world to world scale coefficient. */
    inline float32 simulationToWorldScaleFactor() const { return m_invScale; }

  private:

    /* b2DestructionListener override. Box2D fixture is about to be destroyed. */
  	void SayGoodbye(b2Fixture* fixture) override;
    /* b2DestructionListener override. Box2D joint is about to be destroyed. */
  	void SayGoodbye(b2Joint* joint) override;

  private:

    /*! Box2D world. */
    b2World* m_world;
    /*! Debug draw for Box2D entities. */
    DebugDraw* m_debugDraw;
    /*! Extrnal world to simulation world scale coeficient. */
    float32 m_scale;
    /*! Simulation world to extrnal world scale coeficient. Inverse of scale factor. */
    float32 m_invScale;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSMANAGERODE_H