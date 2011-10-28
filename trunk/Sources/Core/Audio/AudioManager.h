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
    /* Plays sound with given name. */
    EGEResult play(const String& soundName);
    /* Plays given sound. */
    EGEResult play(const PSound& sound);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(AudioManager);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIOMANAGER_H