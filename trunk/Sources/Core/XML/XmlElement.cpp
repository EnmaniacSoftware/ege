#include "Core/Xml/XmlElement.h"

#if EGE_XML_TINYXML
#include "Core/XML/TinyXml/XMLElementTinyXML_p.h"
#endif // EGE_XML_TINYXML

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(XmlElement)
EGE_DEFINE_DELETE_OPERATORS(XmlElement)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElement::XmlElement() : Object(NULL)
{
  m_p = ege_new XmlElementPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElement::XmlElement(XmlElementPrivate* p) : Object(NULL), m_p(p)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElement::~XmlElement()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if element is valid object */
bool XmlElement::isValid() const
{
  return NULL != m_p && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns value of given attribute if present */
String XmlElement::attribute(const String& name) const
{
  if (isValid())
  {
    return p_func()->attribute(name);
  }

  return String();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given attribute exists. */
bool XmlElement::containsAttribute(const String& name) const
{
  if (isValid())
  {
    return p_func()->containsAttribute(name);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns first child element. */
PXmlElement XmlElement::firstChild() const
{
  return ege_new XmlElement(isValid() ? p_func()->firstChild() : NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns next child element after given element. */
PXmlElement XmlElement::nextChild() const
{
  return ege_new XmlElement(isValid() ? p_func()->nextChild() : NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns element name. */
String XmlElement::name() const
{
  if (isValid())
  {
    return p_func()->name();
  }

  return String();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------