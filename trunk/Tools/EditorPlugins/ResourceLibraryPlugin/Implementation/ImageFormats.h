#ifndef RESOURCELIBRARY_IMAGEFORMATS_H
#define RESOURCELIBRARY_IMAGEFORMATS_H

#include <QList>
#include <QString>
#include <QImage>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Image format info structure. */
struct ImageFormatInfo
{
  ImageFormatInfo(QImage::Format imageFormat, const QString& imageDisplayName);

  QImage::Format format;            /*! Format value. */
  QString        displayName;       /*! Display name. */
};

typedef QList<ImageFormatInfo> ImageFormatInfoList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Function returning list of supported image types. */
extern ImageFormatInfoList SupportedImageFormats();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_IMAGEFORMATS_H
