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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void XMLTest::TearDown()
{
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
