#ifndef EGE_CORE_VERTEXBUFFER_H
#define EGE_CORE_VERTEXBUFFER_H

// VertexBuffer is used to define single vertex lookup and to keep certain amount of such vertices in memory
// Vertex data is stored one after another (interleaved)

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class VertexBuffer : public Object
{
  public:

    VertexBuffer(Application* app);
    virtual ~VertexBuffer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    enum ArrayType
    {
      ARRAY_TYPE_NONE          = 0,
      ARRAY_TYPE_POSITION_XYZ  = 1,
      ARRAY_TYPE_NORMAL        = 2,
      ARRAY_TYPE_TEXTURE_UV    = 4,
      ARRAY_TYPE_COLOR_RGBA    = 8,
      ARRAY_TYPE_TANGENT       = 16
    };

    struct SBUFFERSEMANTIC
    {
      /*! Array type. */
      ArrayType type;
      /*! Offset to array start (in bytes). */
      u32 offset;
      /*! Index of array within the same array type. Only valid for some of the array types like texture coords. */
      u32 index;
    };

    /* Creates buffer for requested number of vertices. */
    bool create(u32 count);
    /* Destroys buffer. */
    void destroy();
    /* Locks buffer given part of the buffer for read/write operations. */
    void* lock(u32 offset, u32 count);
    /* Unlocks buffer. */
    void unlock();

    /* Adds given array type to overall semantics. */
    bool addBuffer(ArrayType type);
    /* Returns number of arrays of given type in semantics. */
    u32 bufferCount(ArrayType type) const;

    /* Returns current buffer semantics. */
    const std::vector<SBUFFERSEMANTIC>& semantics() const;

    /* Returns number of allocated vertices. */
    u32 vertexCount() const;
    /* Returns vertex size for current semantics (in bytes). */
    u32 vertexSize() const;

  private:

    /* Reallocates internal buffer to accomodate given number of vertices. */
    bool reallocateBuffer(u32 count);

  private:

    /*! TRUE if buffer is locked. */
    bool m_locked;
    /*! Data buffser. */
    PDataBuffer m_buffer;         
    /*! Cached vertex size (in bytes), 0 if not calculated yet. */
    mutable u32 m_vertexSize;
    /*! Buffer semantics. */
    std::vector<SBUFFERSEMANTIC> m_semantics;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VERTEXBUFFER_H
