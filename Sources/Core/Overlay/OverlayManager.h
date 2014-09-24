#ifndef EGE_CORE_OVERLAYMANAGER_H
#define EGE_CORE_OVERLAYMANAGER_H

#include "EGE.h"
#include "EGEString.h"
#include "EGEList.h"
#include "EGETime.h"
#include "EGEOverlay.h"
#include "Core/Engine/Interface/EngineModule.h"
#include "Core/Overlay/Interface/IOverlayManager.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
class Viewport;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class OverlayManager : public EngineModule<IOverlayManager>
{
  public: 

    OverlayManager();
   ~OverlayManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  private:

    /*! @see EngineModule::uid. */
    u32 uid() const override;
    /*! @see EngineModule::update. */
    void update(const Time& time) override;

    /*! @see IOverlayManager::render. */
    void render(Viewport* viewport, IRenderer* renderer) override;
    /*! @see IOverlayManager::add. */
    EGEResult add(const POverlay& overlay) override;
    /*! @see IOverlayManager::remove. */
    void remove(const String& name) override;
    /*! @see IOverlayManager::remove. */
    void remove(const POverlay& overlay) override;
    /*! @see IOverlayManager::removeAll. */
    void removeAll() override;
    /*! @see IOverlayManager::overlay. */
    POverlay overlay(const String& name) const override;

  private:

    typedef List<POverlay> OverlayList;

  private:

    /*! Pool of overlays. */
    OverlayList m_overlays;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_OVERLAYMANAGER_H
