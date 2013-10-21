#include <gtest/gtest.h>
#include <EGEMemory.h>
#include <EGEXml.h>
#include <EGEDataBuffer.h>
#include <stdio.h>
#include <sys/stat.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class XMLTest : public ::testing::Test
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

  protected:

    /*! Loads file content into the buffer. 
     *  @param  fileName  File which content is to be loaded.
     *  @return On success, returns buffer containing the loaded content. Otherwise, NULL.
     */
    PDataBuffer loadFileToBuffer(const String& fileName) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void XMLTest::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void XMLTest::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void XMLTest::SetUp()
{
  // cleanup
  remove("Xml-test/generated.xml");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void XMLTest::TearDown()
{
  // cleanup
  remove("Xml-test/generated.xml");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PDataBuffer XMLTest::loadFileToBuffer(const String& fileName) const
{
  // open file
  FILE* file = fopen(fileName.toAscii(), "rb");
  EXPECT_TRUE(NULL != file);

  // find file size
  long fileSize = _filelength(_fileno(file));
  EXPECT_NE(-1L, fileSize);

  // allocate buffer of a required size
  PDataBuffer buffer = ege_new DataBuffer();
  EXPECT_TRUE(NULL != buffer);
  EXPECT_EQ(EGE_SUCCESS, buffer->setSize(fileSize));

  // read file content into buffer
  EXPECT_EQ(fileSize, fread(buffer->data(), 1, fileSize, file));

  // close file
  EXPECT_EQ(0, fclose(file));

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(XMLTest, LoadUnexistingDocumentFromFile)
{
  XmlDocument document;
  EXPECT_TRUE(document.isValid());
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, document.load("unexisting-file.xml"));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(XMLTest, LoadInvalidDocumentFromFile)
{
  XmlDocument document;
  EXPECT_TRUE(document.isValid());
  EXPECT_EQ(EGE_ERROR, document.load("Xml-test/ill-formatted.xml"));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(XMLTest, LoadDocumentFromFile)
{
  // load from file
  XmlDocument document;
  EXPECT_TRUE(document.isValid());
  EXPECT_EQ(EGE_SUCCESS, document.load("Xml-test/valid.xml"));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(XMLTest, DecodeRootElementDirectly)
{
  // load from file
  XmlDocument document;
  EXPECT_TRUE(document.isValid());
  EXPECT_EQ(EGE_SUCCESS, document.load("Xml-test/valid.xml"));

  // process root element directly
  PXmlElement root = document.rootElement();
  EXPECT_TRUE(NULL != root);
  EXPECT_TRUE(root->isValid());
  EXPECT_EQ("top-element", root->name());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(XMLTest, DecodeRootElementIndirectly)
{
  // load from file
  XmlDocument document;
  EXPECT_TRUE(document.isValid());
  EXPECT_EQ(EGE_SUCCESS, document.load("Xml-test/valid.xml"));

  // process root element directly
  PXmlElement root = document.firstChild("top-element");
  EXPECT_TRUE(NULL != root);
  EXPECT_TRUE(root->isValid());
  EXPECT_EQ("top-element", root->name());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(XMLTest, DecodeElements)
{
  // load from file
  XmlDocument document;
  EXPECT_TRUE(document.isValid());
  EXPECT_EQ(EGE_SUCCESS, document.load("Xml-test/valid.xml"));

  // process root element directly
  PXmlElement root = document.firstChild("top-element");
  EXPECT_TRUE(NULL != root);
  EXPECT_TRUE(root->isValid());
  EXPECT_EQ("top-element", root->name());

  // process next sub element
  PXmlElement element_lvl_2 = root->firstChild();
  EXPECT_TRUE(NULL != element_lvl_2);
  EXPECT_TRUE(element_lvl_2->isValid());
  EXPECT_EQ("second-level-element-1", element_lvl_2->name());

  // process next sibling element
  element_lvl_2 = element_lvl_2->nextChild();
  EXPECT_TRUE(NULL != element_lvl_2);
  EXPECT_TRUE(element_lvl_2->isValid());
  EXPECT_EQ("second-level-element-2", element_lvl_2->name());

  // verify no more child elements exist for level 2 element
  PXmlElement nonExistingElement = element_lvl_2->nextChild();
  EXPECT_TRUE(NULL != nonExistingElement);
  EXPECT_FALSE(nonExistingElement->isValid());

  // get first child element
  PXmlElement element_lvl_3 = element_lvl_2->firstChild();
  EXPECT_TRUE(NULL != element_lvl_3);
  EXPECT_TRUE(element_lvl_3->isValid());
  EXPECT_EQ("third-level-element", element_lvl_3->name());

  // verify no more child elements exist for level 3 element
  nonExistingElement = element_lvl_3->nextChild();
  EXPECT_TRUE(NULL != nonExistingElement);
  EXPECT_FALSE(nonExistingElement->isValid());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(XMLTest, DecodeAttributes)
{
  // load from file
  XmlDocument document;
  EXPECT_TRUE(document.isValid());
  EXPECT_EQ(EGE_SUCCESS, document.load("Xml-test/valid.xml"));
  EXPECT_TRUE(NULL != document.rootElement());

  // get element without attributes
  PXmlElement element = document.rootElement()->firstChild("second-level-element-2");
  EXPECT_TRUE(NULL != element);
  EXPECT_TRUE(element->isValid());
  EXPECT_EQ("second-level-element-2", element->name());

  // get first attribute
  PXmlAttribute attribute = element->firstAttribute();
  EXPECT_TRUE(NULL == attribute);

  // get element with attributes
  element = document.rootElement()->firstChild("second-level-element-1");
  EXPECT_TRUE(NULL != element);
  EXPECT_TRUE(element->isValid());
  EXPECT_EQ("second-level-element-1", element->name());

  // check if attributes exist
  EXPECT_TRUE(element->hasAttribute("attribute-1"));
  EXPECT_TRUE(element->hasAttribute("attribute-2"));
  EXPECT_TRUE(element->hasAttribute("attribute-3"));
  EXPECT_TRUE(element->hasAttribute("attribute-4"));
  EXPECT_FALSE(element->hasAttribute("attribute-0"));

  // get first attribute
  attribute = element->firstAttribute();
  EXPECT_TRUE(NULL != attribute);
  EXPECT_TRUE(attribute->isValid());
  EXPECT_TRUE(("attribute-1" == attribute->name()) || ("attribute-2" == attribute->name()) || 
              ("attribute-3" == attribute->name()) || ("attribute-4" == attribute->name()));

  // go to next attribute
  attribute = attribute->next();
  EXPECT_TRUE(NULL != attribute);
  EXPECT_TRUE(attribute->isValid());
  EXPECT_TRUE(("attribute-1" == attribute->name()) || ("attribute-2" == attribute->name()) || 
              ("attribute-3" == attribute->name()) || ("attribute-4" == attribute->name()));

  // go to next attribute
  attribute = attribute->next();
  EXPECT_TRUE(NULL != attribute);
  EXPECT_TRUE(attribute->isValid());
  EXPECT_TRUE(("attribute-1" == attribute->name()) || ("attribute-2" == attribute->name()) || 
              ("attribute-3" == attribute->name()) || ("attribute-4" == attribute->name()));

  // go to next attribute
  attribute = attribute->next();
  EXPECT_TRUE(NULL != attribute);
  EXPECT_TRUE(attribute->isValid());
  EXPECT_TRUE(("attribute-1" == attribute->name()) || ("attribute-2" == attribute->name()) || 
              ("attribute-3" == attribute->name()) || ("attribute-4" == attribute->name()));

  // go to next attribute (non existing one)
  attribute = attribute->next();
  EXPECT_TRUE(NULL == attribute);

  // retrive values of attributes
  EXPECT_EQ("text", element->attribute("attribute-1", String("none")));
  EXPECT_EQ("text", element->attribute("attribute-1", "none"));
  EXPECT_EQ(1, element->attribute("attribute-2", 0));
  EXPECT_EQ(2.5f, element->attribute("attribute-3", 0.0f));
  EXPECT_EQ(true, element->attribute("attribute-4", false));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(XMLTest, DecodeText)
{
  // load from file
  XmlDocument document;
  EXPECT_TRUE(document.isValid());
  EXPECT_EQ(EGE_SUCCESS, document.load("Xml-test/valid.xml"));
  EXPECT_TRUE(NULL != document.rootElement());

  // get element without text
  PXmlElement element = document.rootElement()->firstChild("second-level-element-2");
  EXPECT_TRUE(NULL != element);
  EXPECT_TRUE(element->isValid());
  EXPECT_EQ("second-level-element-2", element->name());

  EXPECT_EQ("", element->text());

  // get element with text
  element = element->firstChild("third-level-element");
  EXPECT_TRUE(NULL != element);
  EXPECT_TRUE(element->isValid());
  EXPECT_EQ("third-level-element", element->name());

  EXPECT_EQ("This is some text And even more text here", element->text());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(XMLTest, LoadDocumentFromEmptyBuffer)
{
  PDataBuffer buffer;

  // load XML from buffer
  XmlDocument document;
  EXPECT_TRUE(document.isValid());
  EXPECT_EQ(EGE_ERROR_NOT_FOUND, document.load(buffer));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(XMLTest, LoadDocumentFromBuffer)
{
  // load XML content
  PDataBuffer buffer = loadFileToBuffer("Xml-test/valid.xml");
  EXPECT_TRUE(NULL != buffer);

  // load XML from buffer
  XmlDocument document;
  EXPECT_TRUE(document.isValid());
  EXPECT_EQ(EGE_SUCCESS, document.load(buffer));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(XMLTest, SaveDocument)
{
  // create root element
  XmlElement rootElement("root");
  
  // create element without attributes
  XmlElement elementLvl1NoAttribs("level-1-element-1");

  // create element with attributes
  XmlElement elementLvl1Attribs("level-1-element-2");
  
  EXPECT_TRUE(elementLvl1Attribs.setAttribute("attribute-1", "text"));
  EXPECT_TRUE(elementLvl1Attribs.setAttribute("attribute-2", String("text")));
  EXPECT_TRUE(elementLvl1Attribs.setAttribute("attribute-3", 1));
  EXPECT_TRUE(elementLvl1Attribs.setAttribute("attribute-4", 2.5f));
  EXPECT_TRUE(elementLvl1Attribs.setAttribute("attribute-5", true));

  // create element with no attributes
  XmlElement elementLvl2NoAttribs("level-2-element-1");

  // add elements to root element
  EXPECT_TRUE(rootElement.appendChildElement(elementLvl1NoAttribs));
  EXPECT_TRUE(rootElement.appendChildElement(elementLvl1Attribs));
  EXPECT_TRUE(elementLvl1Attribs.appendChildElement(elementLvl2NoAttribs));

  // add root element to document
  XmlDocument xml;
  EXPECT_TRUE(xml.appendElement(rootElement));

  // generate XML into file and buffer
  DataBuffer buffer;
  EXPECT_EQ(EGE_SUCCESS, xml.save(buffer));
  EXPECT_EQ(EGE_SUCCESS, xml.save("Xml-test/generated.xml"));

  // load generated file content
  PDataBuffer fileBuffer = loadFileToBuffer("Xml-test/generated.xml");
  EXPECT_TRUE(NULL != fileBuffer);

  // compare loaded content and the one in the buffer
  EXPECT_EQ(fileBuffer->size(), buffer.size());
  EXPECT_EQ(0, EGE_MEMCMP(fileBuffer->data(), buffer.data(), static_cast<size_t>(fileBuffer->size())));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
