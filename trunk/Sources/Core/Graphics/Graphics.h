#ifndef EGE_CORE_GRAPHICS_H
#define EGE_CORE_GRAPHICS_H

#include <EGEMap.h>
#include "Core/ConfigParams.h"
#include "Core/Graphics/Render/Renderer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Graphics, PGraphics)
EGE_DECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
EGE_DECLARE_SMART_CLASS(RenderWindow, PRenderWindow)
EGE_DECLARE_SMART_CLASS(Renderer, PRenderer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Graphics : public Object
{
  public:

    Graphics(Application* app, const ConfigParams& params);
    virtual ~Graphics();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Renders all registered targets. */
    void render();
    /* Sets current rendering context. */
    EGEResult setCurrentRenderingContext(PRenderTarget target);
    /*! Returns renderer. */
    inline PRenderer renderer() const { return m_renderer; }
    /*! Returns main render window. */
//    inline PRenderWindow renderWindow() const { return m_renderWindow; }
    /*! Registers render target for use. */
    void registerRenderTarget(PRenderTarget target);
    /*! Returns render target with the given name from registered pool. */
    PRenderTarget renderTarget(const String& name) const;
    /*! Removes render target with the given name from registered pool. */
    void removeRenderTarget(const String& name);
//    const RenderTargetPriorityMap& getRenderTargetMap() const;

  private:

    /*! Unregisteres all render targets. */
    void unregisterAllRenderTargets();

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Graphics)

    /*! Renderer. */
    PRenderer m_renderer;
    /*! Main render window. */
    //PRenderWindow m_renderWindow;
    /*! Current rendering context. NULL if none is set. */
    PRenderTarget m_currentRenderingContext;
    /*! Render targets sorted by priority. */
    MultiMap<s32, PRenderTarget> m_renderTargets; 
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_H