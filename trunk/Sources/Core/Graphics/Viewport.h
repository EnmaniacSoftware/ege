#ifndef EGE_CORE_VIEWPORT_H
#define EGE_CORE_VIEWPORT_H

#include "EGE.h"
#include "Core/Graphics/Color.h"
#include "Core/Graphics/Camera.h"
#include "EGEMath.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Viewport, PViewport)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Viewport : public Object
{
  public:

    Viewport(Application* app, const EGEString& name, PCamera camera);
    virtual ~Viewport();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    enum PolygonMode
    {
      POLYGON_MODE_POINTS,
      POLYGON_MODE_WIREFRAME,
      POLYGON_MODE_SOLID
    };

    enum BufferTypeFlags
    {
      BUFFER_TYPE_COLOR = 0x01,
      BUFFER_TYPE_DEPTH = 0x02
    };

	  EGE_DECLARE_FLAGS(BufferType, BufferTypeFlags);

    /*! Returns name. */
    inline const EGEString& name() const { return m_name; }
    /* Sets viewport rectangle within render target. */
    void setRect(Rectf rect);
    /*! Returns viewport rect within render target. */
    inline const Rectf& rect() const { return m_rect; }
    /*! Returns camera associated with viewport. */
    PCamera camera() const { return m_camera; }
    /* Sets viewport clear color. Can be NULL color if viewport should not be cleared. */
    void setClearColor(const Color& color);
    /*! Returns viewport clear color. */
    inline const Color& clearColor() const { return m_clearColor; }
    /* Sets polygon mode for rendered data. */
    void setPolygonMode(PolygonMode mode);
    /*! Returns polygon mode for rendered data. */
    inline PolygonMode polygonMode() const { return m_polygonMode; }
    /* Sets buffer types to clear. */
    void setClearBufferTypes(BufferType mask);
    /*! Returns buffer types to clear. */
    inline BufferType clearBufferTypes() const { return m_clearBufferTypes; }
    /* Enables/Disables overlays. */
    void enableOverlays(bool enable);
    /*! Returns TRUE if overlays are enabled for this view. */
    inline bool overlaysEnabled() const { return m_overlays; }

    /* Renders viewport from associated camera's point of view. */
    void render();

    /* Sets vertex count rendered last frame. */
    void setVertexCount(u32 count);
    /*! Returns vertex count rendered last frame. */
    inline u32 vertexCount() const { return m_vertexCount; }
    /*! Returns batch count rendered last frame. */
    inline u32 batchCount() const { return m_batchCount; }
    /* Sets batch count rendered last frame. */
    void setBatchCount(u32 count);

  private:

    /*! Name. */
    EGEString m_name;
    /*! Clear color (can be NONE if no viewport buffer should be cleared). */
    Color m_clearColor;
    /*! Rectangular area within render target viewport occupies (in relative units). */
    Rectf m_rect;
    /*! Camera associated with view. */
    PCamera m_camera;
    /*! Polygon mode of rendered data in this viewport. */
    PolygonMode m_polygonMode;
    /*! Buffers masks for clearing. */
    BufferType m_clearBufferTypes;
    /*! Number of vertices rendered last frame. */
    u32 m_vertexCount;
    /*! Number of batches rendered last frame. */
    u32 m_batchCount;
    /*! Overlays flag. If TRUE overlays will be rendered for this viewport. */
    bool m_overlays;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VIEWPORT_H