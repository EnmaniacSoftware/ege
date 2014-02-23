#include "Core/Device/Device.h"
#include <EGECipher.h>
#include <windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static u8 l_defaultCipherKeyData[] = { 0x46,0x84,0x38,0x38,0x3c,0x30,0x66,0xf7,0xec,0x54,0x20,0x07,0x07,0x07,0x07,0x07,0x07, 0xfe,0x67,0x83,0xdc,0x61,0x57 };

// "SOFTWARE\\MICROSOFT\\Windows NT\\CurrentVersion" + NULL
static u8 l_encryptedRegistrySubKeyValue[] = { 0x15,0xcb,0x7e,0x6c,0x6b,0x71,0x34,0xb2,0xb0,0x19,0x69,0x44,0x55,0x48,0x54,0x48,0x41,0xaa,0x3b,0xd4,0xb5,0x0f,
                                               0x33,0x29,0xf3,0x4b,0x18,0x72,0x64,0x3a,0xb4,0x99,0x26,0x52,0x62,0x69,0x73,0x51,0x62,0x75,0x8d,0x0e,0xec,0xb2,
                                               0x61 };

// "ProductId" + NULL
static u8 l_encryptedProductIdValueName[] = { 0x16,0xf6,0x57,0x5c,0x49,0x53,0x12,0xbe,0x88,0x54 };
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

  // prepare default cipher key
  DataBuffer defaultKeyData(l_defaultCipherKeyData, sizeof (l_defaultCipherKeyData));
  CipherKey key(defaultKeyData);

  Cipher cipher(EGECipher::ALGORITHM_XOR, EGECipher::DIRECTION_DECRYPT, key);

  // decrypt registry path
  DataBuffer subKeyData(reinterpret_cast<void*>(l_encryptedRegistrySubKeyValue), sizeof (l_encryptedRegistrySubKeyValue));
  if (EGE_SUCCESS == cipher.addData(subKeyData))
  {
    // open registry key
    if (ERROR_SUCCESS == RegOpenKeyExA(HKEY_LOCAL_MACHINE, reinterpret_cast<char*>(cipher.result()->data()), REG_OPTION_NON_VOLATILE, 
                                       KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hRegistryKey))   
    {
      // decrypt value name
      cipher.reset();
      DataBuffer valueNameData(reinterpret_cast<void*>(l_encryptedProductIdValueName), sizeof (l_encryptedProductIdValueName));
      if (EGE_SUCCESS == cipher.addData(valueNameData))
      {
        BYTE byProductID[256];
        DWORD dwDataLength = sizeof (byProductID);

        // retrieve Product ID
        if (ERROR_SUCCESS == RegQueryValueExA(hRegistryKey, reinterpret_cast<char*>(cipher.result()->data()), NULL, NULL, byProductID, &dwDataLength))
        {
          // store id
          uniqueId = reinterpret_cast<char*>(byProductID);
        }
      }

      // close key
      RegCloseKey(hRegistryKey);
    } 
  }

  return uniqueId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END