#ifndef EGE_CORE_SOUNDMANAGER_H
#define EGE_CORE_SOUNDMANAGER_H

/** Class managing sounds and music.
 */

#include <EGE.h>
#include <EGETime.h>
#include <EGEList.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SoundManager : public Object
{
  public:

    SoundManager(Application* app);
   ~SoundManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Updates manager. */
    void update(const Time& time);

  private:

//    typedef List<PScreen> ScreenList;

  private:

    /*! List of screens. Sorted from the bottom one to top one (currently visible). */
  //  ScreenList m_screens;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOUNDMANAGER_H