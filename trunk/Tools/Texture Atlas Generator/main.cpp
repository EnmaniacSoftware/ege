#include "TextureAtlasGenerator.h"
#include <iostream>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{ 
  TextureAtlasGenerator generator(argc, argv);

  // check if NOT valid
  if (!generator.isValid())
  {
    // show syntax
    generator.printSyntax();
    system("pause");
    return 1;
  }

  // process data
  if (!generator.process())
  {
    // error!
    system("pause");
    return 2;
  }

  system("pause");
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------