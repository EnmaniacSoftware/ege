#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IResource::IResource(Application* app, ResourceGroup* group, const String& typeName, egeObjectDeleteFunc deleteFunc) 
: Object(app, EGE_OBJECT_UID_RESOURCE, deleteFunc), 
  m_manual(false), 
  m_loaded(false),
  m_typeName(typeName), 
  m_group(group)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IResource::~IResource()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if resource is already loaded. */
bool IResource::isLoaded() const
{
  return m_loaded;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns resource type name. */
const String& IResource::typeName() const 
{ 
  return m_typeName; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if resource is manual. */
bool IResource::isManual() const 
{ 
  return m_manual; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns pointer to owning group. */
ResourceGroup* IResource::group() const
{
  return m_group;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns path to resource. */
const String& IResource::path() const
{
  return m_path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END