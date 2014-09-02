#include "iOS/Services/Interface/AdNetworkChartboost.h"
#include "EGEDebug.h"

#import "Chartboost.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KChartboostAdNetworkName = "chartboots-ad-network";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetworkChartboost::AdNetworkChartboost(Engine& engine) 
: AdNetwork(engine)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetworkChartboost::~AdNetworkChartboost()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetwork* AdNetworkChartboost::Create(Engine& engine)
{
  return ege_new AdNetworkChartboost(engine);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AdNetworkChartboost::initialize()
{
  // initialize the Chartboost library
  [Chartboost startWithAppId: @"YOUR_CHARTBOOST_APP_ID" appSignature: @"YOUR_CHARTBOOST_APP_SIGNATURE" delegate: self];

  // show an interstitial ad
  [[Chartboost sharedChartboost] showInterstitial: CBLocationHomeScreen];

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
