#include "Core/Resource/ResourceSequencer.h"
#include "Core/Resource/ResourceManager.h"
#include "EGESequencer.h"
#include "EGEResources.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceSequencerDebugName = "EGEResourceManager";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceSequencer)
EGE_DEFINE_DELETE_OPERATORS(ResourceSequencer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSequencer::ResourceSequencer(Engine& engine, ResourceGroup* group) : IResource(engine, group, RESOURCE_NAME_SEQUENCE) 
                                                                           , m_repeatable(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSequencer::~ResourceSequencer()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceSequencer::Create(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceSequencer(engine, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceSequencer::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceSequencer::create(const String& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

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
    egeWarning(KResourceSequencerDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // check if success
  if (EGE_SUCCESS == result)
  {
    // set state
    m_state = STATE_UNLOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceSequencer::load()
{
  // set flag
  m_state = STATE_LOADED;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceSequencer::unload()
{
  // reset flag
  m_state = STATE_UNLOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceSequencer::setInstance(PSequencer& instance)
{
  if ((NULL == instance) || (STATE_LOADED != m_state))
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