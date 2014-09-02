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

  public:

    /*! Creates instance of this implementation of AdNetwork interface. 
     *  @param  engine  Reference to engine.
     *  @return Created instance. NULL if error occured.
     */
    static AdNetwork* Create(Engine& engine);

  private:

    /*! @see AdNetwork::initialize. */
    bool initialize() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_ADNETWORK_NULL_H
