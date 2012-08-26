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

    inline Debug(DebugMsgType type) : m_referenceCounter(1), m_consoleOutput(true), m_spaceSeperated(true), m_type(type) 
    {
      switch (m_type)
      {
        case DMT_WARNING:   m_buffer = "WARNING: "; break;
        case DMT_CRITICAL:  m_buffer = "CRITICAL: "; break;
      }
    }

    inline Debug(const Debug& other) : m_consoleOutput(other.m_consoleOutput), m_spaceSeperated(other.m_spaceSeperated), m_type(other.m_type)
    {
      m_buffer = other.m_buffer;
      m_referenceCounter = other.m_referenceCounter + 1;
    }

    inline ~Debug()
    {
      if (0 == (--m_referenceCounter))
      {
        if (m_consoleOutput)
        {
          Print(m_buffer.toAscii());
        }
      }
    }

    inline Debug& operator << (bool t) { m_buffer += String::Format("%s", t ? "true" : "false"); return maybeSpace(); }
    inline Debug& operator << (char t) { m_buffer += String::Format("%c", t); return maybeSpace(); }
    inline Debug& operator << (Char t) { m_buffer += String::Format("%C", t); return maybeSpace(); }
    inline Debug& operator << (s16 t) { m_buffer += String::Format("%d", t); return maybeSpace(); }
    inline Debug& operator << (u16 t) { m_buffer += String::Format("%d", t); return maybeSpace(); }
    inline Debug& operator << (s32 t) { m_buffer += String::Format("%d", t); return maybeSpace(); }
    inline Debug& operator << (u32 t) { m_buffer += String::Format("%d", t); return maybeSpace(); }
    inline Debug& operator << (s64 t) { m_buffer += String::Format("%ld", t); return maybeSpace(); }
    inline Debug& operator << (u64 t) { m_buffer += String::Format("%ld", t); return maybeSpace(); }
    inline Debug& operator << (float32 t) { m_buffer += String::Format("%f", t); return maybeSpace(); }
    inline Debug& operator << (const char* t) { m_buffer += t; return maybeSpace(); }
    inline Debug& operator << (const String& t) { m_buffer += t; return maybeSpace(); }
    inline Debug& operator << (const void* t) { m_buffer += String::Format("%p", t); return maybeSpace(); }

    inline Debug& space() { m_spaceSeperated = true; return maybeSpace(); }
    inline Debug& nospace() { m_spaceSeperated = false; return *this; }

  public:

    /* Performs assertion. */
    static void Assert(const char* assertion, const char* file, int line);

  private:

    /* Prints given c-string to output. */
    static void Print(const char* string);

  private:

    inline Debug& maybeSpace() { if (m_spaceSeperated) m_buffer += " "; return *this; }

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
#define QT_NO_QDEBUG_MACRO while (false) egeDebug

#ifndef EGE_FEATURE_DEBUG

inline Debug egeDebug() { return Debug(DMT_NORMAL); }
inline Debug egeWarning() { return Debug(DMT_WARNING); }
inline Debug egeCritical() { return Debug(DMT_CRITICAL); }

#else // EGE_FEATURE_DEBUG

#undef egeDebug
inline EGE::NoDebug egeDebug() { return EGE::NoDebug(); }
inline EGE::NoDebug egeWarning() { return EGE::NoDebug(); }
inline EGE::NoDebug egeCritical() { return EGE::NoDebug(); }
#define egeDebug QT_NO_QDEBUG_MACRO

#endif // EGE_FEATURE_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEBUG_H