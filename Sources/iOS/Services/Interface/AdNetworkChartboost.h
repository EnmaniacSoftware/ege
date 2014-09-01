#ifndef EGE_IOS_SERVICES_ADNETWORK_CHARTBOOST_H
#define EGE_IOS_SERVICES_ADNETWORK_CHARTBOOST_H

/*! NULL implementation of Advertisement Network soulutions.
 */

#include "EGEAdNetwork.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
extern const char* KChartboostAdNetworkName;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AdNetworkChartboost : public AdNetwork
{
  public:

    AdNetworkChartboost(Engine& engine);
   ~AdNetworkChartboost();

  public:

    /*! Creates instance of this implementation of AdNetwork interface. 
     *  @param  engine  Reference to engine.
     *  @return Created instance. NULL if error occured.
     */
    static AdNetwork* Create(Engine& engine);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_SERVICES_ADNETWORK_CHARTBOOST_H