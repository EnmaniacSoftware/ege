#ifndef EGE_CORE_PHYSICSJOINTDISTANCEBOX2D_H
#define EGE_CORE_PHYSICSJOINTDISTANCEBOX2D_H

#ifdef EGE_PHYSICS_BOX2D

#include <EGE.h>

class b2DistanceJoint;

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsJointDistance;
class PhysicsManagerPrivate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsJointDistancePrivate
{
  public:

    PhysicsJointDistancePrivate(PhysicsJointDistance* parent, PhysicsManagerPrivate* managerPrivate);
   ~PhysicsJointDistancePrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(PhysicsJointDistance)

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Sets natural length. */
    void setLength(float32 length);
    /*! Returns natural length. */
    float32 length() const;
    /*! Sets damping ratio.
     *  @param   ration Damping ratio to set. 0 - no damping, 1 - critical damping.
     */
    void setDampingRatio(float32 ratio);
    /*! Returns current damping ratio. */
    float32 dampingRatio() const;
    /*! Sets mass-spring-damper frequency (in Hz). 
     *  @note  This describes the speed of response of the system.
     */
    void setFrequency(float32 frequencyHz);
    /*! Returns current mass-spring-damper frequency (in Hz). */
    float32 frequency() const;

  private:

    /*! Returns pointer to Box2D physics manager. */
    PhysicsManagerPrivate* manager() const { return m_managerPrivate; }

  private:

    /*! Box2D distance joint object. */
    b2DistanceJoint* m_joint;
    /*! Pointer to Box2D physics manager. */
    PhysicsManagerPrivate* m_managerPrivate;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_BOX2D

#endif // EGE_CORE_PHYSICSJOINTDISTANCEBOX2D_H