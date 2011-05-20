#ifndef EGE_CORE_LOG_H
#define EGE_CORE_LOG_H

#include "EGE.h"
#include "EGEFile.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Log
{
  public:

    Log(const EGEString& filePath, bool timeStampEnabled = true);
   ~Log();

    /* Returns TRUE if object is valid. */
    bool isValid() const;

    /* Writes text. */
    EGEResult write(const EGEString& text);
    /* Writes text with EOL. */
    EGEResult writeln(const EGEString& text);

    //void enableTimeStamps( bool bEnable );

  private:

    /* Opens log. */
    EGEResult open();
    /* Closes log. */
    void close();

  private:

    /*! Log file. */
    File m_file;
  
    bool m_timeStampEnabled;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_LOG_H