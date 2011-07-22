#include "TextureAtlasGenerator.h"
#include <EGEFile.h>
#include <iostream>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define VERSION_MAJOR 0
#define VERSION_MINOR 20
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping image format name into framework enum. */
static EGEImage::Format MapImageFormat(const String& formatName)
{
  // rgba
  return EGEImage::RGBA_8888;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function for sorting object from greatest area to smallest. */
static bool SortGreaterArea(ImageEntry* left, ImageEntry* right)
{
  return left->image()->width() * left->image()->height() > right->image()->width() * right->image()->height();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function for sorting object from greatest width to smallest. */
static bool SortGreaterWidth(ImageEntry* left, ImageEntry* right)
{
  return left->image()->width() > right->image()->width();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function for sorting object from greatest height to smallest. */
static bool SortGreaterHeight(ImageEntry* left, ImageEntry* right)
{
  return left->image()->height() > right->image()->height();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAtlasGenerator::TextureAtlasGenerator(int argc, char** argv) : m_outputFormat(EGEImage::NONE), m_outputSize(0), m_sortMethod(SM_NONE)
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
    // check if SIZE switch
    else if ("-size" == opt)
    {
      if (!val.empty())
      {
        m_outputSize = val.toInt();
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
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAtlasGenerator::~TextureAtlasGenerator()
{
  // go thru all elements
  for (MultiMap<String, ImageEntry*>::iterator it = m_imageEntries.begin(); it != m_imageEntries.end();)
  {
    EGE_DELETE(it->second);
    m_imageEntries.erase(it++);
  }

  m_atlasData = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool TextureAtlasGenerator::isValid() const
{
  return (EGEImage::NONE != m_outputFormat) && (0 < m_outputSize) && !m_inputDataFilePath.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints syntax. */
void TextureAtlasGenerator::printSyntax() const
{
  printHeader();

  std::cout << "Usage syntax:" << std::endl;
  std::cout << "tatlasgen.exe -format [rgba] -size [integer] -input [filename]" << std::endl;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints application info header. */
void TextureAtlasGenerator::printHeader() const
{
  std::cout << "Texture Atlas Generator" << std::endl;
  std::cout << "version " << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;
  std::cout << "Albert Banaszkiewicz, Flint Strike Ent., 2011" << std::endl;
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

  // go thru all children
  PXmlElement child = root->firstChild();
  while (child && child->isValid())
  {
    // check if IMAGE child
    if ("image" == child->name())
    {
      ImageEntry* entry = new ImageEntry(child->attribute("path"), child->attribute("group"));
      if ((NULL == entry) || !entry->isValid())
      {
        // error!
        std::cout << "ERROR: memory allocation failed!" << std::endl;
        return false;
      }

      // add to pool
      m_imageEntries.insert(entry->group(), entry);
    }

    // go to next child
    child = child->nextChild();
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

  // process till entries exists
  while (!m_imageEntries.empty())
  {
    // get first element group and do processing for it
    String group = m_imageEntries.begin()->first;
    std::cout << "PROCESSING GROUP " << group << std::endl;

    // reset sort mode
    m_sortMethod = SM_NONE;

    // generate atlas for current group
    if (!generate(group))
    {
      std::cout << "ERROR: Could not generate atlas for group: " << group << ". Skipping!" << std::endl << std::endl;
    }
    else
    {
      std::cout << "SUCCESS" << std::endl << std::endl;
    }

    // remove all entries associated with current group
    for (MultiMap<String, ImageEntry*>::const_iterator it = m_imageEntries.begin(); it != m_imageEntries.end(); )
    {
      if (it->first == group)
      {
        m_imageEntries.erase(it++);
      }
      else
      {
        ++it;
      }
    }
  }

  // save XML data
  return EGE_SUCCESS == m_atlasData->save("ouput.xml");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates atlas for given group. */
bool TextureAtlasGenerator::generate(const String& group)
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
  DynamicArray<ImageEntry*> pool = sortedEntries(group);

  // create atlas texture for group
  PImage atlasImage = ege_new Image(NULL, outputSize(), outputSize(), EGEImage::RGBA_8888);
  if (NULL == atlasImage)
  {
    // error!
    std::cout << "ERROR: Could not create atlas image!" << std::endl;
    return false;
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
  root->m_rect = Recti(0, 0, outputSize(), outputSize());
  
  // create group element
  XmlElement groupElement("group");
  groupElement.setAttribute("name", String::Format("atlas-%s", group.toAscii()));

  // go thru all elements belonging to current group
  for (DynamicArray<ImageEntry*>::const_iterator it = pool.begin(); it != pool.end(); ++it)
  {
    ImageEntry* entry = *it;

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

    // store image for which place was found
    node->m_imageEntry = entry;    

    // create XML corresponding node 
    XmlElement element("image");
    element.setAttribute("path", node->m_imageEntry->path());
    element.setAttribute("pixel-rect", String::Format("%d %d %d %d", node->m_rect.x, node->m_rect.y, node->m_rect.width, node->m_rect.height));
    element.setAttribute("rect", String::Format("%f %f %f %f", node->m_rect.x * 1.0f / atlasImage->width(), node->m_rect.y * 1.0f / atlasImage->height() , 
                                                node->m_rect.width * 1.0f / atlasImage->width(), node->m_rect.height * 1.0f / atlasImage->height()));

    // add to group element
    groupElement.appendChildElement(element);

    // copy data onto atlas image
    ImageUtils::FastCopy(atlasImage, Vector2i(node->m_rect.x, node->m_rect.y), node->m_imageEntry->image());
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
  if (EGE_SUCCESS != atlasImage->save(String::Format("atlas_%s.png", group.toAscii())))
  {
    // error!
    std::cout << "ERROR: Could not save atlas image!" << std::endl;
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns sorted array of image array pointers belonging to given group. */
DynamicArray<ImageEntry*> TextureAtlasGenerator::sortedEntries(const String& group) const
{
  DynamicArray<ImageEntry*> pool;

  // add all elements belonging to given group into array
  for (MultiMap<String, ImageEntry*>::const_iterator it = m_imageEntries.begin(); it != m_imageEntries.end(); ++it)
  {
    if (it->first == group)
    {
      pool.push_back(it->second);
    }
  }

  // sort according to current method
  switch (sortMethod())
  {
    case SM_GREATER_AREA:   std::sort(pool.begin(), pool.end(), SortGreaterArea); break;
    case SM_GREATER_WIDTH:  std::sort(pool.begin(), pool.end(), SortGreaterWidth); break;
    case SM_GREATER_HEIGHT: std::sort(pool.begin(), pool.end(), SortGreaterHeight); break;
  }

  //for (DynamicArray<ImageEntry*>::iterator it = pool.begin(); it != pool.end(); ++it)
  //{
  //  ImageEntry* entry = *it;
  //  std::cout << entry->image()->width() << " " << entry->image()->height() << " " << entry->image()->width() * entry->image()->height() << std::endl;
  //}

  return pool;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
