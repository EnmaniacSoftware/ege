#include "Core/File/Tests/Unittest/FileTestBase.h"
#include <EGEFile.h>
#include <EGEString.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FileReadOnlyModeTest : public FileTestBase
{
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, OpenExisting)
{
  File file(existingFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));
  file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, OpenNonExisting)
{
  File file("File-test/dummy.blah");
  EXPECT_EQ(EGE_ERROR_IO, file.open(EFileModeReadOnly));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, Close)
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
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // try to delete
  EXPECT_FALSE(osRemoveFile(generatedFilePath()));

  // close
  file.close();

  // try to delete
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, ReadAll)
{
  DataBuffer buffer;

  const std::string KGeneratedFileContent = "This is dummy text";

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // create file with content
  EXPECT_TRUE(osCreateFile(generatedFilePath(), KGeneratedFileContent));

  // verify file exists
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // open file for reading
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // read all data from file
  EXPECT_EQ(static_cast<s64>(KGeneratedFileContent.length()), file.read(buffer, -1));

  // verify data
  EXPECT_EQ(0, memcmp(reinterpret_cast<const void*>(KGeneratedFileContent.c_str()), buffer.data(), KGeneratedFileContent.length()));

  // close file
  file.close();

  // try to delete
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, ReadPartially)
{
  DataBuffer buffer;

  const std::string KGeneratedFileContent = "This is dummy text";

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // create file with content
  EXPECT_TRUE(osCreateFile(generatedFilePath(), KGeneratedFileContent));

  // verify file exists
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // open file for reading
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // read all data in chunks
  s64 totalBytesToRead = KGeneratedFileContent.length();
  do
  {
    // read 3 bytes from file
    const s64 bytesRead = file.read(buffer, 3);
    
    // expect at least 1 byte read
    EXPECT_TRUE(0 < bytesRead);

    // update total bytes read
    totalBytesToRead -= bytesRead;
  }
  while (0 < totalBytesToRead);

  // verify data
  EXPECT_EQ(0, memcmp(reinterpret_cast<const void*>(KGeneratedFileContent.c_str()), buffer.data(), KGeneratedFileContent.length()));

  // close file
  file.close();

  // try to delete
  EXPECT_TRUE(osRemoveFile(generatedFilePath()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, Write)
{
  const std::string KGeneratedFileContent = "This is dummy text";
  const std::string KBufferContent = "This is additional text";

  // prepare buffer
  DataBuffer buffer;
  buffer << KBufferContent;
  EXPECT_EQ(static_cast<s64>(KBufferContent.length()), buffer.size());

  // verify there is no such file
  EXPECT_FALSE(osFileExists(generatedFilePath()));

  // create file with content
  EXPECT_TRUE(osCreateFile(generatedFilePath(), KGeneratedFileContent));

  // verify file exists
  EXPECT_TRUE(osFileExists(generatedFilePath()));

  // open file for reading
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // write all data to a file
  EXPECT_EQ(0, file.write(buffer, -1));

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
TEST_F(FileReadOnlyModeTest, SeekModes)
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

  // open file
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // move the file pointer 2 bytes from the begining
  EXPECT_EQ(0, file.seek(2, EFileSeekBegin));

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
TEST_F(FileReadOnlyModeTest, FilePosition)
{
  File file(existingFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // get current position
  EXPECT_EQ(0, file.tell());

  // move the file pointer 2 bytes from the begining
  EXPECT_EQ(0, file.seek(2, EFileSeekBegin));
  EXPECT_EQ(2, file.tell());

  // move file another 2 bytes
  EXPECT_EQ(2, file.seek(2, EFileSeekCurrent));
  EXPECT_EQ(4, file.tell());

  // move file to the end of the file
  EXPECT_EQ(4, file.seek(0, EFileSeekEnd));
  EXPECT_EQ(osFileSize(existingFilePath()), file.tell());

  // move file back to begining
  EXPECT_EQ(osFileSize(existingFilePath()), file.seek(0, EFileSeekBegin));
  EXPECT_EQ(0, file.tell());

  // close file
  file.close();

  // check file position
  EXPECT_TRUE(0 > file.tell());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

