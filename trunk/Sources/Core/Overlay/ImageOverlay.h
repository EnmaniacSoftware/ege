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

    ImageOverlay(Application* app, const String& name);
   ~ImageOverlay();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
  
    /* Sets material. */
    void setMaterialName(const String& name);

  private:

    /* Overlay override. Updates overlay. */
    void update(const Time& time) override;
    /* Overlay override. Renders element. */
    void render(const Viewport* viewport, Renderer* renderer) override;
    /* Updates render data. */
    void updateRenderData();
    /*! Returns material name. */
    inline const String& materialName() const { return m_materialName; }

  private:

    /*! Material name. */
    String m_materialName;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGEOVERLAY_H
