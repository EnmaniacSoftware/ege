#ifndef EGE_CORE_PHYSICSMANAGER_H
#define EGE_CORE_PHYSICSMANAGER_H

#include "EGE.h"
#include "EGETime.h"
#include "EGEDictionary.h"
#include "EGEVector4.h"
#include "Core/Physics/PhysicsJoint.h"
#include "Core/Physics/Interface/IPhysicsManager.h"
#include "Core/Engine/Interface/EngineModule.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsJointDistance;
class PhysicsJointDistancePrivate;
class PhysicsJointAttract;
class PhysicsJointAttractPrivate;
class PhysicsJointPulley;
class PhysicsJointPulleyPrivate;
class PhysicsComponent;
class PhysicsComponentPrivate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsManager : public EngineModule<IPhysicsManager>
{
    /* For registration. */
    friend class PhysicsComponent;
    friend class PhysicsJointDistance;
    friend class PhysicsJointAttract;
    friend class PhysicsJointPulley;

  public:

    PhysicsManager(const Dictionary& params);
    virtual ~PhysicsManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Sets gravity. */
    void setGravity(const Vector4f& gravity);

  private:

    /*! @see IPhysicsManager::render. */
    void render() override;

    /*! @see EngineModule::construct. */
    EGEResult construct() override;
    /*! @see EngineModule::uid. */
    u32 uid() const override;
    /*! @see EngineModule::update. */
    void update(const Time& time) override;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(PhysicsManager)

    /*! Parameters. */
    Dictionary m_params;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PHYSICSMANAGER_H