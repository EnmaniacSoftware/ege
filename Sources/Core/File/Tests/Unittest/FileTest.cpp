#include "Core/File/Tests/Unittest/FileTestBase.h"
#include <EGEFile.h>
#include <EGEString.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FileTest : public FileTestBase
{
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileTest, IsOpenExisting)
{
  // verify there is such file
  EXPECT_TRUE(osFileExists(existingFilePath()));

  File file(existingFilePath());
  EXPECT_FALSE(file.isOpen());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileTest, IsOpenNonExisting)
{
  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  File file(generatedFilePath());
  EXPECT_FALSE(file.isOpen());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileTest, FilePath)
{
  const String KFilePath = "some-dummy-path.tht";

  File file(KFilePath);
  EXPECT_EQ(KFilePath, file.filePath());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
