#ifndef EGE_CORE_IMAGEOVERLAY_H
#define EGE_CORE_IMAGEOVERLAY_H

#include <EGETime.h>
#include <EGEString.h>
#include "Core/Overlay/Overlay.h"
#include "Core/Graphics/Font.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(ImageOverlay, PImageOverlay)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImageOverlay : public Overlay
{
  public: 

    ImageOverlay(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~ImageOverlay();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
  
    /*! @see Overlay::addForRendering. */
    void addForRendering(IRenderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;

  private:

    /*! @see Overlay::initialize. */
    void initialize() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGEOVERLAY_H
