#ifndef EGE_CORE_AUDIO_AUDIOMANAGER_H
#define EGE_CORE_AUDIO_AUDIOMANAGER_H

/** This class represents public interface for AudioManager.
 */

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
extern const char* KDefaultAudioManagerName;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Sound, PSound)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IAudioManager
{
  public:

    IAudioManager() {}
    virtual ~IAudioManager() {}

  public:

    /*! Available states. */
    enum EState
    {
      StateNone = -1,
      StateInitializing,
      StateReady,
      StateClosing,
      StateClosed
    };

  public:

    /*! Creates object. */
    virtual EGEResult construct() = 0;
    /*! Updates manager. */
    virtual void update(const Time& time) = 0;
    /*! Enables/disables manager. 
     *  @note Disabling manager stops all currently played sounds and prevents any sound from being played. Enabling manager will allow sound to be played.
     *        However, it will not resume playback of any sounds stopped during last disabling request.
     */
    virtual void setEnable(bool set) = 0;
    /*! Returns TRUE if manager is enabled. */
    virtual bool isEnabled() const = 0;
    /*! Creates sound object from given data. 
     *  @param  name  Sound name.
     *  @param  data  Sound data.
     *  @return Returns created sound.
     */
    virtual PSound createSound(const String& name, PDataBuffer& data) const = 0;
    /*! Returns current state. */
    virtual EState state() const = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_AUDIOMANAGER_H