#include "Core/XML/TinyXml/XmlElementTinyXml_p.h"
#include "Core/XML/XmlAttribute.h"
#include "Core/XML/TinyXml/XmlAttributeTinyXml_p.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(XmlElementPrivate)
EGE_DEFINE_DELETE_OPERATORS(XmlElementPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::XmlElementPrivate(XmlElement* base) : m_base(base), m_element(NULL), m_deallocElement(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::XmlElementPrivate(XmlElement* base, const String& name) : m_base(base), m_deallocElement(true)
{
  m_element = new TiXmlElement(name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::XmlElementPrivate(TiXmlElement* element, XmlElement* base) : m_base(base), m_deallocElement(false)
{
  m_element = element;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::~XmlElementPrivate()
{
  if (m_deallocElement)
  {
    EGE_DELETE(m_element);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if element is valid object */
bool XmlElementPrivate::isValid() const 
{ 
  return (NULL != m_element); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns value of given attribute if present. Otherwise, returns default value. */
String XmlElementPrivate::attribute(const String& name, const String& defValue) const
{
  return (isValid() && m_element->Attribute(name.toAscii())) ? m_element->Attribute(name.toAscii()) : defValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given attribute exists. */
bool XmlElementPrivate::hasAttribute(const String& name) const
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
/*! Returns first child element. If any name is given returned will be first element with given name. */
XmlElementPrivate* XmlElementPrivate::firstChild(const String& name) const
{
  XmlElementPrivate* priv = NULL;
  
  TiXmlElement* elem = name.empty() ? m_element->FirstChildElement() : m_element->FirstChildElement(name);

  if (elem)
  {
    priv = ege_new XmlElementPrivate(elem, m_base);
  }

  return priv;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns next child element after given element. If any name is given returned will be next element with given name. */
XmlElementPrivate* XmlElementPrivate::nextChild(const String& name) const
{
  XmlElementPrivate* priv = NULL;

  TiXmlElement* elem = name.empty() ? m_element->NextSiblingElement() : m_element->NextSiblingElement(name);
  if (elem)
  {
    priv = ege_new XmlElementPrivate(elem, m_base);
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
void XmlElementPrivate::setElement(TiXmlElement* element)
{
  m_element = element;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Appends new child element. */
void XmlElementPrivate::appendChildElement(const XmlElementPrivate* element)
{
  EGE_ASSERT(isValid());
  if (m_element && element)
  {
    // NOTE: taking over ownership
    m_element->LinkEndChild(element->element(true));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns first attribute. */
PXmlAttribute XmlElementPrivate::firstAttribute() const
{
  EGE_ASSERT(isValid());

  PXmlAttribute attribute;

  if (m_element)
  {
    TiXmlAttribute* attrib = m_element->FirstAttribute();
    if (attrib)
    {
      attribute = ege_new XmlAttribute();
      if (attribute)
      {
        attribute->p_func()->setAttribute(attrib);
      }
    }
  }

  return attribute;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
