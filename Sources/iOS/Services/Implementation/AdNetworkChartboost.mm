#include "iOS/Services/Interface/AdNetworkChartboost.h"
#include "iOS/Services/Implementation/AdNetworkChartboostDelegate.h"
#include "EGEAdNetwork.h"
#include "EGEEngine.h"
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

  // subscribe for event notifications
  if ( ! engine.eventManager()->registerListener(this))
  {
    // error!
    egeWarning(KAdNetworkDebugName) << EGE_FUNC_INFO << "Could not register for event notifications!";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetworkChartboost::~AdNetworkChartboost()
{
  [(AdNetworkChartboostDelegate*) m_delegate release];
  m_delegate = nil;

  // remove from event notifications
  engine().eventManager()->unregisterListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AdNetworkChartboost::initialize()
{
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
  }
  else
  {
    egeWarning(KAdNetworkDebugName) << "Could not initialize AdNetworkChartboost!";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AdNetworkChartboost::onEventRecieved(PEvent event)
{
  switch (event->id())
  {
    case EGE_EVENT_ID_CORE_APP_RESUME:

      initialize();
      break;

    default:
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
