#ifndef EGE_CORE_RESOURCES_HARDWARERESOURCEPROVIDER_H
#define EGE_CORE_RESOURCES_HARDWARERESOURCEPROVIDER_H

/* Interface for hardware resources providers.
 * Such resources include:
 * - textures
 * - vertex buffers
 * etc.
 */

#include <EGEString.h>
#include <EGESignal.h>
#include <EGEVertexBuffer.h>
#include <EGEIndexBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
EGE_DECLARE_SMART_CLASS(Image, PImage)
EGE_DECLARE_SMART_CLASS(Texture2D, PTexture2D)
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IHardwareResourceProvider
{
  public:

    IHardwareResourceProvider();
    virtual ~IHardwareResourceProvider();

  signals:

    /*! Signal emitted when request has been completed. 
     *  @param  requestId   ID of the request.
     *  @param  object      Requested object. NULL if failed.
     *  @note Signal is emitted in RenderSystem's thread. 
     */
    Signal2<u32, PObject> requestComplete;

  public:

    /*! Creates vertex buffer obejct. */
    virtual PVertexBuffer createVertexBuffer(EGEVertexBuffer::UsageType usage) const = 0;
    /*! Creates index buffer obejct. */
    virtual PIndexBuffer createIndexBuffer(EGEIndexBuffer::UsageType usage) const = 0;
    /*! Creates 2D texture from given image. 
     *  @param  name  Name of the texture.
     *  @param  image Image data for texture.
     */
    virtual PTexture2D createTexture2D(const String& name, const PImage& image) = 0;
    /*! Requests creation of 2D texture from given image. 
     *  @param  name  Name of the texture.
     *  @param  image Image data for texture.
     *  @return Returns ID of the request for further delivery check.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     *        Upon completion result will be signalled by requestTexture2DComplete.
     */
    virtual u32 requestTexture2D(const String& name, const PImage& image) = 0;
    /*! Creates 2D texture from given data. 
     *  @param  name  Name of the texture.
     *  @param  image Image data for texture.
     */
    virtual PTexture2D createTexture2D(const String& name, const PDataBuffer& data) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCES_HARDWARERESOURCEPROVIDER_H