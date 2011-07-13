#ifndef EGE_CORE_LOGGER_H
#define EGE_CORE_LOGGER_H

#include <EGEFile.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Logger
{
  public:

    Logger(const EGEString& filePath, bool timeStampEnabled = true);
   ~Logger();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Writes text. */
    EGEResult write(const EGEString& text);

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

#endif // EGE_CORE_LOGGER_H