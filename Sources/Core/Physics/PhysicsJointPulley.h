#ifndef EGE_CORE_PHYSICSJOINTPULLEY_H
#define EGE_CORE_PHYSICSJOINTPULLEY_H

#include "EGE.h"
#include "EGEVector4.h"
#include "Core/Physics/PhysicsJoint.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Engine;

EGE_PREDECLARE_SMART_CLASS(PhysicsJointPulley, PPhysicsJointPulley)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsJointPulley : public PhysicsJoint
{
  public:

    PhysicsJointPulley(Engine& engine, PPhysicsComponent bodyA, PPhysicsComponent bodyB, float32 maxLengthA, float32 maxLengthB, 
                       const Vector4f& groundAnchorA, const Vector4f& groundAnchorB);
   ~PhysicsJointPulley();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
  
    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns length of segment attached to body A. */
    float32 lengthA() const;
    /*! Returns length of segment attached to body B. */
    float32 lengthB() const;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(PhysicsJointPulley)

    /*! Max length of segment attached to body A. For use by private implementation. */
    float32 m_maxLengthA;
    /*! Max length of segment attached to body B. For use by private implementation. */
    float32 m_maxLengthB;
    /*! Ground point in world coordinates for segment A. For use by private implementation. */
    Vector4f m_groundAnchorA;
    /*! Ground point in world coordinates for segment B. For use by private implementation. */
    Vector4f m_groundAnchorB;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSJOINTPULLEY_H