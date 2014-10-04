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
TEST_F(FileTest, Size)
{
  const String KFilePath = "some-dummy-path.tht";

  File file(KFilePath);
  EXPECT_TRUE(0 > file.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileTest, Exists)
{
  EXPECT_TRUE(File::Exists(existingFilePath()));
  EXPECT_FALSE(File::Exists(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileTest, Remove)
{
  // create file and verify its existance
  EXPECT_TRUE(osCreateFile(generatedFilePath(), "dummy content"));
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // remove file
  EXPECT_TRUE(File::Remove(generatedFilePath()));
  EXPECT_FALSE(File::Exists(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------