#ifndef EGE_CORE_PHYSICSJOINTDISTANCE_H
#define EGE_CORE_PHYSICSJOINTDISTANCE_H

#include "EGE.h"
#include "Core/Physics/PhysicsJoint.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(PhysicsJointDistance, PPhysicsJointDistance)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsJointDistance : public PhysicsJoint
{
  public:

    PhysicsJointDistance(Application* app, PPhysicsComponent bodyA, PPhysicsComponent bodyB);
    virtual ~PhysicsJointDistance();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
  
    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Sets natural length. */
    void setLength(float32 length);
    /* Returns natural length. */
    float32 length() const;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(PhysicsJointDistance)
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSJOINTDISTANCE_H