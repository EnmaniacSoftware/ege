#include "Core/File/Tests/Unittest/FileTestBase.h"
#include <EGEDataStream.h>
#include <EGEFile.h>
#include <EGEString.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FileReadOnlyModeTest : public FileTestBase
{
  protected:
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
  // generate file
  createGeneratedFile();

  // open and close file
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // try to delete
  EXPECT_FALSE(osRemoveFile(generatedFilePath()));

  // close
  file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, ReadAllWithBuffer)
{
  DataBuffer buffer;

  // generate file
  createGeneratedFile();

  // open file for reading
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // read all data from file
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.read(buffer, -1));

  // verify data
  EXPECT_EQ(0, memcmp(reinterpret_cast<const void*>(generatedFileContent().c_str()), buffer.data(), generatedFileContent().length()));

  // close file
  file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, ReadPartiallyWithBuffer)
{
  DataBuffer buffer;

  // generate file
  createGeneratedFile();

  // open file for reading
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // read all data in chunks
  s64 totalBytesToRead = generatedFileContent().length();
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
  EXPECT_EQ(0, memcmp(reinterpret_cast<const void*>(generatedFileContent().c_str()), buffer.data(), generatedFileContent().length()));

  // close file
  file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, ReadMoreThanAvailableWithBuffer)
{
  // prepare buffer
  DataBuffer buffer;
  DataStream stream(&buffer);
  stream << generatedFileContent().c_str();
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), buffer.size() - 1);

  // generate file
  createGeneratedFile();

  // open file for reading
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // read more data from file than available
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.read(buffer, generatedFileContent().length() * 2));

  // verify data
  EXPECT_EQ(0, memcmp(reinterpret_cast<const void*>(generatedFileContent().c_str()), buffer.data(), generatedFileContent().length()));

  // close file
  file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, ReadAllRaw)
{
  char buffer[256];
  memset(buffer, 0, sizeof (buffer));

  // generate file
  createGeneratedFile();

  // open file for reading
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // read all data from file
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.read(buffer, generatedFileContent().length()));

  // verify data
  EXPECT_STREQ(generatedFileContent().c_str(), buffer);

  // close file
  file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, ReadPartiallyRaw)
{
  char buffer[256];
  memset(buffer, 0, sizeof (buffer));

  // generate file
  createGeneratedFile();

  // open file for reading
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // read all data in chunks
  s64 totalBytesToRead = generatedFileContent().length();
  do
  {
    // read 3 bytes from file
    const s64 bytesRead = file.read(buffer + generatedFileContent().length() - totalBytesToRead, 3);
    
    // expect at least 1 byte read
    EXPECT_TRUE(0 < bytesRead);

    // update total bytes read
    totalBytesToRead -= bytesRead;
  }
  while (0 < totalBytesToRead);

  // verify data
  EXPECT_STREQ(generatedFileContent().c_str(), buffer);

  // close file
  file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, ReadMoreThanAvailableRaw)
{
  char buffer[256];
  memset(buffer, 0, sizeof (buffer));

  // generate file
  createGeneratedFile();

  // open file for reading
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // read more data from file than available
  EXPECT_EQ(static_cast<s64>(generatedFileContent().length()), file.read(buffer, generatedFileContent().length() * 2));

  // verify data
  EXPECT_STREQ(generatedFileContent().c_str(), buffer);

  // close file
  file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, WriteWithBuffer)
{
  const std::string KBufferContent = "This is additional text";

  // prepare buffer
  DataBuffer buffer;
  DataStream stream(&buffer);
  stream << KBufferContent.c_str();
  EXPECT_EQ(static_cast<s64>(KBufferContent.length()), buffer.size() - 1);

  // generate file
  createGeneratedFile();

  // open file for reading
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // write all data to a file
  EXPECT_GT(0, file.write(buffer, -1));

  // close file
  file.close();

  // verify written content
  const std::string writtenContent = osReadFile(generatedFilePath());

  // verify data
  EXPECT_EQ(generatedFileContent(), writtenContent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, WriteRaw)
{
  const std::string KBufferContent = "This is additional text";

  // generate file
  createGeneratedFile();

  // open file for reading
  File file(generatedFilePath());
  EXPECT_EQ(EGE_SUCCESS, file.open(EFileModeReadOnly));

  // write all data to a file
  EXPECT_GT(0, file.write(KBufferContent.c_str(), KBufferContent.length()));

  // close file
  file.close();

  // verify written content
  const std::string writtenContent = osReadFile(generatedFilePath());

  // verify data
  EXPECT_EQ(generatedFileContent(), writtenContent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(FileReadOnlyModeTest, SeekModes)
{
  // generate file
  createGeneratedFile();

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

