#ifndef EGE_CORE_PHYSICSJOINTATTRACTNULL_H
#define EGE_CORE_PHYSICSJOINTATTRACTNULL_H

#ifdef EGE_PHYSICS_NULL

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsJointAttract;
class PhysicsManagerPrivate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsJointAttractPrivate
{
  public:

    PhysicsJointAttractPrivate(PhysicsJointAttract* parent, PhysicsManagerPrivate* managerPrivate);
   ~PhysicsJointAttractPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(PhysicsJointAttract)

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Sets target position to attract to. */
    void setTarget(const Vector4f& position);
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

    /*! Pointer to Box2D physics manager. */
    PhysicsManagerPrivate* m_managerPrivate;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_NULL

#endif // EGE_CORE_PHYSICSJOINTATTRACTNULL_H