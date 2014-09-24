#ifndef EGE_CORE_PHYSICS_PHYSICSMANAGER_INTERFACE_H
#define EGE_CORE_PHYSICS_PHYSICSMANAGER_INTERFACE_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IPhysicsManager
{
  public:

    virtual ~IPhysicsManager() {}

    /*! Renders data. TAGE - should be probably removed */
    virtual void render() = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICS_PHYSICSMANAGER_INTERFACE_H