#include "Core/XML/TinyXml/XmlElementTinyXml_p.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(XmlElementPrivate)
EGE_DEFINE_DELETE_OPERATORS(XmlElementPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::XmlElementPrivate(XmlElement* base) : m_base(base), m_element(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::XmlElementPrivate(TiXmlElement* element, XmlElement* base) : m_base(base), m_element(element)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::~XmlElementPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if element is valid object */
bool XmlElementPrivate::isValid() const 
{ 
  return (NULL != m_element); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns value of given attribute if present */
EGEString XmlElementPrivate::attribute(const EGEString& name) const
{
  return (isValid() && m_element->Attribute(name.toAscii())) ? m_element->Attribute(name.toAscii()) : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns first child element. */
XmlElementPrivate* XmlElementPrivate::firstChild() const
{
  return ege_new XmlElementPrivate(isValid() ? m_element->FirstChildElement() : NULL, m_base);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns next child element after given element. */
XmlElementPrivate* XmlElementPrivate::nextChild() const
{
  return ege_new XmlElementPrivate(isValid() ? m_element->NextSiblingElement() : NULL, m_base);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns element name. */
EGEString XmlElementPrivate::name() const
{
  return isValid() ? m_element->Value() : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets internal TinyXML element pointer. */
void XmlElementPrivate::setElement(TiXmlElement* element)
{
  m_element = element;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
