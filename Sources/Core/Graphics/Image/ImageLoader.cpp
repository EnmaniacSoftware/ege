#include "Core/Graphics/Image/ImageLoader.h"
#include "Core/Event/Event.h"
#include "Core/Event/EventIDs.h"
#include "Core/Event/EventManager.h"
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
ImageLoader::ImageLoader(Engine& engine) : Object()
                                         , m_p(NULL)
                                         , m_engine(engine)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageLoader::~ImageLoader()
{
  EGE_DELETE(m_p);

  engine().eventManager()->removeListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImageLoader::construct()
{
  // allocate private implementation
  m_p = ege_new ImageLoaderPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // subscribe for event notifications
  if ( ! engine().eventManager()->addListener(this))
  {
    // error!
    egeCritical(KImageLoaderDebugName) << EGE_FUNC_INFO << "Could not register for notifications!";
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
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
void ImageLoader::shutDown()
{
  p_func()->shutDown();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageLoader::State ImageLoader::state() const
{
  return p_func()->state();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageLoader::onEventRecieved(PEvent event)
{
  switch (event->id())
  {
    case EGE_EVENT_ID_CORE_QUIT_REQUEST:

      if ((STATE_CLOSING != p_func()->state()) && (STATE_CLOSED != p_func()->state()))
      {
        // do shouting down
        shutDown();
      }
      break;

    //case EGE_EVENT_ID_CORE_FRAME_END:

    //  if (STATE_READY == p_func()->state())
    //  {
    //    processCommands();
    //  }
    //  break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& ImageLoader::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END