#ifndef EGE_CORE_GRAPHICS_VERTEXDECLARATION_H
#define EGE_CORE_GRAPHICS_VERTEXDECLARATION_H

/** @brief This class declares the format of a set of vertex inputs.
 */

#include "EGE.h"
#include "Core/Graphics/VertexElement.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class VertexDeclaration
{
  public:

    VertexDeclaration();
   ~VertexDeclaration();

    VertexDeclaration& operator = (const VertexDeclaration& other);
    bool operator == (const VertexDeclaration& other) const;
    bool operator != (const VertexDeclaration& other) const;

    /*! Adds given element to overall semantics. */
    bool addElement(NVertexBuffer::VertexElementSemantic semantic);
    /*! Returns number of elements of given type in semantics. */
    s32 elementCount(NVertexBuffer::VertexElementSemantic semantic) const;
    /*! Returns vertex size for current semantics (in bytes). */
    u32 vertexSize() const;
    /*! Clears vertex declaration. */
    void clear();

    /*! Returns vertex element of a given semantic and index. 
     *  @param  semantic  Vertex element semantic to look for.
     *  @param  index     Index of vertex element of a given type. This can apply only to some of elements ie. texture coords.
     *  @return Pointer to found element. NULL if not found.
     */
    const VertexElement* vertexElement(NVertexBuffer::VertexElementSemantic semantic, u32 index = 0) const;
    /*! Returns array of vertex elements. */
    const VertexElementArray& vertexElements() const;

    /*! Helper method returning position semantic. 
     *  @return Position semantic present. If none is defined, returns VES_NONE.
     */
    NVertexBuffer::VertexElementSemantic findPositionSemantic() const;

  private:

    /*! Cached vertex size (in bytes), 0 if not calculated yet. */
    mutable u32 m_vertexSize;
    /*! Buffer semantics. */
    VertexElementArray m_vertexElements;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_VERTEXDECLARATION_H
