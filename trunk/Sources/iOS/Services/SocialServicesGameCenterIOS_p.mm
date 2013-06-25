#if EGE_SOCIAL_PLATFORM_GAMECENTER

#include "EGEApplication.h"
#include "EGEGraphics.h"
#include "Core/Services/SocialServices.h"
#include "iOS/Services/SocialServicesGameCenterIOS_p.h"
#include "iOS/Services/GameCenterDelegate.h"
#include "iOS/Graphics/OpenGL/RenderWindowOGLIOS.h"
#include "EGEDebug.h"
#import <GameKit/GameKit.h>
#import <UIKit/UIKit.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SocialServicesPrivate)
EGE_DEFINE_DELETE_OPERATORS(SocialServicesPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServicesPrivate::SocialServicesPrivate(SocialServices* base) : m_d(base),
                                                                     m_delegate(NULL),
                                                                     m_gameCenterController(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServicesPrivate::~SocialServicesPrivate()
{
  [(GameCenterDelegate*) m_delegate release];
  m_delegate = nil;
  
  [(GKGameCenterViewController*) m_gameCenterController release];
  m_gameCenterController = nil;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::construct()
{
  m_delegate = [[GameCenterDelegate alloc] initWithObject: this];
  if (nil == m_delegate)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  
  m_gameCenterController = [[GKGameCenterViewController alloc] init];
  if (nil == m_gameCenterController)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  
  // set delegate
  ((GKGameCenterViewController*) m_gameCenterController).gameCenterDelegate = (GameCenterDelegate*) m_delegate;
  
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::startAuthentication()
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
      RenderWindowOGLIOS* renderWindow = ege_cast<RenderWindowOGLIOS*>(d_func()->app()->graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME));
      
      UIWindow* window = renderWindow->window();
      
      // show view
      [window.rootViewController presentViewController: viewController animated: YES completion: nil];
    }
    else if (localPlayer.isAuthenticated)
    {
      // authenticated
      d_func()->m_authenticated = true;

      // store user id
      m_userId = [localPlayer.playerID UTF8String];

      egeDebug() << "User authenticated:" << m_userId;
      
      // emit
      emit d_func()->authenticated(EGE_SUCCESS);
    }
    else
    {
      // cound not authenticate
      d_func()->m_authenticated = false;
      
      egeWarning() << "User authentication failed.";

      // emit
      emit d_func()->authenticated(EGE_ERROR);
    }
  };
  
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::loadAchievements()
{
  EGEResult result = EGE_ERROR;
  
  // check if authenticated
  if (d_func()->isAuthenticated())
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
      emit d_func()->achievementsLoaded(result, achievementsList);      
    }];
    
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::saveAchievements(const AchievementDataList& achievements)
{
  EGEResult result = EGE_ERROR;
  
  // check if authenticated
  if (d_func()->isAuthenticated())
  {
    result = EGE_SUCCESS;

    // append
    m_pendingAchievementSaveList << achievements;

    // process next achievement
    result = ((m_pendingAchievementSaveList.size() == achievements.size()) && ! achievements.empty()) ? saveNextAchievement() : EGE_SUCCESS;
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::saveNextAchievement()
{
  EGEResult result = EGE_ERROR;
  
  AchievementData achievementData = m_pendingAchievementSaveList.front();
  m_pendingAchievementSaveList.pop_front();

  egeDebug() << "Submitting achievement" << achievementData.name << "completion" << achievementData.percentageComplete;

  NSString* identifier = [NSString stringWithCString: achievementData.name.c_str() encoding: NSASCIIStringEncoding];

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
        emit d_func()->achievementsSaved(result);
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
            emit d_func()->achievementsSaved(result);
          }
        }
        else
        {
          // done, emit
          emit d_func()->achievementsSaved(result);
        }
      }
    }];
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::saveScore(const String& scoreTable, s32 score)
{
  EGEResult result = EGE_SUCCESS;
  
  // check if authenticated
  if (d_func()->isAuthenticated())
  {
    result = EGE_SUCCESS;
    
    NSString* category = [NSString stringWithCString: scoreTable.c_str() encoding: NSASCIIStringEncoding];
    
    GKScore* scoreReporter = [[GKScore alloc] initWithCategory: category];
    scoreReporter.value = score;
    scoreReporter.context = 0;
  
    [scoreReporter reportScoreWithCompletionHandler:^(NSError* error)
    {
      EGEResult result = (nil == error) ? EGE_SUCCESS : EGE_ERROR;
      
      // emit
      emit d_func()->scoreSaved(result);
    }];
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::showScores(const String& scoreTable)
{
  EGEResult result = EGE_ERROR;

  // check if authenticated
  if (d_func()->isAuthenticated())
  {
    result = EGE_SUCCESS;

    NSString* leaderboardID = [NSString stringWithCString: scoreTable.c_str() encoding: NSASCIIStringEncoding];

    // retrieve root view controller
    UIViewController* rootController = [[[[UIApplication sharedApplication] delegate] window] rootViewController];

    ((GKGameCenterViewController*) m_gameCenterController).viewState            = GKGameCenterViewControllerStateLeaderboards;
    ((GKGameCenterViewController*) m_gameCenterController).leaderboardTimeScope = GKLeaderboardTimeScopeAllTime;
    ((GKGameCenterViewController*) m_gameCenterController).leaderboardCategory  = leaderboardID;
 
    // show view
    [rootController presentViewController: (GKGameCenterViewController*) m_gameCenterController animated: YES completion: nil];
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_SOCIAL_PLATFORM_GAMECENTER