#include "EGEApplication.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Components/Physics/PhysicsComponent.h"
#include "Core/Components/Render/RenderComponent.h"
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

#if EGE_RENDERING_OPENGL_FIXED
  #include "Core/Graphics/OpenGL/Fixed/RenderSystemFixedOGL_p.h"
#endif // EGE_RENDERING_OPENGL_FIXED

EGE_NAMESPACE_BEGIN

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
RenderSystem::RenderSystem(Application* app) : Object(app),
                                               IRenderer(),
                                               IHardwareResourceProvider(),
                                               m_p(NULL),
                                               m_state(STATE_NONE),
                                               m_textureMinFilter(EGETexture::BILINEAR),
                                               m_textureMagFilter(EGETexture::BILINEAR),
                                               m_textureAddressingModeS(EGETexture::AM_CLAMP),
                                               m_textureAddressingModeT(EGETexture::AM_CLAMP),
                                               m_nextRequestID(1)
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystem::~RenderSystem()
{
  EGE_DELETE(m_p);

  app()->eventManager()->removeListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult RenderSystem::construct()
{
  // create private implementation
  m_p = ege_new RenderSystemPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

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

        // create texture
        PImage image = request.objects.front();
        PTexture2D texture = createTexture2D(request.name, image);

        // signal
        emit requestComplete(request.id, texture);

        // check if no one interested so far
        if (1 == texture.object()->referenceCount())
        {
          // delete it
          destroyTexture2D(texture);
        }
      }
      else if (REQUEST_DESTROY_TEXTURE_2D == request.type)
      {
        PTexture2D texture = request.objects.front();
        destroyTexture2D(texture);

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
        PShader shader = request.objects.front();
        destroyShader(shader);

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
        PProgram program = request.objects.front();
        destroyProgram(program);

        // signal
        emit requestComplete(request.id, NULL);
      }
      else if (REQUEST_DESTROY_VERTEX_BUFFER == request.type)
      {
        PVertexBuffer object = request.objects.front();
        destroyVertexBuffer(object);

        // signal
        emit requestComplete(request.id, NULL);
      }
      else if (REQUEST_DESTROY_INDEX_BUFFER == request.type)
      {
        PIndexBuffer object = request.objects.front();
        destroyIndexBuffer(object);

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
void RenderSystem::flush()
{
  EGE_ASSERT(NULL != m_p);
  p_func()->flush();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::clearViewport(const PViewport& viewport)
{
  EGE_ASSERT(NULL != m_p);
  p_func()->clearViewport(viewport);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setViewport(const PViewport& viewport)
{
  EGE_ASSERT(NULL != m_p);

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

  p_func()->setViewport(viewport);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderSystem::addForRendering(const PRenderComponent& component, const Matrix4f& worldMatrix)
{
  bool result = true;

  // check if component is meaningful
  if (0 < component->vertexBuffer()->vertexCount())
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
      if ((10 > component->vertexBuffer()->vertexCount()) && RenderQueue::IsSuitable(EGE_OBJECT_UID_BACTHED_RENDER_QUEUE, component))
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
  m_projectionMatrix = matrix;

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
  m_viewMatrix = matrix;
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
PVertexBuffer RenderSystem::createVertexBuffer(NVertexBuffer::UsageType usage) const
{
  return p_func()->createVertexBuffer(usage);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::destroyVertexBuffer(PVertexBuffer object) const
{
  p_func()->destroyVertexBuffer(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderSystem::requestDestroyVertexBuffer(PVertexBuffer object)
{
  // create request
  RequestData request;
  request.type  = REQUEST_DESTROY_VERTEX_BUFFER;
  request.id    = m_nextRequestID++;
  request.objects << object;

  // queue it
  MutexLocker locker(m_requestsMutex);
  m_requests.push_back(request);

  return request.id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PIndexBuffer RenderSystem::createIndexBuffer(EGEIndexBuffer::UsageType usage) const
{
  return p_func()->createIndexBuffer(usage);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::destroyIndexBuffer(PIndexBuffer object) const
{
  p_func()->destroyIndexBuffer(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderSystem::requestDestroyIndexBuffer(PIndexBuffer object)
{
  // create request
  RequestData request;
  request.type  = REQUEST_DESTROY_INDEX_BUFFER;
  request.id    = m_nextRequestID++;
  request.objects << object;

  // queue it
  MutexLocker locker(m_requestsMutex);
  m_requests.push_back(request);

  return request.id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D RenderSystem::createTexture2D(const String& name, const PImage& image)
{
  return p_func()->createTexture2D(name, image);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D RenderSystem::createTexture2D(const String& name, const PDataBuffer& data)
{
  return p_func()->createTexture2D(name, data);
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
  request.objects << image;

  // queue it
  MutexLocker locker(m_requestsMutex);
  m_requests.push_back(request);

  return request.id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTexture2D RenderSystem::createRenderTexture(const String& name, s32 width, s32 height, PixelFormat format)
{
  return p_func()->createRenderTexture(name, width, height, format);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::destroyTexture2D(PTexture2D texture)
{
  p_func()->destroyTexture2D(texture);
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
PShader RenderSystem::createShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data)
{
  return p_func()->createShader(type, name, data);
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
void RenderSystem::destroyShader(PShader shader)
{
  p_func()->destroyShader(shader);
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
PProgram RenderSystem::createProgram(const String& name, const List<PShader>& shaders)
{
  return p_func()->createProgram(name, shaders);
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
void RenderSystem::destroyProgram(PProgram program)
{
  return p_func()->destroyProgram(program);
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
void RenderSystem::setTextureMinFilter(EGETexture::Filter filter)
{
  m_textureMinFilter = filter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setTextureMagFilter(EGETexture::Filter filter)
{
  m_textureMagFilter = filter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setTextureAddressingModeS(EGETexture::AddressingMode mode)
{
  m_textureAddressingModeS = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystem::setTextureAddressingModeT(EGETexture::AddressingMode mode)
{
  m_textureAddressingModeT = mode;
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
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Constructors
//
//CRenderSystem::CRenderSystem( void ) : m_pcActiveViewport( NULL ), m_pcActiveRenderTarget( NULL ), 
//                                       m_pcSceneManager( NULL ), m_pcRTTManager( NULL )
//{
//  memset( m_uiBindedTextureIDs, 0, sizeof( m_uiBindedTextureIDs ) );
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Destructors
//
//CRenderSystem::~CRenderSystem( void )
//{
//  DELETE_PTR( m_pcRTTManager );
//
//  CE3D::GetSingletonPtr()->getLog()->logEventN( true, "Destroying CRenderSystem" );
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// PUBLICS
//
//CRenderSystem* CRenderSystem::New( void )
//{
//  CE3D::GetSingletonPtr()->getLog()->logEventN( true, "CRenderSystem::New" );
//
//  CRenderSystem* pcObject;
//
//  // allocate object
//  // TAGE - need to be able to choose RTTManager!!!
//  if ( ( pcObject = new CRenderSystem ) == NULL || ( pcObject->m_pcRTTManager = CRTTManagerCopy::New() ) == NULL )
//  {
//    // error!
//    DELETE_PTR( pcObject );
//    return NULL;
//  }
//
//  return pcObject;
//}
//
//void CRenderSystem::render( const CRenderOperation& cRenderOp )
//{
//  // get material
//  PMaterialResource cMaterial = cRenderOp.getMaterial();
//
//  assert( cMaterial.isNull() == false && "No material assigned!!!" );
//
//  u32 uiValue = ( cRenderOp.isIndexBufferInUse() == true ) ? cRenderOp.getIndexBuffer()->getIndexCount() : 
//                                                             cRenderOp.getVertexBuffer()->getVertexCount();
//
//  // update stats
//  m_uiBatchCount += cMaterial->getPassCount();
//  
//  switch( cRenderOp.getType() )
//  {
//    case CRenderOperation::TRIANGLE_LIST:  m_uiTriangleCount += uiValue/3; break;
//    case CRenderOperation::TRIANGLE_STRIP: m_uiTriangleCount += uiValue-2; break;
//  }
//
//  // go thru all passes
//  for ( u32 uiPassNo = 0; uiPassNo < cMaterial->getPassCount(); ++uiPassNo )
//  {
//    CRenderPass* pcPass = cMaterial->getPass( uiPassNo );
//    CShader* pcShader   = NULL;
//
//    s32 iTexturesEnabled = 0;
//
//    // apply pass parameters
//    applyPassSettings( pcPass );
//
//    // check if shader present
//    if ( ( pcShader = pcPass->getShader() ) != NULL )
//    {
//      // bind shader
//      pcShader->bind();
//
//      // update parameters
//      pcShader->updateParameters( m_pcSceneManager->getAutoUniformsDataSource() );
//    }
//
//    // bind textures
//    for ( u32 i = 0; i < MAX_TEXTURE_UNITS; ++i )
//    {
//      CTexture* pcTexture;
//
//      // check if any texture present
//      if ( ( pcTexture = pcPass->getTexture( i ) ) != NULL )
//      {
//        // bind it
//        bindTexture( pcTexture, i );
//        iTexturesEnabled++;
//      }
//    }
//
//    // get buffers semantics
//    VertexBuffer::BufferSemanticVector vsBuffersSemantics = cRenderOp.getVertexBuffer()->getBufferSemantics();
//
//    // lock vertex data
//    void* pVertexData = cRenderOp.getVertexBuffer()->lock( 0, cRenderOp.getVertexBuffer()->getVertexCountInUse() );
//
//    // reset vertex color
//    glColor3f( 1, 1, 1 );
//
//    // go thru all buffers
//    for ( VertexBuffer::BufferSemanticVector::iterator iter = vsBuffersSemantics.begin(); 
//          iter != vsBuffersSemantics.end(); ++iter )
//    {
//      // set according to buffer type
//      switch( iter->eType )
//      {
//        case VertexBuffer::POSITION:
//
//          glVertexPointer( 3, GL_FLOAT, cRenderOp.getVertexBuffer()->getVertexSize(), 
//                           static_cast<char*>( pVertexData )+iter->uiOffset );
//          glEnableClientState( GL_VERTEX_ARRAY );
//          break;
//
//        case VertexBuffer::NORMAL:
//
//          glNormalPointer( GL_FLOAT, cRenderOp.getVertexBuffer()->getVertexSize(), 
//                           static_cast<char*>( pVertexData )+iter->uiOffset );
//          glEnableClientState( GL_NORMAL_ARRAY );
//          break;
//
//        case VertexBuffer::COLOR:
//
//          glColorPointer( 4, GL_FLOAT, cRenderOp.getVertexBuffer()->getVertexSize(), 
//                          static_cast<char*>( pVertexData )+iter->uiOffset );
//          glEnableClientState( GL_COLOR_ARRAY );
//          break;
//    
//        case VertexBuffer::TEXTURE_COORD:
//
//          // TAGE - we assume that each texture unit uses THE SAME texture coords!
//          for ( s32 i = 0; i < iTexturesEnabled; i++ )
//          {
//            glClientActiveTexture( GL_TEXTURE0+i );
//            glTexCoordPointer( 2, GL_FLOAT, cRenderOp.getVertexBuffer()->getVertexSize(), 
//                               static_cast<char*>( pVertexData )+iter->uiOffset );
//            glEnableClientState( GL_TEXTURE_COORD_ARRAY );
//          }
//          break;
//
//        case VertexBuffer::TANGENT:
//
//          // TANGENT is binded to GL_TEXTURE6
//          glClientActiveTexture( GL_TEXTURE6 );
//          glTexCoordPointer( 3, GL_FLOAT, cRenderOp.getVertexBuffer()->getVertexSize(), 
//                             static_cast<char*>( pVertexData )+iter->uiOffset );
//          glEnableClientState( GL_TEXTURE_COORD_ARRAY );
//          break;
//      }
//    }
//
//    // check if INDICIES are to be used
//    if ( cRenderOp.isIndexBufferInUse() == true )
//    {
//      // lock INDEX buffer
//      void* pIndexData = cRenderOp.getIndexBuffer()->lock( 0, cRenderOp.getIndexBuffer()->getIndexCountInUse() );
//
//      // render only if there is anything to render
//      if ( cRenderOp.getIndexBuffer()->getIndexCountInUse() != 0 )
//      {
//        glDrawElements( cRenderOp.getType(), cRenderOp.getIndexBuffer()->getIndexCountInUse(), 
//                        cRenderOp.getIndexBuffer()->getIndexSizeType(), pIndexData );
//      }
//
//      // unlock INDEX buffer
//      cRenderOp.getIndexBuffer()->unlock();
//    }
//    else
//    {
//      // render only if there is anything to render
//      if ( cRenderOp.getVertexBuffer()->getVertexCountInUse() != 0 )
//      {
//        glDrawArrays( cRenderOp.getType(), 0, cRenderOp.getVertexBuffer()->getVertexCountInUse() );
//      }
//    }
//
//    // unlock vertex buffer
//    cRenderOp.getVertexBuffer()->unlock();
//
//    // disable client states
//    glDisableClientState( GL_VERTEX_ARRAY );
//    glDisableClientState( GL_NORMAL_ARRAY );
//    glDisableClientState( GL_COLOR_ARRAY );
//
//    for ( u32 i = 0; i < MAX_TEXTURE_UNITS+1; i++ )
//    {
//      glClientActiveTexture( GL_TEXTURE0+i );
//      glDisableClientState( GL_TEXTURE_COORD_ARRAY );
//    }
//
//    // disable TANGENT coords
//    glClientActiveTexture( GL_TEXTURE6 );
//    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
//
//    // unbind textures
//    for ( u32 i = 0; i < MAX_TEXTURE_UNITS; ++i )
//    {
//      CTexture* pcTexture;
//
//      // check if any texture present
//      if ( ( pcTexture = pcPass->getTexture( i ) ) != NULL )
//      {
//        // bind it
//        unbindTexture( pcTexture, i );
//      }
//    }
//
//    // unbind shader if any
//    if ( pcShader != NULL )
//    {
//      pcShader->unbind();
//    }
//
//    // active first texture unit only (disable multitexturing)
//    glClientActiveTexture( GL_TEXTURE0 );
//    glActiveTexture( GL_TEXTURE0 );
//  }
//}
//
//void CRenderSystem::setViewport( Viewport* pcViewport )
//{
//  // check if different viewport is to be set
//  if ( m_pcActiveViewport != pcViewport )
//  {
//    // store viewport pointer
//    m_pcActiveViewport = pcViewport;
//
//    // set new render target
//    setRenderTarget( pcViewport->getRenderTarget() );
//
//    // get actual viewport dimensions
//    const CRect& cActualRect = m_pcActiveViewport->getActualRect();
//
//    s32 iX      = static_cast<s32>( cActualRect.x );
//    s32 iY      = static_cast<s32>( cActualRect.y );
//    s32 iWidth  = static_cast<s32>( cActualRect.width );
//    s32 iHeight = static_cast<s32>( cActualRect.height );
//
//    // set viewport dimensions
//    glViewport( iX, iY, iWidth, iHeight );
//
//    // configure the viewport clipping
//    glScissor( iX, iY, iWidth, iHeight );
//  }
//}
//void CRenderSystem::clearViewport( void )
//{
//  assert( m_pcActiveViewport != NULL );
//
//  GLbitfield clearFlag = 0;
//
//  // get viewport background color
//  CColor cBackground = m_pcActiveViewport->getBackgroundColor();
//
//  // set clear color
//  glClearColor( cBackground.m_fRed, cBackground.m_fGreen, cBackground.m_fBlue, cBackground.m_fAlpha );
//
//  if ( m_pcActiveViewport->getClearBuffers() & Viewport::COLOR )
//  {
//    clearFlag |= GL_COLOR_BUFFER_BIT;
//  }
//
//  if ( m_pcActiveViewport->getClearBuffers() & Viewport::DEPTH )
//  {
//    clearFlag |= GL_DEPTH_BUFFER_BIT;
//  }
//
//  // we need to temporarily enable scissor test cause clearing relyies on it
//  bool bScissorTestEnabled = glIsEnabled( GL_SCISSOR_TEST ) == GL_TRUE;
//
//  // check if scissor test disabled
//  if ( bScissorTestEnabled == false )
//  {
//    // enable it
//    glEnable( GL_SCISSOR_TEST );
//  }
//
//  // get viewport and scissor rectagles
//  s32 iViewportBox[ 4 ];
//  s32 iScissorBox[ 4 ];
//
//  glGetIntegerv( GL_VIEWPORT, iViewportBox );
//  glGetIntegerv( GL_SCISSOR_BOX, iScissorBox );
//
//  // determine whether boxes are different
//  bool bAreBoxesDifferent = iViewportBox[ 0 ] != iScissorBox[ 0 ] || iViewportBox[ 1 ] != iScissorBox[ 1 ] ||
//                            iViewportBox[ 2 ] != iScissorBox[ 2 ] || iViewportBox[ 3 ] != iScissorBox[ 3 ];
//
//  // check if boxes are different
//  if ( bAreBoxesDifferent )
//  {
//    // setup scissor box so it matches viewport
//    glScissor( iViewportBox[ 0 ], iViewportBox[ 1 ], iViewportBox[ 2 ], iViewportBox[ 3 ] );
//  }
//
//  // clear buffers
//  glClear( clearFlag );
//
//  // check if boxes are different
//  if ( bAreBoxesDifferent )
//  {
//    // restore old scissor box settings
//    glScissor( iScissorBox[ 0 ], iScissorBox[ 1 ], iScissorBox[ 2 ], iScissorBox[ 3 ] );
//  }
//
//  // restore old scissor test settings
//  if ( bScissorTestEnabled == false )
//  {
//    // disable scissor test
//    glDisable( GL_SCISSOR_TEST );
//  }
//}
//
//void CRenderSystem::applyRenderableSettings( const CRenderable* pcRenderable )
//{
//  // setup blending
//  setBlending( pcRenderable->getSourceBlendMode(), pcRenderable->getDestinationBlendMode() );
//
//  // setup scissor box
//  setScissor( pcRenderable->getScissorBox() );
//}
//
//void CRenderSystem::setProjectionMatrix( const CMatrix4& cMatrix )
//{
//  glMatrixMode( GL_PROJECTION );
//  glLoadMatrixf( cMatrix.m_fData_ );
//  glMatrixMode( GL_MODELVIEW );
//
//  m_projectionMatrix = cMatrix;
//}
//
//void CRenderSystem::setViewMatrix( const CMatrix4& cMatrix )
//{
//  glMatrixMode( GL_MODELVIEW );
//  glLoadMatrixf( cMatrix.m_fData_ );
//
//  m_viewMatrix = cMatrix;
//}
//
//void CRenderSystem::setWorldMatrix( const CMatrix4& cMatrix )
//{
//  // create world-view matrix (modelview)
//  CMatrix4 cCombined( m_viewMatrix.concatenate( cMatrix ) );
//
//  glMatrixMode( GL_MODELVIEW );
//  glLoadMatrixf( cCombined.m_fData_ );
//
//  m_cWorldMatrix = cMatrix;
//}
//
//void CRenderSystem::resetStats( void )
//{
//  m_uiBatchCount    = 0;
//  m_uiTriangleCount = 0;
//}
//
//void CRenderSystem::setSceneManager( const SceneManager* pcSceneManager )
//{
//  m_pcSceneManager = pcSceneManager;
//}
//
//CRenderTexture* CRenderSystem::createRenderTexture( const string& strName, u32 uiWidth, u32 uiHeight, 
//                                                    CResource::EType eType, CTexture::EFormat eFormat )
//{
//  CRenderTexture* pcRenderTexture;
//
//  // create object
//  if ( ( pcRenderTexture = CRenderTexture::New( strName, uiWidth, uiHeight, eType, eFormat ) ) == NULL )
//  {
//    // error!
//    return NULL;
//  }
//
//  // attach to render targets pool
//  CE3D::GetSingletonPtr()->attachRenderTarget( pcRenderTexture );
//
//  return pcRenderTexture;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// PRIVATES
//
//void CRenderSystem::setRenderTarget( RenderTarget* pcRenderTarget )
//{
//  // unbind last render target if any
//  if ( m_pcActiveRenderTarget != NULL )
//  {
//    m_pcRTTManager->unbind( m_pcActiveRenderTarget );
//  }
//
//  // set new render target
//  m_pcActiveRenderTarget = pcRenderTarget;
//
//  // bind new render target
//  m_pcRTTManager->bind( pcRenderTarget );
//}
//
//void CRenderSystem::bindTexture( CTexture* pcTexture, u32 uiTextureUnit )
//{
//  assert( uiTextureUnit < MAX_TEXTURE_UNITS );
//
//  bool bActiveOnly = true;
//
//  // check if NOT binded yet
//  if ( pcTexture->getID() != m_uiBindedTextureIDs[ uiTextureUnit ] )
//  {
//    // we need to bind texture
//    bActiveOnly = false;
//
//    // store ID
//    m_uiBindedTextureIDs[ uiTextureUnit ] = pcTexture->getID();
//  }
//
//  // active texture (and bind if required)
//  pcTexture->bind( uiTextureUnit, bActiveOnly );
//}
//
//void CRenderSystem::unbindTexture( CTexture* pcTexture, u32 uiTextureUnit )
//{
//  assert( uiTextureUnit < MAX_TEXTURE_UNITS );
//
//  pcTexture->unbind( uiTextureUnit );
//}
//
//void CRenderSystem::setPolygonMode( Camera::EPolygonMode eMode )
//{
//  GLenum eOGLMode;
//
//  switch( eMode )
//  {
//    case Camera::POINTS:    eOGLMode = GL_POINT; break;
//    case Camera::WIREFRAME: eOGLMode = GL_LINE; break;
//    case Camera::SOLID:     
//    default:
//      
//      eOGLMode = GL_FILL; 
//      break;
//  }
//
//  glPolygonMode( GL_FRONT_AND_BACK, eOGLMode );
//}
//
//void CRenderSystem::applyPassSettings( CRenderPass* pcPass )
//{
//  assert( pcPass != NULL );
//
//  if ( pcPass->isDepthWriteEnabled() == true ) 
//  {
//    glEnable( GL_DEPTH_TEST );
//  }
//  else
//  {
//    glDisable( GL_DEPTH_TEST );
//  }
//
//  if ( pcPass->isLightingEnabled() == true ) 
//  {
//    glEnable( GL_LIGHTING );
//  }
//  else
//  {
//    glDisable( GL_LIGHTING );
//  }
//}
//
//void CRenderSystem::setScissor( const CRect& cRect ) const
//{
//  // check if scissor rect defined
//  if ( cRect.isEmpty() == false )
//  {
//    glEnable( GL_SCISSOR_TEST );
//
//    // setup scissor box so it matches viewport
//    glScissor( static_cast<s32>( cRect.x ), static_cast<s32>( cRect.y ), 
//               static_cast<s32>( cRect.width ), static_cast<s32>( cRect.height ) );
//  }
//  else
//  {
//    glDisable( GL_SCISSOR_TEST );
//  }
//}
//
//void CRenderSystem::setBlending( CRenderable::BlendFactor eSrc, CRenderable::BlendFactor eDst ) const
//{
//  // check if blending can be disabled
//  if ( eSrc == CRenderable::BLEND_ONE && eDst == CRenderable::BLEND_ZERO )
//  {
//    glDisable( GL_BLEND );
//  }
//  else
//  {
//    // enable blending
//    glEnable( GL_BLEND );
//
//    // setup blend mode
//    glBlendFunc( eSrc, eDst );
//  }
//}

EGE_NAMESPACE_END