#ifndef EGE_CORE_GRAPHICS_OPENGL_VERTEXBUFFERVAOGL_H
#define EGE_CORE_GRAPHICS_OPENGL_VERTEXBUFFERVAOGL_H

/** 
 *   VertexBuffer base class specializtion based on Vertex Arrays.
 */

#include "EGE.h"
#include "Core/Graphics/VertexBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class VertexBufferVA : public VertexBuffer
{
  public:

    VertexBufferVA(Application* app, const String& name, const VertexDeclaration& vertexDeclaration);
    virtual ~VertexBufferVA();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! @see VertexBuffer::isValid. */
    bool isValid() const override;
    /*! @see VertexBuffer::setSize. */
    bool setSize(u32 count) override;
    /*! @see VertexBuffer::lock. */
    void* lock(u32 offset, u32 count) override;
    /*! @see VertexBuffer::unlock. */
    void unlock(const void* data) override;
    
    /*! @see VertexBuffer::bind. */
    void bind() override;
    /*! @see VertexBuffer::unbind. */
    void unbind() override;
    
    /*! @see VertexBuffer::vertexCount. */
    u32 vertexCount() const override;

    /*! @see VertexBuffer::offset. */
    void* offset() const override;

  private:

    /*! @see VertexBuffer::destroy. */
    void destroy() override;
    /*! Reallocates internal buffer to accomodate given number of vertices. */
    bool reallocateBuffer(u32 count);

  private:

    /*! Data buffer. */
    PDataBuffer m_buffer;         
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_VERTEXBUFFERVAOGL_H
