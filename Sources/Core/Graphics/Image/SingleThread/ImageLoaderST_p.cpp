#if EGE_IMAGEMANAGER_SINGLE_THREAD

#include "Core/Graphics/Image/SingleThread/ImageLoaderST_p.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ImageLoaderPrivate)
EGE_DEFINE_DELETE_OPERATORS(ImageLoaderPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageLoaderPrivate::ImageLoaderPrivate(ImageLoader* base) 
: m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageLoaderPrivate::~ImageLoaderPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageLoaderPrivate::load(PObject userData, const String& fileName, PixelFormat format)
{
  // load image
  PImage image = Image::Load(fileName, format);

  // singal result
  if (image->isValid())
  {
    emit d_func()->imageLoadComplete(image, userData);
  }
  else
  {
    emit d_func()->imageLoadError(fileName, userData);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageLoaderPrivate::update(const Time& time)
{
  EGE_UNUSED(time)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IMAGEMANAGER_SINGLE_THREAD