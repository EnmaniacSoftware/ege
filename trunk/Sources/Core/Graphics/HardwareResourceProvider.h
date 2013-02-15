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
#include <EGEImage.h>
#include <EGEShader.h>
#include <EGEList.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
EGE_DECLARE_SMART_CLASS(Texture2D, PTexture2D)
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IHardwareResourceProvider
{
  public:

    IHardwareResourceProvider() {}
    virtual ~IHardwareResourceProvider() {}

  signals:

    /*! Signal emitted when request has been completed. 
     *  @param  requestId   ID of the request.
     *  @param  object      Requested object.
     *  @note Signal is emitted in RenderSystem's thread. In case of creation requests, if object is NULL indicates error.
     */
    Signal2<u32, PObject> requestComplete;

  public:

    /*! Creates vertex buffer object. 
     *  @param  usage Intended usage hint.
     */
    virtual PVertexBuffer createVertexBuffer(EGEVertexBuffer::UsageType usage) const = 0;
    /*! Destroys vertex buffer object. 
     *  @param  object Object to destroy.
     */
    virtual void destroyVertexBuffer(PVertexBuffer object) const = 0;
    /*! Reqeusts deletion of vertex buffer object. 
     *  @param  object Object to destroy.
     *  @return Returns ID of the request for further delivery check.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     *        Upon completion result will be signalled by requestComplete.
     */
    virtual u32 requestDestroyVertexBuffer(PVertexBuffer object) = 0;
    /*! Creates index buffer object. 
     *  @param  usage Intended usage hint.
     */
    virtual PIndexBuffer createIndexBuffer(EGEIndexBuffer::UsageType usage) const = 0;
    /*! Destroys index buffer object. 
     *  @param  object Object to destroy.
     */
    virtual void destroyIndexBuffer(PIndexBuffer object) const = 0;
    /*! Reqeusts deletion of index buffer object. 
     *  @param  object Object to destroy.
     *  @return Returns ID of the request for further delivery check.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     *        Upon completion result will be signalled by requestComplete.
     */
    virtual u32 requestDestroyIndexBuffer(PIndexBuffer object) = 0;
    /*! Creates 2D texture from given image. 
     *  @param  name  Name of the object.
     *  @param  image Image data for texture.
     *  @return Returns created texture. NULL if error occured.
     */
    virtual PTexture2D createTexture2D(const String& name, const PImage& image) = 0;
    /*! Creates 2D texture from given data. 
     *  @param  name  Name of the texture.
     *  @param  image Image data for texture.
     *  @return Returns created texture. NULL if error occured.
     */
    virtual PTexture2D createTexture2D(const String& name, const PDataBuffer& data) = 0;
    /*! Requests creation of 2D texture from given image. 
     *  @param  name  Name of the texture.
     *  @param  image Image data for texture.
     *  @return Returns ID of the request for further delivery check.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     *        Upon completion result will be signalled by requestComplete.
     */
    virtual u32 requestCreateTexture2D(const String& name, const PImage& image) = 0;
    /*! Creates render texture. 
     *  @param  name    Name of the texture.
     *  @param  width   Width of the texture (in pixels).
     *  @param  height  Height of the texture (in pixels).
     *  @param  format  Color format of the texture.
     *  @return Returns created texture. NULL if error occured.
     */
    virtual PTexture2D createRenderTexture(const String& name, s32 width, s32 height, PixelFormat format) = 0;
    /*! Destroys 2D texture. 
     *  @param  texture Texture to destroy.
     */
    virtual void destroyTexture2D(PTexture2D texture) = 0;
    /*! Requests deletion of 2D texture. 
     *  @param  texture Texture to destroy.
     *  @return Returns ID of the request for further delivery check.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     *        Upon completion result will be signalled by requestComplete.
     */
    virtual u32 requestDestroyTexture2D(PTexture2D texture) = 0;
    /*! Creates shader from given data. 
     *  @param  type  Type of the shader.
     *  @param  name  Name of the shader.
     *  @param  data  Shader data.
     *  @return Returns created shader. NULL if error occured.
     */
    virtual PShader createShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data) = 0;
    /*! Requests creation shader from given data. 
     *  @param  type  Type of the shader.
     *  @param  name  Name of the shader.
     *  @param  data  Shader data.
     *  @return Returns ID of the request for further delivery check.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     *        Upon completion result will be signalled by requestComplete.
     */
    virtual u32 requestCreateShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data) = 0;
    /*! Destroys shader. 
     *  @param  shader Shader to destroy.
     */
    virtual void destroyShader(PShader shader) = 0;
    /*! Requests deletion of shader. 
     *  @param  shader  Shader to destroy.
     *  @return Returns ID of the request for further delivery check.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     *        Upon completion result will be signalled by requestComplete.
     */
    virtual u32 requestDestroyShader(PShader shader) = 0;
    /*! Creates program. 
     *  @param  name    Name of the program.
     *  @param  shaders List of shaders to attach to program.
     *  @return Returns created object. NULL if error occured.
     */
    virtual PProgram createProgram(const String& name, const List<PShader>& shaders) = 0;
    /*! Requests creation of program. 
     *  @param  name    Name of the program.
     *  @param  shaders List of shaders to attach to program.
     *  @return Returns ID of the request for further delivery check.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     *        Upon completion result will be signalled by requestComplete.
     */
    virtual u32 requestCreateProgram(const String& name, const List<PShader>& shaders) = 0;
    /*! Destroys program. 
     *  @param  program Program to destroy.
     */
    virtual void destroyProgram(PProgram program) = 0;
    /*! Requests deletion of program. 
     *  @param  program  Program to destroy.
     *  @return Returns ID of the request for further delivery check.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     *        Upon completion result will be signalled by requestComplete.
     */
    virtual u32 requestDestroyProgram(PProgram program) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCES_HARDWARERESOURCEPROVIDER_H