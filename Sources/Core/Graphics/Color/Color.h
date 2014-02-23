#ifndef EGE_CORE_GRAPHICS_COLOR_COLOR_H
#define EGE_CORE_GRAPHICS_COLOR_COLOR_H

#include "EGETypes.h"

EGE_NAMESPACE_BEGIN
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Debug;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Color
{
  public:

    Color(float32 red = 1.0f, float32 green = 1.0f, float32 blue = 1.0f, float32 alpha = 1.0f);

    Color& operator =(const Color& color);
    bool   operator==(const Color& color) const;
    bool   operator!=(const Color& color) const;

  public:

    /*! Returns packed 32-bit RGBA color representation. */
    u32 packed() const;

  public:

    /*! Red component value. */
    float32 red;
    /*! Green component value/ */
    float32 green;
    /*! Blue component value. */
    float32 blue;
    /*! Alpha component value. */
    float32 alpha;

  public:

    static const Color BLACK;
    static const Color WHITE;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color PINK;
    static const Color NONE;
    static const Color GRAY;
    static const Color DARK_GRAY;
    static const Color LIGHT_GRAY;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Color& Color::operator=(const Color& color) 
{ 
  red   = color.red; 
  green = color.green; 
  blue  = color.blue; 
  alpha = color.alpha; 
  
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline bool Color::operator==(const Color& color) const 
{ 
  return ! (red != color.red || green != color.green || blue != color.blue || alpha != color.alpha); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline bool Color::operator!=(const Color& color) const 
{ 
  return ! operator==(color); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& operator << (Debug& debug, const Color& obj);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_COLOR_COLOR_H