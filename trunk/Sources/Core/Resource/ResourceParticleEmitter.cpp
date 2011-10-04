#include "Core/Resource/ResourceParticleEmitter.h"
#include "Core/Resource/ResourceManager.h"
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceParticleEmitter)
EGE_DEFINE_DELETE_OPERATORS(ResourceParticleEmitter)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceParticleEmitter::ResourceParticleEmitter(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_SPRITE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceParticleEmitter::~ResourceParticleEmitter()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceParticleEmitter::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceParticleEmitter(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceParticleEmitter::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceParticleEmitter::create(const String& path, const PXmlElement& tag)
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
  if (!m_parameters.contains("name") || !m_parameters.contains("type") || !m_parameters.contains("material"))
  {
    // error!
    EGE_PRINT("ResourceParticleEmitter::create - failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // compose absolute path
 // m_path = path + "/" + m_path;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceParticleEmitter::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    // get material resource
    m_materialResource = manager()->resource(RESOURCE_NAME_MATERIAL, m_parameters["material"]);
    if (m_materialResource)
    {
      // load material
      if (EGE_SUCCESS != (result = m_materialResource->load()))
      {
        // error!
        m_materialResource = NULL;
        return result;
      }
    }
    else
    {
      // material not found
      result = EGE_ERROR_NOT_FOUND;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceParticleEmitter::unload() 
{ 
  EGE_PRINT("ResourceParticleEmitter::unload - %s", name().toAscii());

  m_materialResource = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of sprite object defined by resource. */
//PSprite ResourceParticleEmitter::createInstance()
//{
//	PSprite object = ege_new Sprite(app());
//  if (object)
//  {
//    // set new data
//    if (EGE_SUCCESS != setInstance(object))
//    {
//      object = NULL;
//    }
//  }
//
//  return object;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///* Set given instance of sprite object to what is defined by resource. */
//EGEResult ResourceParticleEmitter::setInstance(const PSprite& instance)
//{
//  // sanity check
//  if (NULL == instance || !isLoaded())
//  {
//    return EGE_ERROR;
//  }
//
//  // generate frame data
//  calculateFrameData();
//  
//  // setup data
//  instance->setDuration(m_duration);
//  instance->setFrameData(m_frameData);
//  instance->setTexture(m_sheet->textureImage()->createInstance());
//
//  return EGE_SUCCESS;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
