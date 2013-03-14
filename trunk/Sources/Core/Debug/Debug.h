#ifndef EGE_CORE_DEBUG_H
#define EGE_CORE_DEBUG_H

/** This object represents debug root for framework. Most debugging tools and functionalities have root in this class. 
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Debug
{
  public:

    Debug(DebugMsgType type);
    Debug(const Debug& other);
   ~Debug();

    Debug& operator << (bool t);
    Debug& operator << (char t);
    Debug& operator << (Char t);
    Debug& operator << (s16 t);
    Debug& operator << (u16 t);
    Debug& operator << (s32 t);
    Debug& operator << (u32 t);
    Debug& operator << (s64 t);
    Debug& operator << (u64 t);
    Debug& operator << (float32 t);
    Debug& operator << (const char* t);
    Debug& operator << (const String& t);
    Debug& operator << (const void* t);

    /*! Enables spaces insertions after each logged message. */
    Debug& space();
    /*! Disables spaces insertions after each logged message. */    
    Debug& nospace();

  public:

    /*! Performs assertion. */
    static void Assert(const char* assertion, const char* file, int line);

  private:

    /*! Prints given c-string to output. */
    static void Print(const char* string);

  private:

    /*! Inserts space into the stream if required. */
    Debug& maybeSpace();

  private:

    /*! Internal buffer. */
    String m_buffer;
    /*! Reference counter. */
    int m_referenceCounter;
    /*! Flag indicating output should be directed to console. */
    bool m_consoleOutput;
    /*! Flag indicating if space should be added after each partial print. */
    bool m_spaceSeperated;
    /*! Message type. */
    DebugMsgType m_type;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class NoDebug
{
  public:

    inline NoDebug() {}
    inline NoDebug(const Debug&) {}
    inline ~NoDebug() {}

    inline NoDebug& space() { return *this; }
    inline NoDebug& nospace() { return *this; }

    template<typename T>
    inline NoDebug& operator << (const T&) { return *this; }
}; 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EGE_NO_DEBUG_MACRO while (false) egeDebug

#ifdef EGE_FEATURE_DEBUG

inline Debug egeDebug() { return Debug(DMT_NORMAL); }
inline Debug egeWarning() { return Debug(DMT_WARNING); }
inline Debug egeCritical() { return Debug(DMT_CRITICAL); }

#else // EGE_FEATURE_DEBUG

#undef egeDebug
inline EGE::NoDebug egeDebug() { return EGE::NoDebug(); }
inline EGE::NoDebug egeWarning() { return EGE::NoDebug(); }
inline EGE::NoDebug egeCritical() { return EGE::NoDebug(); }
#define egeDebug EGE_NO_DEBUG_MACRO

#endif // EGE_FEATURE_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEBUG_H