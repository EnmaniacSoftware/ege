#ifndef EGE_CORE_IMAGEDANIMATION_H
#define EGE_CORE_IMAGEDANIMATION_H

/** Imaged animation objects represent discreet instances of image-based animations. These represent set of images being animated (transformed) during the 
 *  lifetime. 
 */

#include <EGE.h>
#include <EGETime.h>
#include <EGEFlags.h>
#include <EGEDynamicArray.h>
#include <EGESignal.h>
#include <EGEImagedAnimation.h>
#include <EGEMatrix.h>
#include <EGEMap.h>
#include <EGEGraphics.h>
#include <EGEVector.h>
#include <EGEAnimation.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Renderer;
EGE_DECLARE_SMART_CLASS(ImagedAnimation, PImagedAnimation)
EGE_DECLARE_SMART_CLASS(Material, PMaterial)
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
EGE_DECLARE_SMART_CLASS(Sequencer, PSequencer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImagedAnimation : public Object, public IAnimation
{
  public:

    ImagedAnimation();
    ImagedAnimation(Application* app, const String& name);
   ~ImagedAnimation();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:
    
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

    /* IAnimation override. Starts playback with a given sequencer. 
     * @param sequencerName  Name of the sequencer to use for playback.
     * @note If animation for given sequencer is was paused it will be resumed. Otherwise, animation will be started from the begining.
     */
    EGEResult play(const String& sequencerName) override;
    /* IAnimation override. Starts playback with a given sequencer. 
     * @param sequencerIndex Index of the sequencer to use for playback. Negative value replays last sequence if available.
     * @note If animation for given sequencer is was paused it will be resumed. Otherwise, animation will be started from the begining.
     */
    EGEResult play(s32 sequencerIndex = 0) override;
    /* IAnimation override. Stops playback. */
    void stop() override;
    /* IAnimation override. Pauses playback. */
    void pause() override;
    /* IAnimation override. Returns TRUE if animation is being played. */
    bool isPlaying() const override;
    /* IAnimation override. Returns TRUE if animation is paused. */
    bool isPaused() const override;
    /* IAnimation override. Returns TRUE if animation is stopped. */
    bool isStopped() const override;
    /* IAnimation override. Updates animation. */
    void update(const Time& time) override;

    /* Sets frame duration. */
    void setFrameDuration(const Time& duration);
    /* Sets display size. */
    void setDisplaySize(const Vector2f& size);
    /* Sets base display alignment. 
     * @param alignment Alignment animation is originally created for.
     * @note  Animation if always aligned to TOP_LEFT anchor from its base alignment.
     */
    void setBaseAlignment(Alignment alignment);
    /*! Returns name. */
    inline const String& name() const { return m_name; }
    /* Sets name. */
    void setName(const String& name);

    /* Adds object with a given id to animation. 
     * @param objectId    Object Id which is being added.
     * @param size        Object size (in pixels).
     * @param material    Material used by the object.
     * @param baseMatrix  Object's base transformation matrix.
     * @param actions     Array of animation actions of the object. Each entry corresponds to a given frame.
     */
    EGEResult addObject(s32 objectId, const Vector2f& size, PMaterial material, const Matrix4f& baseMatrix);
    /* Adds frame data.
     * @param action  List of action to be processed at given frame.
     * @note  This creates new frame and appends it into existing ones.
     */
    EGEResult addFrameData(const List<EGEImagedAnimation::ActionData>& actions);
    
    /* Adds sequencer. */
    void addSequencer(const PSequencer& sequencer);
    /* Returns current sequencer. */
    PSequencer currentSequencer() const;

    /* Renders animation. */
    void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY);
    /* Sets base render priority. */
    void setBaseRenderPriority(s32 priority);
    /* Clears object. */
    void clear();

  private:

    /*! Object data structure. */
    struct ObjectData
    {
      PRenderComponent renderData;                            /*!< Render data. */
      Matrix4f baseMatrix;                                    /*!< Base transformation matrix. */
      Matrix4f baseFrameMatrix;                               /*!< Combined base and frame transformations matrix. */
    };

    /*! Frame data struct. */
    struct FrameData
    {
      List<EGEImagedAnimation::ActionData> actions;           /*!< Array of actions for all frames. */
    };

    typedef Map<s32, ObjectData> ObjectDataMap;
    typedef DynamicArray<FrameData> FrameDataArray;
    typedef DynamicArray<PSequencer> SequencerArray;

  private:

    /*! Returns current state. */
    inline State state() const { return m_state; }
    /* Returns sequencer of a given name. */
    PSequencer sequencer(const String& name) const;
    
  private slots:

    /* Slot called when sequencer animated into new frame. */
    void onSequencerFrameChanged(PSequencer sequencer, s32 frameId);
    /* Slot called when sequencer finished animation .*/
    void onSequencerFinished(PSequencer sequencer);

  private:

    /*! Current state. */
    State m_state;
    /*! Name. */
    String m_name;
    /*! Frame duration. */
    Time m_frameDuration;
    /*! Map of all objects [objectId, ObjectData]. */
    ObjectDataMap m_objects;
    /*! Base render priority. */
    s32 m_baseRenderPriority; 
    /*! Array of frame data. */
    FrameDataArray m_frames;
    /*! Display size. */
    Vector2f m_displaySize;
    /*! Base display alignment. */
    Alignment m_baseAlignment;
    /*! Array of all sequencers. */
    SequencerArray m_sequencers;
    /*! Current sequencer. */
    PSequencer m_currentSequencer;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGEDANIMATION_H