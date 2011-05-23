#ifndef EGE_CORE_PHYSICSJOINTATTRACTBOX2D_H
#define EGE_CORE_PHYSICSJOINTATTRACTBOX2D_H

#include "EGE.h"
#include "Box2D/Box2D.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsJointAttract;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsJointAttractPrivate
{
  /* To access m_joint data. */
  friend class PhysicsManagerPrivate;

  public:

    PhysicsJointAttractPrivate(PhysicsJointAttract* parent, b2World* world);
   ~PhysicsJointAttractPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(PhysicsJointAttract)

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Sets target position to attract to. */
    void setTarget(const TVector4f& position);

  private:

    /*! Pointer to Box2D world object. */
    b2World* m_world;
    /*! Box2D mouse joint object. */
    b2MouseJoint* m_joint;
    /*! Dummy ground Box2D body. */
    b2Body* m_groundBody;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSJOINTATTRACTBOX2D_H