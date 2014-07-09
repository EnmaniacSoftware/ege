#ifndef EGE_CORE_IMAGEDANIMATION_H
#define EGE_CORE_IMAGEDANIMATION_H

/** Imaged animation objects represent discreet instances of image-based animations. These represent set of images being animated (transformed) during the 
 *  lifetime. 
 */

#include "EGE.h"
#include "EGETime.h"
#include "EGEFlags.h"
#include "EGEDynamicArray.h"
#include "EGESignal.h"
#include "EGEImagedAnimation.h"
#include "EGEMatrix.h"
#include "EGEMap.h"
#include "EGEGraphics.h"
#include "EGEVector2.h"
#include "EGEKeyFrameAnimation.h"
#include "EGERenderable.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
EGE_DECLARE_SMART_CLASS(ImagedAnimation, PImagedAnimation)
EGE_DECLARE_SMART_CLASS(Material, PMaterial)
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImagedAnimation : public KeyFrameAnimation
                      , public Renderable
{
  public:

    ImagedAnimation();
    ImagedAnimation(Application* app, const String& name);
   ~ImagedAnimation();

    using KeyFrameAnimation::play;

  public:

    /*! @see IAnimation::play. */
    EGEResult play(const String& sequencerName) override;

    /*! @see Renderable::addForRendering. */
    EGEResult addForRendering(IRenderer& renderer) override;

    /*! Sets global transformation matrix. 
     *  @param  transform New global transformation matrix for animation.
     */
    void setTransformationMatrix(const Matrix4f& transform);
 
    /*! Sets display size (in pixels). */
    void setDisplaySize(const Vector2f& size);
    /*! Returns display size (in pixels). */
    const Vector2f& displaySize() const;

    /*! Sets base display alignment. 
     *  @param alignment Alignment animation is originally created for.
     *  @note  Animation if always aligned to TOP_LEFT anchor from its base alignment.
     */
    void setBaseAlignment(Alignment alignment);

    /*! Adds animation data.
     *  @param  objects       Map of all unique objects animation is built from.
     *  @param  frameActions  List of actions for each each frame of animation.
     */
    void addData(const EGEImagedAnimation::ObjectMap& objects, const List<EGEImagedAnimation::ActionDataList>& frameActions);

    /*! Sets render priority. */
    void setRenderPriority(s32 priority);
    /*! Clears object. */
    void clear();

    /*! Sets transparency level. */
    void setAlpha(float32 alpha);

  private:

    typedef DynamicArray<PRenderComponent> RenderComponentArray;
    typedef List<EGEImagedAnimation::ActionDataList> ActionDataList;

  private:

    /*! Calculates number of vertices required for a given render frame. */
    u32 calculateFrameVertexCount(const EGEImagedAnimation::ObjectMap& objects, const EGEImagedAnimation::ActionDataList& frameActionList) const;
    /*! Updates render data. */
    void updateRenderData();

  private:

    /*! Base render priority. */
    s32 m_renderPriority; 
    /*! Display size (in pixels). */
    Vector2f m_displaySize;
    /*! Base display alignment. */
    Alignment m_baseAlignment;
    /*! Global color alpha value. */
    float32 m_alpha;
    /*! Render data validity flag. */
    bool m_renderDataNeedsUpdate;
    /*! Global transformation matrix. */
    Matrix4f m_transform;
    /*! Animation objects sorted by object ID. */
    EGEImagedAnimation::ObjectMap m_objects;
    /*! Animation frame action list. */
    ActionDataList m_framesActionsList;
    /*! Array of frame render components. */
    RenderComponentArray m_renderComponents;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGEDANIMATION_H