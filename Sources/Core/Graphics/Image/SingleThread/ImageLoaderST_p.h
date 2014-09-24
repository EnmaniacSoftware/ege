#ifndef EGE_CORE_GRAPHICS_IMAGE_IMAGELOADERST_PRIVATE_H
#define EGE_CORE_GRAPHICS_IMAGE_IMAGELOADERST_PRIVATE_H

#if EGE_IMAGEMANAGER_SINGLE_THREAD

/*! Single threaded implementation for image loader class.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGETime.h"
#include "EGEImage.h"
#include "Core/Graphics/Image/ImageLoader.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImageLoader;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImageLoaderPrivate
{
  public:

    ImageLoaderPrivate(ImageLoader* base);
   ~ImageLoaderPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(ImageLoader)

  public:

    /*! Creates object. */
    EGEResult construct();
    /*! @see ImageLoader::update. */
    void update(const Time& time);
    /*! @see ImageLoader::load. */
    void load(PObject userData, const String& fileName, PixelFormat format);
    /*! @see ImageLoader::shutDown. */
    void shutDown();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IMAGEMANAGER_SINGLE_THREAD

#endif // EGE_CORE_GRAPHICS_IMAGE_IMAGELOADERST_PRIVATE_H
