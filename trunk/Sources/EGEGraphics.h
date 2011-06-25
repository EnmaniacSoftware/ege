#ifndef EGE_GRAPHICS_H
#define EGE_GRAPHICS_H

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGEGraphics
{
    /*! Common predefined render priorites. */
    enum ERenderPriority
    {
      RENDER_PRIORITY_BACKGROUND       = 0,
      RENDER_PRIORITY_MAIN             = 50,
      RENDER_PRIORITY_PHYSICS_DEBUG    = 95,
      RENDER_PRIORITY_MAIN_OVERLAY     = 100,
      RENDER_PRIORITY_MAIN_CURSOR      = 200
    };

    /*! Render primitive types. */
    enum ERenderPrimitiveType
    {
      RENDER_PRIMITIVE_TYPE_TRIANGLES = 0,
      RENDER_PRIMITIVE_TYPE_TRIANGLE_STRIPS,
      RENDER_PRIMITIVE_TYPE_TRIANGLE_FAN,
      RENDER_PRIMITIVE_TYPE_LINES,
      RENDER_PRIMITIVE_TYPE_LINE_LOOP
    };

    /** Blend factors.
     *
     * Following SOURCE factors seems to be valid according to OpenGL Microsoft's documentation:
     * GL_ZERO, GL_ONE, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA_SATURATE
     *  
     * Following SOURCE factors seems to be valid according to OpenGL ES documentation:
     * GL_ZERO, GL_ONE, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA_SATURATE,
     * GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR.
     *
     *
     * Following DESTINATION factors seems to be valid according to OpenGL Microsoft's documentation:
     * GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA
     *
     * Following DESTINATION factors seems to be valid according to OpenGL ES documentation:
     * GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA,
     * GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR
     */
    enum EBlendFactor
    {
      BLEND_FACTOR_UNKNOWN,
      BLEND_FACTOR_ZERO,
      BLEND_FACTOR_ONE,
      BLEND_FACTOR_SRC_COLOR,
      BLEND_FACTOR_DST_COLOR,
      BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
      BLEND_FACTOR_ONE_MINUS_DST_COLOR,
      BLEND_FACTOR_SRC_ALPHA,
      BLEND_FACTOR_DST_ALPHA,
      BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      BLEND_FACTOR_ONE_MINUS_DST_ALPHA
    };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Memory/Memory.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/Font.h"
#include "Core/Graphics/Render/RenderTarget.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Graphics/Render/Renderer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_GRAPHICS_H