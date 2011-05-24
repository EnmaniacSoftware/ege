#ifndef EGE_PHYSICS_H
#define EGE_PHYSICS_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGEPhysics
{
    enum EComponentType
    {
      COMPONENT_DYNAMIC,        // positive mass, non-zero velocity determined by forces, moved by solver
      COMPONENT_STATIC          // zero mass, zero velocity, may be manually moved
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
#include "Core/Physics/PhysicsJointAttract.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_PHYSICS_H