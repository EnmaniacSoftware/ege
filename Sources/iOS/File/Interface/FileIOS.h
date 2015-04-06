#ifndef EGE_IOS_FILE_PRIVATE_H
#define EGE_IOS_FILE_PRIVATE_H

/*! @brief  Private implementation of file for iOS.
 *          In general, applications are sandboxed in iOS. Each application has predefined directory structure.
 *
 *          [ApplicationName].app/
 *
 *          This is the ApplicationBundle directory where your application executable is stored.
 *          The directory structure that you create in your app exists in this directory (for example, images and other file types that you’ve marked
 *          as Resources in your Xamarin Studio project). If you need to access the content files inside your Application Bundle, the path to this directory
 *          is available via the NSBundle.MainBundle.BundlePath property.
 *
 *          Documents/
 *
 *          Use this directory to store user documents and application data files.
 *          The contents of this directory can be made available to the user through iTunes file sharing (although this is disabled by default). 
 *          Add a UIFileSharingEnabled Boolean key to the Info.plist file to allow users to access these files.
 *          Even if an application doesn’t immediately enable file sharing, you should avoid placing files that should be hidden from your users in this 
 *          directory (such as database files, unless you intend to share them). As long as sensitive files remain hidden, these files will not be exposed 
 *          (and potentially moved, modified, or deleted by iTunes) if file sharing is enabled in a future version.
 *          You can use the Environment.GetFolderPath (Environment.SpecialFolder.MyDocuments) method to get the path to the Documents directory for your 
 *          application. The contents of this directory are backed up by iTunes. 
 *
 *          Library/
 *
 *          The Library directory is a good place to store files that are not created directly by the user, such as databases or other 
 *          application-generated files. The contents of this directory are never exposed to the user via iTunes.
 *          You can create your own subdirectories in Library; however, there are already some system-created directories here that you should be aware of, 
 *          including Preferences and Caches. The contents of this directory (except for the Caches subdirectory) are backed up by iTunes. 
 *          Custom directories that you create in Library will be backed up. *
 *
 *          Library/Preferences/
 *
 *          Application-specific preference files are stored in this directory. Do not create these files directly. Instead, use the NSUserDefaults class.
 *          The contents of this directory are backed up by iTunes. 
 *
 *          Library/Caches/
 *
 *          The Caches directory is a good place to store data files that can help your application run, but that can be easily re-created if required. 
 *          The application should create and delete these files as needed and be able to re-create these files if necessary. iOS 5 may also delete these 
 *          files (under extremely low storage situations), however it will not do so while the application is running.
 *          The contents of this directory are NOT backed up by iTunes, which means they will not be present if the user restores a device, and they may not 
 *          be present after an updated version of your application is installed.
 *          For instance, in case your application can't connect to the network, you might use the Caches directory to store data or files to provide a good 
 *          offline experience. The application can save and retrieve this data quickly while waiting for network responses, but it doesn’t need to be backed 
 *          up and can easily be recovered or re-created after a restore or version update.
 *
 *          tmp/
 *
 *          Applications can store temporary files that are only needed for a short period in this directory. To conserve space, files should be deleted when 
 *          they are no longer required. The operating system may also delete files from this directory when an application is not running.
 *          The contents of this directory are NOT backed up by iTunes.
 *          For example, the tmp directory might be used to store temporary files that are downloaded for display to the user (such as Twitter avatars or 
 *          email attachments), but which could be deleted once they've been viewed (and downloaded again if they are required in the future). 
 *
 */

#include "EGE.h"
#include "Core/File/Interface/File.h"
#include "EGEDataBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FilePrivate
{
  public:

    FilePrivate(File* base);
   ~FilePrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(File)
  
    /*! @see File::open. */
    EGEResult open(FileMode mode);
    /*! @see File::close. */
    void close();
    /*! @see File::read. */
    s64 read(const PDataBuffer& dst, s64 size);
    /*! @see File::write. */
    s64 write(const PDataBuffer& src, s64 size);
    /*! @see File::seek. */
    s64 seek(s64 offset, FileSeek mode);
    /*! @see File::tell. */
    s64 tell();
    /*! @see File::isOpen. */
    bool isOpen() const;

  private:

    /*! File handle. */
    void* m_file;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_FILE_PRIVATE_H