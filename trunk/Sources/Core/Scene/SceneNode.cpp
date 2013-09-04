#include "Core/Scene/SceneNode.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Scene/SceneNodeObject.h"
#include "Core/Component/Physics/PhysicsComponent.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SceneNode)
EGE_DEFINE_DELETE_OPERATORS(SceneNode)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SceneNode::SceneNode(const String& name, SceneNode* parent, SceneManager* manager, EGEPhysics::ComponentType componentType)
: Object(manager->app()), 
  Node(manager->app(), name, parent, componentType), 
  m_manager(manager)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SceneNode::~SceneNode()
{
  removeAllAttachedObjects();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SceneNode::attachObject(PSceneNodeObject object)
{
  // check if object of this name is already present
  if (NULL != attachedObject(object->name()))
  {
    // error!
    return NULL;
  }

  // add object into pool
  m_objects.push_back(object);

  // set new parent
  object->setParentNode(this);

  return true;  
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSceneNodeObject SceneNode::attachedObject(const String& name) const
{
  // go thru all objects
  for (List<PSceneNodeObject>::const_iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter)
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
void SceneNode::removeObject(const String& name)
{
  // go thru all objects
  for (List<PSceneNodeObject>::iterator iter = m_objects.begin(); iter != m_objects.end();)
  {
    SceneNodeObject* object = *iter;

    // check if found
    if (object->name() == name)
    {
      // remove parent
      object->setParentNode(NULL);

      *iter = NULL;

      // remove from pool
      m_objects.erase(iter);
      break;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SceneNode::removeAllAttachedObjects()
{
  // go thru all objects
  for (List<PSceneNodeObject>::iterator iter = m_objects.begin(); iter != m_objects.end();)
  {
    SceneNodeObject* object = *iter;

    // remove parent
    object->setParentNode(NULL);

    *iter = NULL;

    iter = m_objects.erase(iter);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SceneNode::update(const Time& time)
{
  // check if NO update required at all
  //if (!m_bNeedUpdate && m_bChildrenNeedUpdate == false )
  //{
  //  // do nothing
  //  return;
  //}

  //// update self
  //Quaternionf orientation = physics()->orientation();
  //Vector4f position = physics()->position();
  //Vector4f scale = physics()->scale();
  //
  //Math::CreateMatrix(&m_worldMatrix, &position, &scale, &orientation);

  //if (NULL != parent())
  //{
  //  m_worldMatrix = parent()->worldMatrix().multiply(m_worldMatrix);
  //}

  // check if children needs to be updated
//  if ( m_bChildrenNeedUpdate == true )
  {
    // go thru all children
    for (List<Node*>::const_iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
      SceneNode* node = (SceneNode*) *iter;
      node->update(time);
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
Node* SceneNode::createChildNodeImpl(const String& name, EGEPhysics::ComponentType componentType)
{
  return ege_new SceneNode(name, this, sceneManager(), componentType);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SceneNode* SceneNode::createChildSceneNode(const String& name, EGEPhysics::ComponentType componentType)
{
  return static_cast<SceneNode*>(createChildNode(name, componentType));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
bool SceneNode::addForRendering(PCamera& camera, IRenderer* renderer) const
{  
  // check if we are NOT visible by camera
  if (!isVisible() /*|| pcCamera->isVisible( m_cWorldBS ) == false || pcCamera->isVisible( m_cWorldAABB ) == false */)
  {
    // done
    return true;
  }

  //// ok we are visible, notify about that
  //notifyVisible();

  // check attached objects
  for (List<PSceneNodeObject>::const_iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter)
  {
    const PSceneNodeObject& object = *iter;

    // add into renderables
    if (object->isVisible())
    {
      if (!object->addForRendering(renderer))
      {
        // error!
        return false;
      }
    }
  }

  // go thru all the children
  for (List<Node*>::const_iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
  {
    const SceneNode* node = (SceneNode*) *iter;

    // find visible object within current child node
    if (!node->addForRendering(camera, renderer))
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
//    // invalidate child transform
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

EGE_NAMESPACE_END