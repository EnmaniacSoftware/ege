#ifndef EGE_CORE_RESOUECE_RESOURCEMANAGER_SINGLETHREAD_H
#define EGE_CORE_RESOUECE_RESOURCEMANAGER_SINGLETHREAD_H

/*! Single threaded implementation for resource manager class.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGEMap.h"
#include "EGEList.h"
#include "EGETime.h"
#include "Core/Resource/Implementation/ResourceManager.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
extern const char* KResourceManagerSingleThreadName;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(IResource, PResource)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerSingleThread : public ResourceManager
{
  public:

    ResourceManagerSingleThread(Engine& engine);
   ~ResourceManagerSingleThread();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates instance of this implementation of ResourceManager interface. 
     *  @param  engine  Reference to engine.
     *  @return Created instance. NULL if error occured.
     */
    static EngineModule<IResourceManager>* Create(Engine& engine);

  private:

    /*! @see IResourceManager::loadGroup. */
    EGEResult loadGroup(const String& name) override;
    /*! @see IResourceManager::unloadGroup. */
    void unloadGroup(const String& name) override;

    /*! @see ResourceManager::processCommands. */
    void processCommands() override;
    /*! @see ResourceManager::resourceProcessPolicy. */
    ResourceManager::ResourceProcessPolicy resourceProcessPolicy() const override;

    /*! @see EngineModule::update. */
    void update(const Time& time) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOUECE_RESOURCEMANAGER_SINGLETHREAD_H
