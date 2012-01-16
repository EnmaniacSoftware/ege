#ifndef EGE_CORE_COLOR_H
#define EGE_CORE_COLOR_H

#include <EGETypes.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Color
{
  public:

    Color(float32 red = 1.0f, float32 green = 1.0f, float32 blue = 1.0f, float32 alpha = 1.0f) : red(red), green(green), blue(blue), alpha(alpha) {}

    inline Color& operator =(const Color& color) { red = color.red; green = color.green; blue = color.blue; alpha = color.alpha; return *this; }
    inline bool   operator==(const Color& color) const { return !(red != color.red || green != color.green || blue != color.blue || alpha != color.alpha); }
    inline bool   operator!=(const Color& color) const { return !operator==(color); }

  public:

    /*! Returns TRUE if color is invalid. */
    inline bool isNull() const { return (0.0f == red) && (0.0f == green) && (0.0f == blue) && (0.0f == alpha); }
    /* Returns packed 32-bit RGBA color representation. */
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

EGE_NAMESPACE_END

#endif // EGE_CORE_COLOR_H