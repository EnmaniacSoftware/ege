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
    std::cout << "Failure!" << std::endl;
    system("pause");
    return 2;
  }

  std::cout << "Success!" << std::endl;
  system("pause");
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------