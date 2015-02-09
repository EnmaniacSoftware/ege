#include "EGEEngine.h"
#include "EGEGraphics.h"
#include "EGESocialServices.h"
#include "EGEDebug.h"
#include "iOS/Services/Interface/SocialServicesIOS.h"
#include "iOS/Services/Implementation/GameCenterDelegate.h"
#include "iOS/Graphics/OpenGL/RenderWindowOGLIOS.h"
#import <GameKit/GameKit.h>
#import <UIKit/UIKit.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServicesIOS::SocialServicesIOS(Engine& engine)
: SocialServices(engine)
, m_delegate(NULL)
, m_authenticated(false)
{
  m_delegate = [[GameCenterDelegate alloc] initWithObject: this];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServicesIOS::~SocialServicesIOS()
{
  [(GameCenterDelegate*) m_delegate release];
  m_delegate = nil;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesIOS::startAuthentication()
{
  GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
  
  // start authentication
  // NOTE: this is asynchronous call
  localPlayer.authenticateHandler = ^(UIViewController* viewController, NSError* error)
  {
    // check if authentication window is required
    if (nil != viewController)
    {
      // show window
      RenderWindowOGLIOS* renderWindow = ege_cast<RenderWindowOGLIOS*>(engine().graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME));
      
      UIWindow* window = renderWindow->window();
      
      // show view
      [window.rootViewController presentViewController: viewController animated: YES completion: nil];
    }
    else if (localPlayer.isAuthenticated)
    {
      // authenticated
      m_authenticated = true;

      // store user id
      m_userId = [localPlayer.playerID UTF8String];

      egeDebug(KSocialServicesDebugName) << "User authenticated:" << m_userId;
      
      // emit
      emit authenticated(EGE_SUCCESS);
    }
    else
    {
      // cound not authenticate
      m_authenticated = false;
      
      egeWarning(KSocialServicesDebugName) << "User authentication failed.";

      // emit
      emit authenticated(EGE_ERROR);
    }
  };
  
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesIOS::loadAchievements()
{
  EGEResult result = EGE_ERROR;
  
  // check if authenticated
  if (isAuthenticated())
  {
    result = EGE_SUCCESS;
    
    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray* achievements, NSError* error)
    {
      AchievementDataList achievementsList;
      
      EGEResult result = EGE_ERROR;
      
      // check if any achievements reported
      if ((nil == error) && (nil != achievements))
      {
        // go thru all reported achievements
        for (GKAchievement* achievement in achievements)
        {
          AchievementData data;
         
          data.name               = [[achievement identifier] UTF8String];
          data.percentageComplete = static_cast<s32>([achievement percentComplete]);
          
          achievementsList.push_back(data);
        }
      }
      
      // emit
      emit achievementsLoaded(result, achievementsList);      
    }];
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesIOS::saveAchievements(const AchievementDataList& achievements)
{
  EGEResult result = EGE_ERROR;
  
  // check if authenticated
  if (isAuthenticated())
  {
    // append
    m_pendingAchievementSaveList << achievements;

    // process next achievement
    result = ((m_pendingAchievementSaveList.size() == achievements.size()) && ! achievements.empty()) ? saveNextAchievement() : EGE_SUCCESS;
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesIOS::saveNextAchievement()
{
  EGEResult result = EGE_ERROR;
  
  AchievementData achievementData = m_pendingAchievementSaveList.front();
  m_pendingAchievementSaveList.pop_front();

  egeDebug(KSocialServicesDebugName) << "Submitting achievement" << achievementData.name << "completion" << achievementData.percentageComplete;

  NSString* identifier = [NSString stringWithCString: achievementData.name.toAscii() encoding: NSASCIIStringEncoding];

  GKAchievement* achievement = [[GKAchievement alloc] initWithIdentifier: identifier];
  if (nil != achievement)
  {
    result = EGE_SUCCESS;
    
    achievement.percentComplete = static_cast<float>(achievementData.percentageComplete);
    [achievement reportAchievementWithCompletionHandler:^(NSError* error)
    {
      EGEResult result = EGE_ERROR;
      
      // check if error
      if (nil != error)
      {
        // clean up pending queue
        m_pendingAchievementSaveList.clear();
        
        // emit
        emit achievementsSaved(result);
      }
      else
      {
        // check if any achievement still in queue
        if ( ! m_pendingAchievementSaveList.empty())
        {
          // proceed
          if (EGE_SUCCESS != (result = saveNextAchievement()))
          {
            // clean up pending queue
            m_pendingAchievementSaveList.clear();
            
            // emit
            emit achievementsSaved(result);
          }
        }
        else
        {
          // done, emit
          emit achievementsSaved(result);
        }
      }
    }];
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesIOS::saveScore(const String& scoreTable, s32 score)
{
  EGEResult result = EGE_SUCCESS;
  
  // check if authenticated
  if (isAuthenticated())
  {
    result = EGE_SUCCESS;
    
    NSString* category = [NSString stringWithCString: scoreTable.toAscii() encoding: NSASCIIStringEncoding];
    
    GKScore* scoreReporter = [[GKScore alloc] initWithCategory: category];
    scoreReporter.value = score;
    scoreReporter.context = 0;
  
    [scoreReporter reportScoreWithCompletionHandler:^(NSError* error)
    {
      EGEResult result = (nil == error) ? EGE_SUCCESS : EGE_ERROR;
      
      // emit
      emit scoreSaved(result);
    }];
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesIOS::showScores(const String& scoreTable)
{
  EGEResult result = EGE_ERROR;

  // check if authenticated
  if (isAuthenticated())
  {
    result = EGE_SUCCESS;
    
    NSString* leaderboardID = [NSString stringWithCString: scoreTable.toAscii() encoding: NSASCIIStringEncoding];

    // show window
    RenderWindowOGLIOS* renderWindow = ege_cast<RenderWindowOGLIOS*>(engine().graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME));
    EGE_ASSERT(NULL != renderWindow);
    
    UIWindow* window = renderWindow->window();
    
    // create controller
    GKGameCenterViewController* gameCenterController = [[GKGameCenterViewController alloc] init];
    if (nil == gameCenterController)
    {
      result = EGE_ERROR_NO_MEMORY;
    }
    else
    {
      // setup controller
      gameCenterController.gameCenterDelegate   = (GameCenterDelegate*) m_delegate;
      gameCenterController.viewState            = GKGameCenterViewControllerStateLeaderboards;
      gameCenterController.leaderboardTimeScope = GKLeaderboardTimeScopeAllTime;
      gameCenterController.leaderboardCategory  = leaderboardID;
      gameCenterController.modalPresentationStyle = UIModalPresentationFullScreen;
      
      // show view
      [window.rootViewController presentViewController: gameCenterController animated: YES completion: nil];
    }
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SocialServicesIOS::isAuthenticated() const
{
  return m_authenticated;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
