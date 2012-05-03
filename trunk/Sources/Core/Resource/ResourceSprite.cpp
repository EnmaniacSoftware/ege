#include "Core/Resource/ResourceSprite.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/Sprite/Sprite.h"
#include "Core/Resource/ResourceTextureImage.h"
#include "Core/Resource/ResourceSequencer.h"
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_SEQUENCE "sequence"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceSprite)
EGE_DEFINE_DELETE_OPERATORS(ResourceSprite)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSprite::ResourceSprite(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_SPRITE), 
                                                                             m_frameDataInvalid(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSprite::~ResourceSprite()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceSprite::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceSprite(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceSprite::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceSprite::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name          = tag->attribute("name");
  m_sheetName     = tag->attribute("sheet");
  m_fps           = tag->attribute("fps").toFloat(&error);
//  m_pingPong      = tag->attribute("ping-pong").toBool(&error);
  //m_frameCount    = tag->attribute("frame-count").toInt(&error);
  //m_beginFrame    = tag->attribute("begin-frame").toInt(&error);
 // m_repeat        = tag->attribute("repeat").toBool(&error);
  m_repeatDelay   = tag->attribute("repeat-delay", "0").toTime(&error);

  // check if obligatory data is wrong
  if (error || m_name.empty() || m_sheetName.empty() /*|| (0 >= m_frameCount)*/ /*|| (0 > m_beginFrame)*/)
  {
    // error!
    EGE_PRINT("ERROR: Failed for name: %s", m_name.toAscii());
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

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is loaded. */
bool ResourceSprite::isLoaded() const 
{ 
  if (NULL == m_sheet)
  {
    // error!
    return false;
  }

  // check if all sequencers are loaded
  for (SequenceResourceList::const_iterator it = m_sequenceResources.begin(); it != m_sequenceResources.end(); ++it)
  {
    PResourceSequencer seqResource = *it;
    if (!seqResource->isLoaded())
    {
      // error!
      return false;
    }
  }

  return true; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceSprite::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    // get sheet resource
    m_sheet = manager()->resource(RESOURCE_NAME_SPRITE_SHEET, sheetName());
    if (m_sheet)
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
      result = EGE_ERROR_NOT_FOUND;
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
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceSprite::unload() 
{ 
  EGE_PRINT("%s", name().toAscii());

  // unload texture
  m_sheet = NULL;

  m_sequenceResources.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of sprite object defined by resource. */
PSprite ResourceSprite::createInstance()
{
	PSprite object = ege_new Sprite(app(), name());
  if (object)
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
/* Set given instance of sprite object to what is defined by resource. */
EGEResult ResourceSprite::setInstance(const PSprite& instance)
{
  // sanity check
  if (NULL == instance || !isLoaded())
  {
    return EGE_ERROR;
  }

  // generate frame data
  calculateFrameData();
  
  // setup data
  instance->setFPS(m_fps);
  //instance->setDuration(m_duration);
  instance->setFrameData(m_frameData);
  instance->setTexture(m_sheet->textureImage()->createInstance());
  instance->setRepeatDelay(m_repeatDelay);

  //Sprite::FinishPolicy finishPolicy = Sprite::FP_STOP;
  //if (m_repeat)
  //{
  //  finishPolicy = Sprite::FP_REPEAT;
  //}
  //else if (m_pingPong)
  //{
  //  finishPolicy = Sprite::FP_PING_PONG;
  //}
  //instance->setFinishPolicy(finishPolicy);

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
/*! Invalidates frame data. */
void ResourceSprite::invalidateFrameData()
{
  m_frameDataInvalid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates frame data. */
void ResourceSprite::calculateFrameData()
{
  const float32 cellWidth  = 1.0f / sheet()->framesPerRow();
  const float32 cellHeight = 1.0f / Math::Ceil(sheet()->frameCount() / static_cast<float32>(sheet()->framesPerRow()));

  if (m_frameDataInvalid)
  {
    TextureImage textureImage(app());
    m_sheet->textureImage()->setInstance(textureImage);

    // create new data for each frame
    m_frameData.clear();
    for (s32 i = 0; i < sheet()->frameCount(); ++i)
    {
      EGESprite::FrameData data;
  
      data.m_rect = textureImage.rect().combine(Rectf(i % sheet()->framesPerRow() * cellWidth, 
                                                      i / sheet()->framesPerRow() * cellHeight, cellWidth, cellHeight));

      // add to pool
      m_frameData.push_back(data);
    }

    // validate
    m_frameDataInvalid = false;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds sequence. */
EGEResult ResourceSprite::addSequence(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // create sequence resource manually
  PResourceSequencer seqRes = ResourceSequencer::Create(app(), manager());
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
