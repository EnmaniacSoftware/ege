#ifndef EGE_GRAPHICS_H
#define EGE_GRAPHICS_H

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace EGEGraphics
{
    /*! Common predefined render priorites. */
    enum RenderPriority
    {
      RP_BACKGROUND       = 0,
      RP_MAIN             = 50,
      RP_PHYSICS_DEBUG    = 95,
      RP_MAIN_OVERLAY     = 100,
      RP_MAIN_CURSOR      = 200
    };

    /*! Render primitive types. */
    enum RenderPrimitiveType
    {
      RPT_TRIANGLES = 0,
      RPT_TRIANGLE_STRIPS,
      RPT_TRIANGLE_FAN,
      RPT_LINES,
      RPT_LINE_LOOP,
      RPT_POINTS
    };

    /*! Render type. */
    enum RenderType
    {
      RT_NORMAL,
      RT_POINT_SPRITE
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
    enum BlendFactor
    {
      BF_UNKNOWN,
      BF_ZERO,
      BF_ONE,
      BF_SRC_COLOR,
      BF_DST_COLOR,
      BF_ONE_MINUS_SRC_COLOR,
      BF_ONE_MINUS_DST_COLOR,
      BF_SRC_ALPHA,
      BF_DST_ALPHA,
      BF_ONE_MINUS_SRC_ALPHA,
      BF_ONE_MINUS_DST_ALPHA
    };

    /** Screen orientation. */
    //enum ScreenOrientation
    //{
    //  SO_NORMAL,
    //  SO_ROT_90,
    //  SO_ROT_270
    //};
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
#include "Core/Graphics/RenderObjectFactory.h"
#include "Core/Graphics/HardwareResourceProvider.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_GRAPHICS_H