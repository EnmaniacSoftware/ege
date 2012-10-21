#ifndef EGE_CORE_RENDERTARGET_H
#define EGE_CORE_RENDERTARGET_H

#include <EGE.h>
#include <EGEDynamicArray.h>
#include <EGEAngle.h>
#include <EGEDictionary.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
EGE_DECLARE_SMART_CLASS(Viewport, PViewport)
EGE_DECLARE_SMART_CLASS(Camera, PCamera)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderTarget : public Object
{
  public:

    RenderTarget(Application* app, const Dictionary& params);
    virtual ~RenderTarget();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Available priorities. */
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
    /* Binds render target. */
    virtual void bind() = 0;
    /* Unbinds render target. */
    virtual void unbind() = 0;
    /* Returns TRUE if texture flipping is required for this render target. */
		virtual bool requiresTextureFlipping() const = 0;

    /*! Returns target name. */
    const String& name() const { return m_name; } 
    /* Adds new viewport for target associated with given camera. */
    PViewport addViewport(const String& name, PCamera camera);
    /* Removes viewport with the given name from target. */
    void removeViewport(const String& name);
    /* Returns viewport with the given name associated with target. */
    PViewport viewport(const String& name) const;
    /* Performs rendering for target. */
    void render();
    /*! Returns target logical width (in pixels). */
    s32 width() const { return m_width; }
    /*! Returns target logical height (in pixels). */
    s32 height() const { return m_height; }
    /*! Returns target physical width (in pixels). */
    s32 physicalWidth() const { return m_physicalWidth; }
    /*! Returns target physical height (in pixels). */
    s32 physicalHeight() const { return m_physicalHeight; }
    /*! Returns zoom factor. */
    float32 zoom() const { return m_zoom; }
    /*! Returns TRUE if target is enabled. */
     bool isEnabled() const { return m_enabled; }
    /* Enables/disables render target. */
    void setEnable(bool enable);
    /*! Returns orientation rotation. */
    const Angle& orientationRotation() const { return m_orientationRotation; }

  private:

    /* Removes all viewport associated with target. */
    void removeAllViewports();

  protected:

    /* It is possible to request size which is internally represented by another. For instance landscape mode 960x640 on portrait device 640x960.
     * In that case logical size is going to be 960x640, however, real surface size will still be 640x960 but properly rotated.
     * Such scenario often may occur while working with primary surfaces as they most of the time are fixed in size and orientation.
     */

    /*! Physical width (in pixels). Physical width corresponds to real device width. */
    s32 m_physicalWidth;
    /*! Physical height (in pixels). Physical width corresponds to real device height. */
    s32 m_physicalHeight;
    /*! Logical width (in pixels). Logical width corresponds to requested width. */
    s32 m_width;
    /*! Logical height (in pixels). Logical height corresponds to requested height. */
    s32 m_height;
    /*! Orientation rotation angle. */
    Angle m_orientationRotation;
    /*! Zoom factor. */
    float32 m_zoom;

  private:

    /*! Targets name. */
    String m_name;
    /*! Pool of all viewports associated with target. */
    DynamicArray<PViewport> m_viewports;
    /*! Number of vertices rendered last frame. */
    u32 m_vertexCount;
    /*! Number of batches rendered last frame. */
    u32 m_batchCount;
    /*! Enable flag. */
    bool m_enabled;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERTARGET_H