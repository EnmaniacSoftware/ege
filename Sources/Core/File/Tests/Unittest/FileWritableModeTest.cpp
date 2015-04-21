#include "Core/File/Tests/Unittest/FileTestBase.h"
#include <EGEFile.h>
#include <EGEString.h>

using namespace ::testing;

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FileWritableModeTest : public FileTestBase
                           , public ::testing::WithParamInterface<FileMode>
{
  protected:

    /*! Returns TRUE if testing write-only access. */
    bool isWriteOnly() const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FileWritableModeTest::isWriteOnly() const
{
  return (EFileModeWriteOnly == GetParam());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(FileWritableModeTest, OpenNonExisting)
{
  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // open and close file
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(GetParam()));
  file.close();

  // verify file exists
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // verify size of the file
  EXPECT_EQ(0, osFileSize(generatedFilePath()));

  // clean up
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(FileWritableModeTest, OpenExisting)
{
  // create generated file
  createGeneratedFile();

  // open and close file
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(GetParam()));
  file.close();

  // verify file exists
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // verify size of the file
  const int KExpectedSize = isWriteOnly() ? 0 : generatedFileContent().length();
  EXPECT_EQ(KExpectedSize, osFileSize(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(FileWritableModeTest, Close)
{
  // create generated file
  createGeneratedFile();

  // open and close file
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(GetParam()));

  // try to delete
  EXPECT_FALSE(osRemoveFile(generatedFilePath()));

  // close
  file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(FileWritableModeTest, ReadWithBuffer)
{
  DataBuffer buffer;

  // open new file for writing
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(GetParam()));

  // try to read some data from file
  EXPECT_GT(0, file.read(buffer, 10));

  // close file
  file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(FileWritableModeTest, ReadRaw)
{
  char buffer[256];
  memset(buffer, 0, sizeof (buffer));

  // open file for writing (ie create new file)
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(GetParam()));

  // try to read some data from file
  EXPECT_GT(0, file.read(buffer, generatedFileContent().length()));

  // close file
  file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(FileWritableModeTest, WriteAllWithBuffer)
{
  const std::string KGeneratedFileContent = "This is dummy text";

  // create data buffer
  DataBuffer buffer;
  buffer << KGeneratedFileContent;
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), buffer.size());

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // open file for writting
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(GetParam()));

  // write all data to a file
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), file.write(buffer, -1));

  // close file
  file.close();

  // verify written content
  const std::string writtenContent = osReadFile(generatedFilePath());

  // verify data
  EXPECT_EQ(KGeneratedFileContent, writtenContent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(FileWritableModeTest, WritePartiallyWithBuffer)
{
  const std::string KGeneratedFileContent = "This is dummy text";

  // create data buffer
  DataBuffer buffer;
  buffer << KGeneratedFileContent;
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), buffer.size());

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // open file for writting
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(GetParam()));

  // write all data in chunks
  s64 totalBytesToWrite = KGeneratedFileContent.length();
  do
  {
    // write 3 bytes to file
    const s64 bytesWritten = file.write(buffer, 3);
    
    // expect at least 1 byte written
    EXPECT_TRUE(0 < bytesWritten);

    // update total bytes read
    totalBytesToWrite -= bytesWritten;
  }
  while (0 < totalBytesToWrite);

  // close file
  file.close();

  // verify written content
  const std::string writtenContent = osReadFile(generatedFilePath());

  // verify data
  EXPECT_EQ(KGeneratedFileContent, writtenContent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(FileWritableModeTest, WriteAllRaw)
{
  const std::string KGeneratedFileContent = "This is dummy text";

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // open file for writting
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(GetParam()));

  // write all data to a file
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), file.write(KGeneratedFileContent.c_str(), KGeneratedFileContent.length()));

  // close file
  file.close();

  // verify written content
  const std::string writtenContent = osReadFile(generatedFilePath());

  // verify data
  EXPECT_EQ(KGeneratedFileContent, writtenContent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(FileWritableModeTest, WritePartiallyRaw)
{
  const std::string KGeneratedFileContent = "This is dummy text";

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // open file for writting
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(GetParam()));

  // write all data in chunks
  s64 totalBytesToWrite = KGeneratedFileContent.length();
  do
  {
    // write 3 bytes to file
    const s64 bytesWritten = file.write(KGeneratedFileContent.c_str() + KGeneratedFileContent.length() - totalBytesToWrite, 3);
    
    // expect at least 1 byte written
    EXPECT_TRUE(0 < bytesWritten);

    // update total bytes read
    totalBytesToWrite -= bytesWritten;
  }
  while (0 < totalBytesToWrite);

  // close file
  file.close();

  // verify written content
  const std::string writtenContent = osReadFile(generatedFilePath());

  // verify data
  EXPECT_EQ(KGeneratedFileContent, writtenContent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(FileWritableModeTest, SeekModes)
{
  // create data buffer
  DataBuffer buffer;
  buffer << generatedFileContent();
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), buffer.size());

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // open file for writting
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(GetParam()));

  // write all data to a file
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.write(buffer, -1));

  // move the file pointer 2 bytes from the begining
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.seek(2, EFileSeekBegin));

  // move file another 2 bytes
  EXPECT_EQ(2, file.seek(2, EFileSeekCurrent));

  // move file to the end of the file
  EXPECT_EQ(4, file.seek(0, EFileSeekEnd));

  // move file back to begining
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.seek(0, EFileSeekBegin));

  // try to move file pointer beyond the file content
  EXPECT_EQ(0, file.seek(124, EFileSeekBegin));
  EXPECT_EQ(124, file.seek(-24, EFileSeekCurrent));
  EXPECT_EQ(100, file.seek(20, EFileSeekEnd));

  // try to move file pointer to location 'before' the content
  EXPECT_TRUE(0 > file.seek(-1, EFileSeekBegin));
  EXPECT_TRUE(0 > file.seek(-100, EFileSeekCurrent));
  EXPECT_TRUE(0 > file.seek(-100, EFileSeekEnd));

  // close file
  file.close();

  // clean up
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_P(FileWritableModeTest, FilePosition)
{
  // create data buffer
  DataBuffer buffer;
  buffer << generatedFileContent();
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), buffer.size());

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // open new file
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(GetParam()));

  // write all data to a file
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.write(buffer, -1));

  // move the file pointer 2 bytes from the begining
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.seek(2, EFileSeekBegin));
  EXPECT_EQ(2, file.tell());

  // move file another 2 bytes
  EXPECT_EQ(2, file.seek(2, EFileSeekCurrent));
  EXPECT_EQ(4, file.tell());

  // move file to the end of the file
  EXPECT_EQ(4, file.seek(0, EFileSeekEnd));
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.tell());

  // move file back to begining
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.seek(0, EFileSeekBegin));
  EXPECT_EQ(0, file.tell());

  // close file
  file.close();

  // check file position
  EXPECT_TRUE(0 > file.tell());

  // clean up
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSTANTIATE_TEST_CASE_P(File, FileWritableModeTest, ::testing::Values(EFileModeWriteOnly, EFileModeWriteAppend));
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
