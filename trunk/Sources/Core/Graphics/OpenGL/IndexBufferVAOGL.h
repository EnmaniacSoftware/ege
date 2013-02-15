#ifndef EGE_CORE_GRAPHICS_OPENGL_INDEXBUFFERVAOGL_H
#define EGE_CORE_GRAPHICS_OPENGL_INDEXBUFFERVAOGL_H

/** 
 *   IndexBuffer base class specialization based on Vertex Arrays.
 */

#include <EGE.h>
#include <EGEIndexBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IndexBufferVA : public IndexBuffer
{
  public:

    IndexBufferVA(Application* app);
    virtual ~IndexBufferVA();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

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

  private:

    /*! @see IndexBuffer::destroy. */
    void destroy() override;
    /*! Reallocates internal buffer to accomodate given number of indicies. */
    bool reallocateBuffer(u32 count);

  private:

    /*! Data buffser. */
    PDataBuffer m_buffer;         
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_INDEXBUFFERVAOGL_H