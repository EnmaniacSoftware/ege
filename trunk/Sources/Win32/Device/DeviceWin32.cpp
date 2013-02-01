#include "Core/Device/Device.h"
#include <EGECipher.h>
#include <windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static u8 l_defaultCipherKeyData[] = { 0x46,0x84,0x38,0x38,0x3c,0x30,0x66,0xf7,0xec,0x54,0x20,0x07,0x07,0x07,0x07,0x07,0x07, 0xfe,0x67,0x83,0xdc,0x61,0x57 };
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEDevice::OS Device::GetOS()
{
  return EGEDevice::OS_WINDOWS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEDevice::Device Device::GetDevice()
{
  return EGEDevice::DEVICE_GENERIC;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Device::SurfaceWidth()
{
  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Device::SurfaceHeight()
{
  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Device::AudioOutputFrequency()
{
  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::Sleep(u32 ms)
{
  ::Sleep(ms);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u64 Device::AvailableMemory()
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u64 Device::TotalMemory()
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String Device::GetUniqueId()
{
  String uniqueId;

  HKEY hRegistryKey;

  // decrypt registry key values
  // NOTE: we keep them encrypted so it is not childlishly easy to inspect the binary and locate them
  DataBuffer defaultKeyData(l_defaultCipherKeyData, sizeof (l_defaultCipherKeyData));

  CipherKey key(defaultKeyData);
  Cipher cipher(EGECipher::ALGORITHM_XOR, EGECipher::DIRECTION_DECRYPT, key);

  // TAGE - this needs to be initially encrypted and decrypted here
  // some default encryption key is required
  const char szSubKeyValue[] = "SOFTWARE\\MICROSOFT\\Windows NT\\CurrentVersion";
  const char szValueName[] = "ProductId";

  // open registry key
  if (ERROR_SUCCESS == RegOpenKeyExA(HKEY_LOCAL_MACHINE, szSubKeyValue, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &hRegistryKey))   
  {
      BYTE byProductID[256];
      DWORD dwDataLength;

      // retrieve Product ID
      if (ERROR_SUCCESS == RegQueryValueExA(hRegistryKey, szValueName, NULL, NULL, byProductID, &dwDataLength))
      {
        // store id
        uniqueId = reinterpret_cast<char*>(byProductID);
      }
  } 

  return uniqueId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END