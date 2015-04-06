#ifndef EGE_CORE_PHYSICSJOINT_H
#define EGE_CORE_PHYSICSJOINT_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(PhysicsJoint, PPhysicsJoint)
EGE_PREDECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsJoint : public Object
{
  public:

    virtual ~PhysicsJoint();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
  
    /*! Returns first body attached to joint. */
    PPhysicsComponent bodyA() const { return m_bodyA; }
    /*! Returns second body attached to joint. */
    PPhysicsComponent bodyB() const { return m_bodyB; }

  protected:

    PhysicsJoint(PPhysicsComponent bodyA, PPhysicsComponent bodyB, u32 uid);

  private:

    /*! First attached body. */
    PPhysicsComponent m_bodyA;
    /*! Second attached body. */
    PPhysicsComponent m_bodyB;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSJOINT_H