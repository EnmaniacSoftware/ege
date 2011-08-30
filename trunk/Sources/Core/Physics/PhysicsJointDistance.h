#ifndef EGE_CORE_PHYSICSJOINTDISTANCE_H
#define EGE_CORE_PHYSICSJOINTDISTANCE_H

#include <EGE.h>
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
    /* Sets damping ratio.
    *
    *  @param   ration Damping ratio to set. 0 - no damping, 1 - critical damping.
    */
    void setDampingRatio(float32 ratio);
    /* Returns current damping ratio. */
    float32 dampingRatio() const;
    /* Sets mass-spring-damper frequency (in Hz). 
    *  @note  This describes the speed of response of the system.
    */
    void setFrequency(float32 frequencyHz);
    /* Returns current mass-spring-damper frequency (in Hz). */
    float32 frequency() const;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(PhysicsJointDistance)
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSJOINTDISTANCE_H