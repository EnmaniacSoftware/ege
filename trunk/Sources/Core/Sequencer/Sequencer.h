#ifndef EGE_CORE_SEQUENCER_H
#define EGE_CORE_SEQUENCER_H

/** Sequencer is an animation object which goes through all defined frame indicies.
    Sequencer emits signal every time new frame is reached. Additional signal is emitted when last frame is reached any no more repeats are to be done.
 */

#include <EGE.h>
#include <EGETime.h>
#include <EGEDynamicArray.h>
#include <EGESignal.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Sequencer, PSequencer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Sequencer : public Object
{
  public:

    Sequencer();
   ~Sequencer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:
    
    /*! Signal emitted when frame changes.
     *  @param  sequencer   Pointer to sequencer object which emitted signal.
     *  @param  frameId     Id of the current (new) frame.
     */
    Signal2<PSequencer, s32> frameChanged;
    /*! Signal emitted when sequence is finished. 
     *  @param  sequencer   Pointer to sequencer object which emitted signal.
     */
    Signal1<PSequencer> finished;

  public:

    /* Starts sequencer. */
    void start();
    /* Updates sequencer. */
    void update(const Time& time);

    /* Sets frame duration. */
    void setFrameDuration(const Time& time);
    /* Sets frame data. */
    void setFrameData(const IntArray& frameIds);
    /* Sets repeat flag. */
    void setRepeatable(bool set);

    /* Returns given frame id. */
    s32 frameId(s32 frameIndex) const;
    /* Returns current frame index. */
    inline s32 currentFrameIndex() const { return m_frameIndex; }
    /* Returns number of frames. */
    inline s32 frameCount() const { return static_cast<s32>(m_framesIds.size()); }

    /* Sets name. */
    void setName(const String& name);
    /*! Returns name. */
    inline const String& name() const { return m_name; }

    /* Returns normalized position between current and next frame. */
    float32 normalizedFrameTime() const;

  private:

    /*! Name. */
    String m_name;
    /*! Frame duration. */
    Time m_frameDuration;
    /*! Current frame time left. */
    Time m_frameTimeLeft;
    /*! Array of all frame ids. */
    IntArray m_framesIds;
    /*! Current frame. This defines position within the array of frames. */
    s32 m_frameIndex;
    /*! Repeat flag. */
    bool m_repeatable;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SEQUENCER_H