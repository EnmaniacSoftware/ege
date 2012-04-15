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

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Renderer;
EGE_DECLARE_SMART_CLASS(ImagedAnimation, PImagedAnimation)
EGE_DECLARE_SMART_CLASS(Material, PMaterial)
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImagedAnimation : public Object
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
    Signal2<const ImagedAnimation*, s32> frameChanged;
    /*! Signal emitted when playback is finished.
     *  @param sprite     ImagedAnimation object for which playback is finished.
     */
    Signal1<const ImagedAnimation*> finished;

  public:

    /*! Available finish policy. */
    enum FinishPolicy
    {
      FP_STOP,                        /*!< ImagedAnimation stops at finish. */
      FP_REPEAT,                      /*!< ImagedAnimation repeats at finish. */
      FP_PING_PONG                    /*!< ImagedAnimation ping-pongs at finish. */
    };

  public:

    /* Updates object. */
    void update(const Time& time);
    /* Starts playing. */
    void play();
    /* Stops playing. */
    void stop();
    /* Sets playback duration. */
    void setDuration(const Time& duration);
    /*! Returns name. */
    inline const String& name() const { return m_name; }
    /* Sets name. */
    void setName(const String& name);
    /*! Returns TRUE if sprite is being played. */
    inline bool isPlaying() const { return (state() & STATE_PLAYING) ? true : false; }
    /* Sets/unsets finish policy. */
    void setFinishPolicy(FinishPolicy policy);
    /* Adds object with a given id to animation. 
     * @param objectId    Object Id which is being added.
     * @param material    Material used by the object.
     * @param baseMatrix  Object's base transformation matrix.
     * @param actions     Array of animation actions of the object. Each entry corresponds to a given frame.
     */
    EGEResult addObject(s32 objectId, PMaterial material, const Matrix4f& baseMatrix, const DynamicArray<EGEImagedAnimation::ActionData>& actions);
    /* Renders animation. */
    void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY);

  private:

    /*! Object data structure. */
    struct ObjectData
    {
      PRenderComponent renderData;                            /*!< Render data. */
      Matrix4f matrix;                                        /*!< Base transformation matrix. */
      bool visible;                                           /*!< TRUE if object is visible. */
      DynamicArray<EGEImagedAnimation::ActionData> actions;   /*!< Array of actions for all frames. */
    };

    /*! Internal state flags. */
    enum StateFlags
    {
      STATE_STOPPED = 0x00,               /*!< Playback is stopped. */
      STATE_PLAYING = 0x01                /*!< Playing. */
    };

	  EGE_DECLARE_FLAGS(State, StateFlags)

    typedef Map<s32, ObjectData> ObjectDataMap;

  private:

    /*! Returns current state. */
    inline State state() const { return m_state; }
    /* Updates objects for current frame index. */
    void updateObjects();

  private:

    /*! State flags. */
    State m_state;
    /*! Name. */
    String m_name;
    /*! Current frame index (within m_frames). */
    s32 m_frameIndex;
    /*! Frame duration time. */
    Time m_frameTime;
    /*! Current frame duration left. */
    Time m_frameTimeLeft;
    /*! Playback duration. */
    Time m_duration;
    /*! Frame count. */
    s32 m_frameCount;
    /*! Finish policy. */
    FinishPolicy m_finishPolicy;
    /*! Map of all objects [objectId, ObjectData]. */
    ObjectDataMap m_objects;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGEDANIMATION_H