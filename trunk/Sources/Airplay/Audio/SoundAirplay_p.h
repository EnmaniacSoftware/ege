#ifndef EGE_AIRPLAY_SOUND_PRIVATE_H
#define EGE_AIRPLAY_SOUND_PRIVATE_H

#include <EGE.h>
#include <EGEDataBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sound;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SoundPrivate
{
  public:

    SoundPrivate(Sound* base, const PDataBuffer& data);
   ~SoundPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Sound)

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns sound data buffer. */
    inline const PDataBuffer& data() const { return m_data; }
    /*! Returns sample rate. */
    inline s32 sampleRate() const { return m_sampleRate; }
    /*! Returns sound data offset (in bytes). */
    inline s32 soundDataOffset() const { return m_soundDataOffset; }
    /*! Returns sound data size (in bytes). */
    inline s32 soundDataSize() const { return m_soundDataSize; }
    /*! Returns number of channels. */
    inline s32 channels() const { return m_channels; }
    /*! Returns number of all samples. */
    inline s32 samplesCount() const { return m_samplesCount; }

    /*! Returns number of played samples. */
    inline s32 samplesPlayed() const { return m_samplesPlayed; }
    /* Sets number of played samples. */
    void setSamplesPlayed(s32 count);

  private:

    /*! Sound data buffer. */
    PDataBuffer m_data;
    /*! Number of channels in the sample. */
    s32 m_channels;
    /*! Sample rate of sound (in Hz). */
    s32 m_sampleRate;
    /*! Number of bits per sample. */
    s32 m_bitsPerSample;
    /*! Offset to sound data within data buffer (in bytes). */
    s32 m_soundDataOffset;
    /*! Sound data size (in bytes). */
    s32 m_soundDataSize;
    /*! Number of samples played already. */
    s32 m_samplesPlayed;
    /*! Number of all samples. */
    s32 m_samplesCount;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_SOUND_PRIVATE_H