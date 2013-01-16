#ifndef EGE_WIN32_GRAPHICS_PRIVATE_H
#define EGE_WIN32_GRAPHICS_PRIVATE_H

#include <EGE.h>
#include <EGEDictionary.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Graphics, PGraphics)
EGE_DECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
EGE_DECLARE_SMART_CLASS(RenderWindow, PRenderWindow)
EGE_DECLARE_SMART_CLASS(Renderer, PRenderer)
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