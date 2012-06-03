#include "Core/Resource/ResourceSequencer.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Sequencer/Sequencer.h"
#include <EGEResources.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceSequencer)
EGE_DEFINE_DELETE_OPERATORS(ResourceSequencer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSequencer::ResourceSequencer(Application* app, ResourceManager* manager) :  IResource(app, manager, RESOURCE_NAME_SEQUENCE), 
                                                                                  m_repeatable(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSequencer::~ResourceSequencer()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceSequencer::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceSequencer(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceSequencer::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceSequencer::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // get data
  bool error = false;
  m_name        = tag->attribute("name");
  m_repeatable  = tag->attribute("repeat", "false").toBool(&error);

  String frameIds = tag->attribute("frames");
  StringArray frames = frameIds.split(" ");

  for (int i = 0; i < static_cast<s32>(frames.size()); ++i)
  {
    m_frameIds.push_back(frames[i].toInt(&error));
  }

  // check if obligatory data is wrong
  if (m_name.empty() || m_frameIds.empty() || error)
  {
    // error!
    EGE_PRINT("ERROR: Failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceSequencer::load()
{
  // nothing to do
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceSequencer::unload()
{
  // nothing to do
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Set given instance of sequencer object to what is defined by resource. */
EGEResult ResourceSequencer::setInstance(PSequencer& instance)
{
  if ((NULL == instance) || !isLoaded())
  {
    // error!
    return EGE_ERROR;
  }

  instance->setName(name());
  instance->setFrameData(m_frameIds);
  instance->setRepeatable(m_repeatable);
  
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of sequencer object defined by resource. */
PSequencer ResourceSequencer::createInstance()
{
  PSequencer sequencer = ege_new Sequencer();
  if (NULL != sequencer)
  {
    // initialize
    if (EGE_SUCCESS != setInstance(sequencer))
    {
      // error!
      sequencer = NULL;
    }
  }

  return sequencer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END