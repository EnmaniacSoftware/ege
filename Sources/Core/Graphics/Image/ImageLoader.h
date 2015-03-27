#ifndef EGE_CORE_GRAPHICS_IMAGE_IMAGELOADER_H
#define EGE_CORE_GRAPHICS_IMAGE_IMAGELOADER_H

/*! Single threaded implementation for resource manager class.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGETime.h"
#include "EGEImage.h"
#include "EGESignal.h"
#include "Core/Engine/Interface/EngineModule.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Engine;

EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImageLoader
{
  public:

    explicit ImageLoader(Engine& engine);
   ~ImageLoader();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:

    /*! Signal is emitted when image has been loaded. 
     *  @param image    Instance of the loaded image.
     *  @param userData User data given at scheduling time.
     *  @note Signal is emitted in loader's thread. 
     */
    Signal2<PImage, PObject> imageLoadComplete;
    /*! Signal is emitted when image load failed. 
     *  @param fileName Name of the image which failed to load.
     *  @param userData User data given at scheduling time.
     *  @note Signal is emitted in loader's thread. 
     */
    Signal2<const String&, PObject> imageLoadError;

  public:

    /*! Updates object. */
    void update(const Time& time);
    /*! Loads image from given file converting it's pixel format to requested one if possible. 
     *  @param userData  User data which can be used to identify image.
     *  @param fileName  File name to load image from.
     *  @param format    Pixel format loaded image should be converted to.
     *  @return EGE_SUCCESS if image was scheduled for loading.
     *  @note If requested pixel format is PF_UNKNOWN no conversion is done.
     */
    EGEResult load(PObject userData, const String& fileName, PixelFormat format = PF_UNKNOWN);

  private:

    /*! Returns engine object. */
    Engine& engine() const;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(ImageLoader)

    /*! Reference to engine. */
    Engine& m_engine;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_IMAGE_IMAGELOADER_H
