#ifndef EGE_CORE_PHYSICSMANAGER_H
#define EGE_CORE_PHYSICSMANAGER_H

#include "EGE.h"
#include "EGETime.h"
#include "EGEDictionary.h"
#include "EGEVector4.h"
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

    PhysicsManager(Application* app);
    virtual ~PhysicsManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates object. */
    EGEResult construct(const Dictionary& params);
    /*! Updates manager. */
    void update(const Time& time);
    /*! Renders data. */
    void render();
    /*! Sets gravity. */
    void setGravity(const Vector4f& gravity);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(PhysicsManager)
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSMANAGER_H