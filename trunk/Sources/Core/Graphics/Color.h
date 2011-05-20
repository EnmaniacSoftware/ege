#ifndef EGE_CORE_COLOR_H
#define EGE_CORE_COLOR_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Color
{
  public:

    Color(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f) : m_red(red), m_green(green), m_blue(blue), m_alpha(alpha) {}

    inline Color& operator=(const Color& color)
    {
      m_red   = color.m_red;
      m_green = color.m_green;
      m_blue  = color.m_blue;
      m_alpha = color.m_alpha;
      return *this;
    }
    
    inline bool operator==(const Color& color) const
    {
      // check if NOT equal
      return !(m_red != color.m_red || m_green != color.m_green || m_blue != color.m_blue || m_alpha != color.m_alpha);
    }

    inline bool operator!=(const Color& color) const
    {
      return !operator==(color);
    }

    /*! Red component value. */
    float m_red;
    /*! Green component value/ */
    float m_green;
    /*! Blue component value. */
    float m_blue;
    /*! Alpha component value. */
    float m_alpha;

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