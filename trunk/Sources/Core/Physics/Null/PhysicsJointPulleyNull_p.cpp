#ifdef EGE_PHYSICS_NULL

#include "Core/Application/Application.h"
#include "Core/Physics/Null/PhysicsJointPulleyNull_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsJointPulleyPrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointPulleyPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointPulleyPrivate::PhysicsJointPulleyPrivate(PhysicsJointPulley* parent, PhysicsManagerPrivate* managerPrivate) : m_d(parent), 
                                                                                                                          m_managerPrivate(managerPrivate)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointPulleyPrivate::~PhysicsJointPulleyPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns length of segment attached to body A. */
float32 PhysicsJointPulleyPrivate::lengthA() const
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns length of segment attached to body B. */
float32 PhysicsJointPulleyPrivate::lengthB() const
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool PhysicsJointPulleyPrivate::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_NULL