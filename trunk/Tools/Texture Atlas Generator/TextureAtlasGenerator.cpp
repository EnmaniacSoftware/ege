#include "TextureAtlasGenerator.h"
#include "AtlasGroupEntry.h"
#include <iostream>
#include <algorithm>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define VERSION_MAJOR 0
#define VERSION_MINOR 8
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping image format name into framework enum. */
static EGEImage::Format MapImageFormat(const String& formatName)
{
  if ("rgb" == formatName)
  {
    return EGEImage::RGB_888;
  }

  // default
  return EGEImage::RGBA_8888;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function for sorting object from greatest area to smallest. */
static bool SortGreaterArea(AtlasGroupEntry* left, AtlasGroupEntry* right)
{
  return left->image()->width() * left->image()->height() > right->image()->width() * right->image()->height();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function for sorting object from greatest width to smallest. */
static bool SortGreaterWidth(AtlasGroupEntry* left, AtlasGroupEntry* right)
{
  return left->image()->width() > right->image()->width();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function for sorting object from greatest height to smallest. */
static bool SortGreaterHeight(AtlasGroupEntry* left, AtlasGroupEntry* right)
{
  return left->image()->height() > right->image()->height();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAtlasGenerator::TextureAtlasGenerator(int argc, char** argv) : m_outputFormat(EGEImage::NONE), m_sortMethod(SM_NONE)
{
  // go thru all parameters
  for (s32 i = 0; i < argc;)
  {
    String opt = argv[i++];
    String val = (i < argc) ? argv[i] : "";

    // check if FORMAT switch
    if ("-format" == opt)
    {
      if (!val.empty())
      {
        m_outputFormat = MapImageFormat(val);
        ++i;
      }
    }
    // check if INPUT switch
    else if ("-input" == opt)
    {
      if (!val.empty())
      {
        m_inputDataFilePath = val;
        ++i;
      }
    }
    // check if OUTPUT switch
    else if ("-output" == opt)
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
TextureAtlasGenerator::~TextureAtlasGenerator()
{
  // go thru all groups
  for (List<AtlasGroup*>::iterator it = m_groups.begin(); it != m_groups.end();)
  {
    EGE_DELETE(*it);
    m_groups.erase(it++);
  }

  m_atlasData = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool TextureAtlasGenerator::isValid() const
{
  return (EGEImage::NONE != m_outputFormat) && !m_inputDataFilePath.empty() && !m_outputXmlPath.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints syntax. */
void TextureAtlasGenerator::printSyntax() const
{
  printHeader();

  std::cout << "Usage syntax:" << std::endl;
  std::cout << "tatlasgen.exe -format [rgba|rgb] -input [filepath] -output [filepath]" << std::endl;
  std::cout << std::endl;
  std::cout << "-format     pixel format of output image(s). Valid values: rgba, rgb" << std::endl;
  std::cout << "-input      path to XML input file containing" << std::endl;
  std::cout << "-output     output XML file path" << std::endl;
  std::cout << std::endl;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints application info header. */
void TextureAtlasGenerator::printHeader() const
{
  std::cout << std::endl;
  std::cout << "Texture Atlas Generator, version " << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;
  std::cout << "Albert Banaszkiewicz, Little Bee Studios Ltd., 2011" << std::endl;
  std::cout << std::endl;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes all data. */
bool TextureAtlasGenerator::process()
{
  // print header
  printHeader();
  std::cout << "PROCESSING INPUT DATA FILE: " << inputDataFilePath() << std::endl;

  XmlDocument doc;

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

  // generate atlases
  return generateAll();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates atlases. */
bool TextureAtlasGenerator::generateAll()
{
  // create XML document
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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates atlas for given group. */
bool TextureAtlasGenerator::generate(AtlasGroup* group)
{
  // select next sort methods
  switch (sortMethod())
  {
    case SM_NONE:           m_sortMethod = SM_GREATER_AREA;   std::cout << "Sorting by area..." << std::endl; break;
    case SM_GREATER_AREA:   m_sortMethod = SM_GREATER_WIDTH;  std::cout << "Sorting by width..." << std::endl; break;
    case SM_GREATER_WIDTH:  m_sortMethod = SM_GREATER_HEIGHT; std::cout << "Sorting by height..." << std::endl; break;

    default:

      // if we reach here means all methods failed
      return false;
  }

  // get sorted entries for current group
  switch (sortMethod())
  {
    case SM_GREATER_AREA:   group->entries().sort(SortGreaterArea); break;
    case SM_GREATER_WIDTH:  group->entries().sort(SortGreaterWidth); break;
    case SM_GREATER_HEIGHT: group->entries().sort(SortGreaterHeight); break;
  }

  // create atlas tree root node
  AtlasNode* root = new AtlasNode();
  if (NULL == root)
  {
    // error!
    std::cout << "ERROR: Could not create atlas tree root node!" << std::endl;
    return false;
  }

  // set root node to max size
  root->m_rect = Recti(0, 0, group->textureImageSize().x, group->textureImageSize().y);
  
  // create group element
  XmlElement groupElement("group");
  groupElement.setAttribute("name", String::Format("atlas-%s", group->name().toAscii()));

  // add atlas texture declaration for current group
  XmlElement textureElement("texture");
  textureElement.setAttribute("type", "2d");
  textureElement.setAttribute("name", String::Format("atlas-%s", group->name().toAscii()));
  textureElement.setAttribute("path", group->textureImageName());
  textureElement.setAttribute("mag-filter", group->textureFiltersName());
  textureElement.setAttribute("min-filter", group->textureFiltersName());
  groupElement.appendChildElement(textureElement);

  // go thru all elements belonging to current group
  for (List<AtlasGroupEntry*>::const_iterator it = group->entries().begin(); it != group->entries().end(); ++it)
  {
    AtlasGroupEntry* entry = *it;

    // try to find node
    AtlasNode* node = root->insert(entry);
    if (NULL == node)
    {
      // error!
      std::cout << "WARNING: Could not insert " << entry->path() << std::endl;
      EGE_DELETE(root);

      // try again with another sort method
      return generate(group);
    }

    // store group entry for which place was found
    node->m_entry = entry;    

    // create XML corresponding node 
    XmlElement element("texture-image");
    element.setAttribute("name", entry->name());
    element.setAttribute("texture", String::Format("atlas-%s", group->name().toAscii()));
    
    // calculate real (without spacing) rect occupied by element
    Recti rect(node->m_rect.x + node->m_entry->spacing().x, node->m_rect.y + node->m_entry->spacing().y, 
               node->m_rect.width - node->m_entry->spacing().x - node->m_entry->spacing().z, 
               node->m_rect.height - node->m_entry->spacing().y - node->m_entry->spacing().w);

#ifdef _DEBUG
    element.setAttribute("pixel-rect", String::Format("%d %d %d %d", rect.x, rect.y, rect.width, rect.height));
#endif _DEBUG
    element.setAttribute("rect", String::Format("%f %f %f %f", rect.x * 1.0f / group->textureImageSize().x, 
                                                               rect.y * 1.0f / group->textureImageSize().y, 
                                                               rect.width * 1.0f / group->textureImageSize().x, 
                                                               rect.height * 1.0f / group->textureImageSize().y));

    // add to group element
    groupElement.appendChildElement(element);

    // copy data onto atlas image
    ImageUtils::FastCopy(group->image(), Vector2i(rect.x, rect.y), node->m_entry->image());
  }

  // clean up
  EGE_DELETE(root);

  // add XML group element into documents root
  PXmlElement rootElement = m_atlasData->rootElement();
  if (NULL == rootElement)
  {
    // error!
    std::cout << "ERROR: Could not retrieve XML data root element!" << std::endl;
    return false;    
  }

  rootElement->appendChildElement(groupElement);

  // save atlas image
  if (EGE_SUCCESS != group->image()->save(group->root() + "/" + group->textureImageName()))
  {
    // error!
    std::cout << "ERROR: Could not save atlas image!" << std::endl;
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
