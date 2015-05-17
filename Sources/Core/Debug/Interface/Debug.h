#ifndef EGE_CORE_DEBUG_DEBUG_H
#define EGE_CORE_DEBUG_DEBUG_H

/** This object represents debug root for framework. Most debugging tools and functionalities have root in this class. 
 */

#include "EGEStringBuffer.h"
#include "EGEString.h"
#include "EGEStringList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available debug message types. */
enum DebugMessageType
{
  ENormal,               /*!< Normal debug message. Usually, informative. */
  EWarning,              /*!< Warning debug message. Usually, for recoverable issues. */
  EError                 /*!< Error debug message. Usually, for unrecoverable issues. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Debug
{
  public:

    Debug(DebugMessageType type, const String& name);
    Debug(const Debug& other);
   ~Debug();
  
  operators:

    Debug& operator << (bool t);
    Debug& operator << (s16 t);
    Debug& operator << (u16 t);
    Debug& operator << (s32 t);
    Debug& operator << (u32 t);
    Debug& operator << (s64 t);
    Debug& operator << (u64 t);
   // Debug& operator << (size_t t);
    Debug& operator << (float32 t);
    Debug& operator << (const char* t);
    Debug& operator << (const String& t);
    Debug& operator << (const void* t);
    Debug& operator = (const Debug& other);

  public:

    /*! Enables spaces insertions after each logged message. */
    Debug& space();
    /*! Disables spaces insertions after each logged message. */    
    Debug& nospace();

  public:

    /*! Prints given c-string to output. */
    static void Print(const char* string);
    /*! Prints given c-string to output. */
    static void PrintWithArgs(const char* string, ...);
    /*! Enables given debug names. 
     *  @param names  Debug names to enable. All debugging with these names will be visible.
     */
    static void EnableNames(const StringList& names);

  protected:

    /*! Returns data buffer. */
    const PStringBuffer& buffer() const;

  protected:

    /*! Warning message type prefix. */
    static const char* KWarningPrefix;
    /*! Error message type prefix. */
    static const char* KErrorPrefix;

  private:

    /*! Inserts space into the stream if required. */
    Debug& maybeSpace();

  private:

    /*! Internal buffer. */
    PStringBuffer m_buffer;
    /*! Flag indicating if space should be added after each partial print. */
    bool m_spaceSeperated;
    /*! Message type. */
    DebugMessageType m_type;
    /*! Enable flag. Only if set debug will be output. */
    bool m_enabled;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class NoDebug
{
  public:

    NoDebug() {}
    NoDebug(const Debug&) {}
   ~NoDebug() {}

    NoDebug& space() { return *this; }
    NoDebug& nospace() { return *this; }

    template<typename T>
    NoDebug& operator << (const T&) { return *this; }
}; 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#if EGE_FEATURE_DEBUG

  inline Debug egeDebug(const String& name) { return Debug(ENormal, name); }
  inline Debug egeWarning(const String& name) { return Debug(EWarning, name); }
  inline Debug egeCritical(const String& name) { return Debug(EError, name); }

#else

  #define EGE_NO_DEBUG_MACRO while (false) egeNoDebug

  inline NoDebug egeNoDebug(const String& name) { return NoDebug(); }

  #define egeDebug EGE_NO_DEBUG_MACRO
  #define egeWarning EGE_NO_DEBUG_MACRO
  #define egeCritical EGE_NO_DEBUG_MACRO

#endif // EGE_FEATURE_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEBUG_DEBUG_H