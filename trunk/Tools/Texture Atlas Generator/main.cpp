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
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl << "FAILURE!" << std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    return 2;
  }

  std::cout << "SUCCESS!" << std::endl;
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------