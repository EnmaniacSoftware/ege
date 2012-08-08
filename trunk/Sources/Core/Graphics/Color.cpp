#include "Core/Graphics/Color.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Color Color::BLACK      = Color(0, 0, 0, 1);
const Color Color::WHITE      = Color(1, 1, 1, 1);
const Color Color::RED        = Color(1, 0, 0, 1);
const Color Color::GREEN      = Color(0, 1, 0, 1);
const Color Color::BLUE       = Color(0, 0, 1, 1);
const Color Color::PINK       = Color(1, 0.5f, 0.75f, 1);
const Color Color::NONE       = Color(0, 0, 0, 0);
const Color Color::GRAY       = Color(0.5f, 0.5f, 0.5f, 1);
const Color Color::DARK_GRAY  = Color(0.66f, 0.66f, 0.66f, 1);
const Color Color::LIGHT_GRAY = Color(0.82f, 0.82f, 0.82f, 1);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns packed 32-bit RGBA color representation. */
u32 Color::packed() const
{
  u32 r = static_cast<u32>(red * 255.0f);
  u32 g = static_cast<u32>(green * 255.0f);
  u32 b = static_cast<u32>(blue * 255.0f);
  u32 a = static_cast<u32>(alpha * 255.0f);

  return (r << 24) | (g << 16) | (b << 8) | a;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& operator << (Debug& debug, const Color& obj)
{
  debug.nospace() << "Color(" << obj.red << "," << obj.green << "," << obj.blue << "," << obj.alpha << ")";
  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END