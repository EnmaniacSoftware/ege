#include "Core/Device/Device.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KDeviceDebugName = "EGEDevice";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::m_renderCapabilities[EGEDevice::RENDER_CAPS_COUNT] = { false };
u32 Device::m_textureUnitsCount = 0;
u32 Device::m_textureMaxSize = 0;
u32 Device::m_surfaceRedChannelBitsCount = 0;
u32 Device::m_surfaceGreenChannelBitsCount = 0;
u32 Device::m_surfaceBlueChannelBitsCount = 0;
u32 Device::m_surfaceAlphaChannelBitsCount = 0;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::HasRenderCapability(EGEDevice::RenderCapability cap)
{
  return m_renderCapabilities[cap];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::SetRenderCapability(EGEDevice::RenderCapability cap, bool set)
{
  m_renderCapabilities[cap] = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::SetTextureUnitsCount(u32 count)
{
  m_textureUnitsCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 Device::TextureUnitsCount()
{
  return m_textureUnitsCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::SetTextureMaxSize(u32 size)
{
  m_textureMaxSize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 Device::TextureMaxSize()
{
  return m_textureMaxSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::SetSurfaceRedChannelBitsCount(u32 count)
{
  m_surfaceRedChannelBitsCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 Device::SurfaceRedChannelBitsCount()
{
  return m_surfaceRedChannelBitsCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::SetSurfaceGreenChannelBitsCount(u32 count)
{
  m_surfaceGreenChannelBitsCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 Device::SurfaceGreenChannelBitsCount()
{
  return m_surfaceGreenChannelBitsCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::SetSurfaceBlueChannelBitsCount(u32 count)
{
  m_surfaceBlueChannelBitsCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 Device::SurfaceBlueChannelBitsCount()
{
  return m_surfaceBlueChannelBitsCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::SetSurfaceAlphaChannelBitsCount(u32 count)
{
  m_surfaceAlphaChannelBitsCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 Device::SurfaceAlphaChannelBitsCount()
{
  return m_surfaceAlphaChannelBitsCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END