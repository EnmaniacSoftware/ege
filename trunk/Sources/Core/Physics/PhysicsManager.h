#ifndef EGE_CORE_PHYSICSMANAGER_H
#define EGE_CORE_PHYSICSMANAGER_H

#include <EGE.h>
#include <EGETime.h>
#include "Core/ConfigParams.h"
#include "Core/Math/Vector4.h"
#include "Core/Physics/PhysicsJoint.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsJointDistance;
class PhysicsJointDistancePrivate;
class PhysicsJointAttract;
class PhysicsJointAttractPrivate;
class PhysicsJointPulley;
class PhysicsJointPulleyPrivate;
class PhysicsComponent;
class PhysicsComponentPrivate;

EGE_DECLARE_SMART_CLASS(PhysicsManager, PPhysicsManager)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsManager : public Object
{
    /* For registration. */
    friend class PhysicsComponent;
    friend class PhysicsJointDistance;
    friend class PhysicsJointAttract;
    friend class PhysicsJointPulley;

  public:

    PhysicsManager(Application* app, const ConfigParams& params);
    virtual ~PhysicsManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if manager is valid. */
    bool isValid() const;
    /* Updates manager. */
    void update(const Time& time);
    /* Renders data. */
    void render();
    /* Sets gravity. */
    void setGravity(const Vector4f& gravity);

  private:

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

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(PhysicsManager)
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSMANAGER_H