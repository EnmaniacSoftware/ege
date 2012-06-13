#include "Core/Resource/ResourceParticleAffector.h"
#include "Core/Resource/ResourceManager.h"
#include <EGEApplication.h>
#include <EGEGraphics.h>
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceParticleAffector)
EGE_DEFINE_DELETE_OPERATORS(ResourceParticleAffector)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceParticleAffector::ResourceParticleAffector(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_PARTICLE_AFFECTOR)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceParticleAffector::~ResourceParticleAffector()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceParticleAffector::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceParticleAffector(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceParticleAffector::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceParticleAffector::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // parse all attributes
  PXmlAttribute attribute = tag->firstAttribute();
  while (attribute)
  {
    // store data
    m_parameters[attribute->name()] = attribute->value();

    // go to next attribute
    attribute = attribute->next();
  }

  // check if obligatory data is wrong
  if (!m_parameters.contains("name") || !m_parameters.contains("type"))
  {
    // error!
    EGE_PRINT("ERROR: Failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // store name separately for further use
  m_name = m_parameters["name"];

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceParticleAffector::load()
{
  EGEResult result = EGE_SUCCESS;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceParticleAffector::unload() 
{ 
  EGE_PRINT("%s", name().toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of particle affector object defined by resource. */
PParticleAffector ResourceParticleAffector::createInstance()
{
  // create instance of particle emitter of a correct type and with given name
  PParticleAffector object = app()->graphics()->particleFactory()->createAffector(m_parameters["type"], m_name);
  if (object)
  {
    // initialize with dictionary
    if (!object->initialize(m_parameters))
    {
      // error!
      EGE_PRINT("ERROR: Could not initialize!");
      object = NULL;
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns TRUE if object is loaded. */
bool ResourceParticleAffector::isLoaded() const 
{ 
  return true; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END