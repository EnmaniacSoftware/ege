#include "Core/Services/Interface/DeviceServices.h"
#include "EGEDebug.h"
#include "EGERandom.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KDeviceServicesDebugName = "EGEDeviceServices";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(DeviceServices)
EGE_DEFINE_DELETE_OPERATORS(DeviceServices)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServices::DeviceServices()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServices::~DeviceServices()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PDataBuffer DeviceServices::aesEnctyptionKey()
{
  EGEResult result = EGE_SUCCESS;

  const String KAESKeyName = "aes-128-key";

  PDataBuffer value = ege_new DataBuffer(16);
  
  // try to retieve value
  if (EGE_ERROR_NOT_FOUND == (result = retrieveConfidentialValue(KAESKeyName, value)))
  {
    egeWarning(KDeviceServicesDebugName) << "AES key not found. Generating...";

    // not found, generate 128-bit (16 bytes) encryption key
    RandomGenerator random;
    
    s32 data[4];
    for (s32 i = 0; i < 4; i++)
    {
      data[i] = random();
    }
    
    if (sizeof (data) != value->write(reinterpret_cast<const void*>(data), sizeof (data)))
    {
      // error!
      egeWarning(KDeviceServicesDebugName) << "Could not generate AES-128 encryption key!";
      value = NULL;
    }
    // store in confidential store
    else if (EGE_SUCCESS != storeConfidentialValue(KAESKeyName, value))
    {
      // error!
      egeWarning(KDeviceServicesDebugName) << "Could not retrieve AES-128 encryption key!";
      value = NULL;
    }
    else
    {
      egeWarning(KDeviceServicesDebugName) << "AES key has been generated!";
    }
  }
  else if (EGE_SUCCESS != result)
  {
    // error!
    value = NULL;
    
    egeWarning(KDeviceServicesDebugName) << "AES could not be retrieved!";
  }
  else
  {
    egeWarning(KDeviceServicesDebugName) << "AES retrieved successfully!";
  }
  
  String keyValue;
  for (s64 i = 0; i < value->size(); ++i)
  {
    const char* data = reinterpret_cast<const char*>(value->data());
    const char dataValue = data[i];
    
    keyValue += String::Format("%02hhX", dataValue);
  }
  egeWarning(KDeviceServicesDebugName) << "AES-128:" << keyValue;

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
