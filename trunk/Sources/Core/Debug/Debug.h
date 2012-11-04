#ifndef EGE_CORE_DEBUG_H
#define EGE_CORE_DEBUG_H

/** This object represents debug root for framework. Most debugging tools and functionalities have root in this class. 
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Debug
{
  public:

    Debug(DebugMsgType type) : m_referenceCounter(1), m_consoleOutput(true), m_spaceSeperated(true), m_type(type) 
    {
      switch (m_type)
      {
        case DMT_WARNING:   m_buffer = "WARNING: "; break;
        case DMT_CRITICAL:  m_buffer = "CRITICAL: "; break;
      }
    }

    Debug(const Debug& other) : m_consoleOutput(other.m_consoleOutput), m_spaceSeperated(other.m_spaceSeperated), m_type(other.m_type)
    {
      m_buffer = other.m_buffer;
      m_referenceCounter = other.m_referenceCounter + 1;
    }

    ~Debug()
    {
      if (0 == (--m_referenceCounter))
      {
        if (m_consoleOutput)
        {
          Print(m_buffer.toAscii());
        }
      }
    }

    Debug& operator << (bool t) { m_buffer += String::Format("%s", t ? "true" : "false"); return maybeSpace(); }
    Debug& operator << (char t) { m_buffer += String::Format("%c", t); return maybeSpace(); }
    Debug& operator << (Char t) { m_buffer += String::Format("%C", t); return maybeSpace(); }
    Debug& operator << (s16 t) { m_buffer += String::Format("%d", t); return maybeSpace(); }
    Debug& operator << (u16 t) { m_buffer += String::Format("%d", t); return maybeSpace(); }
    Debug& operator << (s32 t) { m_buffer += String::Format("%d", t); return maybeSpace(); }
    Debug& operator << (u32 t) { m_buffer += String::Format("%d", t); return maybeSpace(); }
    Debug& operator << (s64 t) { m_buffer += String::Format("%d", (s32)t); return maybeSpace(); }
    Debug& operator << (u64 t) { m_buffer += String::Format("%d", (u64)t); return maybeSpace(); }
    Debug& operator << (float32 t) { m_buffer += String::Format("%f", t); return maybeSpace(); }
    Debug& operator << (const char* t) { m_buffer += t; return maybeSpace(); }
    Debug& operator << (const String& t) { m_buffer += t; return maybeSpace(); }
    Debug& operator << (const void* t) { m_buffer += String::Format("%p", t); return maybeSpace(); }

    Debug& space() { m_spaceSeperated = true; return maybeSpace(); }
    Debug& nospace() { m_spaceSeperated = false; return *this; }

  public:

    /* Performs assertion. */
    static void Assert(const char* assertion, const char* file, int line);

  private:

    /* Prints given c-string to output. */
    static void Print(const char* string);

  private:

    Debug& maybeSpace() { if (m_spaceSeperated) m_buffer += " "; return *this; }

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