#ifndef EGE_CORE_PHYSICSJOINTATTRACTBOX2D_H
#define EGE_CORE_PHYSICSJOINTATTRACTBOX2D_H

#include <EGE.h>
#include <Box2D/Box2D.h>

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

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Sets target position to attract to. */
    void setTarget(const Vector4f& position);

  private:

    /*! Returns pointer to Box2D physics manager. */
    inline PhysicsManagerPrivate* manager() const { return m_managerPrivate; }

  private:

    /*! Box2D mouse joint object. */
    b2MouseJoint* m_joint;
    /*! Dummy ground Box2D body. */
    b2Body* m_groundBody;
    /*! Pointer to Box2D physics manager. */
    PhysicsManagerPrivate* m_managerPrivate;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSJOINTATTRACTBOX2D_H