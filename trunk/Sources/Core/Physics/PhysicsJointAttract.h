#ifndef EGE_CORE_PHYSICSJOINTATTRACT_H
#define EGE_CORE_PHYSICSJOINTATTRACT_H

#include "EGE.h"
#include "Core/Physics/PhysicsJoint.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(PhysicsJointAttract, PPhysicsJointAttract)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsJointAttract : public PhysicsJoint
{
  public:

    PhysicsJointAttract(Application* app, PPhysicsComponent bodyA, PPhysicsComponent bodyB);
    virtual ~PhysicsJointAttract();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
  
    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Sets target position to attract to. */
    void setTarget(const TVector4f& position);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(PhysicsJointAttract)
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSJOINTATTRACT_H