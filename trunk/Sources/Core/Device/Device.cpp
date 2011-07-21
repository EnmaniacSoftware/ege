#include "Core/Device/Device.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::m_renderCapabilities[EGEDevice::RENDER_CAPS_COUNT] = { false };
u32 Device::m_textureUnitsCount = 0;
u32 Device::m_textureMaxSize = 0;
u32 Device::m_surfaceRedChannelBitsCount = 0;
u32 Device::m_surfaceGreenChannelBitsCount = 0;
u32 Device::m_surfaceBlueChannelBitsCount = 0;
u32 Device::m_surfaceAlphaChannelBitsCount = 0;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given render capability is present. */
bool Device::HasRenderCapability(EGEDevice::ERenderCapability cap)
{
  return m_renderCapabilities[cap];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets/Unsets given render capability. */
void Device::SetRenderCapability(EGEDevice::ERenderCapability cap, bool set)
{
  m_renderCapabilities[cap] = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets number of available texture units.*/
void Device::SetTextureUnitsCount(u32 count)
{
  m_textureUnitsCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of available texture units. */
u32 Device::TextureUnitsCount()
{
  return m_textureUnitsCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets maximal texture size (in texels).*/
void Device::SetTextureMaxSize(u32 size)
{
  m_textureMaxSize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns maximal texture size (in texels). */
u32 Device::TextureMaxSize()
{
  return m_textureMaxSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets surface red channel bits count. */
void Device::SetSurfaceRedChannelBitsCount(u32 count)
{
  m_surfaceRedChannelBitsCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns surface red channel bits count. */
u32 Device::SurfaceRedChannelBitsCount()
{
  return m_surfaceRedChannelBitsCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets surface green channel bits count. */
void Device::SetSurfaceGreenChannelBitsCount(u32 count)
{
  m_surfaceGreenChannelBitsCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns surface green channel bits count. */
u32 Device::SurfaceGreenChannelBitsCount()
{
  return m_surfaceGreenChannelBitsCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets surface blue channel bits count. */
void Device::SetSurfaceBlueChannelBitsCount(u32 count)
{
  m_surfaceBlueChannelBitsCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns surface blue channel bits count. */
u32 Device::SurfaceBlueChannelBitsCount()
{
  return m_surfaceBlueChannelBitsCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets surface alpha channel bits count. */
void Device::SetSurfaceAlphaChannelBitsCount(u32 count)
{
  m_surfaceAlphaChannelBitsCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns surface alpha channel bits count. */
u32 Device::SurfaceAlphaChannelBitsCount()
{
  return m_surfaceAlphaChannelBitsCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
