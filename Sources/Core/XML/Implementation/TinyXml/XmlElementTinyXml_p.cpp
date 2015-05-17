#include "Core/XML/Implementation/TinyXml/XmlElementTinyXml_p.h"
#include "Core/XML/Implementation/TinyXml/XmlAttributeTinyXml_p.h"
#include "Core/XML/Interface/XmlAttribute.h"
#include "EGEAssert.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(XmlElementPrivate)
EGE_DEFINE_DELETE_OPERATORS(XmlElementPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::XmlElementPrivate(XmlElement* base) 
: m_base(base) 
, m_element(NULL) 
, m_deallocElement(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::XmlElementPrivate(XmlElement* base, const String& name) 
: m_base(base)
, m_deallocElement(true)
{
  m_element = new TiXmlElement(name.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate::XmlElementPrivate(TiXmlElement* element, XmlElement* base) 
: m_base(base)
, m_deallocElement(false)
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
bool XmlElementPrivate::isValid() const 
{ 
  return (NULL != m_element); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String XmlElementPrivate::attribute(const String& name, const String& defValue) const
{
  return (isValid() && m_element->Attribute(name.toAscii())) ? m_element->Attribute(name.toAscii()) : defValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElementPrivate::hasAttribute(const String& name) const
{
  return isValid() && m_element->Attribute(name.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElementPrivate::setAttribute(const String& name, const String& value)
{
  if (isValid())
  {
    m_element->SetAttribute(name.toAscii(), value.toAscii());
    return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate* XmlElementPrivate::firstChild(const String& name) const
{
  XmlElementPrivate* priv = NULL;
  
  TiXmlElement* elem = name.isEmpty() ? m_element->FirstChildElement() : m_element->FirstChildElement(name.toAscii());

  if (elem)
  {
    priv = ege_new XmlElementPrivate(elem, m_base);
  }

  return priv;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElementPrivate* XmlElementPrivate::nextChild(const String& name) const
{
  XmlElementPrivate* priv = NULL;

  TiXmlElement* elem = name.isEmpty() ? m_element->NextSiblingElement() : m_element->NextSiblingElement(name.toAscii());
  if (elem)
  {
    priv = ege_new XmlElementPrivate(elem, m_base);
  }

  return priv;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String XmlElementPrivate::name() const
{
  return isValid() ? m_element->Value() : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void XmlElementPrivate::setElement(TiXmlElement* element)
{
  m_element = element;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElementPrivate::appendChildElement(const XmlElementPrivate* element)
{
  EGE_ASSERT(isValid());
  if (m_element && element)
  {
    // NOTE: taking over ownership
    return (NULL != m_element->LinkEndChild(element->element(true)));
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
String XmlElementPrivate::text() const
{
  EGE_ASSERT(isValid());

  String text;

  if (NULL != m_element)
  {
    // go thru all child siblings
	  for (TiXmlNode* node = m_element->FirstChild(); NULL != node; node = node->NextSibling()) 
	  {
      // check if text node
      if (TiXmlNode::TINYXML_TEXT == node->Type())
      {
        // get text
        text = node->ToText()->Value();
      
        // done
        break;
      }
    }
  }

  return text;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END