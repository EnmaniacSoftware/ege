#ifndef EGE_CORE_GRAPHICS_RENDEROBJECTFACTORY_H
#define EGE_CORE_GRAPHICS_RENDEROBJECTFACTORY_H

/** 
*   Factory (utility) class creating predefined render objects.
*/

#include "EGE.h"
#include "EGEVertexBuffer.h"
#include "EGEGraphics.h"
#include "EGEAlignment.h"
#include "EGEList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CubicSpline;
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderObjectFactory
{
  public:

    /*! Predefined vertex semantics. */
    enum VertexDeclarationSymbol
    {
      VS_V3_T2_C4 = 0,
      VS_V2_T2_C4,
      VS_V3_T2,
      VS_V2_T2,
      VS_V3_C4,
      VS_V2_C4,
      VS_V3,
      VS_V2
    };

  public:

    /*! Creates render component with 2D quad. 
     *  @param app                Pointer to application.
     *  @param name               Name of render component.
     *  @param position           Quad position (local-space).
     *  @param size               Size of quad.
     *  @param origin             Quad's origin alignment.
     *  @param flipU              If TRUE U texture coordinates are flipped
     *  @param flipV              If TRUE V texture coordinates are flipped
     *  @param vertexDeclaration  Render component's vertex declaration.
     *  @param priority           Render priority.
     *  @param primitive          Render primitive type.
     *  @param vertexUsage        Vertex buffer usage.
     *  @return Returns render component. NULL if failed.
     */
    static PRenderComponent CreateQuadXY(Engine& engine, const String& name, Vector4f position, Vector2f size, Alignment origin, bool flipU, bool flipV,
                                         VertexDeclarationSymbol vertexDeclaration, s32 priority, EGEGraphics::RenderPrimitiveType primitive,
                                         NVertexBuffer::UsageType vertexUsage);
    static PRenderComponent CreateQuadXY(Engine& engine, const String& name, Vector4f position, Vector2f size, Alignment origin, bool flipU, bool flipV,
                                         const VertexDeclaration& vertexDeclaration, s32 priority, EGEGraphics::RenderPrimitiveType primitive,
                                         NVertexBuffer::UsageType vertexUsage);

    /*! Creates render component composed of 4 quads (2x2). 
     *  @param app                Pointer to application.
     *  @param name               Name of render component.
     *  @param position           Quad position (local-space).
     *  @param size               Size of quad.
     *  @param origin             Quad's origin alignment.
     *  @param flipU              If TRUE U texture coordinates are flipped
     *  @param flipV              If TRUE V texture coordinates are flipped
     *  @param vertexDeclaration  Render component's vertex declaration.
     *  @param priority           Render priority.
     *  @param primitive          Render primitive type.
     *  @param vertexUsage        Vertex buffer usage.
     *  @return Returns render component. NULL if failed.
     *  @note Each quad has texture coordinates mirrored.
     */
    static PRenderComponent CreateQuatroQuadXY(Engine& engine, const String& name, Vector4f position, Vector2f size, Alignment origin, bool flipU, bool flipV,
                                               VertexDeclarationSymbol vertexDeclaration, s32 priority, EGEGraphics::RenderPrimitiveType primitive,
                                               NVertexBuffer::UsageType vertexUsage);
    static PRenderComponent CreateQuatroQuadXY(Engine& engine, const String& name, Vector4f position, Vector2f size, Alignment origin, bool flipU, bool flipV,
                                               const VertexDeclaration& vertexDeclaration, s32 priority, EGEGraphics::RenderPrimitiveType primitive,
                                               NVertexBuffer::UsageType vertexUsage);

    /*! Creates render component for given curve. 
     *  @param spline             Pointer to spline for which render component is to be generated.
     *  @param app                Pointer to application.
     *  @param name               Name of render component.
     *  @param offset             Curve position offset (local-space).
     *  @param vertexDeclaration  Render component's vertex declaration.
     *  @param priority           Render priority.
     *  @return Returns render component. NULL if failed.
     */
    static PRenderComponent Create(const CubicSpline* spline, Engine& engine, const String& name, Vector3f offset, VertexDeclarationSymbol vertexDeclaration, 
                                   s32 priority);
    static PRenderComponent Create(const CubicSpline* spline, Engine& engine, const String& name, Vector3f offset, 
                                   const VertexDeclaration& vertexDeclaration, s32 priority);

  private:

    static bool DoCreateQuadXY(PRenderComponent& component, Vector4f position, Vector2f size, Alignment origin, EGEGraphics::RenderPrimitiveType primitive,
                               bool flipU, bool flipV);
    static bool DoCreateQuatroQuadXY(PRenderComponent& component, Vector4f position, Vector2f size, Alignment origin, 
                                     EGEGraphics::RenderPrimitiveType primitive, bool flipU, bool flipV);
    static VertexDeclaration ConvertVertexDeclarationSymbol(VertexDeclarationSymbol vertexDeclaration);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDEROBJECTFACTORY_H
