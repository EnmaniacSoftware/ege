#ifndef EGE_CORE_PHYSICSCOMPONENT_H
#define EGE_CORE_PHYSICSCOMPONENT_H

#include "EGE.h"
#include "Core/Components/Component.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Vector4.h"
#include "Core/Physics/PhysicsJoint.h"
#include "EGEPhysics.h"
#include "EGEList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsManager;

EGE_DECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
EGE_DECLARE_SMART_CLASS(PhysicsJoint, PPhysicsJoint)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsComponent : public IComponent
{
    /* Private joint implementations may need access to private implementation of component. */
    friend class PhysicsJointDistancePrivate;
    friend class PhysicsJointAttractPrivate;
    /* For access to joint list. */
    friend class PhysicsJoint;

  public:

    PhysicsComponent(Application* app, const EGEString& name, EGEPhysics::EComponentType type = EGEPhysics::COMPONENT_DYNAMIC);
    virtual ~PhysicsComponent();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* IComponent override. Returns TRUE if component is valid. */
    virtual bool isValid() const override;
    /*! Returns component type. */
    inline EGEPhysics::EComponentType type() const { return m_type; }
    /* Sets position vector. */
    void setPosition(const TVector4f& position);
    /* Returns position vector. */
    TVector4f position() const;
    /* Sets linear velocity vector. */
    void setLinearVelocity(const TVector4f& velocity);
    /* Returns velocity vector. */
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
    /*! Returns orientation quaternion. */
    TQuaternionf orientation() const;
    /* Sets mass. */
    void setMass(float32 mass);
    /* Returns mass. */
    float32 mass() const;

  private:

    /*! Returns pointer to physics manager. */
    inline PhysicsManager* manager() const { return m_manager; }

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(PhysicsComponent)

    /*! Type. */
    EGEPhysics::EComponentType m_type;
    /*! Position vector. */
    TVector4f m_position;
    /*! Linear velocity vector. */
    TVector4f m_linearVelocity;
    /*! Force vector. */
    TVector4f m_force;
    /*! Orientation quaternion. */
    TQuaternionf m_orientation;
    /*! Mass. */
    float32 m_mass;
    /*! List of joints attached. */
    EGEList<PhysicsJoint*> m_joints;
    /*! Pointer to physics manager. */
    PhysicsManager* m_manager;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSCOMPONENT_H