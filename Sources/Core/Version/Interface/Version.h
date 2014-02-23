#ifndef EGE_CORE_VERSION_VERSION_H
#define EGE_CORE_VERSION_VERSION_H

/*! Class encapsulating version information. */

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Version
{
  public:

    Version(s32 major, s32 minor, s32 revision);
   ~Version();

  public:

    /*! Returns major part number. */
    s32 major() const;
    /*! Returns minor part number. */
    s32 minor() const;
    /*! Returns revision part number. */
    s32 revision() const;
    /*! Returns full version number in string format. 
     *  @note Returned version is in the following format: Major.Minor.Revision.
     */
    String toString() const;

  private:

    /*! Major number. */
    s32 m_major;
    /*! Minor number. */
    s32 m_minor;
    /*! Revision number. */
    s32 m_revision;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VERSION_VERSION_H