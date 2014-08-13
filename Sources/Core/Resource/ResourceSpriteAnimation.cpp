#include "Core/Resource/ResourceSpriteAnimation.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceTextureImage.h"
#include "Core/Resource/ResourceSequencer.h"
#include "EGESpriteAnimation.h"
#include "EGEXml.h"
#include "EGEDebug.h"
#include "EGEResources.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceSpriteAnimationDebugName = "EGEResourceSpriteAnimation";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_SEQUENCE "sequence"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceSpriteAnimation)
EGE_DEFINE_DELETE_OPERATORS(ResourceSpriteAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSpriteAnimation::ResourceSpriteAnimation(Engine& engine, ResourceGroup* group) : IResource(engine, group, RESOURCE_NAME_SPRITE_ANIMATION)
                                                                                       , m_frameDataInvalid(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSpriteAnimation::~ResourceSpriteAnimation()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceSpriteAnimation::Create(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceSpriteAnimation(engine, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceSpriteAnimation::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceSpriteAnimation::create(const String& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name      = tag->attribute("name");
  m_sheetName = tag->attribute("sheet");
  m_fps       = tag->attribute("fps").toFloat(&error);

  // check if obligatory data is wrong
  if (error || m_name.empty() || m_sheetName.empty())
  {
    // error!
    egeWarning(KResourceSpriteAnimationDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all sub nodes
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    if (NODE_SEQUENCE == child->name())
    {
      // add sequence
      result = addSequence(child);
    }

    // check if failed
    if (EGE_SUCCESS != result)
    {
      // error, done!
      break;
    }

    // go to next child
    child = child->nextChild();
  }

  // invalidate frame data
  invalidateFrameData();

  // check if success
  if (EGE_SUCCESS == result)
  {
    // set state
    m_state = STATE_UNLOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceSpriteAnimation::load()
{
  EGEResult result = EGE_SUCCESS;

  if (STATE_LOADED != m_state)
  {
    // get sheet resource
    m_sheet = group()->manager()->resource(RESOURCE_NAME_SPRITE_SHEET, sheetName());
    if (NULL != m_sheet)
    {
      // load sheet
      if (EGE_SUCCESS != (result = m_sheet->load()))
      {
        // error!
        m_sheet = NULL;
        return result;
      }
    }
    else
    {
      // material not found
      return EGE_ERROR_NOT_FOUND;
    }

    // load all sequencers
    for (SequenceResourceList::iterator it = m_sequenceResources.begin(); it != m_sequenceResources.end(); ++it)
    {
      PResourceSequencer seqResource = *it;
      if (EGE_SUCCESS != (result = seqResource->load()))
      {
        // error!
        return result;
      }
    }

    // set flag
    m_state = STATE_LOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceSpriteAnimation::unload() 
{ 
  // unload texture
  m_sheet = NULL;

  // reset flag
  m_state = STATE_UNLOADED;

 // m_sequenceResources.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSpriteAnimation ResourceSpriteAnimation::createInstance()
{
	PSpriteAnimation object = ege_new SpriteAnimation(engine(), name());
  if (NULL != object)
  {
    // set new data
    if (EGE_SUCCESS != setInstance(object))
    {
      object = NULL;
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceSpriteAnimation::setInstance(const PSpriteAnimation& instance)
{
  EGEResult result;

  // sanity check
  if ((NULL == instance) || (STATE_LOADED != m_state))
  {
    return EGE_ERROR;
  }

  // construct
  if (EGE_SUCCESS != (result = instance->construct()))
  {
    // error!
    return result;
  }

  // generate frame data
  calculateFrameData();
  
  // setup data
  instance->setKeysPerSecond(m_fps);
  instance->setFrameData(m_frameData);

  // create copy of the texture and reset texture rectangle as information is included in frames data
  PTextureImage texture = ege_new TextureImage();
  texture->copy(m_sheet->textureImage());
  texture->setRect(Rectf::UNIT);
  instance->setTexture(texture);

  // add sequencers
  for (SequenceResourceList::iterator it = m_sequenceResources.begin(); it != m_sequenceResources.end(); ++it)
  {
    PResourceSequencer seqRes = *it;

    // create instance of sequencer
    PSequencer seq = seqRes->createInstance();
    if (NULL == seq)
    {
      // error!
      return EGE_ERROR_NO_MEMORY;
    }

    instance->addSequencer(seq);
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceSpriteAnimation::invalidateFrameData()
{
  m_frameDataInvalid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceSpriteAnimation::calculateFrameData()
{
  const float32 cellWidth  = 1.0f / sheet()->framesPerRow();
  const float32 cellHeight = 1.0f / Math::Ceil(sheet()->frameCount() / static_cast<float32>(sheet()->framesPerRow()));

  if (m_frameDataInvalid)
  {
    // create new data for each frame
    m_frameData.clear();
    for (s32 i = 0; i < sheet()->frameCount(); ++i)
    {
      SpriteAnimationFrameData data;
  
      data.m_rect = m_sheet->textureImage()->rect().combine(Rectf(i % sheet()->framesPerRow() * cellWidth, 
                                                                  i / sheet()->framesPerRow() * cellHeight, cellWidth, cellHeight));

      // add to pool
      m_frameData.push_back(data);
    }

    // validate
    m_frameDataInvalid = false;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceSpriteAnimation::addSequence(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // create sequence resource manually
  PResourceSequencer seqRes = ResourceSequencer::Create(engine(), group());
  if (NULL == seqRes)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // deserialize resource
  if (EGE_SUCCESS != (result = seqRes->create("", tag)))
  {
    // error!
    return result;
  }

  // check if such sequence already exists
  for (SequenceResourceList::const_iterator it = m_sequenceResources.begin(); it != m_sequenceResources.end(); ++it)
  {
    const PResourceSequencer& resource = *it;
    if (resource->name() == seqRes->name())
    {
      // error!
      return EGE_ERROR_ALREADY_EXISTS;
    }
  }

  // add to pool
  m_sequenceResources.push_back(seqRes);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END