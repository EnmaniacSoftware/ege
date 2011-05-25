//#include "Core/EGEngine.h"
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
SceneNodeObject::SceneNodeObject(SceneNode* parentNode, const EGEString& name) : Object(parentNode->app()), m_name(name), m_parentNode(parentNode)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SceneNodeObject::~SceneNodeObject()
{
  for (std::vector<PComponent>::iterator iter = m_components.begin(); iter != m_components.end();)
  {
    *iter = NULL;

    iter = m_components.erase(iter);
  }

  // check if we are attached to some node
  //if ( m_pcNode != NULL )
  //{
  //  // detach us first
  //  m_pcNode->detachObject( getName() );
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderComponent SceneNodeObject::renderComponent(const EGEString& name) const
{
  PRenderComponent requestedComponent;

  // go thru all components
  for (std::vector<PComponent>::const_iterator iter = m_components.begin(); iter != m_components.end(); ++iter)
  {
    PComponent component = *iter;
    if (EGE_OBJECT_UID_RENDER_COMPONENT == component->uid() && name == component->name())
    { 
      // found
      requestedComponent = component;
      break;
    }
  }

  return requestedComponent;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PPhysicsComponent SceneNodeObject::physicsComponent(const EGEString& name) const
{
  PPhysicsComponent requestedComponent;

  // go thru all components
  for (std::vector<PComponent>::const_iterator iter = m_components.begin(); iter != m_components.end(); ++iter)
  {
    PComponent component = *iter;
    if (EGE_OBJECT_UID_PHYSICS_COMPONENT == component->uid() && name == component->name())
    { 
      // found
      requestedComponent = component;
      break;
    }
  }

  return requestedComponent;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SceneNodeObject::addComponent(PComponent component)
{
  m_components.push_back(component);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds object render data for rendering with given renderer. */
bool SceneNodeObject::addForRendering(Renderer* renderer) const
{
  bool bResult = true;

  Matrix4f worldMatrix;

  // add all render components for rendering
  for (std::vector<PComponent>::const_iterator iter = m_components.begin(); iter != m_components.end(); ++iter)
  {
    PComponent component = *iter;
    if (EGE_OBJECT_UID_RENDER_COMPONENT == component->uid())
    {
      PRenderComponent renderComponent = component;

      // get corresponding physics component by name
      PPhysicsComponent physicsComponent = this->physicsComponent(component->name());
      if (NULL != physicsComponent)
      {
        Math::CreateMatrix(worldMatrix, physicsComponent->position(), Vector4f::ONE, physicsComponent->orientation());
      }
      else
      {
        worldMatrix = Matrix4f::IDENTITY;
      }

      // combine with parent node world matrix
      worldMatrix = parentNode()->worldMatrix().multiply(worldMatrix);

      // add render component for rendering
      if (!renderer->addForRendering(worldMatrix, renderComponent))
      {
        bResult = false;
      }
    }
  }

  return bResult;
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