#ifndef EGE_CORE_SERVICES_ADNETWORK_NULL_H
#define EGE_CORE_SERVICES_ADNETWORK_NULL_H

/*! NULL implementation of Advertisement Network soulutions.
 */

#include "EGEAdNetwork.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AdNetworkNull : public AdNetwork
{
  public:

    AdNetworkNull(Engine& engine);
   ~AdNetworkNull();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_ADNETWORK_NULL_H
