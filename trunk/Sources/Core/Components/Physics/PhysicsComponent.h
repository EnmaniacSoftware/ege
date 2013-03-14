#ifndef EGE_CORE_PHYSICSCOMPONENT_H
#define EGE_CORE_PHYSICSCOMPONENT_H

/* PhysicsComponent defines object which exists in space. Can be shapeless or any number of any shapes can be assigned. 
   PhysicsComponent can be used by physics simulators.
*/

#include "EGE.h"
#include "Core/Components/Component.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Matrix4.h"
#include "Core/Physics/PhysicsJoint.h"
#include "EGEPhysics.h"
#include "EGEList.h"
#include "EGEDynamicArray.h"
#include "EGESignal.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsManager;

EGE_DECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
EGE_DECLARE_SMART_CLASS(PhysicsJoint, PPhysicsJoint)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsComponent : public IComponent
{
    /* For access to private implementation. */
    friend class PhysicsJointDistancePrivate;
    friend class PhysicsJointAttractPrivate;
    friend class PhysicsJointPulleyPrivate;
    /* For access to joint list. */
    friend class PhysicsJoint;

  public:

    /* Constructor used to create non-managable component. */
    PhysicsComponent();
    /* Constructor used to create managable component. */
    PhysicsComponent(Application* app, const String& name, EGEPhysics::ComponentType type = EGEPhysics::COMPONENT_DYNAMIC);
    virtual ~PhysicsComponent();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:

    /*! Signal emitted when any of transformation values have been changed (position, scale or orientation). */
    Signal0<> transformationChanged;

    /*! @see IComponent::isValid. */
    bool isValid() const override;
    /*! Returns component type. */
    EGEPhysics::ComponentType type() const { return m_type; }
    /*! Sets position vector. */
    void setPosition(const Vector4f& position);
    /*! Returns position vector. */
    Vector4f position() const;
    /*! Sets linear velocity vector. */
    void setLinearVelocity(const Vector4f& velocity);
    /*! Returns velocity vector. */
    Vector4f linearVelocity() const;
    /*! Applies a force vector.
     *  @param  force    force vector to apply.
     *  @param  worldPos world position where force is to be applied. 
     */
    void applyForce(const Vector4f& force, const Vector4f& worldPos);
    /*! Applies an impulse at the giveb point.
     *  @param  force    impulse vector to apply.
     *  @param  worldPos world position where impulse is to be applied. 
     *  @note   This immediatly affects linear velocity.
     */
    void applyLinearImpulse(const Vector4f& impulse, const Vector4f& worldPos);
    /*! Returns force vector. */
    Vector4f force() const;
    /*! Sets orientation quaternion. */
    void setOrientation(const Quaternionf& orientation);
    /*! Returns orientation quaternion. */
    Quaternionf orientation() const;
    /*! Sets mass. */
    void setMass(float32 mass);
    /*! Returns mass. */
    float32 mass() const;
    /*! Sets scale vector. */
    void setScale(const Vector4f& scale);
    /*! Returns scale vector. */
    Vector4f scale() const;
    /*! Returns TRUE if component is 'awaken'. Awaken components are the ones processed by solver. */
    bool isAwake() const;
    /*! Sets 'awake' state. */
    void setAwake(bool set);
    /*! Sets whether object is allowed to sleep. */
    void setAllowSleep(bool set);

    /*! Adds circular shape. */
    bool addCircleShape(float32 radius, float32 density, EGEPhysics::CollisionData colissionData = EGEPhysics::CollisionData());
    /*! Adds polygonal shape. 
     *  @param points  vertices of polygon shape.
     *  @param density shape density. Affects mass.
     */
    bool addPolygonShape(const DynamicArray<Vector4f>& points, float32 density, EGEPhysics::CollisionData colissionData = EGEPhysics::CollisionData());

    /*! Returns transformation matrix. */
    const Matrix4f& transformationMatrix() const;

  private:

    /*! Returns pointer to physics manager. */
    PhysicsManager* manager() const { return m_manager; }

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(PhysicsComponent)

    /*! Type. */
    EGEPhysics::ComponentType m_type;
    /*! Position vector. */
    Vector4f m_position;
    /*! Linear velocity vector. */
    Vector4f m_linearVelocity;
    /*! Force vector. */
    Vector4f m_force;
    /*! Orientation quaternion. */
    Quaternionf m_orientation;
    /*! Cached tranformation matrix. */
    mutable Matrix4f m_transformationMatrix;
    /*! Transformation matrix validity flag. */
    mutable bool m_transformationMatrixValid;
    /*! Mass. */
    float32 m_mass;
    /*! Scale vector. */
    Vector4f m_scale;
    /*! Awake flag. */
    bool m_awake;
    /*! Sleep allowness flag. If set component can be marked 'sleep' by solver if no processing for it is to be done to save CPU. */
    bool m_allowSleep;
    /*! List of joints attached. */
    List<PhysicsJoint*> m_joints;
    /*! Pointer to physics manager. Can be NULL if component is not managable. */
    PhysicsManager* m_manager;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSCOMPONENT_H