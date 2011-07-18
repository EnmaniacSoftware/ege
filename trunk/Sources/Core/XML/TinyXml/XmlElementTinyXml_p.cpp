#include "Core/XML/TinyXml/XmlElementTinyXml_p.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(XmlElementPrivate)
EGE_DEFINE_DELETE_OPERATORS(XmlElementPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::XmlElementPrivate(XmlElement* base) : m_base(base)
{
  m_element = new TiXmlElement("");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::XmlElementPrivate(XmlElement* base, const String& name) : m_base(base)
{
  m_element = new TiXmlElement(name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::XmlElementPrivate(const TiXmlElement& element, XmlElement* base) : m_base(base)
{
  m_element = new TiXmlElement(element);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::~XmlElementPrivate()
{
  EGE_DELETE(m_element);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if element is valid object */
bool XmlElementPrivate::isValid() const 
{ 
  return (NULL != m_element); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns value of given attribute if present */
String XmlElementPrivate::attribute(const String& name) const
{
  return (isValid() && m_element->Attribute(name.toAscii())) ? m_element->Attribute(name.toAscii()) : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given attribute exists. */
bool XmlElementPrivate::containsAttribute(const String& name) const
{
  return isValid() && m_element->Attribute(name.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets attribute with a given value. 
 * @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
 */
void XmlElementPrivate::setAttribute(const String& name, const String& value)
{
  if (isValid())
  {
    m_element->SetAttribute(name, value);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns first child element. */
XmlElementPrivate* XmlElementPrivate::firstChild() const
{
  XmlElementPrivate* priv = NULL;
  
  const TiXmlElement* elem = m_element->FirstChildElement();
  if (elem)
  {
    priv = ege_new XmlElementPrivate(*elem, m_base);
  }

  return priv;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns next child element after given element. */
XmlElementPrivate* XmlElementPrivate::nextChild() const
{
  XmlElementPrivate* priv = NULL;

  const TiXmlElement* elem = m_element->NextSiblingElement();
  if (elem)
  {
    priv = ege_new XmlElementPrivate(*elem, m_base);
  }

  return priv;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns element name. */
String XmlElementPrivate::name() const
{
  return isValid() ? m_element->Value() : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer to TinyXML element object. */
void XmlElementPrivate::setElement(const TiXmlElement* element)
{
  EGE_ASSERT(isValid());

  if (isValid() && element)
  {
    *m_element = *element;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
