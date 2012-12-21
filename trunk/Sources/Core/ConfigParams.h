#ifndef EGE_CORE_CONFIGPARAMS_H
#define EGE_CORE_CONFIGPARAMS_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Obligatory name for primary render target. */
#define EGE_PRIMARY_RENDER_TARGET_NAME "PrimaryRenderTarget"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// render target specific
#define EGE_RENDER_TARGET_PARAM_COLOR_BITS "color-bits"
#define EGE_RENDER_TARGET_PARAM_NAME       "name"
#define EGE_RENDER_TARGET_PARAM_WIDTH      "width"
#define EGE_RENDER_TARGET_PARAM_HEIGHT     "height"
#define EGE_RENDER_TARGET_PARAM_ZOOM       "zoom"

// render window specific
#define EGE_RENDER_WINDOW_PARAM_DEPTH_BITS      "depth-bits"
#define EGE_RENDER_WINDOW_PARAM_FULLSCREEN      "fullscreen"
#define EGE_RENDER_WINDOW_PARAM_TITLE           "title"

// engine specific
//#define EGE_ENGINE_PARAM_RENDER_SYSTEM      "renderSystem"
#define EGE_ENGINE_PARAM_UPDATES_PER_SECOND "updates-per-second"
#define EGE_ENGINE_PARAM_LANDSCAPE_MODE     "landscape"
#define EGE_ENGINE_PARAM_RENDERS_PER_SECOND "renders-per-second"

// physics specifc

/*! Real world to physics world scale factor. */
#define EGE_PHYSICS_PARAM_SCALE_FACTOR "physics:scale-factor"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONFIGPARAMS_H