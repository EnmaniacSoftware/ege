#ifndef EGE_CORE_TESTFRAMEWORK_ENGINE_MOCK_H
#define EGE_CORE_TESTFRAMEWORK_ENGINE_MOCK_H

#include <gmock/gmock.h>
#include <EGEEngine.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineMock : public Engine
{
  public:

    MOCK_METHOD1(construct, EGEResult(const Dictionary&));

    MOCK_CONST_METHOD0(configurationDictionary, const Dictionary&());
    MOCK_CONST_METHOD0(graphics, IGraphics*());
    MOCK_CONST_METHOD0(eventManager, IEventManager*());
    MOCK_CONST_METHOD0(physicsManager, IPhysicsManager*());
    MOCK_CONST_METHOD0(sceneManager, ISceneManager*());
    MOCK_CONST_METHOD0(resourceManager, IResourceManager*());
    MOCK_CONST_METHOD0(pointer, Pointer*());
    MOCK_CONST_METHOD0(overlayManager, IOverlayManager*());
    MOCK_CONST_METHOD0(screenManager, IScreenManager*());
    MOCK_CONST_METHOD0(audioManager, IAudioManager*());
    MOCK_CONST_METHOD0(debug, Debug*());
    MOCK_CONST_METHOD0(deviceServices, DeviceServices*());
    MOCK_CONST_METHOD0(purchaseServices, PurchaseServices*());
    MOCK_CONST_METHOD0(socialServices, SocialServices*());
    MOCK_CONST_METHOD0(imageLoader, ImageLoader*());
    MOCK_CONST_METHOD0(resourceFactory, ResourceFactory*());

    MOCK_METHOD1(setLanguage, void(const String&));
    MOCK_CONST_METHOD0(language, const String&());
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_CORE_TESTFRAMEWORK_ENGINE_MOCK_H