//#include "Core/EGEngine.h"
#include "Core/Scene/SceneNode.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Scene/SceneNodeObject.h"
#include "Core/Components/Physics/PhysicsComponent.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(SceneNode)
EGE_DEFINE_DELETE_OPERATORS(SceneNode)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SceneNode::SceneNode(const EGEString& name, SceneNode* parent, SceneManager* manager) : Object(manager->app()), m_name(name), m_manager(manager), 
                                                                                        m_parent(parent)
{
  m_physics = ege_new PhysicsComponent(app(), "scenenode");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SceneNode::~SceneNode()
{
  removeAllAttachedObjects();
  deleteAllChildNodes();

  m_physics = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool SceneNode::isValid() const
{
  return NULL != m_physics || !m_physics->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates child node with a given name and attaches it. */
SceneNode* SceneNode::createChildNode(const EGEString& name)
{
  SceneNode* node;

  // check if there is already node with the given name
  if (NULL != childNode(name))
  {
    // cannot create
    return NULL;
  }

  // create node
  node = ege_new SceneNode(name, this, m_manager);
  if (NULL != node && node->isValid())
  {
    // add into vector
    m_children.push_back(node);
  }
  else
  {
    // error!
    EGE_DELETE(node);
  }

  return node;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Deletes child node with a given name and detaches it. */
void SceneNode::deleteChildNode(const EGEString& name)
{
  // go thru all child nodes
  for (std::vector<SceneNode*>::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
  {
    SceneNode* node = *iter;

    // check if found
    if (node->name() == name)
    {
      // delete node
      node = NULL;

      // remove from vector
      m_children.erase(iter);

      // we are done
      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of child nodes. */
u32 SceneNode::childNodeCount() const
{
  return static_cast<u32>(m_children.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns child node with a given name. Returns NULL if no such node exists. */
SceneNode* SceneNode::childNode(const EGEString& name) const
{
  // go thru all child nodes
  for (std::vector<SceneNode*>::const_iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
  {
    SceneNode* node = *iter;

    // check if found
    if (node->name() == name)
    {
      // found
      return node;
    }
  }

  // not found
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns child node with a given index. Returns NULL if no such node exists. */
SceneNode* SceneNode::childNode(u32 index) const
{
  // check if OUT of range
  if (index >= childNodeCount())
  {
    // error!
    return NULL;
  }

  return m_children[index];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Deletes and detaches all child nodes. */
void SceneNode::deleteAllChildNodes()
{
  for (std::vector<SceneNode*>::iterator iter = m_children.begin(); iter != m_children.end();)
  {
    SceneNode* node = *iter;

    EGE_DELETE(node);

    iter = m_children.erase(iter);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates and attaches new object to node. */
PSceneNodeObject SceneNode::attachNewObject(const EGEString& name)
{
  PSceneNodeObject pObject;

  // check if object of this name is already present
  if (NULL != attachedObject(name))
  {
    // error!
    return NULL;
  }

  // allocate new object
  pObject = ege_new SceneNodeObject(this, name);
  if (NULL != pObject)
  {
    // add object into pool
    m_objects.push_back(pObject);
  }

  return pObject;  
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns attached object with a given name. Returns NULL if no such object exists. */
PSceneNodeObject SceneNode::attachedObject(const EGEString& name) const
{
  // go thru all objects
  for (std::vector<PSceneNodeObject>::const_iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter)
  {
    PSceneNodeObject pObject = *iter;

    // check if found
    if (pObject->name() == name)
    {
      // found
      return pObject;
    }
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes scene object of a given name. */
void SceneNode::removeObject(const EGEString& name)
{
  // go thru all objects
  for (std::vector<PSceneNodeObject>::iterator iter = m_objects.begin(); iter != m_objects.end();)
  {
    // check if found
    if ((*iter)->name() == name)
    {
      *iter = NULL;

      // remove from pool
      m_objects.erase(iter);
      break;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes all objects. */
void SceneNode::removeAllAttachedObjects()
{
  // go thru all objects
  for (std::vector<PSceneNodeObject>::iterator iter = m_objects.begin(); iter != m_objects.end();)
  {
    *iter = NULL;

    iter = m_objects.erase(iter);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates node. */
void SceneNode::update(const Time& time)
{
  // check if NO update required at all
  //if (!m_bNeedUpdate && m_bChildrenNeedUpdate == false )
  //{
  //  // do nothing
  //  return;
  //}

  // update self
  Math::CreateMatrix(m_worldMatrix, physics()->position(), TVector4f::ONE, physics()->orientation());

  if (NULL != m_parent)
  {
    m_worldMatrix = m_parent->m_worldMatrix.multiply(m_worldMatrix);
  }

  // check if children needs to be updated
//  if ( m_bChildrenNeedUpdate == true )
  {
    // go thru all children
    for (std::vector<SceneNode*>::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
      (*iter)->update(time);
    }

    // reset flag
//    m_bChildrenNeedUpdate = false;
  }

  // update bounds
 // updateBounds();

  // reset flags
  //m_bNeedUpdate = false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Constructors
//
//SceneNode::SceneNode( const string& strName, SceneNode* pcParentNode, SceneManager* pcManager ) 
//: m_bVisible( true ), m_bNeedUpdate( false ), m_pcParentNode( pcParentNode ), m_manager( pcManager ),
//  m_bTransformNeedUpdate( true ), m_cOrientation( CQuaternion::IDENTITY ), 
//  m_cDerivedOrientation( CQuaternion::IDENTITY ), m_cPosition( CVector3::ZERO ), m_cDerivedPosition( CVector3::ZERO ),
//  m_bChildrenNeedUpdate( true ), m_pcWireBoundingBox( NULL ), m_pcWireBoundingSphere( NULL ), 
//  m_cWorldTransform( CMatrix4::IDENTITY )
//{
//  // set data
//  m_strName = strName;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Destructors
//
//SceneNode::~SceneNode( void )
//{
//  DELETE_PTR( m_pcWireBoundingBox );
//  DELETE_PTR( m_pcWireBoundingSphere );
//
//  // notify we are going to die
//  notifyDestroyed();
//
//  // delete all child nodes
//  deleteAllChildNodes();
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// PUBLICS
//
//SceneNode* SceneNode::New( const string& strName, SceneNode* pcParentNode, SceneManager* pcManager )
//{
//  SceneNode* pcObject;
//
//  // allocate object
//  if ( ( pcObject = new SceneNode( strName, pcParentNode, pcManager ) ) == NULL || 
//       ( pcObject->m_pcWireBoundingBox = CWireBoundingBox::New() ) == NULL ||
//       ( pcObject->m_pcWireBoundingSphere = CWireBoundingSphere::New() ) == NULL )
//  {
//    // error!
//    DELETE_PTR( pcObject );
//    return NULL;
//  }
//
//  // make sure manager will listen for our events
//  pcObject->addListener( pcManager );
//
//  return pcObject;
//}
//
//void SceneNode::setVisible( bool bEnable )
//{
//  // set flag
//  m_bVisible = bEnable;
//}
//
//void SceneNode::attachObject( SceneNodeObject* pcObject )
//{
//  // detach old object
//  detachObject( pcObject->getName() );
//
//  // store object
//  m_mpcObjects[ pcObject->getName() ] = pcObject;
//
//  // add object 
//  addListener( pcObject );
//
//  // notify object
//  pcObject->onSceneNodeAttached( this );
//
//  // invalidate
//  invalidate();
//}
//
//void SceneNode::detachAllObjects( void )
//{
//  // go thru all attached object
//  for ( AttachedObjectsMap::iterator iter = m_mpcObjects.begin(); iter != m_mpcObjects.end(); )
//  {
//    // remove from listening pool
//    removeListener( iter->second );
//
//    // notify object
//    iter->second->onSceneNodeAttached( NULL );
//
//    // remove it from map
//    iter = m_mpcObjects.erase( iter );
//
//    // we need update (bounding boxes etc.)
//    invalidate();
//  }
//}
//
//SceneNode::AttachedObjectsMap& SceneNode::getAttachedObjects( void )
//{
//  return m_mpcObjects;
//}
//
//SceneNodeObject* SceneNode::getAttachedObject( const string& strName ) const
//{
//  // try to retirve the object
//  AttachedObjectsMap::const_iterator iter = m_mpcObjects.find( strName );
//
//  // check if not found
//  if ( iter == m_mpcObjects.end() )
//  {
//    // not found
//    return NULL;
//  }
//
//  return iter->second;
//}
//
//SceneNodeObject* SceneNode::detachObject( const string& strName )
//{
//  // try to retirve the object
//  AttachedObjectsMap::iterator iter = m_mpcObjects.find( strName );
//
//  // check if found
//  if ( iter != m_mpcObjects.end() )
//  {
//    SceneNodeObject* pcObject = iter->second;
//
//    // remove it from map
//    m_mpcObjects.erase( iter );
//
//    // remove from listening
//    removeListener( pcObject );
//
//    // notify object
//    pcObject->onSceneNodeAttached( NULL );
//
//    // we need update (bounding boxes etc.)
//    invalidate();
//
//    return pcObject;
//  }
//
//  // no object attcached
//  return NULL;
//}
//
//void SceneNode::setPosition( const CVector3& cPosition )
//{
//  // store position
//  m_cPosition = cPosition;
//
//  // notify child nodes and attached objects
//  notifyTransformChanged();
//
//  invalidate();
//}
//
//const CVector3& SceneNode::getDerivedPosition( void ) 
//{ 
//  // check if transformation is out of date or parent transform changed
//  if ( m_bTransformNeedUpdate == true )
//  {
//    // update transformation
//    updateTransformation();
//  }
//
//  return m_cDerivedPosition; 
//}
//
//void SceneNode::setOrientation( const CQuaternion& cOrientation )
//{
//  // store position
//  m_cOrientation = cOrientation;
//
//  // notify child nodes and attached objects
//  notifyTransformChanged();
//
//  invalidate();
//}
//
//const CQuaternion& SceneNode::getDerivedOrientation( void ) 
//{
//  // check if transformation is out of date
//  if ( m_bTransformNeedUpdate == true )
//  {
//    // update transformation
//    updateTransformation();
//  }
//
//  return m_cDerivedOrientation; 
//}
//
//
bool SceneNode::addForRendering(PCamera& pCamera, Renderer* pcRenderer) const
{  

  //// check if we are NOT visible by camera
  //if ( m_bVisible == false || pcCamera->isVisible( m_cWorldBS ) == false || 
  //     pcCamera->isVisible( m_cWorldAABB ) == false )
  //{
  //  // done
  //  return true;
  //}

  //// ok we are visible, notify about that
  //notifyVisible();

  // check attached objects
  for (std::vector<PSceneNodeObject>::const_iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter)
  {
    const PSceneNodeObject& pObject = *iter;

    // add into renderables (let the node to add itself)
    if (!pObject->addForRendering(pcRenderer))
    {
      // error!
      return false;
    }
  }

  // go thru all the children
  for (std::vector<SceneNode*>::const_iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
  {
    SceneNode* pcObject = *iter;

    // find visible object within current child node
    if (!pcObject->addForRendering(pCamera, pcRenderer))
    {
      // error!
      return false;
    }
  }

  // check if bounding boxes are to be shown
  //if ( m_manager->getShowBoundingBoxes() == true )
  //{
  //  // create frame bounding box
  //  m_pcWireBoundingBox->create( m_cWorldAABB );

  //  // add into render queue
  //  if ( m_manager->getRenderQueue()->addRenderable( m_pcWireBoundingBox ) == false )
  //  {
  //    // error!
  //    return false;
  //  }
  //}

  //// check if bounding spheres are to be shown
  //if ( m_manager->getShowBoundingSpheres() == true )
  //{
  //  // create frame bounding sphere
  //  m_pcWireBoundingSphere->create( m_cWorldBS );

  //  // add into render queue
  //  if ( m_manager->getRenderQueue()->addRenderable( m_pcWireBoundingSphere ) == false )
  //  {
  //    // error!
  //    return false;
  //  }
  //}

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// PRIVATES
//
//void SceneNode::updateBounds( void )
//{
//  // clear
//  m_cWorldAABB.setNull();
//  m_cWorldBS.setNull();
//
//  // go thru all attached object
//  for ( AttachedObjectsMap::iterator iter = m_mpcObjects.begin(); iter != m_mpcObjects.end(); ++iter )
//  {
//    // combine local AABBs
//    m_cWorldAABB.merge( iter->second->getLocalAABB() );
//    
//    // combine local BSs
//    m_cWorldBS.merge( &iter->second->getLocalBS() );
//  }
//
//  // apply current node transformation to obtain world coordinates from local ones
//  m_cWorldAABB.transformAffine( &getWorldTransformMatrix() );
//  m_cWorldBS.transformAffine( &getWorldTransformMatrix() );
//
//  // go thru all children
//  for ( SceneNodesVector::iterator iter = m_vpcChildNodes.begin(); iter != m_vpcChildNodes.end(); ++iter )
//  {
//    // NOTE: child world AABB and BS are up to date cause children are updated first
//
//    // combine world AABBs
//    m_cWorldAABB.merge( ( *iter )->getWorldAABB() );
//
//    // combine world BSs
//    m_cWorldBS.merge( &( *iter )->getWorldBS() );
//  }
//}
//
//void SceneNode::notifyVisible( void )
//{
//  // go thru all listeners
//  for ( u32 i = 0; i < m_vpcListeners.size(); i++ )
//  {
//    // notify
//    m_vpcListeners[ i ]->onSceneNodeVisible( this );
//  }
//}
//
//void SceneNode::updateTransformation( void )
//{
//  // check if parent node is present
//  if ( m_pcParentNode != NULL )
//  {
//    // get parent node orientation
//    const CQuaternion& cParentQuaternion = m_pcParentNode->getDerivedOrientation();
//
//    // update derived orientation
//    m_cDerivedOrientation = cParentQuaternion.concatenate( m_cOrientation );
//
//    // update derived position
//    m_cDerivedPosition = cParentQuaternion.transform( &m_cPosition );
//    m_cDerivedPosition += m_pcParentNode->getDerivedPosition();
//  }
//  else
//  {
//    // we are root node
//    m_cDerivedOrientation = m_cOrientation;
//    m_cDerivedPosition = m_cPosition;
//  }
//
//  // update world transform
//  m_cWorldTransform.makeTransform( &m_cDerivedPosition, &CVector3::ONE, &m_cDerivedOrientation );
//
//  // rest flag
//  m_bTransformNeedUpdate = false;
//}
//
//void SceneNode::notifyTransformChanged( void )
//{
//  // notify listeners (scene objects attached)
//  for ( u32 i = 0; i < m_vpcListeners.size(); i++ )
//  {
//    // notify
//    m_vpcListeners[ i ]->onSceneNodeTransformChanged( this );
//  }
//
//  // go thru all children
//  for ( SceneNodesVector::iterator iter = m_vpcChildNodes.begin(); iter != m_vpcChildNodes.end(); ++iter )
//  {
//    // invalidate child transfrom
//    ( *iter )->m_bTransformNeedUpdate = true;
//
//    // make sure this propagades down
//    ( *iter )->notifyTransformChanged();
//  }
//}
//
//void SceneNode::notifyDestroyed( void )
//{
//  // notify listeners (scene objects attached)
//  for ( u32 i = 0; i < m_vpcListeners.size(); i++ )
//  {
//    // notify
//    m_vpcListeners[ i ]->onSceneNodeDestroyed( this );
//  }
//}