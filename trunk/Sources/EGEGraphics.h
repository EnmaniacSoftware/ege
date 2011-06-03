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
      RENDER_PRIORITY_MAIN_OVERLAY     = 100,
      RENDER_PRIORITY_MAIN_CURSOR      = 200
    };

    /*! Render primitive types. */
    enum ERenderPrimitiveType
    {
      RENDER_PRIMITIVE_TYPE_TRIANGLES = 0,
      RENDER_PRIMITIVE_TYPE_LINES
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