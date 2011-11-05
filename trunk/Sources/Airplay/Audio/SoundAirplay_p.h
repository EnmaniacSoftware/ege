#ifndef EGE_AIRPLAY_SOUND_PRIVATE_H
#define EGE_AIRPLAY_SOUND_PRIVATE_H

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <EGETime.h>
#include <EGEList.h>
#include <EGEDynamicArray.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sound;
class AudioManagerPrivate;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SoundPrivate
{
  public:

    SoundPrivate(Sound* base);
   ~SoundPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Sound)

  public:

    typedef List<PDataBuffer> BuffersList;

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates buffers. This is called by AudioManagerPrivate. */
    void updateBuffers();
    /*! Returns list of data buffers. */
    const BuffersList& buffers() const { return m_buffers; }
    /*! Returns TRUE if all data from codec has been read. */
    inline bool isDone() const { return m_done; }
    /* Locks buffers. */
    void lockBuffers();
    /* Unlocks buffers. */
    void unlockBuffers();
    /*! Returns TRUE if buffers are locked. */
    inline bool areBuffersLocked() const { return m_buffersLocked; }
    /*! Returns keep alive timer. */
    Time& keepAliveTimer() { return m_keepAlive; }
    /* Initializes audio filter. */
    void initializeFilter();
    /* Sets pointer to private implementation of audio manager through which playback is being done. */
    void setAudioManagerPrivate(AudioManagerPrivate* manager);
    /*! Returns pointer to private implementation of audio manager through which playback is being done. */
    AudioManagerPrivate* audioManager() const { return m_audioManager; }

  private:

    /*! List of data buffer filled with sound samples to be played. Buffers should be processed in the order of the list. */
    BuffersList m_buffers;
    /*! Flag indicating that all data from codec has been read. */
    bool m_done;
    /*! Flag indicating that data buffers are locked for writting. */
    bool m_buffersLocked;
    /*! Keep alive timer. It is started after sound has ended. After predefined keep-alive period sound will be automatically removed. */
    Time m_keepAlive;
    /*! Pointer to private implementation of audio manager through which playback is being done. */
    AudioManagerPrivate* m_audioManager;

public:

    DynamicArray<s16> m_filterBuffer;
    DynamicArray<float32> m_filterCoefficients;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_SOUND_PRIVATE_H