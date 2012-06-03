#ifdef EGE_PHYSICS_NULL

#include "Core/Application/Application.h"
#include "Core/Physics/Null/PhysicsJointDistanceNull_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PhysicsJointDistancePrivate)
EGE_DEFINE_DELETE_OPERATORS(PhysicsJointDistancePrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistancePrivate::PhysicsJointDistancePrivate(PhysicsJointDistance* parent, PhysicsManagerPrivate* managerPrivate) : m_d(parent),  
                                                                                                                                m_managerPrivate(managerPrivate)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsJointDistancePrivate::~PhysicsJointDistancePrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets natural length. */
void PhysicsJointDistancePrivate::setLength(float32 length)
{
  EGE_UNUSED(length);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns natural length. */
float32 PhysicsJointDistancePrivate::length() const
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool PhysicsJointDistancePrivate::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets damping ratio.
*
*  @param   ration Damping ratio to set. 0 - no damping, 1 - critical damping.
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PhysicsJointDistancePrivate::setDampingRatio(float32 ratio)
{
  EGE_UNUSED(ratio);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current damping ratio. */
float32 PhysicsJointDistancePrivate::dampingRatio() const
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets mass-spring-damper frequency (in Hz). 
*  @note  This describes the speed of response of the system.
*/
void PhysicsJointDistancePrivate::setFrequency(float32 frequencyHz)
{
  EGE_UNUSED(frequencyHz);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current mass-spring-damper frequency (in Hz). */
float32 PhysicsJointDistancePrivate::frequency() const
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_NULL