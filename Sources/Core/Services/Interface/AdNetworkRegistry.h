#ifndef EGE_CORE_SERVICES_ADNETWORKREGISTRY_H
#define EGE_CORE_SERVICES_ADNETWORKREGISTRY_H

/*! Registry object for all suppliers of Advertisement Network soulutions.
 */

#include "EGE.h"
#include "EGEEngine.h"
#include "EGEAdNetwork.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AdNetworkRegistry
{
  public:

    AdNetworkRegistry(Engine& engine);
    virtual ~AdNetworkRegistry();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
  
  public:
  
    /*! Returns engine reference. */
    Engine& engine() const;

  private:
  
    /*! Engine. */
    Engine& m_engine;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_ADNETWORKREGISTRY_H