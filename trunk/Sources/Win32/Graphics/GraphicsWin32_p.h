#ifndef EGE_WIN32_GRAPHICS_PRIVATE_H
#define EGE_WIN32_GRAPHICS_PRIVATE_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Graphics;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class GraphicsPrivate
{
  public:

    GraphicsPrivate(Graphics* base);
   ~GraphicsPrivate();

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Graphics)

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates object. */
    EGEResult construct();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_GRAPHICS_PRIVATE_H