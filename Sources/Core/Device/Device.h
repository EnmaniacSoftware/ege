#ifndef EGE_CORE_DEVICE_H
#define EGE_CORE_DEVICE_H

/** Class representing miscellaneous information about device framework is running.
 *  Some information is being supplied by other framework subsystems once its available (ie. render system).
 */

#include "EGE.h"
#include "EGEDevice.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Device
{
  public:

    /*! Returns current Operating System ID. */
    static EGEDevice::OS GetOS();
    /*! Returns current device ID. */
    static EGEDevice::Device GetDevice();
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
    static bool HasRenderCapability(EGEDevice::RenderCapability cap);
    /*! Sets/Unsets given render capability. */
    static void SetRenderCapability(EGEDevice::RenderCapability cap, bool set);
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
    static bool m_renderCapabilities[EGEDevice::RENDER_CAPS_COUNT];
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