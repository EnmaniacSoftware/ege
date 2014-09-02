#include "iOS/Services/Interface/AdNetworkChartboost.h"
#include "iOS/Services/Implementation/AdNetworkChartboostDelegate.h"
#include "EGEDebug.h"

#import <UIKit/UIKit.h>
#import "Chartboost.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KChartboostAdNetworkName = "chartboost-ad-network";

static const NSString* KEGEChartboostAppIdKey        = @"ege-chartboost-app-id";
static const NSString* KEGEChartboostAppSignatureKey = @"ege-chartboost-app-signature";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetworkChartboost::AdNetworkChartboost(Engine& engine) 
: AdNetwork(engine)
, m_delegate(nil)
{
  m_delegate = [[AdNetworkChartboostDelegate alloc] initWithObject: this];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetworkChartboost::~AdNetworkChartboost()
{
  [(AdNetworkChartboostDelegate*) m_delegate release];
  m_delegate = nil;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetwork* AdNetworkChartboost::Create(Engine& engine)
{
  return ege_new AdNetworkChartboost(engine);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AdNetworkChartboost::initialize()
{
  bool result = false;
  
  // retrieve data from .plist file
  NSDictionary* infoDictionary = [[NSBundle mainBundle] infoDictionary];
  
  NSString* appId        = [infoDictionary objectForKey: KEGEChartboostAppIdKey];
  NSString* appSignature = [infoDictionary objectForKey: KEGEChartboostAppSignatureKey];
  
  if ((nil != appId) && (nil != appSignature))
  {
    // initialize the Chartboost library
    [Chartboost startWithAppId: appId appSignature: appSignature delegate: (AdNetworkChartboostDelegate*) m_delegate];
    
    // show an interstitial ad
    [[Chartboost sharedChartboost] showInterstitial: CBLocationHomeScreen];
    
    // done
    result = true;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
