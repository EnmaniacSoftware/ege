#ifndef EGE_CORE_GRAPHICS_H
#define EGE_CORE_GRAPHICS_H

#include "EGEMultiMap.h"
#include "EGEDictionary.h"
#include "EGEList.h"
#include "EGETime.h"
#include "Core/Engine/Interface/EngineModule.h"
#include "Core/Graphics/Interface/IGraphics.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ParticleFactory;
class RenderSystem;
class WidgetFactory;
class IRenderSystem;
class IRenderer;
class IHardwareResourceProvider;

EGE_DECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
EGE_DECLARE_SMART_CLASS(RenderWindow, PRenderWindow)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Graphics : public EngineModule<IGraphics>
{
  public:

    Graphics(Engine& engine, const Dictionary& params);
    virtual ~Graphics();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates object. */
    EGEResult construct();

  private:

    /*! @see IGraphics::render. */
    void render() override;
    /*! @see IGraphics::renderer. */
    IRenderer* renderer() override;
    /*! @see IGraphics::hardwareResourceProvider. */
    IHardwareResourceProvider* hardwareResourceProvider() override;
    /*! @see IGraphics::renderSystem. */
    IRenderSystem* renderSystem() const override;
    /*! @see IGraphics::removeRenderTarget. */
    void removeRenderTarget(const String& name) override;
    /*! @see IGraphics::particleFactory. */
    ParticleFactory* particleFactory() const override;
    /*! @see IGraphics::widgetFactory. */
    WidgetFactory* widgetFactory() const override;
    /*! @see IGraphics::registerRenderTarget. */
    void registerRenderTarget(PRenderTarget target) override;
    /*! @see IGraphics::renderTarget. */
    PRenderTarget renderTarget(const String& name) const override;
    /*! @see IGraphics::setRenderingEnabled. */
    void setRenderingEnabled(bool set) override;

    /*! Unregisteres all render targets. */
    void unregisterAllRenderTargets();
    /*! Returns engine object. */
    Engine& engine() const;

    /*! @see EngineModule::uid. */
    u32 uid() const override;
    /*! @see EngineModule::update. */
    void update(const Time& time) override;

  private:

    typedef MultiMap<s32, PRenderTarget> RenderTargetMap;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Graphics)

    /*! Reference to engine. */
    Engine& m_engine;
    /*! Renderer system. */
    RenderSystem* m_renderSystem;
    /*! Render targets sorted by priority. */
    RenderTargetMap m_renderTargets; 
    /*! Particles factory. */
    ParticleFactory* m_particleFactory;
    /*! Widgets factory. */
    WidgetFactory* m_widgetFactory;
    /*! Rendering enabled flag. */
    bool m_renderingEnabled;
    /*! Creation parameters. */
    Dictionary m_params;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_H