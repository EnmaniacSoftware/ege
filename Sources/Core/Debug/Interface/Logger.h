#ifndef EGE_CORE_DEBUG_LOGGER_H
#define EGE_CORE_DEBUG_LOGGER_H

/** Class for logging data to file. 
 */

#include "EGEString.h"
#include "EGEStringBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Logger
{
  public:

    Logger(const String& logFileName = "ege.log", bool timeStamp = false);
    Logger(const Logger& other);
   ~Logger();

  operators:

    Logger& operator << (bool t);
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
    Logger& operator = (const Logger& other);
    
  public:

    /*! Enables spaces insertions after each logged message. */
    Logger& space();
    /*! Disables spaces insertions after each logged message. */    
    Logger& nospace();

  protected:

    /*! Returns data buffer. */
    const PStringBuffer& buffer() const;

  private:

    /*! Inserts space into the stream if required. */
    Logger& maybeSpace();
    /*! Writes current buffer to file. */
    void write();

  private:

    /*! File name. */
    String m_fileName;
    /*! Internal buffer. */
    PStringBuffer m_buffer;
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
  inline NoLogger egeLog() { return NoLogger(); }

  #define egeLog EGE_NO_LOG_MACRO

#endif // EGE_FEATURE_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEBUG_LOGGER_H