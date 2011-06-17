#ifndef EGE_CORE_RENDERTARGET_H
#define EGE_CORE_RENDERTARGET_H

#include <EGE.h>
#include "Core/ConfigParams.h"
#include <EGEList.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
EGE_DECLARE_SMART_CLASS(Viewport, PViewport)
EGE_DECLARE_SMART_CLASS(Camera, PCamera)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderTarget : public Object
{
  public:

    RenderTarget(Application* app, const ConfigParams& params);
    virtual ~RenderTarget();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    enum Priority
    {
      PRIORITY_RENDER_TEXTURE = 0,
      PRIORITY_RENDER_WINDOW
    };

    /* Returns TRUE if object is valid. */
    virtual bool isValid() const;
    /*! Returns target priority. */
    virtual Priority priority() const = 0;
    /*! Makes itself current rendering context. */
    virtual EGEResult makeCurrentContext() = 0;
    /*! Releases itself from being current rendering context. */
    virtual void releaseCurrentContext() = 0;
    /*! Returns target name. */
    inline const EGEString& name() const { return m_name; } 
    /* Adds new viewport for target associated with given camera. */
    PViewport addViewport(const EGEString& name, PCamera pCamera);
    /* Removes viewport with the given name from target. */
    void removeViewport(const EGEString& name);
    /* Returns viewport with the given name associated with target. */
    PViewport viewport(const EGEString& name) const;
    /* Performs rendering for target. */
    void render();
    /*! Returns target width (in pixels). */
    inline s32 width() const { return m_width; }
    /*! Returns target height (in pixels). */
    inline s32 height() const { return m_height; }

  private:

    /* Removes all viewport associated with target. */
    void removeAllViewports();

  protected:

    /*! Width (in pixels). */
    s32 m_width;
    /*! Height (in pixels). */
    s32 m_height;

  private:

    /*! Targets name. */
    EGEString m_name;
    /*! Pool of all viewports associated with target. */
    EGEList<PViewport> m_viewports;
    /*! Number of vertices rendered last frame. */
    u32 m_vertexCount;
    /*! Number of batches rendered last frame. */
    u32 m_batchCount;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERTARGET_H