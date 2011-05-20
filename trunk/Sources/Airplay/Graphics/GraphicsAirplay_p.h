#ifndef EGE_AIRPLAY_GRAPHICS_PRIVATE_H
#define EGE_AIRPLAY_GRAPHICS_PRIVATE_H

#include "EGE.h"

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

    GraphicsPrivate(Graphics* base, const ConfigParams& params);
   ~GraphicsPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  private:

    /*! Pointer to base object. */
    Graphics* m_base;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_GRAPHICS_PRIVATE_H