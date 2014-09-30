#include "Core/Graphics/Image/ImageLoader.h"
#include "EGEEngine.h"
#include "EGEDebug.h"

#if EGE_IMAGEMANAGER_SINGLE_THREAD
  #include "Core/Graphics/Image/SingleThread/ImageLoaderST_p.h"
#elif EGE_IMAGEMANAGER_MULTI_THREAD
  #include "Core/Graphics/Image/MultiThread/ImageLoaderST_p.h"
#endif // EGE_RESOURCE_MANAGER_SINGLE_THREAD

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KImageLoaderDebugName = "EGEImageLoader";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ImageLoader)
EGE_DEFINE_DELETE_OPERATORS(ImageLoader)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageLoader::ImageLoader(Engine& engine) 
: m_p(NULL)
, m_engine(engine)
{
  // allocate private implementation
  m_p = ege_new ImageLoaderPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageLoader::~ImageLoader()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageLoader::update(const Time& time)
{
  p_func()->update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImageLoader::load(PObject userData, const String& fileName, PixelFormat format)
{
  p_func()->load(userData, fileName, format);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& ImageLoader::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END