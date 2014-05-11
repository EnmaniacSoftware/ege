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
#include "EGEAnimation.h"
#include "EGESequencer.h"
#include "EGEMatrix.h"
#include "EGERenderable.h"
#include "Core/Graphics/TextureImage.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
EGE_DECLARE_SMART_CLASS(SpriteAnimation, PSpriteAnimation)
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SpriteAnimation : public Object
                      , public IAnimation
                      , public Renderable
{
  public:

    SpriteAnimation(Application* app, const String& name);
   ~SpriteAnimation();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:
    
    /*! Signal emitted on frame change.
     *  @param sprite     SpriteAnimation object for which frame changed.
     *  @param frameIndex New frame index.
     */
    Signal2<PSpriteAnimation, s32> frameChanged;
    /*! Signal emitted when playback is finished.
     *  @param sprite     SpriteAnimation object for which playback is finished.
     */
    Signal1<PSpriteAnimation> finished;

  public:

    /*! Constructs objects. */
    EGEResult construct();

    /*! @see IAnimation::play. */
    EGEResult play(const String& sequencerName) override;
    /*! @see IAnimation::play. */
    EGEResult play(s32 sequencerIndex = 0) override;
    /*! @see IAnimation::stop. */
    void stop() override;
    /*! @see IAnimation::pause. */
    void pause() override;
    /*! @see IAnimation::isPlaying. */
    bool isPlaying() const override;
    /*! @see IAnimation::isPaused. */
    bool isPaused() const override;
    /*! @see IAnimation::isStopped. */
    bool isStopped() const override;
    /*! @see IAnimation::update. */
    void update(const Time& time) override;

    /*! @see Renderable::addForRendering. */
    EGEResult addForRendering(IRenderer& renderer) override;

    /*! Sets FPS playback value. */
    void setFPS(float32 fps);

    /*! Sets display size (in pixels). */
    void setDisplaySize(const Vector2f& size);
    /*! Returns display size (in pixels). */
    const Vector2f& displaySize() const;

    /*! Sets frame data. */
    void setFrameData(const DynamicArray<EGESprite::FrameData>& data);
    /*! Sets texture image containing sprite data. */
    void setTexture(const PTextureImage& texture);
    /*! Returns name. */
    const String& name() const { return m_name; }
    /*! Sets name. */
    void setName(const String& name);

    /*! Sets global transformation matrix. 
     *  @param  transform New global transformation matrix for animation.
     */
    void setTransformationMatrix(const Matrix4f& transform);

    /*! Sets base display alignment. 
     *  @param alignment Alignment animation is originally created for.
     *  @note  Animation if always aligned to TOP_LEFT anchor from its base alignment.
     */
    void setBaseAlignment(Alignment alignment);

    /*! Adds sequencer. */
    void addSequencer(const PSequencer& sequencer);
    /*! Returns current sequencer. */
    PSequencer currentSequencer() const;

    /*! Returns physics data component. */
    const PPhysicsComponent& physics() const { return m_physicsData; }

    /*! Sets render priority. */
    void setRenderPriority(s32 priority);

    /*! Sets transparency level. */
    void setAlpha(float32 alpha);

  private:

    /*! Returns current state. */
    State state() const { return m_state; }
    /*! Returns sequencer of a given name. */
    PSequencer sequencer(const String& name) const;
    /*! Returns texture image for current frame. */
    PTextureImage frameTexture() const;
     /*! Updates render data. */
    void updateRenderData();

  private slots:

    /*! Slot called when sequencer animated into new frame. */
    void onSequencerFrameChanged(PSequencer sequencer, s32 frameId);
    /*! Slot called when sequencer finished animation .*/
    void onSequencerFinished(PSequencer sequencer);
    /*! Slot called when internal transformation matrix is chanaged. */
    void onTransformationChanged();

  private:

    typedef DynamicArray<PSequencer> SequencerArray;

  private:

    /*! Current state. */
    State m_state;
    /*! Name. */
    String m_name;
    /*! Frame duration time. */
    Time m_frameDuration;
    /*! List of all frames in correct sequence for playback. */
    DynamicArray<EGESprite::FrameData> m_frameData;
    /*! Texture image with sprite pixel data. */
    PTextureImage m_textureImage;
    /*! Array of all sequencers. */
    SequencerArray m_sequencers;
    /*! Current sequencer. */
    PSequencer m_currentSequencer;
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