#ifndef RESOURCELIBRARY_IMAGEFORMATS_H
#define RESOURCELIBRARY_IMAGEFORMATS_H

#include <QList>
#include <QString>
#include <QImage>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Image format info structure. */
struct ImageFormatInfo
{
  ImageFormatInfo(const QString& formatName, const QString& formatDisplayName, QImage::Format imageFormat);

  QImage::Format  format;            /*!< Format value. */
  QString         displayName;       /*!< Display name. */
  QString         name;              /*!< Common use name (locale agnostic). */
};

typedef QList<ImageFormatInfo> ImageFormatInfoList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Function returning list of supported image format . */
extern ImageFormatInfoList SupportedImageFormats();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_IMAGEFORMATS_H
