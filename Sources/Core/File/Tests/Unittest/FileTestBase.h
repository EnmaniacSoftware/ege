#include "TestFramework/Interface/TestBase.h"
#include <string>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FileTestBase : public TestBase
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

  protected:

    /*! Checks if a file exists at the given path. 
     *  @param  filePath  File to check for existance.
     *  @return TRUE if file exists. Otherwise, FALSE.
     */
    bool osFileExists(const std::string& filePath) const;
    /*! Removes given file. 
     *  @param  filePath  File to be removed.
     *  @return TRUE if file has been removed. Otherwise, FALSE.
     */
    bool osRemoveFile(const std::string& filePath) const;
    /*! Creates file with given content.
     *  @param  filePath  File to be created.
     *  @param  content   Text context of the file.
     *  @return TRUE if file with given content has been successfully created. Otherwise, FALSE. 
     */
    bool osCreateFile(const std::string& filePath, const std::string& content) const;
    /*! Reads file data.
     *  @param  filePath  File to be read.
     *  @return Text content of the file.
     */
    std::string osReadFile(const std::string& filePath) const;
    /*! Returns file size.
     *  @param  filePath  File to test the size of.
     *  @return File size in bytes. Negative value is returned in case of an error.
     */
    int osFileSize(const std::string& filePath) const;
    /*! Returns file path to generated file. */
    const char* generatedFilePath() const;
    /*! Returns file path to existing file. */
    const char* existingFilePath() const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
