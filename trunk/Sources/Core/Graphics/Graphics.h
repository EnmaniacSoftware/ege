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

    /*! Signal emitted just before target is rendered. */
    Signal1<PRenderTarget> preRender;
    /*! Signal emitted just after target has been rendered. */
    Signal1<PRenderTarget> postRender;

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Renders all registered targets. */
    void render();
    /* Sets current rendering context. */
    EGEResult setCurrentRenderingContext(PRenderTarget target);
    /*! Returns renderer. */
    inline PRenderer renderer() const { return m_renderer; }
    /* Registers render target for use. */
    void registerRenderTarget(PRenderTarget target);
    /* Returns render target with the given name from registered pool. */
    PRenderTarget renderTarget(const String& name) const;
    /* Removes render target with the given name from registered pool. */
    void removeRenderTarget(const String& name);
    /*! Returns pointer to particle factory. */
    inline ParticleFactory* particleFactory() const { return m_particleFactory; }
    /* Creates vertex buffer obejct. */
    PVertexBuffer createVertexBuffer(EGEVertexBuffer::UsageType usage) const;
    /* Creates index buffer obejct. */
    PIndexBuffer createIndexBuffer(EGEIndexBuffer::UsageType usage) const;

  private:

    /* Unregisteres all render targets. */
    void unregisterAllRenderTargets();

  private:

    typedef MultiMap<s32, PRenderTarget> RenderTargetMap;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Graphics)

    /*! Renderer. */
    PRenderer m_renderer;
    /*! Current rendering context. NULL if none is set. */
    PRenderTarget m_currentRenderingContext;
    /*! Render targets sorted by priority. */
    RenderTargetMap m_renderTargets; 
    /*! Particle emitter factory. */
    ParticleFactory* m_particleFactory;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_H