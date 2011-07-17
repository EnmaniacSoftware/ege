#ifndef EGE_CORE_DEVICE_H
#define EGE_CORE_DEVICE_H

/** Class representing miscellaneous information about device framework is running.
 *  Some information is being supplied by other framework subsystems once its available (ie. render system).
 */

#include <EGE.h>
#include <EGEDevice.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Device
{
  public:

    /* Returns current Operating System ID. */
    static EGEDevice::EOS GetOS();
    /* Returns current device ID. */
    static EGEDevice::EDevice GetDevice();
    /* Returns physical surface width. */
    static s32 SurfaceWidth();
    /* Returns physical surface height. */
    static s32 SurfaceHeight();
    /* Returns TRUE if given render capability is present. */
    static bool HasRenderCapability(EGEDevice::ERenderCapability cap);
    /* Sets/Unsets given render capability. */
    static void SetRenderCapability(EGEDevice::ERenderCapability cap, bool set);
    /* Sets number of available texture units.*/
    static void SetTextureUnitsCount(u32 count);
    /* Gets number of available texture units. */
    static u32 GetTextureUnitsCount();
    /* Sets maximal texture size (in texels).*/
    static void SetTextureMaxSize(u32 size);
    /* Gets maximal texture size (in texels). */
    static u32 GetTextureMaxSize();

  private:

    /*! Render capabilities. */
    static bool m_renderCapabilities[EGEDevice::RENDER_CAPS_COUNT];
    /*! Available number of texture units. */
    static u32 m_textureUnitsCount;
    /*! Maximum texture size (in texels). */
    static u32 m_textureMaxSize;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEVICE_H