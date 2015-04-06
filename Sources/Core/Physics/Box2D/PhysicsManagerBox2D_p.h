#ifndef EGE_CORE_PHYSICSMANAGERBOX2D_H
#define EGE_CORE_PHYSICSMANAGERBOX2D_H

#ifdef EGE_PHYSICS_BOX2D

#include <EGE.h>
#include <EGETime.h>
#include <Box2D/Box2d.h>
#include "Core/Physics/PhysicsManager.h"
#include "Core/Physics/PhysicsJointDistance.h"
#include "Core/Physics/PhysicsJointAttract.h"
#include "Core/Physics/PhysicsJointPulley.h"
#include "Core/Component/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
EGE_PREDECLARE_SMART_CLASS(PhysicsJointDistance, PPhysicsJointDistance)
EGE_PREDECLARE_SMART_CLASS(PhysicsJointAttract, PPhysicsJointAttract)
EGE_PREDECLARE_SMART_CLASS(PhysicsJointPulley, PPhysicsJointPulley)

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

    /*! Creates object. */
    EGEResult construct(const Dictionary& params);
    /*! Updates manager. */
    void update(const Time& time);
    /*! Renders data. */
    void render();
    /*! Sets gravity. */
    void setGravity(const Vector4f& gravity);
    /*! Returns Box2D world. */
    b2World* world() const { return m_world; }
    /*! Returns world to simulation world scale coefficient. */
    float32 worldToSimulationScaleFactor() const { return m_scale; }
    /*! Returns simulation world to world scale coefficient. */
    float32 simulationToWorldScaleFactor() const { return m_invScale; }

  private:

    /*! b2DestructionListener override. Box2D fixture is about to be destroyed. */
  	void SayGoodbye(b2Fixture* fixture) override;
    /*! b2DestructionListener override. Box2D joint is about to be destroyed. */
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

#endif // EGE_PHYSICS_BOX2D

#endif // EGE_CORE_PHYSICSMANAGERODE_H