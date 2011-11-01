#ifndef EGE_AIRPLAY_SOUND_PRIVATE_H
#define EGE_AIRPLAY_SOUND_PRIVATE_H

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <EGETime.h>
#include <EGEList.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sound;

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

  public
    :
    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates buffers. This is called by AudioManagerPrivate. */
    void updateBuffers();
    /*! Returns list of data buffers. */
    const BuffersList& buffers() const { return m_buffers; }
    /*! Returns TRUE if all data from codec has been read. */
    inline bool isDone() const { return m_done; }

  private:

    /*! List of data buffer filled with sound samples to be played. Buffers should be processed in the order of the list. */
    BuffersList m_buffers;
    /*! Flag indicating that all data from codec has been read. */
    bool m_done;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_SOUND_PRIVATE_H