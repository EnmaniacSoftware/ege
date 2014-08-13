#if EGE_IMAGEMANAGER_SINGLE_THREAD

#include "Core/Graphics/Image/SingleThread/ImageLoaderST_p.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ImageLoaderPrivate)
EGE_DEFINE_DELETE_OPERATORS(ImageLoaderPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageLoaderPrivate::ImageLoaderPrivate(ImageLoader* base) : m_d(base)
                                                          , m_state(ImageLoader::STATE_NONE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageLoaderPrivate::~ImageLoaderPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImageLoaderPrivate::construct()
{
  // set state
  m_state = ImageLoader::STATE_READY;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageLoaderPrivate::update(const Time& time)
{
  EGE_UNUSED(time)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageLoaderPrivate::shutDown()
{
  // we are done
  m_state = ImageLoader::STATE_CLOSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageLoader::State ImageLoaderPrivate::state() const
{
  return m_state;
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

#endif // EGE_IMAGEMANAGER_SINGLE_THREAD