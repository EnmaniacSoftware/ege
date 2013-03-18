#ifndef EGE_CORE_TOOLS_COMMANDLINEPARSERGENERIC_H
#define EGE_CORE_TOOLS_COMMANDLINEPARSERGENERIC_H

#include "EGE.h"
#include "EGEDictionary.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CommandLineParser
{
  public:
    
    explicit CommandLineParser(char* commandLine);
    CommandLineParser(int argc, char** argv);
   ~CommandLineParser();

    /*! Returns dictionary with command line parameters. */
    const Dictionary& dictionary() const;

  private:

    /*! Parses the command line and generates dictionary. */
    void parse(const String& commandLine);
    /*! Returns the position of next switch token
     *  @param  commandLine String with entire command line.
     *  @param  pos         Position within commandline from which the search should be done.
     *  @return Position within commandline where the token has been found. Returns String::npos if nothing was found.
     */
    size_t findNextSwitchToken(const String& commandLine, size_t pos);
    /*! Retrieves the name from the command line.
     *  @param  commandLine String with entire command line.
     *  @param  pos         Position within commandline from which name should be retrieved.
     *  @return Name found at given location.
     *  @note Name is the first n-chracters found before SPACE, EQUAL appears or end of commanline is reached.
     */
    String retrieveName(const String& commandLine, size_t pos);

  private:

    /*! Parameters. */
    Dictionary m_values;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TOOLS_COMMANDLINEPARSERGENERIC_H