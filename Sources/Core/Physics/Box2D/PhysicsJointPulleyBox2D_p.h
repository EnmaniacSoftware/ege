#ifndef EGE_CORE_PHYSICSJOINTPULLEYBOX2D_H
#define EGE_CORE_PHYSICSJOINTPULLEYBOX2D_H

#ifdef EGE_PHYSICS_BOX2D

#include <EGE.h>

class b2PulleyJoint;

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsJointPulley;
class PhysicsManagerPrivate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsJointPulleyPrivate
{
  public:

    PhysicsJointPulleyPrivate(PhysicsJointPulley* parent, PhysicsManagerPrivate* managerPrivate);
   ~PhysicsJointPulleyPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(PhysicsJointPulley)

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns length of segment attached to body A. */
    float32 lengthA() const;
    /*! Returns length of segment attached to body B. */
    float32 lengthB() const;

  private:

    /*! Returns pointer to Box2D physics manager. */
    PhysicsManagerPrivate* manager() const { return m_managerPrivate; }

  private:

    /*! Box2D distance joint object. */
    b2PulleyJoint* m_joint;
    /*! Pointer to Box2D physics manager. */
    PhysicsManagerPrivate* m_managerPrivate;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_BOX2D

#endif // EGE_CORE_PHYSICSJOINTPULLEYBOX2D_H