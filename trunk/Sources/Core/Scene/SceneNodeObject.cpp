#include "Core/Scene/SceneNodeObject.h"
#include "Core/Graphics/Render/Renderer.h"
#include "Core/Components/Physics/PhysicsComponent.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(SceneNodeObject)
EGE_DEFINE_DELETE_OPERATORS(SceneNodeObject)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SceneNodeObject::SceneNodeObject(const String& name, u32 uid) : Object(NULL, uid), 
                                                                m_name(name), 
                                                                m_parentNode(NULL), 
                                                                m_visible(true)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SceneNodeObject::~SceneNodeObject()
{
  // check if we are attached to some node
  //if ( m_pcNode != NULL )
  //{
  //  // detach us first
  //  m_pcNode->detachObject( getName() );
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds object render data for rendering with given renderer. */
bool SceneNodeObject::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  EGE_UNUSED(renderer);
  EGE_UNUSED(transform);

  // do nothing
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets parent node. */
void SceneNodeObject::setParentNode(SceneNode* parent)
{
  if (parent != m_parentNode)
  {
    // notify first
    parentChanged(m_parentNode, parent);

    // store new node
    m_parentNode = parent;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets visibility flag. */
void SceneNodeObject::setVisible(bool set)
{
  m_visible = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Called when parent node changes. */
void SceneNodeObject::parentChanged(SceneNode* oldNode, SceneNode* newNode)
{
  EGE_UNUSED(oldNode);
  EGE_UNUSED(newNode);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void SceneNodeObject::queryLights( void )
{
  assert( m_pcNode != NULL && "Object not attached!" );

  SceneManager::LightsVector& cLightsVector = m_pcNode->getSceneManager()->getLightsAffectingFrustum();

  // clear lights pool
  m_vpcLights.clear();

  // go thru all lights affecting frustum
  for ( SceneManager::LightsVector::iterator iter = cLightsVector.begin(); iter != cLightsVector.end(); ++iter )
  {
    CLight* pcLight = *iter;

    // process according to light type
    switch( pcLight->getType() )
    {
      case CLight::DIRECTIONAL:

        // no distance, always affecting
        pcLight->setDistanceSquared( 0 );
        break;

      default:

        // calculate light source squared distance from node object
        pcLight->setDistanceSquared( ( pcLight->getDerivedPosition()-m_pcNode->getDerivedPosition() ).getSquaredMagnitude() );

        // check if light is NOT affecting object
        if ( Math::Pow2( pcLight->getAttenuationRange() ) < pcLight->getDistanceSquared() )
        {
          // go on
          continue;
        }
        break;
    }

    // add into renderable pool
    m_vpcLights.push_back( *iter );
  }

  // sort lights from closest to farthest
  std::stable_sort( m_vpcLights.begin(), m_vpcLights.end(), SLIGHTLESS() );
}

void SceneNodeObject::onSceneNodeAttached( SceneNode* pcNode )
{
  // store node we are attached to
  m_pcNode = pcNode;
}

void SceneNodeObject::onSceneNodeDestroyed( SceneNode* pcNode )
{
  // detach ourself from node
  m_pcNode = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATES

bool SceneNodeObject::SLIGHTLESS::operator()( const CLight* pcLight1, const CLight* pcLight2 ) const
{
  return pcLight1->getDistanceSquared() < pcLight2->getDistanceSquared();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROTECTED

*/