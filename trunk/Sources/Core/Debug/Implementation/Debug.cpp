#include "Core/Debug/Interface/Debug.h"
#include <stdarg.h>

EGE_NAMESPACE_BEGIN
  
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* Debug::KWarningPrefix = "WARNING: ";
const char* Debug::KErrorPrefix   = "ERROR: ";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static StringList l_enabledName;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug::Debug(DebugMessageType type, const String& name) : m_spaceSeperated(true)
                                                        , m_type(type)
                                                        , m_enabled(false)
{
  // check if enabled
  if (l_enabledName.contains(name))
  {
    m_enabled = true;

    // allocate buffer
    m_buffer = ege_new StringBuffer();
    if (NULL != m_buffer)
    {
      // init depending on type
      switch (m_type)
      {
        case EWarning:  *m_buffer << KWarningPrefix; break;
        case EError:    *m_buffer << KErrorPrefix; break;
      
        default:
          break;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug::Debug(const Debug& other)
{
  *this = other;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug::~Debug()
{
  // check if last instance
  if ((NULL != m_buffer) && (1 == m_buffer->referenceCount()))
  {
    // check if printable
    if (m_enabled)
    {
      Print(m_buffer->string().toAscii());
    }
  }

  // clean up
  m_buffer = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator << (bool t)
{ 
  if (m_enabled)
  {
    *m_buffer << t;
  }

  return maybeSpace();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator << (s16 t) 
{ 
  if (m_enabled)
  {
    *m_buffer << t;
  }

  return maybeSpace();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator << (u16 t) 
{ 
  if (m_enabled)
  {
    *m_buffer << t;
  }
    
  return maybeSpace();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator << (s32 t) 
{ 
  if (m_enabled)
  {
    *m_buffer << t;
  }

  return maybeSpace();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator << (u32 t) 
{ 
  if (m_enabled)
  {
    *m_buffer << t;
  }

  return maybeSpace();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//Debug& Debug::operator << (size_t t)
//{
//  m_buffer += String::Format("%lu", t);
//  return maybeSpace();
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator << (s64 t) 
{ 
  if (m_enabled)
  {
    *m_buffer << t;
  }

  return maybeSpace();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator << (u64 t) 
{ 
  if (m_enabled)
  {
    *m_buffer << t;
  }

  return maybeSpace();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator << (float32 t) 
{ 
  if (m_enabled)
  {
    *m_buffer << t;
  }

  return maybeSpace();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator << (const char* t) 
{ 
  if (m_enabled)
  {
    *m_buffer << t;
  }

  return maybeSpace();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator << (const String& t)
{
  if (m_enabled)
  {
    *m_buffer << t;
  }

  return maybeSpace();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator << (const void* t) 
{ 
  if (m_enabled)
  {
    // TAGE - proper implementation needed once ISerializable is fxed
    //EGE_ASSERT(4 == sizeof (t));
    *m_buffer << reinterpret_cast<u32>(t);
  }

  return maybeSpace();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::operator = (const Debug& other)
{
  if (this != &other)
  {
    m_buffer          = other.m_buffer;
    m_spaceSeperated  = other.m_spaceSeperated;
    m_type            = other.m_type;
    m_enabled         = other.m_enabled;
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::space()
{ 
  m_spaceSeperated = true; 
  return maybeSpace(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::nospace() 
{ 
  m_spaceSeperated = false; 
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug& Debug::maybeSpace() 
{ 
  if (m_enabled && m_spaceSeperated)
  {
    *m_buffer << " ";
  }
  
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::PrintWithArgs(const char* string, ...)
{
  char buffer[256];

	va_list arg;
	va_start(arg, string);
	vsprintf(buffer, string, arg);
	va_end(arg);

  Print(buffer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::EnableNames(const StringList& names)
{
  for (StringList::const_iterator it = names.begin(); it != names.end(); ++it)
  {
    if ( ! l_enabledName.contains(*it))
    {
      // add to pool
      l_enabledName.push_back(*it);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const PStringBuffer& Debug::buffer() const
{
  return m_buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END