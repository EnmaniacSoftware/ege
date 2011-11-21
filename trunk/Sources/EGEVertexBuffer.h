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
    AT_NONE               = 0,
    AT_POSITION_XYZ       = 1,
    AT_POSITION_XY        = 2,
    AT_NORMAL             = 4,
    AT_TEXTURE_UV         = 8,
    AT_COLOR_RGBA         = 16,
    AT_TANGENT            = 32,
    AT_POINT_SPRITE_SIZE  = 64
  };

  /*! Predefined semantic types. */
  enum SemanticType
  {
    ST_V3_T2_C4 = 0,
    ST_V2_T2_C4,
    ST_V3_T2,
    ST_V2_T2,
    ST_V3_C4,
    ST_V2_C4,
    ST_V3,
    ST_V2
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