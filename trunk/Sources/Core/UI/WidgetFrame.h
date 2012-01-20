#ifndef EGE_CORE_WIDGETFRAME_H
#define EGE_CORE_WIDGETFRAME_H

/** Widget frame class represents renderable (stretchable) visual base for widgets. 
 *  Each frame is divided into 9 seperate rectangular regions. Middle regions are stretched to match the required size.
 */

#include <EGEMath.h>
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Components/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class WidgetFrame
{
  public: 

    WidgetFrame(Application* app);
   ~WidgetFrame();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Renders dialog. */
    void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY);
    /* Sets material. */
    void setMaterial(const PMaterial& material);
    /* Sets rectanlges (in pixels). */
    void setRects(const Recti& topLeft, const Recti& topMiddle, const Recti& topRight, const Recti& middleLeft, const Recti& fill, const Recti& middleRight, 
                  const Recti& bottomLeft, const Recti& bottomMiddle, const Recti& bottomRight);
    /* Sets size. */
    void setSize(const Vector2f& size);
    /* Returns minimal size (in pixels). */
    Vector2f minSize() const;

  private:

    /* Generates render data. */
    void generateRenderData();

  private:

    /*! Render component. */
    PRenderComponent m_renderData;
    /*! Render data invalid flag. */
    bool m_renderDataInvalid;
    /*! Frame size (in pixels). */
    Vector2f m_size;
    /*! Top-left rectangle part (in pixels). This also defines texture coordinates. */
    Recti m_topLeftRect;
    /*! Top-middle rectangle part (in pixels). This also defines texture coordinates. */
    Recti m_topMiddleRect;
    /*! Top-right v (in pixels). This also defines texture coordinates. */
    Recti m_topRightRect;
    /*! Middle-left rectangle part (in pixels). This also defines texture coordinates. */
    Recti m_middleLeftRect;
    /*! Fill rectangle part (in pixels). This also defines texture coordinates. */
    Recti m_fillRect;
    /*! Middle-right rectangle part (in pixels). This also defines texture coordinates. */
    Recti m_middleRightRect;
    /*! Bottom-left rectangle part (in pixels). This also defines texture coordinates. */
    Recti m_bottomLeftRect;
    /*! Bottom-middle rectangle part (in pixels). This also defines texture coordinates. */
    Recti m_bottomMiddleRect;
    /*! Bottom-right rectangle part (in pixels). This also defines texture coordinates. */
    Recti m_bottomRightRect;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_WIDGETFRAME_H