#ifndef EGE_CORE_PHYSICSJOINTDISTANCEBOX2D_H
#define EGE_CORE_PHYSICSJOINTDISTANCEBOX2D_H

#include "EGE.h"
#include "Box2D/Box2D.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsJointDistance;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PhysicsJointDistancePrivate
{
  /* To access m_joint data. */
  friend class PhysicsManagerPrivate;

  public:

    PhysicsJointDistancePrivate(PhysicsJointDistance* parent, b2World* world);
   ~PhysicsJointDistancePrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(PhysicsJointDistance)

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Sets natural length. */
    void setLength(float32 length);
    /* Returns natural length. */
    float32 length() const;

  private:

    /*! Pointer to Box2D world object. */
    b2World* m_world;
    /*! Box2D distance joint object. */
    b2DistanceJoint* m_joint;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSJOINTDISTANCEBOX2D_H