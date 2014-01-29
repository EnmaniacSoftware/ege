#ifndef RESOURCELIBRARY_TEXTUREADDRESSINGMODES_H
#define RESOURCELIBRARY_TEXTUREADDRESSINGMODES_H

#include <QString>
#include <QList>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available texture addressing modes. */
enum TextureAddressMode
{
  EAddressModeClamp,
  EAddressModeRepeat
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct TextureAddressingModeInfo
{
  TextureAddressingModeInfo(const QString& modeName, const QString& modeDisplayName, TextureAddressMode textureAddressMode);

  TextureAddressMode mode;            /*!< Mode value. */
  QString            displayName;     /*!< Mode display name. */
  QString            name;            /*!< Common use name (locale agnostic). */
};

typedef QList<TextureAddressingModeInfo> TextureAddressingModeInfoList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Function returning list of supported texture addressing modes. */
extern TextureAddressingModeInfoList SupportedTextureAddressingModes();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_TEXTUREADDRESSINGMODES_H
