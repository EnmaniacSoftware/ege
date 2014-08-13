#ifndef EGE_CORE_SPRITEANIMATION_H
#define EGE_CORE_SPRITEANIMATION_H

/** SpriteAnimation objects represent discreet instances of sprites. Sprites represent rectangular subset of spritesheets which define their look.
 *  Usually, sprites change their look (area within sheet) while time passes. This can be used to achieve some basic frame based animated images.
 */

#include "EGE.h"
#include "EGETime.h"
#include "EGEFlags.h"
#include "EGEDynamicArray.h"
#include "EGESignal.h"
#include "EGESpriteAnimation.h"
#include "EGEKeyFrameAnimation.h"
#include "EGEMatrix.h"
#include "EGERenderable.h"
#include "EGERect.h"
#include "Core/Graphics/TextureImage.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Structure defining frame data info. */
struct SpriteAnimationFrameData
{
  /*! Frame rectangle on texture (in normalized local coords). */
  Rectf m_rect;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
EGE_DECLARE_SMART_CLASS(SpriteAnimation, PSpriteAnimation)
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SpriteAnimation : public KeyFrameAnimation
                      , public Renderable
{
  public:

    SpriteAnimation(Engine& engine, const String& name);
   ~SpriteAnimation();

  public:

    /*! Constructs objects. */
    EGEResult construct();

    /*! @see Renderable::addForRendering. */
    EGEResult addForRendering(IRenderer& renderer) override;

    /*! Sets display size (in pixels). */
    void setDisplaySize(const Vector2f& size);
    /*! Returns display size (in pixels). */
    const Vector2f& displaySize() const;

    /*! Sets frame data. */
    void setFrameData(const DynamicArray<SpriteAnimationFrameData>& data);
    /*! Sets texture image containing sprite data. */
    void setTexture(const PTextureImage& texture);

    /*! Sets global transformation matrix. 
     *  @param  transform New global transformation matrix for animation.
     */
    void setTransformationMatrix(const Matrix4f& transform);

    /*! Sets base display alignment. 
     *  @param alignment Alignment animation is originally created for.
     *  @note  Animation if always aligned to TOP_LEFT anchor from its base alignment.
     */
    void setBaseAlignment(Alignment alignment);

    /*! Returns physics data component. */
    const PPhysicsComponent& physics() const { return m_physicsData; }

    /*! Sets render priority. */
    void setRenderPriority(s32 priority);

    /*! Sets transparency level. */
    void setAlpha(float32 alpha);

  private:

     /*! Updates render data. */
    void updateRenderData();

  private slots:

    /*! Slot called when internal transformation matrix is chanaged. */
    void onTransformationChanged();
    /*! @see KeyFrameAnimation::onSequencerFrameChanged. */
    void onSequencerFrameChanged(s32 frameId) override;

  private:

    /*! List of all frames in correct sequence for playback. */
    DynamicArray<SpriteAnimationFrameData> m_frameData;
    /*! Render data. */
    PRenderComponent m_renderData;
    /*! Physics data. */
    PPhysicsComponent m_physicsData;
    /*! Base display alignment. */
    Alignment m_baseAlignment;
    /*! Global color alpha value. */
    float32 m_alpha;
    /*! Render data validity flag. */
    bool m_renderDataNeedsUpdate;
    /*! Global transformation matrix. */
    Matrix4f m_transform;
    /*! Display size (in pixels). */
    Vector2f m_displaySize;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SPRITEANIMATION_H