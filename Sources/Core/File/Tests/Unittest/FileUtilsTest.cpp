#include "Core/File/Tests/Unittest/FileTestBase.h"
#include <EGEFileUtils.h>
#include <EGEString.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FileUtilsTest : public FileTestBase
{
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileUtilsTest, Size)
{
  const String KFilePath = "some-dummy-path.tht";

  EXPECT_TRUE(0 > FileUtils::Size(KFilePath));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileUtilsTest, Exists)
{
  EXPECT_TRUE(FileUtils::Exists(existingFilePath()));
  EXPECT_FALSE(FileUtils::Exists(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileUtilsTest, Remove)
{
  // create file and verify its existance
  EXPECT_TRUE(osCreateFile(generatedFilePath(), "dummy content"));
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // remove file
  EXPECT_TRUE(FileUtils::Remove(generatedFilePath()));
  EXPECT_FALSE(FileUtils::Exists(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------