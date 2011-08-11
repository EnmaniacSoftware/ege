#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <EGEScene.h>
#include <EGEGraphics.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderObject : public EGE::SceneNodeObject
{
  public:

    RenderObject(EGE::Application* app, const EGE::String& name);
   ~RenderObject();
   
  public:

    /* Creates instance representing the triangle. */
    static RenderObject* CreateTriangle(EGE::Application* app, const EGE::String& name, EGE::s32 size, bool color, bool uv);
    /* Creates instance representing the triangle. */
    static RenderObject* CreateRectangle(EGE::Application* app, const EGE::String& name, EGE::s32 width, EGE::s32 height, bool color, bool uv);

  public:

    /*! Returns render data. */
    inline EGE::PRenderComponent& renderData() { return m_renderData; }

  private:

    /* SceneNodeObject override. Adds object render data for rendering with given renderer. */
    bool addForRendering(EGE::Renderer* renderer) override;

  private:

    /* Creates base object. */
    static RenderObject* CreateBase(EGE::Application* app, const EGE::String& name, bool color, bool uv);

  private:

    /*! Render data. */
    EGE::PRenderComponent m_renderData;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RENDER_OBJECT_H