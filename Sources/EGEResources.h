#ifndef EGE_RESOURCES_H
#define EGE_RESOURCES_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define DEFAULT_GROUP_NAME              "default-group"
#define DEFAULT_MATERIAL_NAME           "default"

#define RESOURCE_NAME_TEXTURE           "texture"
#define RESOURCE_NAME_TEXTURE_IMAGE     "texture-image"
#define RESOURCE_NAME_MATERIAL          "material"
#define RESOURCE_NAME_FONT              "font"
#define RESOURCE_NAME_DATA              "data"
#define RESOURCE_NAME_SPRITE_SHEET      "spritesheet"
#define RESOURCE_NAME_SPRITE_ANIMATION  "sprite-animation"
#define RESOURCE_NAME_CURVE             "curve"
#define RESOURCE_NAME_PARTICLE_EMITTER  "particle-emitter"
#define RESOURCE_NAME_PARTICLE_AFFECTOR "particle-affector"
#define RESOURCE_NAME_TEXT              "text"
#define RESOURCE_NAME_SOUND             "sound"
#define RESOURCE_NAME_WIDGET            "widget"
#define RESOURCE_NAME_IMAGED_ANIMATION  "imaged-animation"
#define RESOURCE_NAME_SEQUENCE          "sequence"
#define RESOURCE_NAME_SHADER            "shader"
#define RESOURCE_NAME_PROGRAM           "program"

#define TR(engine, name)     (engine).resourceManager()->textResource((name))->text()
#define TRN(engine, name, n) (engine).resourceManager()->textResource((name))->text((n))

extern const char* KResourceManagerDebugName;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Core/Resource/Interface/Resource.h"
#include "Core/Resource/Interface/ResourceData.h"
#include "Core/Resource/Interface/ResourceMaterial.h"
#include "Core/Resource/Interface/ResourceTexture.h"
#include "Core/Resource/Interface/ResourceTextureImage.h"
#include "Core/Resource/Interface/ResourceSpritesheet.h"
#include "Core/Resource/Interface/ResourceSpriteAnimation.h"
#include "Core/Resource/Interface/ResourceCurve.h"
#include "Core/Resource/Interface/ResourceFont.h"
#include "Core/Resource/Interface/ResourceParticleEmitter.h"
#include "Core/Resource/Interface/ResourceParticleAffector.h"
#include "Core/Resource/Interface/ResourceText.h"
#include "Core/Resource/Interface/ResourceSound.h"
#include "Core/Resource/Interface/ResourceWidget.h"
#include "Core/Resource/Interface/ResourceImagedAnimation.h"
#include "Core/Resource/Interface/ResourceGroup.h"
#include "Core/Resource/Interface/ResourceManager.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_RESOURCES_H