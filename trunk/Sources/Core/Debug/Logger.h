#ifndef EGE_CORE_LOGGER_H
#define EGE_CORE_LOGGER_H

#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Logger
{
  public:

    Logger(const String& logFileName = "ege.log", bool timeStamp = false);
    Logger(const Logger& other);
   ~Logger();

    Logger& operator << (bool t);
    Logger& operator << (char t);
    Logger& operator << (Char t);
    Logger& operator << (s16 t);
    Logger& operator << (u16 t);
    Logger& operator << (s32 t);
    Logger& operator << (u32 t);
    Logger& operator << (s64 t);
    Logger& operator << (u64 t);
    //Logger& operator << (size_t t);
    Logger& operator << (float32 t);
    Logger& operator << (const char* t);
    Logger& operator << (const String& t);
    Logger& operator << (const void* t);

    /*! Enables spaces insertions after each logged message. */
    Logger& space();
    /*! Disables spaces insertions after each logged message. */    
    Logger& nospace();

  private:

    /*! Inserts space into the stream if required. */
    Logger& maybeSpace();
    /*! Writes current buffer to file. */
    void write();

  private:

    /*! File name. */
    String m_fileName;
    /*! Internal buffer. */
    String m_buffer;
    /*! Reference counter. */
    s32 m_referenceCounter;
    /*! Flag indicating if space should be added after each partial print. */
    bool m_spaceSeperated;
    /*! TRUE if time stamps should be generated. */
    bool m_timeStampEnabled;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class NoLogger
{
  public:

    NoLogger() {}
    NoLogger(const Logger&) {}
   ~NoLogger() {}

    NoLogger& space() { return *this; }
    NoLogger& nospace() { return *this; }

    template<typename T>
    inline NoLogger& operator << (const T&) { return *this; }
}; 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EGE_NO_LOG_MACRO while (false) egeLog

#ifdef EGE_FEATURE_DEBUG

inline Logger egeLog() { return Logger(); }

#else // EGE_FEATURE_DEBUG

#undef egeLog
inline EGE::NoLogger egeLog() { return EGE::NoLogger(); }
#define egeLog EGE_NO_LOG_MACRO

#endif // EGE_FEATURE_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_LOGGER_H