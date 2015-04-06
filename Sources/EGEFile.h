#ifndef EGE_FILE_H
#define EGE_FILE_H

#include "Core/Platform.h"

#if EGE_PLATFORM_WIN32
  #include "Win32/File/Interface/FileWin32.h"
#elif EGE_PLATFORM_IOS
  #include "iOS/File/Interface/FileIOS.h"
#endif // EGE_PLATFORM_WIN32

#include "Core/File/Interface/File.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef TFile<FilePrivate> File;
typedef EGE::SmartPointer<File> PFile;
//EGE_DECLARE_SMART_CLASS(File, PFile)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_FILE_H