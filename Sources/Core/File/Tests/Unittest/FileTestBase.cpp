#include "Core/File/Tests/Unittest/FileTestBase.h"
#include <EGEMemory.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FileTestBase::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FileTestBase::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FileTestBase::SetUp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FileTestBase::TearDown()
{
  osRemoveFile(generatedFilePath());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FileTestBase::osFileExists(const std::string& filePath) const
{
  bool exists = false;

  // try to open file
  FILE* file = NULL;
  if (0 == fopen_s(&file, filePath.c_str(), "r"))
  {
    // exists
    exists = true;

    // close file
    fclose(file);
  }
  
  return exists;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FileTestBase::osRemoveFile(const std::string& filePath) const
{
  return (0 == ::remove(filePath.c_str()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FileTestBase::osCreateFile(const std::string& filePath, const std::string& content) const
{
  // open file for writting
  FILE* file = NULL;
  if (0 != fopen_s(&file, filePath.c_str(), "wt"))
  {
    // error!
    return false;
  }

  // write data
  if (1 != fwrite(content.c_str(), content.length(), 1, file))
  {
    // error!
    fclose(file);

    // remove file
    osRemoveFile(filePath);

    return false;
  }

  // close file
  fclose(file);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int FileTestBase::osFileSize(const std::string& filePath) const
{
  // try to open file
  FILE* file = NULL;
  if (0 != fopen_s(&file, filePath.c_str(), "rb"))
  {
    // error!
    return -1;
  }

  // move file pointer to the end of the file
  if (0 != fseek(file, 0, SEEK_END))
  {
    // error!
    fclose(file);
    return -1;
  }

  // get file position
  int size = ftell(file);

  // close file
  fclose(file);

  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::string FileTestBase::osReadFile(const std::string& filePath) const
{
  std::string data;

  const int fileSize = osFileSize(filePath);
  if (0 < fileSize)
  {
    char buffer[1024];
    memset(buffer, 0, sizeof (buffer));
    EXPECT_TRUE(1024 > fileSize);    

    // try to open file
    FILE* file = NULL;
    if (0 == fopen_s(&file, filePath.c_str(), "rb"))
    {
      // read all data
      const size_t bytesRead = fread_s(buffer, fileSize, 1, fileSize, file);
      if (bytesRead == fileSize)
      {
        data = buffer;
      }

      // close file
      fclose(file);
    }
  }

  return data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* FileTestBase::generatedFilePath() const
{
  return "File-test/generated.dat";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* FileTestBase::existingFilePath() const
{
  return "File-test/example.dat";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::string FileTestBase::generatedFileContent() const
{
  return "This is dummy content for FileWritableModeTest! Yaya!";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FileTestBase::createGeneratedFile() const
{
  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // create file with content
  EXPECT_TRUE(osCreateFile(generatedFilePath(), generatedFileContent()));

  // verify file exists
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // verify size of the file
  EXPECT_EQ(generatedFileContent().length(), osFileSize(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
