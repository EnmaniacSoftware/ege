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
    return 1;
  }

  // process data
  if (!generator.process())
  {
    // error!
    std::cout << "Failure!" << std::endl;
    return 2;
  }

  std::cout << "Success!" << std::endl;
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------