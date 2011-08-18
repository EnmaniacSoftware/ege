#ifndef EGE_RESOURCES_H
#define EGE_RESOURCES_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define DEFAULT_GROUP_NAME          "default-group"
#define RESOURCE_NAME_TEXTURE       "texture"
#define RESOURCE_NAME_TEXTURE_IMAGE "texture-image"
#define RESOURCE_NAME_MATERIAL      "material"
#define RESOURCE_NAME_FONT          "font"
#define RESOURCE_NAME_DATA          "data"
#define RESOURCE_NAME_SPRITE_SHEET  "spritesheet"
#define RESOURCE_NAME_SPRITE        "sprite"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Resource/Resource.h"
#include "Core/Resource/ResourceData.h"
#include "Core/Resource/ResourceMaterial.h"
#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceTextureImage.h"
#include "Core/Resource/ResourceSpritesheet.h"
#include "Core/Resource/ResourceSprite.h"
#include "Core/Resource/ResourceFont.h"
#include "Core/Resource/ResourceGroup.h"
#include "Core/Resource/ResourceManager.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_RESOURCES_H