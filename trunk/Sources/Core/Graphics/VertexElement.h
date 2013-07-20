#ifndef EGE_CORE_GRAPHICS_VERTEXELEMENT_H
#define EGE_CORE_GRAPHICS_VERTEXELEMENT_H

/** @brief This class declares the usage of a single vertex buffer as a component of a complete VertexDeclaration.
 */

#include "EGE.h"
#include "EGEDynamicArray.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace NVertexBuffer
{
  /*! Vertex element semantic type. */
  enum VertexElementSemantic
  {
    VES_NONE               = 0,
    VES_POSITION_XYZ       = 1,
    VES_POSITION_XY        = 2,
    VES_NORMAL             = 4,
    VES_TEXTURE_UV         = 8,
    VES_COLOR_RGBA         = 16,
    VES_TANGENT            = 32,
    VES_POINT_SPRITE_SIZE  = 64
  };

  typedef DynamicArray<VertexElementSemantic> VertexElementSemanticArray;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class VertexElement
{
  public:

    VertexElement(NVertexBuffer::VertexElementSemantic semantic, u32 offset, u32 index);
   ~VertexElement();

    bool operator == (const VertexElement& other) const;

    /*! Returns element semantic. */
    NVertexBuffer::VertexElementSemantic semantic() const;
    /*! Returns element's offset. */
    u32 offset() const;
    /*! Returns element's index. */
    u32 index() const;

  private:

    /*! Element semantic. */
    NVertexBuffer::VertexElementSemantic m_semantic;
    /*! Offset from vertex buffer start (in bytes). */
    u32 m_offset;
    /*! Index of semantic within the same vertex buffer. Only valid for some of the elements like texture coords. */
    u32 m_index;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef DynamicArray<VertexElement> VertexElementArray;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_VERTEXELEMENT_H
