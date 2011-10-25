#ifndef EGE_CORE_AUDIOMANAGER_PRIVATE_H
#define EGE_CORE_AUDIOMANAGER_PRIVATE_H

#include <EGE.h>
#include <EGETime.h>
#include <EGEDynamicArray.h>
#include <al/al.h>
#include <al/alc.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AudioManager;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AudioManagerPrivate
{
  public:

    AudioManagerPrivate(AudioManager* base);
   ~AudioManagerPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates manager. */
    void update(const Time& time);

  private:

    typedef DynamicArray<ALuint> ChannelsArray;

  private:

    /*! Audio device. */
    ALCdevice* m_device;
    /*! Audio context. */
    ALCcontext* m_context;
    /*! Available channels. */
    ChannelsArray m_channels;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIOMANAGER_PRIVATE_H