#ifndef EGE_VERTEX_BUFFER_H
#define EGE_VERTEX_BUFFER_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGEVertexBuffer
{
  /*! Vertex array types. */
  enum ArrayType
  {
    ARRAY_TYPE_NONE          = 0,
    ARRAY_TYPE_POSITION_XYZ  = 1,
    ARRAY_TYPE_NORMAL        = 2,
    ARRAY_TYPE_TEXTURE_UV    = 4,
    ARRAY_TYPE_COLOR_RGBA    = 8,
    ARRAY_TYPE_TANGENT       = 16
  };

  /*! Data structure describing single array semantic. */
  struct SARRAYSEMANTIC
  {
    /*! Array type. */
    ArrayType type;
    /*! Offset to array start (in bytes). */
    u32 offset;
    /*! Index of array within the same array type. Only valid for some of the array types like texture coords. */
    u32 index;
  };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Graphics/VertexBuffer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_VERTEX_BUFFER_H