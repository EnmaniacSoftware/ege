#ifndef EGE_CORE_GRAPHICS_OPENGL_INDEXBUFFERVBOOGL_H
#define EGE_CORE_GRAPHICS_OPENGL_INDEXBUFFERVBOOGL_H

/** 
 *   IndexBuffer base class specialization based on Vertex Buffer Objects extension.
 */

#include "EGE.h"
#include "Core/Graphics/IndexBuffer.h"
#include "EGEOpenGL.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IndexBufferVBO : public IndexBuffer
{
  friend class RenderSystemPrivate;

  public:

    IndexBufferVBO(Application* app, EGEIndexBuffer::UsageType usage);
   ~IndexBufferVBO();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see IndexBuffer::isValid. */
    bool isValid() const override;
    /*! @see IndexBuffer::setSize. */
    bool setSize(u32 count) override;
    /*! @see IndexBuffer::lock. */
    void* lock(u32 offset, u32 count) override;
    /*! @see IndexBuffer::unlock. */
    void unlock(void* data) override;

    /*! @see IndexBuffer::indexCount. */
    u32 indexCount() const override;
    /*! @see IndexBuffer::indexCapacity. */
    u32 indexCapacity() const;

    /*! Binds buffer. */
    void bind();
    /*! Unbinds buffer. */
    void unbind();

  private:

    /*! @see IndexBuffer::destroy. */
    void destroy() override;
    /*! Reallocates internal buffer to accomodate given number of vertices. */
    bool reallocateBuffer(u32 count);

  private:

    /*! OpenGL VBO identifier. */
    GLuint m_id;
    /*! Index count. */
    u32 m_indexCount;
    /*! Index capacity. */
    u32 m_indexCapacity;
    /*! Shadow data buffer. Used when mapping API is not available. Used in lock/unlock periods. */
    PDataBuffer m_shadowBuffer;  
    /*! Lock offset (in vertices). */
    u32 m_lockOffset;
    /*! Lock length (in vertices). */
    u32 m_lockLength;
    /*! Begining of mapped memory. Used in lock/unlock periods. */
    void* m_mapping;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline u32 IndexBufferVBO::indexCount() const 
{ 
  return m_indexCount; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline u32 IndexBufferVBO::indexCapacity() const 
{ 
  return m_indexCapacity; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_INDEXBUFFERVBOOGL_H
