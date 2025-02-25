#include "Core/EGE.h"

using namespace EGE;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

CObject::CObject() : m_uiReferences(0)
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

CObject::~CObject()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

u32 CObject::getUID() const
{
  return EGE_OBJECT_UID_GENERIC;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGEResult CObject::construct(CEGEngine* /*pcEngine*/)
{
  return EGE_SUCCESS;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
