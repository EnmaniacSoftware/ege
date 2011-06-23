#ifndef EGE_CORE_RENDERER_PRIVATE_H
#define EGE_CORE_RENDERER_PRIVATE_H

#include <EGE.h>
#include "Core/Graphics/Render/Renderer.h"
#include "Core/Components/Render/RenderComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Renderer;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RendererPrivate
{
  public:

    RendererPrivate(Renderer* base);
   ~RendererPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Renderer)

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Clears given viewport. */
    void clearViewport(const PViewport& viewport);
    /* Sends all geometry through the geometry pipeline to hardware. */
    void flush();
    /* Applies material. */
    void applyMaterial(const PMaterial& material);

  private:

	  /* Detects and initializes extensions. */
	  void detectExtensions();
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERER_PRIVATE_H