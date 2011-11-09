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
     * @param sound       Sound to play.
     * @param repeatCount Number of times sound should be repeated.
     * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
     * @note  When repeatCount is set to zero the sound is going to be played exactly once. For negative values sound will be played forever.
     */
    EGEResult play(const PSound& sound, s32 repeatCount = 0);
    /* Returns TRUE if sound of a given name is being played. */
    bool isPlaying(const String& soundName) const;
    /* Stops playback of the sound with a given name. 
     * @param soundName     Name of the sound to stop playback.
     * @param fadeDuration  Time interval during which sound should be faded out.
     */
    void stop(const String& soundName, const Time& fadeDuration = 0.0f);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(AudioManager);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIOMANAGER_H