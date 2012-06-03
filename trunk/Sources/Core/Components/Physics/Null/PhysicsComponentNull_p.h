#ifndef EGE_CORE_PHYSICSCOMPONENTNULL_H
#define EGE_CORE_PHYSICSCOMPONENTNULL_H

#ifdef EGE_PHYSICS_NULL

#include <EGE.h>
#include <EGEVector.h>
#include <EGEDynamicArray.h>
#include <EGEPhysics.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsComponent;
class PhysicsManagerPrivate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsComponentPrivate
{
  public:

    PhysicsComponentPrivate(PhysicsComponent* parent, PhysicsManagerPrivate* managerPrivate);
   ~PhysicsComponentPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(PhysicsComponent)

    /* Returns TRUE if component is valid. */
    bool isValid() const;
    /* Sets position vector. */
    void setPosition(const Vector4f& position);
    /* Returns position vector. */
    Vector4f position() const;
    /* Sets linear velocity vector. */
    void setLinearVelocity(const Vector4f& velocity);
    /* Returns linear velocity vector. */
    Vector4f linearVelocity() const;
    /* Applies a force vector.
    * 
    *  @param  force    force vector to apply.
    *  @param  worldPos world position where force is to be applied. 
    */
    void applyForce(const Vector4f& force, const Vector4f& worldPos);
    /* Applies an impulse at the giveb point.
    * 
    *  @param  force    impulse vector to apply.
    *  @param  worldPos world position where impulse is to be applied. 
    *  @note   This immediatly affects linear velocity.
    */
    void applyLinearImpulse(const Vector4f& impulse, const Vector4f& worldPos);
    /* Returns force vector. */
    Vector4f force() const;
    /* Sets orientation quaternion. */
    void setOrientation(const Quaternionf& orientation);
    /* Returns orientation quaternion. */
    Quaternionf orientation() const;
    /* Sets mass. */
    void setMass(float32 mass);
    /* Returns mass. */
    float32 mass() const;
    /* Sets scale vector. */
    void setScale(const Vector4f& scale);
    /* Returns scale vector. */
    Vector4f scale() const;
    /* Returns TRUE if component is 'awaken'. Awaken components are the ones processed by solver. */
    bool isAwake() const;
    /* Sets 'awake' state. */
    void setAwake(bool set);
    /* Sets whether object is allowed to sleep. */
    void setAllowSleep(bool set);

    /* Adds circular shape. */
    bool addCircleShape(float32 radius, float32 density, EGEPhysics::CollisionData colissionData);
    /* Adds polygonal shape. 
    *  @param points  vertices of polygon shape.
    *  @param density shape density. Affects mass.
    */
    bool addPolygonShape(const DynamicArray<Vector4f>& points, float32 density, EGEPhysics::CollisionData colissionData);

  private:

    /*! Returns pointer to physics manager. */
    inline PhysicsManagerPrivate* manager() const { return m_managerPrivate; }

  private:

    /*! Pointer to physics manager. */
    PhysicsManagerPrivate* m_managerPrivate;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_NULL

#endif // EGE_CORE_PHYSICSCOMPONENTNULL_H