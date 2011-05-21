#ifndef EGE_CORE_PHYSICSMANAGER_H
#define EGE_CORE_PHYSICSMANAGER_H

#include "EGE.h"
#include "Core/Timer/Time.h"
#include "Core/Math/Vector4.h"
#include "Core/Physics/PhysicsJoint.h"
#include "EGEList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsJointDistance;
class PhysicsJointDistancePrivate;
class PhysicsComponent;
class PhysicsComponentPrivate;

EGE_DECLARE_SMART_CLASS(PhysicsManager, PPhysicsManager)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsManager : public Object
{
    /* For registration. */
    friend class PhysicsComponent;
    friend class PhysicsJointDistance;

  public:

    PhysicsManager(Application* app);
    virtual ~PhysicsManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if manager is valid. */
    bool isValid() const;
    /* Updates manager. */
    void update(const Time& time);
    /* Sets gravity. */
    void setGravity(const TVector4f& gravity);

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

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(PhysicsManager)

    /*! List of all joints. This list is merly for keeping reference count up. */
    EGEList<PPhysicsJoint> m_joints;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSMANAGER_H