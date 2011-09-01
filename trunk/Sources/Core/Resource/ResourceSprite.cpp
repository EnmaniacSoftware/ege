#include "Core/Resource/ResourceSprite.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/Sprite/Sprite.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceSprite)
EGE_DEFINE_DELETE_OPERATORS(ResourceSprite)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSprite::ResourceSprite(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_SPRITE), m_frameDataInvalid(false)
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
  m_name        = tag->attribute("name");
  m_sheetName   = tag->attribute("sheet");
  m_duration    = tag->attribute("duration").toFloat(&error);
  m_pingPong    = tag->attribute("ping-pong").toBool(&error);
  m_frameCount  = tag->attribute("frame-count").toInt(&error);
  m_beginFrame  = tag->attribute("begin-frame").toInt(&error);
  m_repeat      = tag->attribute("repeat").toBool(&error);

  // check if obligatory data is wrong
  if (error || m_name.empty() || m_sheetName.empty() || (m_pingPong && m_repeat) || (0 >= m_frameCount) || (0 > m_beginFrame))
  {
    // error!
    EGE_PRINT("ResourceSprite::create - failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // invalidate frame data
  invalidateFrameData();

  // compose absolute path
 // m_path = path + "/" + m_path;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceSprite::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    // get sheet resource
    m_sheet = manager()->resource("spritesheet", sheetName());
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
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceSprite::unload() 
{ 
  EGE_PRINT("ResourceSprite::unload - %s", name().toAscii());

  // unload texture
  m_sheet = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of sprite object defined by resource. */
PSprite ResourceSprite::createInstance()
{
	PSprite object = ege_new Sprite(app());
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
  instance->setDuration(m_duration);
  instance->setFrameData(m_frameData);
  instance->setTexture(m_sheet->textureImage()->createInstance());

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
    TextureImage textureImage(app(), "");
    m_sheet->textureImage()->setInstance(textureImage);

    // create new data for each frame
    FameDataArray allFramesData;
    for (s32 i = 0; i < m_frameCount; ++i)
    {
      EGESprite::FrameData data;
  
      data.m_rect = textureImage.rect().combine(Rectf((m_beginFrame + i) % sheet()->framesPerRow() * cellWidth, 
                                                      (m_beginFrame + i) / sheet()->framesPerRow() * cellHeight, cellWidth, cellHeight));

      // add to pool
      allFramesData.push_back(data);
    }

    // clear old data
    m_frameData.clear();

    // fill in frames for single cycle
    if (m_repeat)
    {
      // for REPEAT mode, cycle consists of all frames from begin one to the end. After reaching end playback should repeat itself
      for (s32 i = 0; i < m_frameCount; ++i)
      {
        m_frameData.push_back(allFramesData[i]);
      }
    }
    else if (m_pingPong)
    {
      // for PING-PONG mode, cycle consists of all frames from begin one to the end and back. However, last frame should be counted only once and final frame
      // should end before first frame so in next cycle first frame is only processed once (by new cycle only)
      for (s32 i = 0; i < m_frameCount; ++i)
      {
        m_frameData.push_back(allFramesData[i]);
      }

      for (s32 i = m_frameCount - 1; i > 0; --i)
      {
        m_frameData.push_back(allFramesData[i]);
      }
    }
    else
    {
      EGE_ASSERT(false && "Not implemented mode");
    }

    // validate
    m_frameDataInvalid = false;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
