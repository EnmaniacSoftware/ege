#ifndef EGE_CORE_SOUND_H
#define EGE_CORE_SOUND_H

/** Objects of this class represents discreet sound effect.
*/

#include <EGE.h>
#include <EGEDataBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Sound, PSound)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sound : public Object
{
  public:

    Sound(const PDataBuffer& data);
    virtual ~Sound();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Sound);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOUND_H