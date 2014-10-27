#include "Core/Resource/Interface/ResourceFactory.h"
#include "Core/Resource/Interface/ResourceCurve.h"
#include "Core/Resource/Interface/ResourceProgram.h"
#include "Core/Resource/Interface/ResourceSequencer.h"
#include "Core/Resource/Interface/ResourceShader.h"
#include "EGEDebug.h"
#include "EGEResources.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static IResource* CreateResourceCurve(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceCurve(engine, group);
}

static IResource* CreateResourceData(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceData(engine, group);
}

static IResource* CreateResourceFont(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceFont(engine, group);
}

static IResource* CreateResourceMaterial(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceMaterial(engine, group);
}

static IResource* CreateResourceParticleAffector(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceParticleAffector(engine, group);
}

static IResource* CreateResourceParticleEmitter(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceParticleEmitter(engine, group);
}

static IResource* CreateResourceProgram(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceProgram(engine, group);
}

static IResource* CreateResourceSequencer(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceSequencer(engine, group);
}

static IResource* CreateResourceShader(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceShader(engine, group);
}

static IResource* CreateResourceSound(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceSound(engine, group);
}

static IResource* CreateResourceSpriteAnimation(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceSpriteAnimation(engine, group);
}

static IResource* CreateResourceImagedAnimation(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceImagedAnimation(engine, group);
}

static IResource* CreateResourceSpritesheet(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceSpritesheet(engine, group);
}

static IResource* CreateResourceText(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceText(engine, group);
}

static IResource* CreateResourceTexture(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceTexture(engine, group);
}

static IResource* CreateResourceTextureImage(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceTextureImage(engine, group);
}

static IResource* CreateResourceWidget(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceWidget(engine, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KResourceFactoryDebugName = "EGEResourceFactory";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceFactory)
EGE_DEFINE_DELETE_OPERATORS(ResourceFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceFactory::ResourceFactory(Engine& engine) 
: Factory1<IResource*, ResourceGroup*>(engine)
{
  // register default interfaces
  if ((EGE_SUCCESS != registerInterface(RESOURCE_NAME_TEXTURE, CreateResourceTexture)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_TEXTURE_IMAGE, CreateResourceTextureImage)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_MATERIAL, CreateResourceMaterial)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_FONT, CreateResourceFont)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_DATA, CreateResourceData)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_SPRITE_SHEET, CreateResourceSpritesheet)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_SPRITE_ANIMATION, CreateResourceSpriteAnimation)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_CURVE, CreateResourceCurve)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_PARTICLE_EMITTER, CreateResourceParticleEmitter)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_PARTICLE_AFFECTOR, CreateResourceParticleAffector)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_TEXT, CreateResourceText)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_SOUND, CreateResourceSound)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_WIDGET, CreateResourceWidget)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_IMAGED_ANIMATION, CreateResourceImagedAnimation)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_SEQUENCE, CreateResourceSequencer)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_SHADER, CreateResourceShader)) ||
      (EGE_SUCCESS != registerInterface(RESOURCE_NAME_PROGRAM, CreateResourceProgram)))
  {
    egeWarning(KResourceFactoryDebugName) << "Could not register default ResourceFactory interfaces!";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceFactory::~ResourceFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

