#ifndef EGE_CORE_CONFIG_PARAMS_H
#define EGE_CORE_CONFIG_PARAMS_H

#include "EGEString.h"
#include "EGEMap.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef EGEMap<EGEString, EGEString> ConfigParams;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/*! Obligatory name for primary render target. */
#define EGE_PRIMARY_RENDER_TARGET_NAME "PrimaryRenderTarget"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

// render target specific
#define EGE_RENDER_TARGET_PARAM_COLOR_BITS "colorBits"
#define EGE_RENDER_TARGET_PARAM_NAME       "name"

// render window specific
#define EGE_RENDER_WINDOW_PARAM_DEPTH_BITS      "depthBits"
#define EGE_RENDER_WINDOW_PARAM_FULLSCREEN      "fullscreen"
#define EGE_RENDER_WINDOW_PARAM_TITLE           "title"
#define EGE_RENDER_WINDOW_PARAM_WIDTH           "width"
#define EGE_RENDER_WINDOW_PARAM_HEIGHT          "height"

// engine specific
//#define EGE_ENGINE_PARAM_RENDER_SYSTEM      "renderSystem"
#define EGE_ENGINE_PARAM_UPDATES_PER_SECOND "updatesPerSecond"
#define EGE_ENGINE_PARAM_LANDSCAPE_MODE     "landscape"

// physics specifc

/*! Real world to physics world scale factor. */
#define EGE_PHYSICS_PARAM_SCALE_FACTOR "physics:scale-factor"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONFIG_PARAMS_H