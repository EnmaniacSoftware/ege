#include "Core/Debug/Debug.h"
#include "Core/File/File.h"
#include "Core/Data/DataBuffer.h"

#ifdef EGE_PLATFORM_WIN32
#include "Win32/File/FileWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
#include "Airplay/File/FileAirplay_p.h"
#endif

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(File)
EGE_DEFINE_DELETE_OPERATORS(File)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
File::File(const String& filePath) : Object(NULL, EGE_OBJECT_UID_FILE), 
                                     m_p(NULL), 
                                     m_filePath(filePath)
{
  m_p = ege_new FilePrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
File::~File()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool File::isValid() const
{
  return NULL != m_p;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns file path. */
const String& File::filePath() const
{
  return m_filePath;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Opens the given file with requested mode. */
EGEResult File::open(EGEFile::EMode mode)
{
  if (isValid())
  {
    return p_func()->open(mode);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Closes file. */
void File::close()
{
  if (isValid())
  {
    p_func()->close();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads given amount of data into destination buffer. */
/// @return Returns number of bytes read.
s64 File::read(const PDataBuffer& dst, s64 size)
{
  if (isValid())
  {
    return p_func()->read(dst, size);
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets new position within file. Returns old position or -1 if error occured. */
s64 File::seek(s64 offset, EGEFile::ESeekMode mode) 
{
  if (isValid())
  {
    return p_func()->seek(offset, mode);
  }

  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current position in file. Returns -1 if error occured. */
s64 File::tell()
{
  if (isValid())
  {
    return p_func()->tell();
  }

  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Writes given amount of data from destination buffer. */
/// @return Returns number of bytes written.
s64 File::write(const PDataBuffer& src, s64 size)
{
  if (isValid())
  {
    return p_func()->write(src, size);
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator << (u8 value)
{
  if (isValid())
  {
    DataBuffer buf(&value, sizeof (value));
    p_func()->write(buf, sizeof (value));
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator << (s8 value)
{
  if (isValid())
  {
    DataBuffer buf(&value, sizeof (value));
    p_func()->write(buf, sizeof (value));
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator << (u16 value)
{
  if (isValid())
  {
    u8 data[2] = { value & 0x00ff, (value & 0xff00) >> 8 };

    DataBuffer buf(&data, sizeof (data));
    p_func()->write(buf, sizeof (value));
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator << (s16 value)
{
  if (isValid())
  {
    u8 data[2] = { value & 0x00ff, (value & 0xff00) >> 8 };

    DataBuffer buf(&data, sizeof (data));
    p_func()->write(buf, sizeof (value));
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator << (u32 value)
{
  if (isValid())
  {
    u8 data[4] = { (u8)(value & 0x000000ff), (u8)((value & 0x0000ff00) >> 8), (u8)((value & 0x00ff0000) >> 16), (u8)((value & 0xff000000) >> 24) };

    DataBuffer buf(&data, sizeof (data));
    p_func()->write(buf, sizeof (value));
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator << (s32 value)
{
  if (isValid())
  {
    u8 data[4] = { (u8)(value & 0x000000ff), (u8)((value & 0x0000ff00) >> 8), (u8)((value & 0x00ff0000) >> 16), (u8)((value & 0xff000000) >> 24) };

    DataBuffer buf(&data, sizeof (data));
    p_func()->write(buf, sizeof (value));
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator << (u64 value)
{
  if (isValid())
  {
    u8 data[8] = { (u8)((value & 0x00000000000000ffLL) >> 00), (u8)((value & 0x000000000000ff00LL) >> 8), 
                   (u8)((value & 0x0000000000ff0000LL) >> 16), (u8)((value & 0x00000000ff000000LL) >> 24),
                   (u8)((value & 0x000000ff00000000LL) >> 32), (u8)((value & 0x0000ff0000000000LL) >> 40), 
                   (u8)((value & 0x00ff000000000000LL) >> 48), (u8)((value & 0xff00000000000000LL) >> 56) };

    DataBuffer buf(&data, sizeof (data));
    p_func()->write(buf, sizeof (value));
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator << (s64 value)
{
  if (isValid())
  {
    u8 data[8] = { (u8)((value & 0x00000000000000ffLL) >> 0), (u8)((value & 0x000000000000ff00LL) >> 8), 
                   (u8)((value & 0x0000000000ff0000LL) >> 16), (u8)((value & 0x00000000ff000000LL) >> 24),
                   (u8)((value & 0x000000ff00000000LL) >> 32), (u8)((value & 0x0000ff0000000000LL) >> 40), 
                   (u8)((value & 0x00ff000000000000LL) >> 48), (u8)((value & 0xff00000000000000LL) >> 56) };

    DataBuffer buf(&data, sizeof (data));
    p_func()->write(buf, sizeof (value));
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator << (bool value)
{
  return this->operator<< ((u8)(value ? 1 : 0));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator << (float32 value)
{
  if (isValid())
  {
    DataBuffer buf(&value, sizeof (value));
    p_func()->write(buf, sizeof (value));
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator << (float64 value)
{
  if (isValid())
  {
    DataBuffer buf(&value, sizeof (value));
    p_func()->write(buf, sizeof (value));
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator >> (u8& value)
{
  if (isValid())
  {
    DataBuffer buf;
    p_func()->read(buf, sizeof (value));
    buf >> value;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator >> (s8& value)
{
  if (isValid())
  {
    DataBuffer buf;
    p_func()->read(buf, sizeof (value));
    buf >> value;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator >> (u16& value)
{
  if (isValid())
  {
    DataBuffer buf;
    p_func()->read(buf, sizeof (value));
    buf >> value;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator >> (s16& value)
{
  if (isValid())
  {
    DataBuffer buf;
    p_func()->read(buf, sizeof (value));
    buf >> value;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator >> (u32& value)
{
  if (isValid())
  {
    DataBuffer buf;
    p_func()->read(buf, sizeof (value));
    buf >> value;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator >> (s32& value)
{
  if (isValid())
  {
    DataBuffer buf;
    p_func()->read(buf, sizeof (value));
    buf >> value;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator >> (u64& value)
{
  if (isValid())
  {
    DataBuffer buf;
    p_func()->read(buf, sizeof (value));
    buf >> value;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator >> (s64& value)
{
  if (isValid())
  {
    DataBuffer buf;
    p_func()->read(buf, sizeof (value));
    buf >> value;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator >> (bool& value)
{
  u8 data;
  this->operator>>(data);
  value = (1 == data);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator >> (float32& value)
{
  if (isValid())
  {
    DataBuffer buf;
    p_func()->read(buf, sizeof (value));
    buf >> value;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE::File& File::operator >> (float64& value)
{
  if (isValid())
  {
    DataBuffer buf;
    p_func()->read(buf, sizeof (value));
    buf >> value;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if file is opened. */
bool File::isOpen() const
{
  if (isValid())
  {
    return p_func()->isOpen();
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns file size. Returns -1 if error occured. */
s64 File::size()
{
  if (isValid())
  {
    return p_func()->size();
  }

  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if file exists. */
bool File::exists() const
{
  if (isValid())
  {
    return p_func()->exists();
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
