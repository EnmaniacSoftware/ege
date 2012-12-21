#include "Core/XML/TinyXml/XmlAttributeTinyXml_p.h"
#include "Core/XML/XmlAttribute.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(XmlAttributePrivate)
EGE_DEFINE_DELETE_OPERATORS(XmlAttributePrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlAttributePrivate::XmlAttributePrivate(XmlAttribute* base) : m_base(base), 
                                                               m_attribute(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlAttributePrivate::~XmlAttributePrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlAttributePrivate::isValid() const 
{ 
  return (NULL != m_attribute); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String XmlAttributePrivate::name() const
{
  if (isValid())
  {
    return m_attribute->Name();
  }

  return String();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String XmlAttributePrivate::value() const
{
  if (isValid())
  {
    return m_attribute->Value();
  }

  return String();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PXmlAttribute XmlAttributePrivate::next() const
{
  PXmlAttribute attribute;

  if (isValid())
  {
    TiXmlAttribute* next = m_attribute->Next();
    if (next)
    {
      attribute = ege_new XmlAttribute();
      if (attribute)
      {
        attribute->p_func()->setAttribute(next);
      }
    }
  }

  return attribute;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void XmlAttributePrivate::setAttribute(TiXmlAttribute* attribute)
{
  m_attribute = attribute;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END