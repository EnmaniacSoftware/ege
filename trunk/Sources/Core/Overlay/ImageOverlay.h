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
  
    /* Sets material. */
    void setMaterialName(const String& name);
    /* Overlay override. Returns TRUE if object is valid. */
    bool isValid() const override;
    /* Overlay override. Sets alignment. */
    void setAlignment(Alignment align) override;
    /* Overlay override. Renders overlay. */
    void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;
    /* Overlay override. Updates overlay. */
    void update(const Time& time) override;
    /* Overlay override. Sets transparency level. */
    void setAlpha(float32 alpha) override;

  private:

    /* Overlay override. Initializes object. */
    void initialize() override;
    /* Updates render data. */
    void updateRenderData();
    /*! Returns material name. */
    inline const String& materialName() const { return m_materialName; }

  private:

    /*! Material name. */
    String m_materialName;
    /*! Local material. */
    PMaterial m_material;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGEOVERLAY_H
