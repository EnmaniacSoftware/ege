#ifndef EGE_CORE_PHYSICSMANAGERNULL_H
#define EGE_CORE_PHYSICSMANAGERNULL_H

#ifdef EGE_PHYSICS_NULL

#include "EGE.h"
#include "EGETime.h"
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
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsManagerPrivate
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
    /*! Returns world to simulation world scale coefficient. */
    float32 worldToSimulationScaleFactor() const { return m_scale; }
    /*! Returns simulation world to world scale coefficient. */
    float32 simulationToWorldScaleFactor() const { return m_invScale; }

  private:

    /*! Extrnal world to simulation world scale coeficient. */
    float32 m_scale;
    /*! Simulation world to extrnal world scale coeficient. Inverse of scale factor. */
    float32 m_invScale;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_NULL

#endif // EGE_CORE_PHYSICSMANAGERNULL_H