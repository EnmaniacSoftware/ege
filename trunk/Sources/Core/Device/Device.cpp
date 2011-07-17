#include "Core/Device/Device.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::m_renderCapabilities[EGEDevice::RENDER_CAPS_COUNT] = { false };
u32 Device::m_textureUnitsCount = 0;
u32 Device::m_textureMaxSize = 0;
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
/*! Gets number of available texture units. */
u32 Device::GetTextureUnitsCount()
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
/*! Gets maximal texture size (in texels). */
u32 Device::GetTextureMaxSize()
{
  return m_textureMaxSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
