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
/*! Returns TRUE if element is valid object */
bool XmlAttributePrivate::isValid() const 
{ 
  return (NULL != m_attribute); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns name. */
String XmlAttributePrivate::name() const
{
  if (isValid())
  {
    return m_attribute->Name();
  }

  return String();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns value. */
String XmlAttributePrivate::value() const
{
  if (isValid())
  {
    return m_attribute->Value();
  }

  return String();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns next attribute. NULL if this is last one. */
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
/*! Sets internal TinyXML attribute pointer. */
void XmlAttributePrivate::setAttribute(TiXmlAttribute* attribute)
{
  m_attribute = attribute;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END