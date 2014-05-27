#ifndef EGE_CORE_DEVICE_H
#define EGE_CORE_DEVICE_H

/** Class representing miscellaneous information about device framework is running.
 *  Some information is being supplied by other framework subsystems once its available (ie. render system).
 */

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
extern const char* KDeviceDebugName;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Operating systems available. */
enum DeviceOS
{
  EUnknown = 0,
  EIOS,
  EWindows,
  EMacOS
};

/*! Devices available. */
enum DeviceType
{
  EDeviceGeneric = 0,
  EDeviceEmulator,
  EDeviceiPhone,
  EDeviceiPhone3G,
  EDeviceiPhone3GS,
  EDeviceiPhone4,
  EDeviceiPhone4S,
  EDeviceiPhone5,
  EDeviceiPhone5S,
  EDeviceiPhone5C,
  EDeviceiPad,
  EDeviceiPad2,
  EDeviceiPad3,
  EDeviceiPad4,
  EDeviceiPadMini,
  EDeviceiPadMini2,       // retina
  EDeviceiPadAir,
  EDeviceiPodTouch1,
  EDeviceiPodTouch2,
  EDeviceiPodTouch3,
  EDeviceiPodTouch4,
  EDeviceiPodTouch5
};

/*! Rendering capability flags. */
enum DeviceRenderCapability
{
  ERenderCapabilityMultitexturing = 0,                      /*!< Multitexturing supported (GL_ARB_multitexture). */
  ERenderCapabilityFrameBufferObjects,                      /*!< Frame buffer object supported (GL_EXT_framebuffer_object). */
  ERenderCapabilityVertexBufferObjects,                     /*!< Vertex buffer object supported (GL_ARB_vertex_buffer_object). */
  ERenderCapabilityPointSprites,                            /*!< Point sprite supported (GL_ARB_point_sprite && GL_ARB_point_parameters). */
  ERenderCapabilityMapBuffer,                               /*!< Mapping of VBO starage in client memory space (thru glMapBuffer etc). */
  ERenderCapabilityTextureCompressionPVRTC,                 /*!< Tetxure compression using PVRTC (GL_IMG_texture_compression_pvrtc). */
  ERenderCapabilityTextureCompressionS3TC,                  /*!< Tetxure compression using S3TC (GL_EXT_texture_compression_s3tc). */
  ERenderCapabilityVertexShaders,                           /*!< Vertex shader support (GL_ARB_vertex_shader). */
  ERenderCapabilityFragmentShaders,                         /*!< Fragment shader support (GL_ARB_fragment_shader). */
  ERenderCapabilityVertexArrayObjects,                      /*!< Vertex array objects support (GL_ARB_vertex_array_object). */
  ERenderCapabilityElementIndexUnsignedInt,                 /*!< Indexing vertices with Uint32 support (GL_OES_element_index_uint). */
  ERenderCapabilityAutoMipmapping,                          /*!< Auto mipmapping support. */
  ERenderCapabilityCount
};
  
/*! Available orientations. */
enum DeviceOrientation
{
  EOrientationUnknown = 0,                              /*!< The orientation of the device cannot be determined. */
  EOrientationPortrait,                                 /*!< The device is in portrait mode. */
  EOrientationPortraitUpsideDown,                       /*!< The device is in portrait mode but upside down. */
  EOrientationLandscapeLeft,                            /*!< The device is in landscape mode, portrait rotateted 90degs counter-clockwise. */
  EOrientationLandscapeRight,                           /*!< The device is in landscape mode, portrait rotateted 90degs clockwise. */
  EOrientationFaceUp,                                   /*!< The device is held parallel to the ground with the screen facing upwards. */
  EOrientationFaceDown                                  /*!< The device is held parallel to the ground with the screen facing downwards. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Device
{
  public:

    /*! Returns current Operating System ID. */
    static DeviceOS GetOS();
    /*! Returns current device ID. */
    static DeviceType GetDevice();
    /*! Returns device unique identifier. 
     *  @note Empty string is returned in case of an error.
     */
    static String GetUniqueId();

    /*! Returns physical surface width. */
    static s32 SurfaceWidth();
    /*! Returns physical surface height. */
    static s32 SurfaceHeight();
    /*! Sets surface red channel bits count. */
    static void SetSurfaceRedChannelBitsCount(u32 count);
    /*! Returns surface red channel bits count. */
    static u32 SurfaceRedChannelBitsCount();
    /*! Sets surface green channel bits count. */
    static void SetSurfaceGreenChannelBitsCount(u32 count);
    /*! Returns surface green channel bits count. */
    static u32 SurfaceGreenChannelBitsCount();
    /*! Sets surface blue channel bits count. */
    static void SetSurfaceBlueChannelBitsCount(u32 count);
    /*! Returns surface blue channel bits count. */
    static u32 SurfaceBlueChannelBitsCount();
    /*! Sets surface alpha channel bits count. */
    static void SetSurfaceAlphaChannelBitsCount(u32 count);
    /*! Returns surface alpha channel bits count. */
    static u32 SurfaceAlphaChannelBitsCount();
    /*! Returns free RAM memory (in bytes). */
    static u64 AvailableMemory();
    /*! Returns total RAM memory (in bytes). */
    static u64 TotalMemory();

    /*! Returns TRUE if given render capability is present. */
    static bool HasRenderCapability(DeviceRenderCapability cap);
    /*! Sets/Unsets given render capability. */
    static void SetRenderCapability(DeviceRenderCapability cap, bool set);
    /*! Sets number of available texture units.*/
    static void SetTextureUnitsCount(u32 count);
    /*! Returns number of available texture units. */
    static u32 TextureUnitsCount();
    /*! Sets maximal texture size (in texels).*/
    static void SetTextureMaxSize(u32 size);
    /*! Returns maximal texture size (in texels). */
    static u32 TextureMaxSize();

    /*! Returns audio output device frequency (in Hz). */
    static s32 AudioOutputFrequency();

    /*! Sleeps current thread for a specified number of miliseconds. */
    static void Sleep(u32 ms);

  private:

    /*! Render capabilities. */
    static bool m_renderCapabilities[ERenderCapabilityCount];
    /*! Available number of texture units. */
    static u32 m_textureUnitsCount;
    /*! Maximum texture size (in texels). */
    static u32 m_textureMaxSize;
    /*! Surface red channel bits count. */
    static u32 m_surfaceRedChannelBitsCount;
    /*! Surface green channel bits count. */
    static u32 m_surfaceGreenChannelBitsCount;
    /*! Surface blue channel bits count. */
    static u32 m_surfaceBlueChannelBitsCount;
    /*! Surface alpha channel bits count. */
    static u32 m_surfaceAlphaChannelBitsCount;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEVICE_H