#ifndef EGE_CORE_GRAPHICS_H
#define EGE_CORE_GRAPHICS_H

#include "EGEMap.h"
#include "EGESignal.h"
#include "EGEDictionary.h"
#include "EGEList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ParticleFactory;
class WidgetFactory;
class RenderSystem;
class IRenderer;
class IHardwareResourceProvider;

EGE_DECLARE_SMART_CLASS(Graphics, PGraphics)
EGE_DECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
EGE_DECLARE_SMART_CLASS(RenderWindow, PRenderWindow)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Graphics : public Object
{
  public:

    Graphics(Application* app, const Dictionary& params);
    virtual ~Graphics();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:

    /*! Signal emitted just before target is rendered. 
     *  @param renderTarget Render target for which the rendering will be done.
     */
    Signal1<PRenderTarget> preRender;
    /*! Signal emitted just after target has been rendered. 
     *  @param renderTarget Render target for which the rendering was done.
     */
    Signal1<PRenderTarget> postRender;

  public:

    /*! Creates object. */
    EGEResult construct();
    /*! Updates object. */
    void update();
    /*! Renders all registered targets. */
    void render();
    /*! Returns renderer interface. */
    IRenderer* renderer();
    /*! Returns hardware resource provider interface. */
    IHardwareResourceProvider* hardwareResourceProvider();
    /*! Returns render system. */
    RenderSystem* renderSystem() const;
    /*! Removes render target with the given name from registered pool. */
    void removeRenderTarget(const String& name);
    /*! Returns pointer to particle factory. */
    ParticleFactory* particleFactory() const;
    /*! Returns pointer to widget factory. */
    WidgetFactory* widgetFactory() const;
    /*! Registers render target for use. */
    void registerRenderTarget(PRenderTarget target);
    /*! Returns render target with the given name from registered pool. */
    PRenderTarget renderTarget(const String& name) const;
    /*! Enables/disables rendering. */
    void setRenderingEnabled(bool set);

  private:

    /*! Unregisteres all render targets. */
    void unregisterAllRenderTargets();

  private:

    typedef MultiMap<s32, PRenderTarget> RenderTargetMap;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Graphics)

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
inline RenderSystem* Graphics::renderSystem() const 
{ 
  return m_renderSystem; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline ParticleFactory* Graphics::particleFactory() const 
{ 
  return m_particleFactory; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline WidgetFactory* Graphics::widgetFactory() const 
{ 
  return m_widgetFactory; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_H