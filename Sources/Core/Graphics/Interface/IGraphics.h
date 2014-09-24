#ifndef EGE_CORE_GRAPHICS_GRAPHICS_INTERFACE_H
#define EGE_CORE_GRAPHICS_GRAPHICS_INTERFACE_H

#include "EGEString.h"
#include "EGESignal.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ParticleFactory;
class WidgetFactory;
class IRenderSystem;
class IRenderer;
class IHardwareResourceProvider;

EGE_DECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IGraphics
{
  public:

    virtual ~IGraphics() {}

  public signals:

    /*! Signal emitted just before target is rendered. 
     *  @param renderTarget Render target for which the rendering will be done.
     */
    Signal1<PRenderTarget> preRender;
    /*! Signal emitted just after target has been rendered. 
     *  @param renderTarget Render target for which the rendering was done.
     */
    Signal1<PRenderTarget> postRender;
    /*! Signal emitted before rendering starts. */
    Signal0<> renderStart;
    /*! Signal emitted after rendering ends. */
    Signal0<> renderEnd;

  public:

    /*! Renders all registered targets. TAGE - should be removed. */
    virtual void render() = 0;

    /*! Returns renderer interface. */
    virtual IRenderer* renderer() = 0;
    /*! Returns hardware resource provider interface. */
    virtual IHardwareResourceProvider* hardwareResourceProvider() = 0;
    /*! Returns render system. */
    virtual IRenderSystem* renderSystem() const = 0;
    /*! Removes render target with the given name from registered pool. */
    virtual void removeRenderTarget(const String& name) = 0;
    /*! Returns pointer to particle factory. */
    virtual ParticleFactory* particleFactory() const = 0;
    /*! Returns pointer to widget factory. */
    virtual WidgetFactory* widgetFactory() const = 0;
    /*! Registers render target for use. */
    virtual void registerRenderTarget(PRenderTarget target) = 0;
    /*! Returns render target with the given name from registered pool. */
    virtual PRenderTarget renderTarget(const String& name) const = 0;
    /*! Enables/disables rendering. */
    virtual void setRenderingEnabled(bool set) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_GRAPHICS_INTERFACE_H