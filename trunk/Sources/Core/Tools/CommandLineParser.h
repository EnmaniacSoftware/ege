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
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TOOLS_COMMANDLINEPARSERGENERIC_H