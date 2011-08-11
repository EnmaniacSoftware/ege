#include "Core/Graphics/Render/Renderer.h"
#include "Core/Components/Physics/PhysicsComponent.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/Render/RenderQueue.h"

#if EGE_RENDERING_OPENGL_2
#include "Core/Graphics/OpenGL/GL 2.0/RendererOGL2_p.h"
#endif // EGE_RENDERING_OPENGL_2

#if EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/ES 1.0/RendererOGLES1_p.h"
#endif // EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Renderer)
EGE_DEFINE_DELETE_OPERATORS(Renderer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Renderer::Renderer(Application* app) : Object(app)
{
  m_p = ege_new RendererPrivate(this);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Renderer::isValid() const
{
  return NULL != m_p;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sends all geometry through the geometry pipeline to hardware. */
void Renderer::flush()
{
  if (isValid())
  {
    p_func()->flush();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears given viewport. */
void Renderer::clearViewport(const PViewport& viewport)
{
  if (isValid())
  {
    p_func()->clearViewport(viewport);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets given viewport. */
void Renderer::setViewport(const PViewport& viewport)
{
  if (isValid())
  {
    p_func()->setViewport(viewport);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Applies material. */
void Renderer::applyMaterial(const PMaterial& material)
{
  if (isValid())
  {
    p_func()->applyMaterial(material);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets orientation rotation. */
void Renderer::setOrientationRotation(const Angle& angle)
{
  m_orientationRotation = angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds given data for rendering. */
bool Renderer::addForRendering(const Matrix4f& worldMatrix, const PRenderComponent& component)
{
  EGE_ASSERT(component);

  // check if no queue with such priority exists yet
  if (!m_renderQueues.contains(component->priority()))
  {
    // create new queue
    PRenderQueue queue = ege_new RenderQueue(app());
    if (NULL == queue)
    {
      // error!
      return false;
    }

    // add it into queues
    m_renderQueues.insert(component->priority(), queue);
  }

  // add data into queue
  return m_renderQueues[component->priority()]->addForRendering(worldMatrix, component);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets projection matrix. */
void Renderer::setProjectionMatrix(const Matrix4f& matrix)
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
/*! Sets view matrix. */
void Renderer::setViewMatrix(const Matrix4f& matrix)
{
  m_viewMatrix = matrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Resets statistics. */
void Renderer::resetStats()
{
  m_batchCount  = 0;
  m_vertexCount = 0;
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
//void CRenderSystem::setBlending( CRenderable::EBlendFactor eSrc, CRenderable::EBlendFactor eDst ) const
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
