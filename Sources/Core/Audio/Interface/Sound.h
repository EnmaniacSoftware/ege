#ifndef EGE_CORE_AUDIO_SOUND_H
#define EGE_CORE_AUDIO_SOUND_H

/** This class represents public interface for Sound object.
*/

#include "EGE.h"
#include "EGEString.h"
#include "EGESignal.h"
#include "EGETime.h"
#include "EGEDataBuffer.h"
#include "EGEList.h"
#include "Core/Audio/Interface/Effects/SoundEffect.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const s32 KRepeatSoundForever = -1;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioCodec;
EGE_DECLARE_SMART_CLASS(Sound, PSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Sound : public Object
{
  public:

    virtual ~Sound();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:

    /*! Signal emitted when sound playback has come to an end. 
     *  @note Signal is NOT guaranteed to be sent from main thread.
     */
    Signal0<> finished;
    /*! Signal emitted when sound playback has come to an end. 
     *  @param sound  Sound which is finished.
     *  @note Signal is NOT guaranteed to be sent from main thread.
     */
    Signal1<PSound> finishedWithSelf;

    /*! Signal emitted when sound playback has been stopped (by any means). 
     *  @note Signal is NOT guaranteed to be sent from main thread.
     */
    Signal0<> stopped;
    /*! Signal emitted when sound playback has been stopped (by any means). 
     *  @param sound  Sound which is stopped.
     *  @note Signal is NOT guaranteed to be sent from main thread.
     */
    Signal1<PSound> stoppedWithSelf;

    /*! Signal emitted when volume changes. 
     *  @param  Sound for which volume has changed.
     *  @param  oldVolume Old volume level.
     *  @note Signal is NOT guaranteed to be sent from main thread.
     */
    Signal2<PSound, float32> volumeChanged;

  public slots:
  
    /*! Stops playback. 
     *  @note Stopping playback resets internal state of the sound ie. volume, codecs etc.
     */
    virtual void stop() = 0;

  public:

    /*! Constructs object. */
    virtual EGEResult construct() = 0;
    /*! Sets pitch value. */
    virtual void setPitch(float32 value) = 0;
    /*! Returns pitch value. */
    virtual float32 pitch() const = 0;
    /*! Sets volume. */
    virtual void setVolume(float32 volume) = 0;
    /*! Returns current volume. */
    virtual float32 volume() const = 0;
    
    /*! Starts playback. 
     *  @param repeatCount  Number of time the playback should be repeated. Negative for infinite number of repetitions.
     *  @return EGE_SUCCESS on success. EGE_ERROR_ALREADY_EXISTS is returned when sound is already being played.
     *  @note If sound was paused, repeatCount is not taken into account.
     */
    virtual EGEResult play(s32 repeatCount = 0) = 0;
    /*! Returns TRUE if sound is being played. */
    virtual bool isPlaying() const = 0;
    /*! Returns TRUE if sound is paused. */
    virtual bool isPaused() const = 0;
    /*! Pauses the playback. */
    virtual void pause() = 0;

    /*! Updates object. */
    virtual void update(const Time& time);

    /*! Attaches effect to sound. 
     *  @param  effect  Effect to attach to sound.
     *  @return TRUE if attached successfully.
     *  @note Effect is removed when finished.
     */
    bool addEffect(PSoundEffect effect);
    /*! Returns list of sound effects of a given type. 
     *  @param  uid Object ID of the effects to be returned.
     *  @return List of effects with given id.
     *  @note If EGE_OBJECT_UID_GENERIC is used all effects will be returned.
     */
    SoundEffectList effects(u32 uid) const;

    /*! Returns name. */
    const String& name() const;

  protected:

    /*! Constructor available only for actual implementations. */
    Sound(const String& name, const PDataBuffer& data);
    /*! Notifies sound has finished playback. */
    void notifyFinished();
    /*! Notifies sound has stopped playback. */
    void notifyStopped();
    /*! Notifies sound volume has changed. */
    void notifyVolumeChanged(float32 oldVolume);
    /*! Updates sound effects. */
    void updateSoundEffects(const Time& time);

  protected:

    /*! Audio codec. */
    AudioCodec* m_codec;

  private:

    /*! Name. */
    String m_name;
    /*! Data buffer. */
    PDataBuffer m_data;
    /*! List of attached effects. */
    SoundEffectList m_effects;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef List<PSound> SoundList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_SOUND_H