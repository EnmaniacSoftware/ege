#include "Core/Resource/ResourceSpriteAnimation.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/SpriteAnimation/SpriteAnimation.h"
#include "Core/Resource/ResourceTextureImage.h"
#include "Core/Resource/ResourceSequencer.h"
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_SEQUENCE "sequence"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceSpriteAnimation)
EGE_DEFINE_DELETE_OPERATORS(ResourceSpriteAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSpriteAnimation::ResourceSpriteAnimation(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_SPRITE_ANIMATION), 
                                                                                               m_frameDataInvalid(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSpriteAnimation::~ResourceSpriteAnimation()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceSpriteAnimation::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceSpriteAnimation(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceSpriteAnimation::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceSpriteAnimation::create(const String& path, const PXmlElement& tag)
{
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
    egeWarning() << "Failed for name:" << m_name;
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
/*! IResource override. Returns TRUE if object is loaded. */
bool ResourceSpriteAnimation::isLoaded() const 
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
EGEResult ResourceSpriteAnimation::load()
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
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceSpriteAnimation::unload() 
{ 
  egeDebug() << name();

  // unload texture
  m_sheet = NULL;

 // m_sequenceResources.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of sprite object defined by resource. */
PSpriteAnimation ResourceSpriteAnimation::createInstance()
{
	PSpriteAnimation object = ege_new SpriteAnimation(app(), name());
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
EGEResult ResourceSpriteAnimation::setInstance(const PSpriteAnimation& instance)
{
  EGEResult result;

  // sanity check
  if (NULL == instance || !isLoaded())
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
  instance->setFPS(m_fps);
  instance->setFrameData(m_frameData);
  instance->setTexture(m_sheet->textureImage()->createInstance());

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
void ResourceSpriteAnimation::invalidateFrameData()
{
  m_frameDataInvalid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates frame data. */
void ResourceSpriteAnimation::calculateFrameData()
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
EGEResult ResourceSpriteAnimation::addSequence(const PXmlElement& tag)
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

EGE_NAMESPACE_END