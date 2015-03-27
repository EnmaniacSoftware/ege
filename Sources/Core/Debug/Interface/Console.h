#ifndef EGE_CORE_CONSOLE_H
#define EGE_CORE_CONSOLE_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Engine;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Console : public Object
{
  public:

    Console(Engine& engine);
    virtual ~Console();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  private:

};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONSOLE_H