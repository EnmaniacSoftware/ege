#include "Core/File/Tests/Unittest/FileTestBase.h"
#include <EGEFile.h>
#include <EGEString.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FileWriteAppendModeTest : public FileTestBase
{
  protected:

    /*! Returns generated file text content. */
    std::string generatedFileContent() const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::string FileWriteAppendModeTest::generatedFileContent() const
{
  return "This is dummy text";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileWriteAppendModeTest, OpenNonExisting)
{
  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // open and close file
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeWriteAppend));
  file.close();

  // verify file exists
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // verify size of the file
  EXPECT_EQ(0, osFileSize(generatedFilePath()));

  // clean up
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileWriteAppendModeTest, OpenExisting)
{
  const std::string KGeneratedFileContent = "This is dummy text";

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // create file with content
  EXPECT_TRUE(osCreateFile(generatedFilePath(), KGeneratedFileContent));

  // verify file exists
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // verify size of the file
  EXPECT_EQ(KGeneratedFileContent.length(), osFileSize(generatedFilePath()));

  // open and close file
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeWriteAppend));
  file.close();

  // verify file exists
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // verify size of the file
  EXPECT_EQ(KGeneratedFileContent.length(), osFileSize(generatedFilePath()));

  // clean up
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileWriteAppendModeTest, Close)
{
  const std::string KGeneratedFileContent = "This is dummy text";

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // create file with content
  EXPECT_TRUE(osCreateFile(generatedFilePath(), KGeneratedFileContent));

  // verify file exists
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // verify size of the file
  EXPECT_EQ(KGeneratedFileContent.length(), osFileSize(generatedFilePath()));

  // open and close file
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeWriteAppend));

  // try to delete
  EXPECT_FALSE(osRemoveFile(generatedFilePath()));

  // close
  file.close();

  // try to delete
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileWriteAppendModeTest, Read)
{
  DataBuffer buffer;

  // open file for writting
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeWriteAppend));

  // read all data from file
  EXPECT_EQ(0, file.read(buffer, -1));

  // close file
  file.close();

  // try to delete
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileWriteAppendModeTest, WriteAll)
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
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeWriteAppend));

  // write all data to a file
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), file.write(buffer, -1));

  // close file
  file.close();

  // verify written content
  const std::string writtenContent = osReadFile(generatedFilePath());

  // verify data
  EXPECT_EQ(KGeneratedFileContent, writtenContent);

  // try to delete
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileWriteAppendModeTest, WritePartially)
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
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeWriteAppend));

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

  // try to delete
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileWriteAppendModeTest, SeekModes)
{
  const std::string KGeneratedFileContent = "This is dummy text";

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // create file with content
  EXPECT_TRUE(osCreateFile(generatedFilePath(), KGeneratedFileContent));

  // verify file exists
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // open file for writting
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeWriteAppend));

  // move the file pointer 2 bytes from the begining
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), file.seek(2, EFileSeekBegin));

  // move file another 2 bytes
  EXPECT_EQ(2, file.seek(2, EFileSeekCurrent));

  // move file to the end of the file
  EXPECT_EQ(4, file.seek(0, EFileSeekEnd));

  // move file back to begining
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), file.seek(0, EFileSeekBegin));

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
TEST_F(FileWriteAppendModeTest, FilePosition)
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
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeWriteOnly));

  // write all data to a file
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), file.write(buffer, -1));

  // move the file pointer 2 bytes from the begining
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), file.seek(2, EFileSeekBegin));
  EXPECT_EQ(2, file.tell());

  // move file another 2 bytes
  EXPECT_EQ(2, file.seek(2, EFileSeekCurrent));
  EXPECT_EQ(4, file.tell());

  // move file to the end of the file
  EXPECT_EQ(4, file.seek(0, EFileSeekEnd));
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), file.tell());

  // move file back to begining
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), file.seek(0, EFileSeekBegin));
  EXPECT_EQ(0, file.tell());

  // close file
  file.close();

  // check file position
  EXPECT_TRUE(0 > file.tell());

  // clean up
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileWriteAppendModeTest, Size)
{
  // create data buffer
  DataBuffer buffer;
  buffer << generatedFileContent();
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), buffer.size());

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // open file for writting
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeWriteAppend));

  // write all data to a file
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.write(buffer, -1));

  // verify size
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.size());

  // close file
  file.close();

  // clean up
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------