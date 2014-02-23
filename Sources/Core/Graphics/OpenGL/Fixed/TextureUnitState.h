#ifndef EGE_CORE_GRAPHICS_OPENGL_FIXED_TEXTUREUNITSTATE_H
#define EGE_CORE_GRAPHICS_OPENGL_FIXED_TEXTUREUNITSTATE_H

/*! Data struct containing information about current state of texture unit.
 */

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct TextureUnitState
{
  /*! Texture coord sets within vertex buffer semantic unit uses. */
  u32 m_textureCoordIndex;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_FIXED_TEXTUREUNITSTATE_H
