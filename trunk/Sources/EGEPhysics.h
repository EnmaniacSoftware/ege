#ifndef EGE_PHYSICS_H
#define EGE_PHYSICS_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGEPhysics
{
    enum JointType
    {
      JOINT_UNKNOWN = -1,
      JOINT_DISTANCE,
    };

    enum ComponentType
    {
      COMPONENT_UNKNOWN = -1,
      COMPONENT_DYNAMIC
    };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Memory/Memory.h"
#include "Core/Components/Physics/PhysicsComponent.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Physics/PhysicsJoint.h"
#include "Core/Physics/PhysicsJointDistance.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_PHYSICS_H