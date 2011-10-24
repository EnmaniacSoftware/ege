#include "Core/Resource/ResourceSound.h"
#include "Core/Resource/ResourceManager.h"
#include <EGEApplication.h>
#include <EGEGraphics.h>
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceSound)
EGE_DEFINE_DELETE_OPERATORS(ResourceSound)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSound::ResourceSound(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_PARTICLE_AFFECTOR)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSound::~ResourceSound()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceSound::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceSound(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceSound::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceSound::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceSound::load()
{
  EGEResult result = EGE_SUCCESS;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceSound::unload() 
{ 
  EGE_PRINT("ResourceSound::unload - %s", name().toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of particle affector object defined by resource. */
//PParticleAffector ResourceSound::createInstance()
//{
//  // create instance of particle emitter of a correct type and with given name
//  PParticleAffector object = app()->graphics()->particleFactory()->createAffector(m_parameters["type"], m_name);
//  if (object)
//  {
//    // initialize with dictionary
//    if (!object->initialize(m_parameters))
//    {
//      // error!
//      EGE_PRINT("ResourceSound::createInstance - Could not initialize!");
//      object = NULL;
//    }
//  }
//
//  return object;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
