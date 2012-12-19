#ifndef EGE_CORE_GRAPHICS_H
#define EGE_CORE_GRAPHICS_H

#include <EGEMap.h>
#include <EGESignal.h>
#include <EGEDictionary.h>
#include <EGEVertexBuffer.h>
#include <EGEIndexBuffer.h>
#include "Core/Graphics/Render/Renderer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ParticleFactory;
class WidgetFactory;

EGE_DECLARE_SMART_CLASS(Graphics, PGraphics)
EGE_DECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
EGE_DECLARE_SMART_CLASS(RenderWindow, PRenderWindow)
EGE_DECLARE_SMART_CLASS(Renderer, PRenderer)
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
    /*! Renders all registered targets. */
    void render();
    /*! Returns renderer. */
    PRenderer renderer() const { return m_renderer; }
    /*! Registers render target for use. */
    void registerRenderTarget(PRenderTarget target);
    /*! Returns render target with the given name from registered pool. */
    PRenderTarget renderTarget(const String& name) const;
    /*! Removes render target with the given name from registered pool. */
    void removeRenderTarget(const String& name);
    /*! Returns pointer to particle factory. */
    ParticleFactory* particleFactory() const { return m_particleFactory; }
    /*! Returns pointer to widget factory. */
    WidgetFactory* widgetFactory() const { return m_widgetFactory; }
    /*! Creates vertex buffer obejct. */
    PVertexBuffer createVertexBuffer(EGEVertexBuffer::UsageType usage) const;
    /*! Creates index buffer obejct. */
    PIndexBuffer createIndexBuffer(EGEIndexBuffer::UsageType usage) const;
    /*! Enables/disables rendering. */
    void setRenderingEnabled(bool set);
    /*! Initializes rendering context for worker thread. 
     *  @note This method needs to be called from worker thread.
     */
    void initializeWorkThreadRenderingContext();
    /*! Deinitializes rendering context for worker thread. 
     *  @note This method needs to be called from worker thread.
     */
    void deinitializeWorkThreadRenderingContext();

  private:

    /*! Unregisteres all render targets. */
    void unregisterAllRenderTargets();

  private:

    typedef MultiMap<s32, PRenderTarget> RenderTargetMap;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Graphics)

    /*! Renderer. */
    PRenderer m_renderer;
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