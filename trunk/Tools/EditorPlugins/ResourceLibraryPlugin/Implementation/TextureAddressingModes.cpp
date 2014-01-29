#include "TextureAddressingModes.h"
#include <QObject>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAddressingModeInfo::TextureAddressingModeInfo(const QString& modeName, const QString& modeDisplayName, TextureAddressMode textureAddressMode)
  : mode(textureAddressMode),
    displayName(modeDisplayName),
    name(modeName)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAddressingModeInfoList SupportedTextureAddressingModes()
{
  TextureAddressingModeInfoList list;

  list << TextureAddressingModeInfo("clamp", QObject::tr("Clamp"), EAddressModeClamp);
  list << TextureAddressingModeInfo("repeat", QObject::tr("Repeat"), EAddressModeRepeat);

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
