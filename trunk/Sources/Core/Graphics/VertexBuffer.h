#ifndef EGE_CORE_GRAPHICS_VERTEXBUFFER_H
#define EGE_CORE_GRAPHICS_VERTEXBUFFER_H

/** 
 *   VertexBuffer base class. Instances of this class are used to define vertex array of arbitrary type(s).
 *   Vertex arrays data are stored one after another (interleaved)
 */

#include "EGE.h"
#include "Core/Graphics/VertexDeclaration.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace NVertexBuffer
{
  /*! Buffer available usages. */
  enum UsageType
  {
    UT_STATIC_WRITE   = 1,                  /*!< Created once, used many times. Data will be sent from application to GL. */
    UT_DYNAMIC_WRITE  = 2,                  /*!< Frequently changable. Data will be sent from application to GL. */
    UT_DISCARDABLE    = 4,                  /*!< Flag indicating that content before it is overwritten is completely out-of-interest to us. 
                                                 This allows for some optimization. Makes sense with UT_DYNAMIC_WRITE. */   
    
    UT_DYNAMIC_WRITE_DONT_CARE = UT_DYNAMIC_WRITE | UT_DISCARDABLE
  };
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class VertexBuffer : public Object
{
  public:

    virtual ~VertexBuffer();

    /*! Returns TRUE if object is valid. */
    virtual bool isValid() const = 0;
    /*! Sets buffer to given size. 
     *  @param count Number of vertices buffer should contain.
     *  @return Returns TRU33E if success. Otherwise, FALSE.
     */
    virtual bool setSize(u32 count) = 0;

    /*! Locks buffer's given part of the buffer for read/write operations. 
     *  @param offset  0-based vertex offset from which locking should be done. 
     *  @param count   Number of vertices to lock.
     *  @return Pointer to the begining of the locked part of the buffer. NULL if lock could not be set.
     *  @note Locking fails if requested block lies outside of the buffer.
     */
    virtual void* lock(u32 offset, u32 count) = 0;
    /*! Unlocks buffer. */
    virtual void unlock(void* data) = 0;

    /*! Returns vertex declaration. */
    const VertexDeclaration& vertexDeclaration() const;
    /*! Sets vertex declaration. */
    void setVertexDeclaration(const VertexDeclaration& vertexDeclaration);

    /*! Returns number of vertices currently in use. */
    virtual u32 vertexCount() const = 0;

  protected:

    /*! Constructing only via RenderSystem. */
    VertexBuffer(Application* app, u32 uid);
    /*! Destroys buffer. */
    virtual void destroy();

  protected:

    /*! TRUE if buffer is locked. */
    bool m_locked;
    /*! Vertex declaration. */
    VertexDeclaration m_vertexDeclaration;
    /*! Usage. */
    NVertexBuffer::UsageType m_usage;

  private:

    /*! Clears vertex data. */
    virtual void clear();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_VERTEXBUFFER_H
