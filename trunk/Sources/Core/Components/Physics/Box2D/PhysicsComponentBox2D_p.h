#ifndef EGE_CORE_PHYSICSCOMPONENTBOX2D_H
#define EGE_CORE_PHYSICSCOMPONENTBOX2D_H

#include "EGE.h"
#include "Box2D/Box2D.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsComponent;

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
    void setPosition(const TVector4f& position);
    /* Returns position vector. */
    TVector4f position() const;
    /* Sets linear velocity vector. */
    void setLinearVelocity(const TVector4f& velocity);
    /* Returns linear velocity vector. */
    TVector4f linearVelocity() const;
    /* Applies a force vector.
    * 
    *  @param  force    force vector to apply.
    *  @param  worldPos world position where force is to be applied. 
    */
    void applyForce(const TVector4f& force, const TVector4f& worldPos);
    /* Applies an impulse at the giveb point.
    * 
    *  @param  force    impulse vector to apply.
    *  @param  worldPos world position where impulse is to be applied. 
    *  @note   This immediatly affects linear velocity.
    */
    void applyLinearImpulse(const TVector4f& impulse, const TVector4f& worldPos);
    /* Returns force vector. */
    TVector4f force() const;
    /* Sets orientation quaternion. */
    void setOrientation(const TQuaternionf& orientation);
    /* Returns orientation quaternion. */
    TQuaternionf orientation() const;
    /* Sets mass. */
    void setMass(float32 mass);
    /* Returns mass. */
    float32 mass() const;
    /*! Returns Box2D representation. */
    inline b2Body* body() const { return m_body; }

  private:

    /*! Returns pointer to Box2D physics manager. */
    inline PhysicsManagerPrivate* manager() const { return m_managerPrivate; }

  private:

    /*! Pointer to Box2D body object representation. */
    b2Body* m_body;
    /*! Pointer to Box2D physics manager. */
    PhysicsManagerPrivate* m_managerPrivate;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSCOMPONENTBOX2D_H