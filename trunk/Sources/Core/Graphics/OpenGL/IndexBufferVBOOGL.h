#ifndef EGE_CORE_INDEXBUFFERVBOOGL_H
#define EGE_CORE_INDEXBUFFERVBOOGL_H

/** 
 *   IndexBuffer base class specialization based on Vertex Buffer Objects extension.
 */

#include <EGE.h>
#include "Core/Graphics/IndexBuffer.h"
#include <EGEOpenGL.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IndexBufferVBO : public IndexBuffer
{
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
    u32 indexCount() const override { return m_indexCount; }
    /*! @see IndexBuffer::indexCapacity. */
    u32 indexCapacity() const override { return m_indexCapacity; }

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
    /*! Shadow data buffer. */
    PDataBuffer m_shadowBuffer;  
    /*! Flag indicating if current lock is done on shadow buffer directly. */
    bool m_shadowBufferLock;
    /*! Lock offset (in vertices). */
    u32 m_lockOffset;
    /*! Lock length (in vertices). */
    u32 m_lockLength;
    /*! Map pointer. Only valid if mapping API is used for ie. m_shadowBufferLock is FALSE. */
    void* m_mapping;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_INDEXBUFFERVBOOGL_H
