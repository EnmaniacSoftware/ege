#include "EGEApplication.h"
#include "EGERenderComponent.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Component/Physics/PhysicsComponent.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Event/Event.h"
#include "Core/Event/EventIDs.h"
#include "Core/Event/EventManager.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/ObjectUIDs.h"
#include "EGERenderQueues.h"
#include "EGEOpenGL.h"
#include "EGEDevice.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KRenderSystemDebugName = "EGERenderSystem";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Function calculating render queues hash.
 *  @param  priority      Priority of render component.
 *  @param  primitiveType Primitive type to be rendered.
 *  @return Calculated hash value.
 */
u32 CalculateRenderQueueHash(u32 priority, EGEGraphics::RenderPrimitiveType primitiveType)
{
  // NOTE: priority is stored in upper 24 bits while primitive is stored in least significant 8 bits.
  //       Due to such composition priority is always leading as rendering is supposed to following priority settings.
  return (priority << 8) | primitiveType;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RenderSystem)
EGE_DEFINE_DELETE_OPERATORS(RenderSystem)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystem::RenderSystem(Application* app) : Object(app)
                                             , m_state(STATE_NONE)
                                             , m_textureMinFilter(TF_NEAREST)
                                             , m_textureMagFilter(TF_NEAREST)
                                             , m_textureAddressingModeS(AM_CLAMP)
                                             , m_textureAddressingModeT(AM_CLAMP)
                                             , m_textureMipMapping(false)
                                             , m_nextRequestID(1)
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystem::~RenderSystem()
{
  app()->eventManager()->removeListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult RenderSystem::construct()
{
  // create access mutex
  m_requestsMutex = ege_new Mutex(app());
  if (NULL == m_requestsMutex)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // subscribe for event notifications
  if ( ! app()->eventManager()->addListener(this))
  {
    // error!
    egeCritical(KRenderSystemDebugName) << EGE_FUNC_INFO << "Could not register for notifications!";
    return EGE_ERROR;
  }

  // set state
  m_state = STATE_READY;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::update()
{
  if ( ! m_requests.empty())
  {
    // copy for processing
    m_requestsMutex->lock();
    RequestDataList queue(m_requests);
    m_requests.clear();
    m_requestsMutex->unlock();

    // process locally
    for (RequestDataList::iterator it = queue.begin(); it != queue.end(); ++it)
    {
      RequestData& request = *it;

      if (REQUEST_CREATE_TEXTURE_2D == request.type)
      {
        // apply texture params
        setTextureMinFilter(request.textureMinFilter);
        setTextureMagFilter(request.textureMagFilter);
        setTextureAddressingModeS(request.textureAddressingModeS);
        setTextureAddressingModeT(request.textureAddressingModeT);
        setTextureMipMapping(request.textureMipMapping);

        // create texture
        PImage image = request.objects.front();
        PTexture2D texture = createTexture2D(request.name, image);

        // signal
        emit requestComplete(request.id, texture);
      }
      else if (REQUEST_DESTROY_TEXTURE_2D == request.type)
      {
        // NOTE: if the last remaining instance, underlying object will be destroyed here
        //       This is safe as this function is called from main thread
        request.objects.clear();

        // signal
        emit requestComplete(request.id, NULL);
      }
      else if (REQUEST_CREATE_SHADER == request.type)
      {
        PDataBuffer data = request.objects.front();
        PShader shader = createShader(request.shaderType, request.name, data);

        // signal
        emit requestComplete(request.id, shader);
      }
      else if (REQUEST_DESTROY_SHADER == request.type)
      {
        // NOTE: if the last remaining instance, underlying object will be destroyed here
        //       This is safe as this function is called from main thread
        request.objects.clear();

        // signal
        emit requestComplete(request.id, NULL);
      }
      else if (REQUEST_CREATE_PROGRAM == request.type)
      {
        List<PShader> shadersList;
        for (ObjectList::const_iterator itObject = request.objects.begin(); itObject != request.objects.end(); ++itObject)
        {
          shadersList << *itObject;
        }

        PProgram program = createProgram(request.name, shadersList);

        // signal
        emit requestComplete(request.id, program);
      }
      else if (REQUEST_DESTROY_PROGRAM == request.type)
      {
        // NOTE: if the last remaining instance, underlying object will be destroyed here
        //       This is safe as this function is called from main thread
        request.objects.clear();

        // signal
        emit requestComplete(request.id, NULL);
      }
    }
  }
  else if (STATE_CLOSING == m_state)
  {
    // NOTE: wait till resource manager is done processing
    if (ResourceManager::STATE_CLOSED == app()->resourceManager()->state())
    {
      m_state = STATE_CLOSED;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setViewport(const PViewport& viewport)
{
  // change render target
  if (m_renderTarget != viewport->renderTarget())
  {
    // unbind current render target
    if (NULL != m_renderTarget)
    {
      m_renderTarget->unbind();
    }

    m_renderTarget = viewport->renderTarget();

    // bind new target
    m_renderTarget->bind();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderSystem::addForRendering(const PRenderComponent& component, const Matrix4f& worldMatrix)
{
  bool result = true;

  // check if component is meaningful
  if ((0 < component->vertexBuffer()->vertexCount()) && (NULL != component->material()))
  {
    bool added = false;

    // calculate hash
    // NOTE: at this stage components are separated into unique priorities and primitive types
    u32 hash = CalculateRenderQueueHash(component->priority(), component->primitiveType());

    // check if proper queue exists already
    bool renderQueueForHashPresent = m_renderQueues.contains(hash);
    if (renderQueueForHashPresent)
    {
      List<PRenderQueue>& queueList = m_renderQueues.at(hash);

      // try to add to one of the existing render queues
      for (List<PRenderQueue>::iterator it = queueList.begin(); it != queueList.end(); ++it)
      {
        PRenderQueue& queue = *it;

        // add data into queue
        EGEResult addResult = queue->addForRendering(component, worldMatrix);
        if (EGE_SUCCESS == addResult)
        {
          // set flag
          added = true;

          // done
          break;
        }
        else if (EGE_ERROR == addResult)
        {
          // general failure
          result = false;

          // done
          break;
        }
      }
    }

    // check if no suitable queue has been found (but no error occured)
    if (result && ! added)
    {
      result = false;

      // create appriopriate queue
      PRenderQueue queue;

      // batch only really small buffers
      // NOTE: batching enabled only for non-VBO rendering due to issues with updating batch data ie no way to get vertex data from VBO in write-only mode
      if ((10 > component->vertexBuffer()->vertexCount()) && 
          ! Device::HasRenderCapability(EGEDevice::RENDER_CAPS_VERTEX_BUFFER_OBJECT) && 
          RenderQueue::IsSuitable(EGE_OBJECT_UID_BACTHED_RENDER_QUEUE, component))
      {
        queue = RenderQueueFactory::Create(app(), EGE_OBJECT_UID_BACTHED_RENDER_QUEUE, component->priority(), component->primitiveType());
      }
      else
      {
        queue = RenderQueueFactory::Create(app(), EGE_OBJECT_UID_SIMPLE_RENDER_QUEUE, component->priority(), component->primitiveType());
      }

      // check if properly allocated
      if (NULL != queue)
      {
        // add to newly created queue
        result = (EGE_SUCCESS == queue->addForRendering(component, worldMatrix));
      }

      // add it into pool if ok
      if (result)
      {
        if (renderQueueForHashPresent)
        {
          m_renderQueues.at(hash).push_back(queue);
        }
        else
        {
          m_renderQueues.insert(hash, queue);
        }
      }
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderSystem::addForRendering(const PRenderQueue& queue)
{
  bool result = false;
 
  // calculate hash
  // NOTE: at this stage components are separated into unique priorities and primitive types
  u32 hash = CalculateRenderQueueHash(queue->priority(), queue->primitiveType());

  // check if some queues exist already
  bool renderQueueForHashPresent = m_renderQueues.contains(hash);
  if (renderQueueForHashPresent)
  {
    List<PRenderQueue>& queueList = m_renderQueues.at(hash);

    // try to find the same queue
    for (List<PRenderQueue>::iterator it = queueList.begin(); it != queueList.end(); ++it)
    {
      PRenderQueue& curQueue = *it;

      // check if the same
      if (curQueue == queue)
      {
        // substitute
        curQueue = queue;

        // done
        result = true;
        break;
      }
    }

    // check if not found yet
    if ( ! result)
    {
      // add to list
      queueList.push_back(queue);

      // done
      result = true;
    }
  }
  else
  {
    // insert into pool
    m_renderQueues.insert(hash, queue);

    // done
    result = true;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setProjectionMatrix(const Matrix4f& matrix)
{
  // store value
  m_projectionMatrix = matrix;

  // check if Y axis flipping is required
  if (m_renderTarget->requiresTextureFlipping())
  {
    // invert transfrormed Y
    m_projectionMatrix.data[1]  = -m_projectionMatrix.data[1];
    m_projectionMatrix.data[5]  = -m_projectionMatrix.data[5];
    m_projectionMatrix.data[9]  = -m_projectionMatrix.data[9];
    m_projectionMatrix.data[13] = -m_projectionMatrix.data[13];
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setViewMatrix(const Matrix4f& matrix)
{
  // set value
  m_viewMatrix = matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Matrix4f& RenderSystem::viewMatrix() const 
{ 
  return m_viewMatrix; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::resetStats()
{
  m_batchCount  = 0;
  m_vertexCount = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Rectf RenderSystem::applyRotation(const Rectf& rect, const Angle& angle) const
{
  Rectf out;

  // 0 degrees rotation
  if (0.0f == Math::ZeroRoundOff(angle.degrees() - 0.0f))
  {
    // do nothing
    out = rect;

    //EGE_PRINT("1. %.2f %.2f %.2f %.2f -> %.2f %.2f %.2f %2.f @ %.2f", rect.x, rect.y, rect.width, rect.height, out.x, out.y, out.width, out.height, angle.degrees());
  }
  // 90 degrees rotation
  else if (0.0f == Math::ZeroRoundOff(angle.degrees() - 90.0f))
  {
    out.x       = rect.y;
    out.y       = m_renderTarget->width() - rect.width - rect.x;
    out.width   = rect.height;
    out.height  = rect.width;

    //EGE_PRINT("2. %.2f %.2f %.2f %.2f -> %.2f %.2f %.2f %2.f @ %.2f", rect.x, rect.y, rect.width, rect.height, out.x, out.y, out.width, out.height, angle.degrees());
  }
  // 180 degrees rotation
  else if (0.0f == Math::ZeroRoundOff(angle.degrees() - 180.0f))
  {
    out.x       = m_renderTarget->width() - rect.width - rect.x;
    out.y       = m_renderTarget->height() - rect.height - rect.y;
    out.width   = rect.width;
    out.height  = rect.height;

    //EGE_PRINT("3. %.2f %.2f %.2f %.2f -> %.2f %.2f %.2f %2.f @ %.2ff", rect.x, rect.y, rect.width, rect.height, out.x, out.y, out.width, out.height, angle.degrees());
  }
  // 270 degrees rotation
  else if (0.0f == Math::ZeroRoundOff(angle.degrees() - 270.0f))
  {
    out.x       = m_renderTarget->height() - rect.height - rect.y;
    out.y       = rect.x;
    out.width   = rect.height;
    out.height  = rect.width;

    //EGE_PRINT("4. %.2f %.2f %.2f %.2f -> %.2f %.2f %.2f %2.f @ %.2f", rect.x, rect.y, rect.width, rect.height, out.x, out.y, out.width, out.height, angle.degrees());
  }
  else
  {
    egeWarning(KRenderSystemDebugName) << "Unsupported angle:" << angle.degrees();
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderSystem::requestCreateTexture2D(const String& name, const PImage& image)
{
  // create request
  RequestData request;
  request.type                    = REQUEST_CREATE_TEXTURE_2D;
  request.id                      = m_nextRequestID++;
  request.name                    = name;
  request.textureMinFilter        = m_textureMinFilter;
  request.textureMagFilter        = m_textureMagFilter;
  request.textureAddressingModeS  = m_textureAddressingModeS;
  request.textureAddressingModeT  = m_textureAddressingModeT;
  request.textureMipMapping       = m_textureMipMapping;
  request.objects << image;

  // queue it
  MutexLocker locker(m_requestsMutex);
  m_requests.push_back(request);

  return request.id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderSystem::requestDestroyTexture2D(PTexture2D texture)
{
  // create request
  RequestData request;
  request.type    = REQUEST_DESTROY_TEXTURE_2D;
  request.id      = m_nextRequestID++;
  request.objects << texture;

  // queue it
  MutexLocker locker(m_requestsMutex);
  m_requests.push_back(request);

  egeDebug(KRenderSystemDebugName) << "Requested texture destroy:" << texture->name();

  return request.id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderSystem::requestCreateShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data)
{
  // create request
  RequestData request;
  request.type       = REQUEST_CREATE_SHADER;
  request.id         = m_nextRequestID++;
  request.name       = name;
  request.shaderType = type;
  request.objects << data;

  // queue it
  MutexLocker locker(m_requestsMutex);
  m_requests.push_back(request);

  return request.id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderSystem::requestDestroyShader(PShader shader)
{
  // create request
  RequestData request;
  request.type = REQUEST_DESTROY_SHADER;
  request.id   = m_nextRequestID++;
  request.objects << shader;

  // queue it
  MutexLocker locker(m_requestsMutex);
  m_requests.push_back(request);

  return request.id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderSystem::requestCreateProgram(const String& name, const List<PShader>& shaders)
{
  // create request
  RequestData request;
  request.type = REQUEST_CREATE_PROGRAM;
  request.id   = m_nextRequestID++;
  request.name = name;

  for (List<PShader>::const_iterator it = shaders.begin(); it != shaders.end(); ++it)
  {
    request.objects << *it;
  }

  // queue it
  MutexLocker locker(m_requestsMutex);
  m_requests.push_back(request);

  return request.id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderSystem::requestDestroyProgram(PProgram program)
{
  // create request
  RequestData request;
  request.type    = REQUEST_DESTROY_PROGRAM;
  request.id      = m_nextRequestID++;
  request.objects << program;

  // queue it
  MutexLocker locker(m_requestsMutex);
  m_requests.push_back(request);

  return request.id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setTextureMinFilter(TextureFilter filter)
{
  m_textureMinFilter = filter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setTextureMagFilter(TextureFilter filter)
{
  m_textureMagFilter = filter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setTextureAddressingModeS(TextureAddressingMode mode)
{
  m_textureAddressingModeS = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setTextureAddressingModeT(TextureAddressingMode mode)
{
  m_textureAddressingModeT = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setTextureMipMapping(bool set)
{
  m_textureMipMapping = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderSystem::batchCount() const 
{ 
  return m_batchCount; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderSystem::vertexCount() const 
{ 
  return m_vertexCount; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderTarget RenderSystem::currentRenderTarget() const 
{ 
  return m_renderTarget; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystem::State RenderSystem::state() const
{
  return m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::onEventRecieved(PEvent event)
{
  switch (event->id())
  {
    case EGE_EVENT_ID_CORE_QUIT_REQUEST:

      m_state = STATE_CLOSING;
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setActiveRenderComponent(const PRenderComponent& component)
{
  m_renderComponent = component;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const PRenderComponent& RenderSystem::activeRenderComponent() const
{
  return m_renderComponent;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
