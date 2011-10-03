#ifndef EGE_DEVICE_H
#define EGE_DEVICE_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGEDevice
{
  /*! Operating systems available. */
  enum OS
  {
    OS_UNKOWN = 0,
    OS_IPHONE,
    OS_WINDOWS
  };

  /*! Devices available. */
  enum Device
  {
    DEVICE_GENERIC = 0,
    DEVICE_EMULATOR,
    DEVICE_IPHONE_3G,
    DEVICE_IPHONE_3GS,
    DEVICE_IPHONE_4,
    DEVICE_IPAD,
    DEVICE_IPOD_TOUCH_1,
    DEVICE_IPOD_TOUCH_2,
    DEVICE_IPOD_TOUCH_3,
    DEVICE_IPOD_TOUCH_4
  };

  /*! Rendering capability flags. */
  enum RenderCapability
  {
    RENDER_CAPS_APPLE_LIMITED_NPOT_TEXTURE = 0,           /*!< Limited non-power of two 2D texture extension for iOS. */
    RENDER_CAPS_MULTITEXTURE,                             /*!< Multitexturing supported (GL_ARB_multitexture). */
    RENDER_CAPS_FBO,                                      /*!< Frame buffer object supported (GL_EXT_framebuffer_object). */
    RENDER_CAPS_VBO,                                      /*!< Vertex buffer object supported (GL_ARB_vertex_buffer_object). */
    RENDER_CAPS_COMBINE_TEXTURE_ENV,                      /*!< Combine texture environment mode supported (GL_ARB_texture_env_combine). */
    RENDER_CAPS_POINT_SPRITE,                             /*!< Point sprite supported (GL_ARB_point_sprite && GL_ARB_point_parameters). */
    RENDER_CAPS_POINT_SPRITE_SIZE,                        /*!< Point sprite size array supported (ie. glPointSizePointerOES). */
    RENDER_CAPS_COUNT
  };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Device/Device.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_DEVICE_H