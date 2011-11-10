#ifndef EGE_CORE_AUDIOMANAGER_H
#define EGE_CORE_AUDIOMANAGER_H

/** Class managing audio.
 */

#include <EGE.h>
#include <EGETime.h>
#include <EGEList.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Sound, PSound)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AudioManager : public Object
{
  public:

    AudioManager(Application* app);
   ~AudioManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates manager. */
    void update(const Time& time);
    /* Plays sound with given name. 
     * @param soundName   Name of the sound to play.
     * @param repeatCount Number of times sound should be repeated.
     * @return  Returns EGE_SUCCESS if sound is sucessfully started, EGE_ERROR_NOT_FOUND if sound could not be found or EGE_ERROR if sound could not be started.
     * @note  When repeatCount is set to zero the sound is going to be played exactly once. For negative values sound will be played forever.
     */
    EGEResult play(const String& soundName, s32 repeatCount = 0);
    /* Plays given sound.
     * @param sound Sound to play.
     * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
     */
    EGEResult play(const PSound& sound);
    /* Returns TRUE if sound of a given name is being played. */
    bool isPlaying(const String& soundName) const;
    /* Returns TRUE if given sound is being played. */
    bool isPlaying(const PSound& sound) const;
    /* Stops playback of all sounds with a given name. */
    EGEResult stop(const String& soundName);
    /* Stops playback of the given sound. */
    EGEResult stop(const PSound& sound);
    /* Returns list of sounds being played with the given name. */
    List<PSound> sounds(const String& soundName) const;

  private:

    typedef List<PSound> SoundList;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(AudioManager);

    /*! List of sounds being played. */
    SoundList m_sounds;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIOMANAGER_H