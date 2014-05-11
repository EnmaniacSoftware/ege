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
#include "EGEAnimation.h"
#include "EGERenderable.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
EGE_DECLARE_SMART_CLASS(ImagedAnimation, PImagedAnimation)
EGE_DECLARE_SMART_CLASS(Material, PMaterial)
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
EGE_DECLARE_SMART_CLASS(Sequencer, PSequencer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImagedAnimation : public Object
                      , public IAnimation
                      , public Renderable
{
  public:

    ImagedAnimation();
    ImagedAnimation(Application* app, const String& name);
   ~ImagedAnimation();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:
    
    /*! Signal emitted on frame change.
     *  @param sprite     ImagedAnimation object for which frame changed.
     *  @param frameIndex New frame index.
     */
    Signal2<PImagedAnimation, s32> frameChanged;
    /*! Signal emitted when playback is finished.
     *  @param sprite     ImagedAnimation object for which playback is finished.
     */
    Signal1<PImagedAnimation> finished;

  public:

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

    /*! Sets global transformation matrix. 
     *  @param  transform New global transformation matrix for animation.
     */
    void setTransformationMatrix(const Matrix4f& transform);

    /*! Sets FPS playback value. */
    void setFPS(float32 fps);
    
    /*! Sets display size (in pixels). */
    void setDisplaySize(const Vector2f& size);
    /*! Returns display size (in pixels). */
    const Vector2f& displaySize() const;

    /*! Sets base display alignment. 
     *  @param alignment Alignment animation is originally created for.
     *  @note  Animation if always aligned to TOP_LEFT anchor from its base alignment.
     */
    void setBaseAlignment(Alignment alignment);
    /*! Returns name. */
    const String& name() const;
    /*! Sets name. */
    void setName(const String& name);

    /*! Adds animation data.
     *  @param  objects       Map of all unique objects animation is built from.
     *  @param  frameActions  List of actions for each each frame of animation.
     */
    void addData(const EGEImagedAnimation::ObjectMap& objects, const List<EGEImagedAnimation::ActionDataList>& frameActions);

    /*! Adds sequencer. */
    void addSequencer(const PSequencer& sequencer);
    /*! Returns current sequencer. */
    PSequencer currentSequencer() const;

    /*! Sets render priority. */
    void setRenderPriority(s32 priority);
    /*! Clears object. */
    void clear();

    /*! Sets transparency level. */
    void setAlpha(float32 alpha);

  private:

    typedef DynamicArray<PSequencer> SequencerArray;
    typedef DynamicArray<PRenderComponent> RenderComponentArray;
    typedef List<EGEImagedAnimation::ActionDataList> ActionDataList;

  private:

    /*! Returns current state. */
    State state() const { return m_state; }
    /*! Returns sequencer of a given name. */
    PSequencer sequencer(const String& name) const;
    /*! Calculates number of vertices required for a given render frame. */
    u32 calculateFrameVertexCount(const EGEImagedAnimation::ObjectMap& objects, const EGEImagedAnimation::ActionDataList& frameActionList) const;
    /*! Updates render data. */
    void updateRenderData();

  private slots:

    /*! Slot called when sequencer animated into new frame. */
    void onSequencerFrameChanged(PSequencer sequencer, s32 frameId);
    /*! Slot called when sequencer finished animation .*/
    void onSequencerFinished(PSequencer sequencer);

  private:

    /*! Current state. */
    State m_state;
    /*! Name. */
    String m_name;
    /*! Frame duration. */
    Time m_frameDuration;
    /*! Base render priority. */
    s32 m_renderPriority; 
    /*! Display size (in pixels). */
    Vector2f m_displaySize;
    /*! Base display alignment. */
    Alignment m_baseAlignment;
    /*! Array of all sequencers. */
    SequencerArray m_sequencers;
    /*! Current sequencer. */
    PSequencer m_currentSequencer;
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