#ifndef EGE_CORE_RESOURCES_HARDWARERESOURCEPROVIDER_H
#define EGE_CORE_RESOURCES_HARDWARERESOURCEPROVIDER_H

/* Interface for hardware resources providers.
 * Such resources include:
 * - textures
 * - vertex buffers
 * etc.
 */

#include "EGEString.h"
#include "EGESignal.h"
#include "EGEVertexBuffer.h"
#include "EGEIndexBuffer.h"
#include "EGEImage.h"
#include "EGEShader.h"
#include "EGEList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
EGE_PREDECLARE_SMART_CLASS(Texture2D, PTexture2D)
EGE_PREDECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_PREDECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Delegate1<PObject> HardwareResourceProviderSlot;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IHardwareResourceProvider
{
  public:

    IHardwareResourceProvider() {}
    virtual ~IHardwareResourceProvider() {}

  public:

    /*! Registers render component. 
     *  @param  component         Component to register.
     *  @param  vertexUsage       Vertex buffer usage hint.
     *  @param  vertexDeclaration Vertex semantics declaration.
     *  @param  indexUsage        Index buffer usage hint.
     *  @return TRUE on success.
     */
    virtual bool registerComponent(PRenderComponent& component, NVertexBuffer::UsageType vertexUsage, const VertexDeclaration& vertexDeclaration, 
                                   EGEIndexBuffer::UsageType indexUsage) = 0;

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
     *  @param  slot  Slot to be called once request is handled.
     *  @return TRUE if request has been accepted. FALSE otherwise.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     */
    virtual bool requestCreateTexture2D(const String& name, const PImage& image, const HardwareResourceProviderSlot& slot = HardwareResourceProviderSlot()) = 0;
    /*! Creates render texture. 
     *  @param  name    Name of the texture.
     *  @param  width   Width of the texture (in pixels).
     *  @param  height  Height of the texture (in pixels).
     *  @param  format  Color format of the texture.
     *  @return Returns created texture. NULL if error occured.
     */
    virtual PTexture2D createRenderTexture(const String& name, s32 width, s32 height, PixelFormat format) = 0;
    /*! Requests deletion of 2D texture. 
     *  @param  texture Texture to destroy.
     *  @param  slot    Slot to be called once request is handled.
     *  @return TRUE if request has been accepted. FALSE otherwise.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     */
    virtual bool requestDestroyTexture2D(PTexture2D texture, const HardwareResourceProviderSlot& slot = HardwareResourceProviderSlot()) = 0;
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
     *  @param  slot  Slot to be called once request is handled.
     *  @return TRUE if request has been accepted. FALSE otherwise.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     */
    virtual bool requestCreateShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data,
                                     const HardwareResourceProviderSlot& slot = HardwareResourceProviderSlot()) = 0;
    /*! Requests deletion of shader. 
     *  @param  shader  Shader to destroy.
     *  @param  slot    Slot to be called once request is handled.
     *  @return TRUE if request has been accepted. FALSE otherwise.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     */
    virtual bool requestDestroyShader(PShader shader, const HardwareResourceProviderSlot& slot = HardwareResourceProviderSlot()) = 0;
    /*! Creates program. 
     *  @param  name    Name of the program.
     *  @param  shaders List of shaders to attach to program.
     *  @return Returns created object. NULL if error occured.
     */
    virtual PProgram createProgram(const String& name, const List<PShader>& shaders) = 0;
    /*! Requests creation of program. 
     *  @param  name    Name of the program.
     *  @param  shaders List of shaders to attach to program.
     *  @param  slot    Slot to be called once request is handled.
     *  @return Returns ID of the request for further delivery check.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     */
    virtual bool requestCreateProgram(const String& name, const List<PShader>& shaders,
                                      const HardwareResourceProviderSlot& slot = HardwareResourceProviderSlot()) = 0;
    /*! Requests deletion of program. 
     *  @param  program Program to destroy.
     *  @param  slot    Slot to be called once request is handled.
     *  @return TRUE if request has been accepted. FALSE otherwise.
     *  @note This method queues the request and will process it later. Processing always takes place in the rendering thread.
     */
    virtual bool requestDestroyProgram(PProgram program, const HardwareResourceProviderSlot& slot = HardwareResourceProviderSlot()) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCES_HARDWARERESOURCEPROVIDER_H