#ifndef EGE_CORE_TOOLS_COMMANDLINEPARSERGENERIC_H
#define EGE_CORE_TOOLS_COMMANDLINEPARSERGENERIC_H

#include <EGE.h>
#include <EGEDictionary.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CommandLineParser
{
  public:
    
    CommandLineParser(int argc, char** argv);
   ~CommandLineParser();

    /*! Returns dictionary with command line parameters. */
    const Dictionary& dictionary() const;

  private:

    /*! Parses the command line and generates dictionary. */
    void parse(const String& commandLine);

  private:

    /*! Parameters. */
    Dictionary m_values;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TOOLS_COMMANDLINEPARSERGENERIC_H