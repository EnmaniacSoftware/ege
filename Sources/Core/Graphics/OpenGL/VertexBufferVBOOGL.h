#ifndef EGE_CORE_GRAPHICS_OPENGL_VERTEXBUFFERVBOOGL_H
#define EGE_CORE_GRAPHICS_OPENGL_VERTEXBUFFERVBOOGL_H

/** 
 *   VertexBuffer base class specializtion based on Vertex Buffer Objects extension.
 */

#include "EGE.h"
#include "Core/Graphics/VertexBuffer.h"
#include "EGEOpenGL.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class VertexBufferVBO : public VertexBuffer
{
  public:

    VertexBufferVBO(Application* app, const String& name, const VertexDeclaration& vertexDeclaration, NVertexBuffer::UsageType usage);
   ~VertexBufferVBO();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see VertexBuffer::isValid. */
    bool isValid() const override;
    /*! @see VertexBuffer::setSize. */
    bool setSize(u32 count) override;
    /*! @see VertexBuffer::lock. */
    void* lock(u32 offset, u32 count) override;
    /*! @see VertexBuffer::unlock. */
    void unlock(const void* data) override;

    /*! @see VertexBuffer::vertexCount. */
    u32 vertexCount() const override;
    /*! @see VertexBuffer::vertexCapacity. */
    u32 vertexCapacity() const;

    /*! @see VertexBuffer::bind. */
    void bind() override;
    /*! @see VertexBuffer::unbind. */
    void unbind() override;

    /*! @see VertexBuffer::offset. */
    void* offset() const override;

    /*! Returns OpenGL identifier. */
    GLuint id() const;

  private:

    /*! @see VertexBuffer::destroy. */
    void destroy() override;
    /*! Reallocates internal buffer to accomodate given number of vertices. */
    bool reallocateBuffer(u32 count);

  private:

    /*! OpenGL VBO identifier. */
    GLuint m_id;
    /*! Current number of vertices in use. */
    u32 m_vertexCount;
    /*! Number of vertices which can used with reallocation. */
    u32 m_vertexCapacity;
    /*! Shadow data buffer. Used when mapping API is not available. Used in lock/unlock periods. */
    PDataBuffer m_shadowBuffer;  
    /*! Lock offset (in vertices). */
    u32 m_lockOffset;
    /*! Lock length (in vertices). */
    u32 m_lockLength;
    /*! Begining of mapped memory. Used in lock/unlock periods. */
    void* m_mapping;
    /*! Usage. */
    NVertexBuffer::UsageType m_usage;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline u32 VertexBufferVBO::vertexCount() const 
{ 
  return m_vertexCount; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline u32 VertexBufferVBO::vertexCapacity() const 
{ 
  return m_vertexCapacity; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_VERTEXBUFFERVBOOGL_H
