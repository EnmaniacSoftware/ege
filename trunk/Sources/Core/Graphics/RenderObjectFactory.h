#ifndef EGE_CORE_RENDEROBJECTFACTORY_H
#define EGE_CORE_RENDEROBJECTFACTORY_H

/** 
*   Factory (utility) class creating predefined render objects.
*/

#include <EGE.h>
#include <EGEVertexBuffer.h>
#include <EGEGraphics.h>
#include <EGEAlignment.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderObjectFactory
{
  public:

    /* Creates render component with 2D quad. 
     * @param app       Pointer to application.
     * @param name      Name of render component.
     * @param position  Quad position (local-space).
     * @param size      Size of quad.
     * @param origin    Quad's origin alignment.
     * @param semantics Render component's vertex buffer semantics.
     * @param priority  Render priority.
     * @param primitive Render primitive type.
     * @return Returns render component. NULL if failed.
     */
    static PRenderComponent CreateQuadXY(Application* app, const String& name, Vector4f position, Vector2f size, Alignment origin, 
                                         EGEVertexBuffer::SemanticType semantics, s32 priority, EGEGraphics::RenderPrimitiveType primitive);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDEROBJECTFACTORY_H
