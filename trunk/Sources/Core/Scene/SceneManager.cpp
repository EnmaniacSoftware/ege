#include "Core/Application/Application.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Graphics/Render/Renderer.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Scene/SceneNode.h"
#include "Core/Components/Physics/PhysicsComponent.h"
#include "EGEGraphics.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(SceneManager)
EGE_DEFINE_DELETE_OPERATORS(SceneManager)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SceneManager::SceneManager(Application* app) : Object(app), m_rootNode(NULL)
{
  // create root node
  m_rootNode = ege_new SceneNode("root", NULL, this);
  if (!m_rootNode->isValid())
  {
    EGE_DELETE(m_rootNode);
  }

  // initialize
  //if (EGE_SUCCESS != (eResult = m_rootNode->initialize()))
  //{
  //  // error!
  //  return eResult;
  //}
}

//SceneManager::SceneManager( const string& strName ) : m_strTypeName( "generic" ), m_rootNode( NULL ), 
//                                                        m_pcRenderSystem( NULL ), m_bResetModelViewMatrix( false ), 
//                                                        m_bResetProjectionMatrix( false ), m_pcSkyBoxNode( NULL ),
//                                                        m_bShowBoundingBoxes( false ), m_pcCursor( NULL ), 
//                                                        m_bShowBoundingSpheres( false ), 
//                                                        m_pcAutoUniformDataSource( NULL ), m_strName( strName )
//{
//}
//
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SceneManager::~SceneManager()
{
  destroy();
  EGE_DELETE(m_rootNode);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Returns TRUE if object is valid. */
bool SceneManager::isValid() const
{
  return NULL != m_rootNode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders scene. */
void SceneManager::render(PCamera camera, PViewport viewport)
{
  Renderer* renderer = app()->graphics()->renderer();

  // setup render system
  renderer->setProjectionMatrix(camera->projectionMatrix());
  renderer->setViewMatrix(camera->viewMatrix());

  //m_pcRenderSystem->setPolygonMode( pcCamera->getPolygonMode() );
  //m_pcRenderSystem->setSceneManager( this );

  //// store camera
  //m_pcCamera = pcCamera;

  // update
  //update();

  //// find lights affecting frustum
  //findLightsAffectingFrustum();

  // traverse the graph and add all visible object for rendering
  addForRendering(camera, renderer);

  // check if overlays are to be rendered
  if (viewport->overlaysEnabled())
  {
    app()->overlayManager()->render(viewport, renderer);

    //// find visible GUI elements
    //CGUIManager::GetSingletonPtr()->findVisibleElements( pcViewport, m_pcRenderQueue );

    //// queue mouse cursor for rendering
    //queueMouseCursorForRendering();
  }

  //// queue skybox for rendering
  //queueSkyForRendering();

  //// queue camera lens flares for rendering
  //queueLensFlaresForRendering();

  //// clear viewport
  //if ( pcViewport->getClearEveryFrame() == true )
  //{
  renderer->clearViewport(viewport);
  //}

  //// update auto uniform data source
  //m_pcAutoUniformDataSource->setCamera( pcCamera );

  renderer->resetStats();

  renderer->flush();

  viewport->setVertexCount(renderer->vertexCount());
  viewport->setBatchCount(renderer->batchCount());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void SceneManager::update(const Time& time)
{
  // update tree nodes
  m_rootNode->update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Destroys manager data effectively resetting it. */
void SceneManager::destroy()
{
  m_rootNode->removeAllAttachedObjects();
  m_rootNode->deleteAllChildNodes();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

//SceneManager::~SceneManager( void )
//{
//  // remove cameras
//  destroyAllCameras();
//
//  // delete skybox node
//  if ( m_pcSkyBoxNode != NULL )
//  {
//    // detach sky box...
//    SceneNodeObject* pcObject = m_pcSkyBoxNode->detachObject( "SkyBox" );
//
//    // ...and delete it
//    DELETE_PTR( pcObject );
//
//    // delete node
//    DELETE_PTR( m_pcSkyBoxNode );
//  }
//
//  // destroy lights
//  destroyAllLights();
//
//  // destroy mouse cursor
//  DELETE_PTR( m_pcCursor );
//
//  // destroy root node
//  DELETE_PTR( m_rootNode );
//
//  // destroy render queue
//  DELETE_PTR( m_pcRenderQueue );
//
//  // destroy auto uniforms data source for GPU programs
//  DELETE_PTR( m_pcAutoUniformDataSource );
//
//  CE3D::GetSingletonPtr()->getLog()->logEventN( true, "Destroying SceneManager" );
//}
//
//
//
//void SceneManager::setRenderSystem( CRenderSystem* pcRenderSystem )
//{
//  m_pcRenderSystem = pcRenderSystem;
//}
//
//bool SceneManager::setSkyBox( PMaterialResource& pcSkyBoxMaterial )
//{
//  SceneNodeObject* pcObject;
//
//  // check if NO node present
//  if ( m_pcSkyBoxNode == NULL )
//  {
//    // allocate node
//    if ( ( m_pcSkyBoxNode = SceneNode::New( "SkyBoxNode", NULL, this ) ) == NULL )
//    {
//      // error!
//      return false;
//    }
//  }
//  else
//  {
//    // detach sky box...
//    SceneNodeObject* pcObject = m_pcSkyBoxNode->detachObject( "SkyBox" );
//
//    // ...and delete it
//    DELETE_PTR( pcObject );
//  }
//
//  // create new skybox
//  if ( ( pcObject = CSkyBox::New( "SkyBox", pcSkyBoxMaterial ) ) == NULL )
//  {
//    // error!
//    return false;
//  }
//
//  // attach it to node
//  m_pcSkyBoxNode->attachObject( pcObject );
//
//  return true;
//}
//
//bool SceneManager::isSkyBoxEnabled( void ) const
//{
//  return m_pcSkyBoxNode != NULL && m_pcSkyBoxNode->isVisible() == true;
//}
//
//void SceneManager::enableSkyBox( bool bEnable )
//{
//  // check if sky box node is present
//  if ( m_pcSkyBoxNode != NULL )
//  {
//    m_pcSkyBoxNode->setVisible( bEnable );
//  }
//}
//
//void SceneManager::setShowBoundingBoxes( bool bEnable )
//{
//  m_bShowBoundingBoxes = bEnable;
//}
//
//void SceneManager::setShowBoundingSpheres( bool bEnable )
//{
//  m_bShowBoundingSpheres = bEnable;
//}
//
//u32 SceneManager::getVisibleNodeCount( void ) const 
//{ 
//  return static_cast<u32>( m_vpcVisibleNodes.size() ); 
//}
//
//SceneManager::SceneNodesVector& SceneManager::getVisibleNodesVector( void )
//{
//  return m_vpcVisibleNodes; 
//}
//
//bool SceneManager::setCursor( PCursorResource& cCursorResource )
//{
//  assert( m_pcCursor != NULL );
//
//  // check if NO cursor supplied
//  if ( cCursorResource.isNull() == true )
//  {
//    // destroy cursor
//    m_pcCursor->destroy();
//
//    // disable custom cursor
//    enableCursor( false );
//  }
//  else
//  {
//    // create cursor
//    if ( m_pcCursor->create( cCursorResource ) == false )
//    {
//      // error!
//      return false;
//    }
//
//    // enable custom cursor
//    enableCursor( true );
//  }
//
//  return true;
//}
//
//bool SceneManager::isCursorEnabled( void )
//{
//  assert( m_pcCursor != NULL );
//
//  return m_pcCursor->isEnabled();
//}
//
//void SceneManager::enableCursor( bool bEnable )
//{
//  assert( m_pcCursor != NULL );
//
//  // check if want to enable custom cursor
//  if ( bEnable == true )
//  {
//    // hide default cursor
//    ShowCursor( FALSE );
//  }
//  else
//  {
//    // show default cursor
//    ShowCursor( TRUE );
//  }
//
//  m_pcCursor->enable( bEnable );
//}
//
//CLight* SceneManager::createLight( const string& strName )
//{
//  CLight* pcObject;
//
//  // allocate object
//  if ( ( pcObject = CLight::New( strName ) ) == NULL )
//  {
//    // error!
//    return NULL;
//  }
//
//  // add into pool
//  m_vpcLights.push_back( pcObject );
//
//  return pcObject;
//}
//
//void SceneManager::destroyLight( CLight* pcLight )
//{
//  // TAGE - should we remove pointer from m_vpcLightsAffectingFrustum ?
//
//  // try to find given light
//  LightsVector::iterator iter = find( m_vpcLights.begin(), m_vpcLights.end(), pcLight );
//
//  // check if found
//  if ( iter != m_vpcLights.end() )
//  {
//    // delete light
//    DELETE_PTR( pcLight );
//
//    // remove from pool
//    m_vpcLights.erase( iter );
//  }
//}
//
//void SceneManager::destroyAllLights( void )
//{
//  // TAGE - should we remove pointer from m_vpcLightsAffectingFrustum ?
//
//  // go thru all lights
//  for ( LightsVector::iterator iter = m_vpcLights.begin(); iter != m_vpcLights.end(); )
//  {
//    CLight* pcLight = *iter;
//
//    // delete light
//    DELETE_PTR( pcLight );
//
//    // remove from vector
//    iter = m_vpcLights.erase( iter );
//  }
//}
//
//CAutoUniformDataSource* SceneManager::getAutoUniformsDataSource( void ) const
//{
//  return m_pcAutoUniformDataSource;
//}
//
//SceneManager::LightsVector& SceneManager::getLightsAffectingFrustum( void )
//{
//  return m_vpcLightsAffectingFrustum;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// PRIVATES
//
//void SceneManager::update( void )
//{
//  // update tree nodes
//  m_rootNode->update();
//}
//
//void SceneManager::renderVisibleObjects( void )
//{
//  // go thru all queue groups
//  for ( CRenderQueue::RenderQueueGroupMap::const_iterator iter = m_pcRenderQueue->getQueueGroups().begin(); 
//        iter != m_pcRenderQueue->getQueueGroups().end(); ++iter )
//  {
//    CRenderQueueGroup* pcGroup = iter->second;
//
//    // go thru all renderables in current group
//    for ( CRenderQueueGroup::RenderablesVector::const_iterator groupIter = pcGroup->getSolidRenderables().begin(); 
//          groupIter != pcGroup->getSolidRenderables().end(); ++groupIter )
//    {
//      CRenderable* pcRenderable = *groupIter;
//
//      // apply world matrix first
//      m_pcRenderSystem->setWorldMatrix( pcRenderable->getWorldTransformMatrix() );
//
//      // setup matrices for renderable (which may override current modelview/projection matrices)
//      // NOTE: VIEW and PROJECTION matrices has been already setup according to camera in use
//      useRenderableMatrices( pcRenderable );
//
//      // update auto uniform data source
//      m_pcAutoUniformDataSource->setRenderable( pcRenderable );
//      
//      // call pre-render and check if we still want to render it (it is last chance to skip rendering)
//      if ( pcRenderable->preRender( m_pcCamera, m_pcRenderSystem->getViewport() ) == true )
//      {
//        // apply renderable settings
//        m_pcRenderSystem->applyRenderableSettings( pcRenderable );
//
//        // render object
//        m_pcRenderSystem->render( pcRenderable->getRenderOperation() );
//      }
//
//      // reset matrics
//      resetMatrices();
//    }
//  }
//}
//
//void SceneManager::useRenderableMatrices( const CRenderable* pcRenderable )
//{
//  // check if we need to enable identity model view matrix
//  if ( pcRenderable->getIdentityModelViewMatrixUse() == true )
//  {
//    m_pcRenderSystem->setViewMatrix( CMatrix4::IDENTITY );
//    m_bResetModelViewMatrix = true;
//  }
//
//  // check if we need to enable identity projection matrix
//  if ( pcRenderable->getIdentityProjectionMatrixUse() == true )
//  {
//    m_pcRenderSystem->setProjectionMatrix( CMatrix4::IDENTITY );
//    m_bResetProjectionMatrix = true;
//  }
//}
//
//void SceneManager::resetMatrices( void )
//{
//  // check if model view matrix is to be reset
//  if ( m_bResetModelViewMatrix == true )
//  {
//    // set camera's view matrix
//    m_pcRenderSystem->setViewMatrix( m_pcCamera->getViewMatrix() );
//
//    // reset flag
//    m_bResetModelViewMatrix = false;
//  }
//
//  // check if projection matrix is to be reset
//  if ( m_bResetProjectionMatrix == true )
//  {
//    // set camera's projection matrix
//    m_pcRenderSystem->setProjectionMatrix( m_pcCamera->getProjectionMatrix() );
//
//    // reset flag
//    m_bResetProjectionMatrix = false;
//  }
//}
//
//void SceneManager::queueSkyForRendering( void )
//{
//  // check if skybox assigned
//  if ( isSkyBoxEnabled() == true )
//  {
//    // get sky box
//    CSkyBox* pcSkyBox = reinterpret_cast<CSkyBox*>( m_pcSkyBoxNode->getAttachedObject( "SkyBox" ) );
//
//    // move node so camera is in center of the box
//    m_pcSkyBoxNode->setPosition( m_pcCamera->getPosition() );
//
//    // add skybox for rendering
//    if ( pcSkyBox->addToRenderQueue( m_pcRenderQueue ) == false )
//    {
//      // error!
//      CE3D::GetSingletonPtr()->getLog()->logEventN( true, "ERROR: SceneManager::queueSkyForRendering" );
//    }
//  }
//}
//
//void SceneManager::queueLensFlaresForRendering( void )
//{
//  CLensFlaresPtr& cLensFlares = m_pcCamera->getLensFlares();
//
//  // check if lens flares are set
//  if ( cLensFlares.isNull() == false && cLensFlares->isEnabled() == true )
//  {
//    // go thru all flares
//    for ( u32 i = 0; i < cLensFlares->getFlareCount(); ++i )
//    {
//      // add flare to rendering queue
//      if ( m_pcRenderQueue->addRenderable( cLensFlares->getFlare( i ), CRenderQueueGroup::PRE_OVERLAY ) == NULL )
//      {
//        // error!
//        CE3D::GetSingletonPtr()->getLog()->logEventN( true, "ERROR: SceneManager::queueLensFlaresForRendering" );
//      }
//    }
//  }
//}
//
//void SceneManager::queueMouseCursorForRendering( void )
//{
//  // check if cursor assigned
//  if ( isCursorEnabled() == true )
//  {
//    // add cursor for rendering
//    if ( m_pcRenderQueue->addRenderable( m_pcCursor, CRenderQueueGroup::CURSOR ) == false )
//    {
//      // error!
//      CE3D::GetSingletonPtr()->getLog()->logEventN( true, "ERROR: SceneManager::queueMouseCursorForRendering" );
//    }
//  }
//}
//
//void SceneManager::onSceneNodeVisible( SceneNode* pcNode )
//{
//  // add into pool
//  m_vpcVisibleNodes.push_back( pcNode );
//}
//
//void SceneManager::findLightsAffectingFrustum( void )
//{
//  // clear list
//  m_vpcLightsAffectingFrustum.clear();
//
//  // go thru all lights
//  for ( LightsVector::iterator iter = m_vpcLights.begin(); iter != m_vpcLights.end(); ++iter )
//  {
//    CLight* pcLight = *iter;
//
//    // check if DIECTIONAL light
//    if ( pcLight->getType() == CLight::DIRECTIONAL )
//    {
//      // always visible
//      m_vpcLightsAffectingFrustum.push_back( pcLight );
//    }
//    else
//    {
//      CSphere cBS( pcLight->getDerivedPosition(), pcLight->getAttenuationRange() );
//
//      // check if visible
//      if ( m_pcCamera->isVisible( cBS ) == true )
//      {
//        // add to pool
//        m_vpcLightsAffectingFrustum.push_back( pcLight );
//      }
//    }
//  }
//}
//
void SceneManager::addForRendering(PCamera& camera, Renderer* renderer)
{
  // clear render queue
//  m_pcRenderQueue->clear();

  // clear visible nodes vector
 // m_vpcVisibleNodes.clear();

  // traverse tree from ROOT and store visible objects
  m_rootNode->addForRendering(camera, renderer);
}
//
//bool SceneManager::initialize( void )
//{
//  // allocate objects
//  if ( ( m_pcRenderQueue = CRenderQueue::New() ) == NULL || ( m_pcCursor = CCursor::New() ) == NULL ||
//       ( m_pcAutoUniformDataSource = CAutoUniformDataSource::New( this ) ) == NULL )
//  {
//    // error!
//    return false;
//  }
//
//  return true;
//}
