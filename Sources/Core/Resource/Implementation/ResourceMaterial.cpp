#include "Core/Resource/Interface/ResourceMaterial.h"
#include "Core/Resource/Interface/ResourceTexture.h"
#include "Core/Resource/Interface/ResourceTextureImage.h"
#include "Core/Resource/Interface/ResourceProgram.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/TextureImage.h"
#include "EGEAssert.h"
#include "EGEEngine.h"
#include "EGEResources.h"
#include "EGEResourceManager.h"
#include "EGEStringUtils.h"
#include "EGETexture.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceMaterialDebugName = "EGEResourceMaterial";

static const String KNodeTexture     = "texture";
static const String KNodeTextureRef  = "texture-ref";
static const String KNodePass        = "pass";
static const String KNodeProgramRef  = "program-ref";

static const String KAttributeTextureName           = "name";
static const String KAttributeTextureRect           = "rect";
static const String KAttributeTextureEnvMode        = "env-mode";
static const String KAttributeTextureManual         = "manual";
static const String KAttributeTextureRotation       = "rotation";
static const String KAttributeTextureTexCoordsIndex = "tex-coord";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping texture environment mode name into value. */
TextureEnvironmentMode MapTextureEnvironmentMode(const String& name, TextureEnvironmentMode defaultValue)
{
  if ("replace" == name)
  {
    return EM_REPLACE;
  }
  else if ("modulate" == name)
  {
    return EM_MODULATE;
  }
  else if ("decal" == name)
  {
    return EM_DECAL;
  }
  else if ("blend" == name)
  {
    return EM_BLEND;
  }
  else if ("add" == name)
  {
    return EM_ADD;
  }
  else if ("combine" == name)
  {
    return EM_COMBINE;
  }

  return defaultValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping blend factor's name into value. */
EGEGraphics::BlendFactor MapBlendFactor(const String& name, EGEGraphics::BlendFactor defaultValue)
{
  // check if no data to convert
  if (name.isEmpty())
  {
    // return default value
    return defaultValue;
  }

  // map value
  EGEGraphics::BlendFactor factor = EGEGraphics::BF_UNKNOWN;

  if ("zero" == name)
  {
    factor = EGEGraphics::BF_ZERO;
  }
  else if ("one" == name)
  {
    factor = EGEGraphics::BF_ONE;
  }
  else if ("src-color" == name)
  {
    factor = EGEGraphics::BF_SRC_COLOR;
  }
  else if ("dst-color" == name)
  {
    factor = EGEGraphics::BF_DST_COLOR;
  }
  else if ("one-minus-src-color" == name)
  {
    factor = EGEGraphics::BF_ONE_MINUS_SRC_COLOR;
  }
  else if ("one-minus-dst-color" == name)
  {
    factor = EGEGraphics::BF_ONE_MINUS_DST_COLOR;
  }
  else if ("src-alpha" == name)
  {
    factor = EGEGraphics::BF_SRC_ALPHA;
  }
  else if ("dst-alpha" == name)
  {
    factor = EGEGraphics::BF_DST_ALPHA;
  }
  else if ("one-minus-src-alpha" == name)
  {
    factor = EGEGraphics::BF_ONE_MINUS_SRC_ALPHA;
  }
  else if ("one-minus-dst-alpha" == name)
  {
    factor = EGEGraphics::BF_ONE_MINUS_DST_ALPHA;
  }

  return factor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceMaterial)
EGE_DEFINE_DELETE_OPERATORS(ResourceMaterial)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceMaterial::ResourceMaterial(Engine& engine, ResourceGroup* group) 
: IResource(engine, group, RESOURCE_NAME_MATERIAL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceMaterial::~ResourceMaterial()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceMaterial::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceMaterial::create(const String& path, const PObject& data)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  PXmlElement tag = ege_pcast<PXmlElement>(data);

  bool error = false;
  bool defaultPassInUse = false;

  PassData defaultPass;

  // get data
  m_name = tag->attribute("name");

  // also read data for default pass
  defaultPass.m_srcBlend        = MapBlendFactor(tag->attribute("src-blend").toLower(), EGEGraphics::BF_ONE);
  defaultPass.m_dstBlend        = MapBlendFactor(tag->attribute("dst-blend").toLower(), EGEGraphics::BF_ZERO);
  defaultPass.m_diffuseColor    = StringUtils::ToColor(tag->attribute("diffuse-color", "1 1 1 1"), &error);
  defaultPass.m_ambientColor    = StringUtils::ToColor(tag->attribute("ambient-color", "1 1 1 1"), &error);
  defaultPass.m_specularColor   = StringUtils::ToColor(tag->attribute("specular-color", "0 0 0 1"), &error);
  defaultPass.m_emissionColor   = StringUtils::ToColor(tag->attribute("emission-color", "0 0 0 1"), &error);
  defaultPass.m_shininess       = tag->attribute("shininess", "0").toFloat(&error);

  // check if obligatory data is wrong
  if (error || m_name.isEmpty())
  {
    // error!
    egeWarning(KResourceMaterialDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all sub node
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (KNodeTexture == child->name())
    {
      EGE_ASSERT(false);
      result = EGE_ERROR;
    }
    else if (KNodeTextureRef == child->name())
    {
      // textures without pass add to default one
      result = addTextureReference(child, defaultPass);
      
      // mark to indicate default pass is in use
      defaultPassInUse = true;
    }
    else if (KNodePass == child->name())
    {
      // add defined pass
      result = addPass(child);
    }
    else if (KNodeProgramRef == child->name())
    {
      // add program reference (shaders without pass add to default one)
      result = addProgramReference(child, defaultPass);

      // mark to indicate default pass is in use
      defaultPassInUse = true;
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

  // check if no single pass has been defined
  if (m_passes.isEmpty() || defaultPassInUse)
  {
    // add default one to the front
    EGE_ASSERT(m_passes.isEmpty());
    m_passes.append(defaultPass);
    // TAGE - check what is going on here ie if 2 texture-refs are defined are there 2 different default passes created ?!
//    m_passes.insert(m_passes.begin(), defaultPass);
  }

  // apply override modifier to all passes
  for (PassDataArray::Iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PassData& pass = *it;

    if (tag->hasAttribute("src-blend"))
    {
      pass.m_srcBlend = defaultPass.m_srcBlend;
    }

    if (tag->hasAttribute("dst-blend"))
    {
      pass.m_dstBlend = defaultPass.m_dstBlend;
    }

    if (tag->hasAttribute("diffuse-color"))
    {
      pass.m_diffuseColor = defaultPass.m_diffuseColor;
    }

    if (tag->hasAttribute("ambient-color"))
    {
      pass.m_ambientColor = defaultPass.m_ambientColor;
    }

    if (tag->hasAttribute("specular-color"))
    {
      pass.m_specularColor = defaultPass.m_specularColor;
    }

    if (tag->hasAttribute("emission-color"))
    {
      pass.m_emissionColor = defaultPass.m_emissionColor;
    }

    if (tag->hasAttribute("shininess"))
    {
      pass.m_shininess = defaultPass.m_shininess;
    }
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
EGEResult ResourceMaterial::load()
{
  EGEResult result = EGE_SUCCESS;

  if (STATE_LOADED != m_state)
  {
    // try to load all dependencies
    result = loadDependencies();
    if (EGE_SUCCESS == result)
    {
      // fetch data to passes
      for (PassDataArray::Iterator passIt = m_passes.begin(); passIt != m_passes.end(); ++passIt)
      {
        PassData& pass = *passIt;

        for (TextureImageDataList::ConstIterator it = pass.m_textureImageData.begin(); it != pass.m_textureImageData.end(); ++it)
        {
          const TextureImageData& textureImageData = *it;

          pass.m_textureImages.append(textureImageData.textureImage);
        }
      }

      // set loaded
      m_state = STATE_LOADED;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceMaterial::unload()
{
  if (STATE_LOADED == m_state)
  {
    for (PassDataArray::Iterator passIt = m_passes.begin(); passIt != m_passes.end(); ++passIt)
    {
      PassData& pass = *passIt;

      for (TextureImageList::Iterator it = pass.m_textureImages.begin(); it != pass.m_textureImages.end(); ++it)
      {
        *it = NULL;
      }
      pass.m_textureImages.clear();

      // unload all textures for current pass
      for (TextureImageDataList::Iterator it = pass.m_textureImageData.begin(); it != pass.m_textureImageData.end(); ++it)
      {
        TextureImageData& textureImageData = *it;

        textureImageData.textureImage = NULL;

        PResourceTextureImage textureImageRes = engine().resourceManager()->resource(RESOURCE_NAME_TEXTURE_IMAGE, textureImageData.name);
        if (textureImageRes)
        {
          textureImageRes->unload();
        }
        else
        {
          PResourceTexture textureRes = engine().resourceManager()->resource(RESOURCE_NAME_TEXTURE, textureImageData.name);
          if (textureRes)
          {
            textureRes->unload();
          }
        }
      }

      // unload all program for current pass
      PResourceProgram resource = engine().resourceManager()->resource(RESOURCE_NAME_PROGRAM, pass.m_programName);
      if (NULL != resource)
      {
        resource->unload();
      }
      pass.m_program = NULL;
    }

    // reset flag
    m_state = STATE_UNLOADED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceMaterial::addTextureReference(const PXmlElement& tag, PassData& pass)
{
  EGEResult result = EGE_SUCCESS;

  TextureImageData textureData;

  bool error = false;

  // get data
  textureData.name              = tag->attribute(KAttributeTextureName);
  textureData.rect              = StringUtils::ToRectf(tag->attribute(KAttributeTextureRect, "0 0 1 1"), &error);
  textureData.envMode           = MapTextureEnvironmentMode(tag->attribute(KAttributeTextureEnvMode, "modulate"), EM_MODULATE);
  textureData.manual            = tag->attribute(KAttributeTextureManual, "false").toBool(&error);
  textureData.rotationAngle     = StringUtils::ToAngle(tag->attribute(KAttributeTextureRotation, "0"), &error);
  textureData.textureCoordIndex = tag->attribute(KAttributeTextureTexCoordsIndex, pass.m_textureImageData.size());

  // check if obligatory data is wrong
  if (error || textureData.name.isEmpty())
  {
    // error!
    egeWarning(KResourceMaterialDebugName) << "Failed for name:" << name();
    return EGE_ERROR_BAD_PARAM;
  }

  // add into pool
  pass.m_textureImageData.append(textureData);

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceMaterial::addPass(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // add new pass to pool
  m_passes.append(PassData());

  // retrieve reference to it
  PassData& pass = m_passes.last();

  bool error = false;

  // get data
  pass.m_srcBlend        = MapBlendFactor(tag->attribute("src-blend").toLower(), EGEGraphics::BF_ONE);
  pass.m_dstBlend        = MapBlendFactor(tag->attribute("dst-blend").toLower(), EGEGraphics::BF_ZERO);
  pass.m_diffuseColor    = StringUtils::ToColor(tag->attribute("diffuse-color", "1 1 1 1"), &error);
  pass.m_ambientColor    = StringUtils::ToColor(tag->attribute("ambient-color", "1 1 1 1"), &error);
  pass.m_specularColor   = StringUtils::ToColor(tag->attribute("specular-color", "0 0 0 1"), &error);
  pass.m_emissionColor   = StringUtils::ToColor(tag->attribute("emission-color", "0 0 0 1"), &error);
  pass.m_shininess       = tag->attribute("shininess", "0").toFloat(&error);

  if (error)
  {
    // error!
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all sub node
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (KNodeTexture == child->name())
    {
      EGE_ASSERT(false);
      result = EGE_ERROR;
    }
    else if (KNodeTextureRef == child->name())
    {
      result = addTextureReference(child, pass);
    }
    else if (KNodeProgramRef == child->name())
    {
      result = addProgramReference(child, pass);
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

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceMaterial::addProgramReference(const PXmlElement& tag, PassData& pass)
{
  // get data
  String name = tag->attribute("name", "");

  // check if invalid data
  if (name.isEmpty())
  {
    // error!
    return EGE_ERROR_BAD_PARAM;
  }

  // set name
  pass.m_programName = name;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PMaterial ResourceMaterial::createInstance() const
{
	PMaterial object = ege_new Material();
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
EGEResult ResourceMaterial::setInstance(const PMaterial& instance) const
{
  // sanity check
  if ((NULL == instance) || (STATE_LOADED != m_state))
  {
    // error!
    return EGE_ERROR;
  }

  // clean up
  instance->clear();

  // create render passes
  for (PassDataArray::ConstIterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    const PassData& pass = *it;

    // add new pass
    PRenderPass renderPass = instance->addPass(NULL);
    if (NULL == renderPass)
    {
      // error!
      return EGE_ERROR;
    }

    // fill in data
    renderPass->setSrcBlendFactor(pass.m_srcBlend);
    renderPass->setDstBlendFactor(pass.m_dstBlend);

    renderPass->setAmbientColor(pass.m_ambientColor);
    renderPass->setDiffuseColor(pass.m_diffuseColor);
    renderPass->setSpecularColor(pass.m_specularColor);
    renderPass->setEmissionColor(pass.m_emissionColor);

    renderPass->setShininess(pass.m_shininess);

    renderPass->setProgram(pass.m_program);

    for (TextureImageList::ConstIterator itTexture = pass.m_textureImages.begin(); itTexture != pass.m_textureImages.end(); ++itTexture)
    {
      // allocate new texture image
      PTextureImage texImg = ege_new TextureImage();
      if (NULL == texImg)
      {
        // error!
        return EGE_ERROR_NO_MEMORY;
      }

      // copy data into new texture image
      texImg->copy(*itTexture);

      // add to render pass
      EGEResult result;
      if (EGE_SUCCESS != (result = renderPass->addTexture(texImg)))
      {
        // error!
        return result;
      }
    }
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEGraphics::BlendFactor ResourceMaterial::srcBlendFactor(u32 pass) const 
{ 
  return (pass < passCount()) ? m_passes[pass].m_srcBlend : EGEGraphics::BF_ONE; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEGraphics::BlendFactor ResourceMaterial::dstBlendFactor(u32 pass) const 
{ 
  return (pass < passCount()) ? m_passes[pass].m_dstBlend : EGEGraphics::BF_ZERO; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Color& ResourceMaterial::ambientColor(u32 pass) const
{ 
  return (pass < passCount()) ? m_passes[pass].m_ambientColor : Color::WHITE; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Color& ResourceMaterial::diffuseColor(u32 pass) const 
{ 
  return (pass < passCount()) ? m_passes[pass].m_diffuseColor : Color::WHITE; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Color& ResourceMaterial::specularColor(u32 pass) const 
{
  return (pass < passCount()) ? m_passes[pass].m_specularColor : Color::BLACK; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Color& ResourceMaterial::emissionColor(u32 pass) const
{ 
  return (pass < passCount()) ? m_passes[pass].m_emissionColor : Color::BLACK; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 ResourceMaterial::shininess(u32 pass) const 
{ 
  return (pass < passCount()) ? m_passes[pass].m_shininess : 0.0f; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceMaterial::loadDependencies()
{
  // try to load (if necessary) textures, shaders (for all passes)
  for (PassDataArray::Iterator passIt = m_passes.begin(); passIt != m_passes.end(); ++passIt)
  {
    PassData& pass = *passIt;

    // load all textures for current pass
    for (TextureImageDataList::Iterator it = pass.m_textureImageData.begin(); it != pass.m_textureImageData.end(); ++it)
    {
      TextureImageData& textureImageData = *it;

      // check if loaded already
      if (NULL != textureImageData.textureImage)
      {
        // skip
        continue;
      }

      // check if manual texture
      if (textureImageData.manual)
      {
        // add placeholder only
        textureImageData.textureImage = ege_new TextureImage();
        if (NULL == textureImageData.textureImage)
        {
          // error!
          return EGE_ERROR_NO_MEMORY;
        }

        textureImageData.textureImage->setName(textureImageData.name);
        continue;
      }

      PObject texture;

      // material referred texture space in use
      Rectf texRect(0, 0, 1, 1);

      // NOTE: Material can refer to Texture or TextureImage (ie. from atlas)

      // try to find TextureImage of a given name
      PResourceTextureImage textureImageRes = engine().resourceManager()->resource(RESOURCE_NAME_TEXTURE_IMAGE, textureImageData.name);
      if (textureImageRes)
      {
        // load texture image
        EGEResult result = textureImageRes->load();
        if (EGE_SUCCESS != result)
        {
          return result;
        }

        // retrieve referred texture
        TextureImage textureImage;
        if (EGE_SUCCESS != (result = textureImageRes->setInstance(textureImage)))
        {
          // error!
          return result;
        }

        // store referred texture
        texture = ege_pcast<PObject>(textureImage.texture());

        // set new texture space in use
        texRect = textureImage.rect();
      }
      else
      {
        // try to find Texture of a given name
        PResourceTexture textureRes = engine().resourceManager()->resource(RESOURCE_NAME_TEXTURE, textureImageData.name);
        if (textureRes)
        {
          // load texture
          EGEResult result = textureRes->load();
          if (EGE_SUCCESS != result)
          {
            return result;
          }

          // retrieve referred texture
          texture = textureRes->texture();
        }
      }
      
      // check if not found
      if (NULL == texture)
      {
        // texture not found
        egeWarning(KResourceMaterialDebugName) << "Material texture not found:" << textureImageData.name;
        return EGE_ERROR;
      }

      // calculate final referred rectangle
      Rectf finalRect = texRect.combine(textureImageData.rect);

      textureImageData.textureImage = ege_new TextureImage(ege_pcast<PTexture2D>(texture), finalRect);
      if ((NULL == textureImageData.textureImage) || ! textureImageData.textureImage->isValid())
      {
        // error!
        return EGE_ERROR;
      }

      // set texture data
      textureImageData.textureImage->setEnvironmentMode(textureImageData.envMode);
      textureImageData.textureImage->setRotationAngle(textureImageData.rotationAngle);
      textureImageData.textureImage->setTextureCoordIndex(textureImageData.textureCoordIndex);
    }

    // try to load program for current pass (if any)
    if ((NULL == pass.m_program) && ! pass.m_programName.isEmpty())
    {
      // try to find program resource of a given name
      PResourceProgram resource = engine().resourceManager()->resource(RESOURCE_NAME_PROGRAM, pass.m_programName);
      if (NULL == resource)
      {
        // error!
        return EGE_ERROR_NOT_FOUND;
      }

      // load it
      EGEResult result = resource->load();
      if (EGE_SUCCESS != result)
      {
        return result;
      }

      // store it
      pass.m_program = resource->program();
    }
  }

  // all dependencies loaded
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END