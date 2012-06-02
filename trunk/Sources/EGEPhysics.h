#ifndef EGE_PHYSICS_H
#define EGE_PHYSICS_H

#include <EGETypes.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace EGEPhysics
{
    /*! Available physics component types. */
    enum ComponentType
    {
      COMPONENT_DYNAMIC,        /*!< positive mass, non-zero velocity determined by forces, moved by solver. */
      COMPONENT_STATIC,         /*!< zero mass, zero velocity, may be manually moved. */
      COMPONENT_KINEMATIC       /*!< zero mass, non-zero velocity set by user, moved by solver. */
    };

    /*! Collidion data structure. */
    struct CollisionData
    {
      CollisionData() : group(0), collideGroup(0), policy(-1) {}

      /*! Colission group bit mask. This is group given object belongs to. */
      u16 group;
      /*! Colission enable bit mask. This is group given object can collide with. */
      u16 collideGroup;
      /*! Colission policy. The following values are allowed: 
       *  -1 - never collide. Discard rest of data.
       *   0 - collide exactly according to settings. 
       *   1 - Always collide. Discard rest of data.
       */
      u16 policy;
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
#include "Core/Physics/PhysicsJointPulley.h"

#ifdef EGE_PHYSICS_BOX2D
#include "Core/Components/Physics/Box2D/PhysicsComponentBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsJointDistanceBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsJointAttractBox2D_p.h"
#include "Core/Physics/Box2D/PhysicsJointPulleyBox2D_p.h"
#elif defined EGE_PHYSICS_NULL
#include "Core/Components/Physics/Null/PhysicsComponentNull_p.h"
#include "Core/Physics/Null/PhysicsManagerNull_p.h"
#include "Core/Physics/Null/PhysicsJointDistanceNull_p.h"
#include "Core/Physics/Null/PhysicsJointAttractNull_p.h"
#include "Core/Physics/Null/PhysicsJointPulleyNull_p.h"
#endif // EGE_PHYSICS_BOX2D

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_PHYSICS_H