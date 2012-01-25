#include "FontDataGenerator.h"
#include <iostream>
#include <algorithm>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define VERSION_MAJOR 0
#define VERSION_MINOR 1
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FontDataGenerator::FontDataGenerator(int argc, char** argv)
{
  // go thru all parameters
  for (s32 i = 0; i < argc;)
  {
    String opt = argv[i++];
    String val = (i < argc) ? argv[i] : "";

    // check if INPUT DATA switch
    if ("--input-data" == opt)
    {
      if (!val.empty())
      {
        m_inputDataFilePath = val;
        ++i;
      }
    }
    // check if INPUT texture switch
    else if ("--input-textire" == opt)
    {
      if (!val.empty())
      {
        m_inputTextureFilePath = val;
        ++i;
      }
    }
    // check if OUTPUT switch
    else if ("--output" == opt)
    {
      if (!val.empty())
      {
        m_outputXmlPath = val;
        ++i;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FontDataGenerator::~FontDataGenerator()
{
  m_atlasData = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool FontDataGenerator::isValid() const
{
  return !m_inputDataFilePath.empty() && !m_outputXmlPath.empty() && !m_inputTextureFilePath.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints syntax. */
void FontDataGenerator::printSyntax() const
{
  printHeader();

  std::cout << "Usage syntax:" << std::endl;
  std::cout << "fntdatagen.exe --input-data [filepath] --input-texture [filepath] --output [filepath]" << std::endl;
  std::cout << std::endl;
  std::cout << "--input-data    path to font XML data file." << std::endl;
  std::cout << "--input-texture path to font texture file." << std::endl;
  std::cout << "--output        output XML file path" << std::endl;
  std::cout << std::endl;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints application info header. */
void FontDataGenerator::printHeader() const
{
  std::cout << std::endl;
  std::cout << "Font Data Generator, version " << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;
  std::cout << "Albert Banaszkiewicz, Little Bee Studios Ltd., 2012" << std::endl;
  std::cout << std::endl;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes all data. */
bool FontDataGenerator::process()
{
  // print header
  printHeader();

  XmlDocument doc;
  /*
  // load input data file
  if (EGE_SUCCESS != doc.load(inputDataFilePath()))
  {
    // error!
    std::cout << "ERROR: Could not load data file!" << std::endl;
    return false;
  }

  // get root element
  PXmlElement root = doc.firstChild("resources");
  if (NULL == root || !root->isValid())
  {
    // error!
    std::cout << "ERROR: Could not locate root element (resources)!" << std::endl;
    return false;
  }

  // go thru all atlas groups
  PXmlElement groupElement = root->firstChild("atlas-group");
  while (groupElement && groupElement->isValid() && groupElement->hasAttribute("name") && groupElement->hasAttribute("texture-size") && 
         groupElement->hasAttribute("root") && groupElement->hasAttribute("texture-image"))
  {
    AtlasGroup* group = new AtlasGroup(groupElement->attribute("name"), groupElement->attribute("root"), groupElement->attribute("texture-image"), 
                                       groupElement->attribute("texture-filters", "bilinear"), groupElement->attribute("texture-size").toVector2i(), outputFormat());
    if (!group || !group->isValid())
    {
      // error!
      EGE_DELETE(group);
      std::cout << "ERROR: Could not create atlas group: " << groupElement->attribute("name") << std::endl;
      return false;
    }
    
    // add into pool
    m_groups.push_back(group);

    // go thru all IMAGE entries in current group
    PXmlElement imageElement = groupElement->firstChild("image");
    while (imageElement && imageElement->isValid())
    {
      AtlasGroupEntry* entry = new AtlasGroupEntry(imageElement->attribute("name"), imageElement->attribute("path"), imageElement->attribute("spacing").toVector4i());
      if ((NULL == entry) || !entry->isValid())
      {
        // error!
        std::cout << "ERROR: Invalid image! Name: " << imageElement->attribute("name").toAscii() << " Path: " << imageElement->attribute("path").toAscii() << " Group: " 
                  << groupElement->attribute("name").toAscii() << std::endl;
        return false;
      }

      // add to group
      group->addEntry(entry);

      // go to next child
      imageElement = imageElement->nextChild("image");
    }

    // go to next child
    groupElement = groupElement->nextChild("atlas-group");
  }

  std::cout << "DONE!" << std::endl << std::endl;
  */
  // generate atlases
  return generateAll();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates atlases. */
bool FontDataGenerator::generateAll()
{
/*  // create XML document
  m_atlasData = ege_new XmlDocument();
  if (NULL == m_atlasData || !m_atlasData->isValid())
  {
    // error!
    std::cout << "ERROR: Could not create XML document!" << std::endl;
    return false;
  }

  // add root element
  XmlElement rootElement("resources");
  m_atlasData->appendElement(rootElement);

  // process all groups one by one
  for (List<AtlasGroup*>::iterator it = m_groups.begin(); it != m_groups.end();)
  {
    AtlasGroup* group = *it;
    std::cout << "PROCESSING GROUP " << group->name() << std::endl;

    // reset sort mode
    m_sortMethod = SM_NONE;

    // generate atlas for current group
    if (!generate(group))
    {
      std::cout << "ERROR: Could not generate atlas for group: " << group->name() << ". Skipping!" << std::endl << std::endl;
    }
    else
    {
      std::cout << "SUCCESS" << std::endl << std::endl;
    }

    // remove group
    m_groups.erase(it++);
  }

  // save XML data
  return EGE_SUCCESS == m_atlasData->save(outputXmlPath());
  */
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
