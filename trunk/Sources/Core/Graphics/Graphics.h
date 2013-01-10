#ifndef EGE_CORE_GRAPHICS_H
#define EGE_CORE_GRAPHICS_H

#include <EGEMap.h>
#include <EGESignal.h>
#include <EGEDictionary.h>
#include <EGEList.h>
#include "Core/Graphics/Render/Renderer.h"

#include <EGETexture.h>
#include <EGEMutex.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ParticleFactory;
class WidgetFactory;
class RenderSystem;

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
    /*! Signal emitted when 2D texture has been created. 
     *  @param handle   Texture creation request handle.
     *  @oaram texture  Created texture.
     *  @param result   Result.
     *  @note Signal is emitted from main thread.
     */
    Signal3<u32, PTexture2D, EGEResult> texture2DCreated;

  public:

    /*! Creates object. */
    EGEResult construct();
    /*! Renders all registered targets. */
    void render();
    /*! Returns renderer interface. */
    IRenderer* renderer();
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
    /*! Creates 2d texture object from given image. 
     *  @param image  Image from which texture is to be created.
     *  @return Handle value which can be used to identifiy requested texture when signalled.
     *  @note When image is created result is signaled by texture2DCreated.
     */
    u32 createTexture2D(const PImage& image);
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

    void update();

  private:

    /*! Unregisteres all render targets. */
    void unregisterAllRenderTargets();

  private:

    /*! Struct containing texture creation info. */
    struct TextureCreateRequest
    {
      PImage image;           /*!< Image from which texture should be created. */
      u32    handle;          /*!< Texture creation handle. */
    };

    typedef List<TextureCreateRequest> TextureCreateRequestList;
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
    /*! List of 2D textures to create. */
    TextureCreateRequestList m_texture2DRequests;
    /*! Next valid handle value. */
    u32 m_nextHandle;
    /*! Resource data access mutex. */
    PMutex m_mutex;
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