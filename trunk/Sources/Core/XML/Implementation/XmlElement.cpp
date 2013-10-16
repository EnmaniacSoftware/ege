#include "Core/Xml/Interface/XmlElement.h"
#include "Core/Xml/Interface/XmlAttribute.h"
#include "EGEStringUtils.h"

#if EGE_XML_TINYXML
  #include "Core/XML/Implementation/TinyXml/XMLElementTinyXML_p.h"
#endif // EGE_XML_TINYXML

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(XmlElement)
EGE_DEFINE_DELETE_OPERATORS(XmlElement)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElement::XmlElement() : Object(NULL)
{
  m_p = ege_new XmlElementPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElement::XmlElement(const String& name) : Object(NULL)
{
  m_p = ege_new XmlElementPrivate(this, name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElement::XmlElement(XmlElementPrivate* p) : Object(NULL), 
                                               m_p(p)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlElement::~XmlElement()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElement::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String XmlElement::attribute(const String& name, const String& defValue) const
{
  if (isValid())
  {
    return p_func()->attribute(name, defValue);
  }

  return defValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String XmlElement::attribute(const String& name, const char* defValue) const
{
  if (isValid())
  {
    return p_func()->attribute(name, String(defValue));
  }

  return String(defValue);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 XmlElement::attribute(const String& name, s32 defValue) const
{
  s32 value = defValue;

  if (isValid())
  {
    bool error = false;
    value = p_func()->attribute(name, "").toInt(&error);
    if (error)
    {
      value = defValue;
    }
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElement::attribute(const String& name, bool defValue) const
{
  bool value = defValue;

  if (isValid())
  {
    bool error = false;
    value = p_func()->attribute(name, "").toBool(&error);
    if (error)
    {
      value = defValue;
    }
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 XmlElement::attribute(const String& name, s64 defValue) const
{
  s64 value = defValue;

  if (isValid())
  {
    bool error = false;
    value = p_func()->attribute(name, "").toInt64(&error);
    if (error)
    {
      value = defValue;
    }
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 XmlElement::attribute(const String& name, float32 defValue) const
{
  float32 value = defValue;

  if (isValid())
  {
    bool error = false;
    value = p_func()->attribute(name, "").toFloat(&error);
    if (error)
    {
      value = defValue;
    }
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Time XmlElement::attribute(const String& name, const Time& defValue) const
{
  Time value = defValue;

  if (isValid())
  {
    bool error = false;
    value = StringUtils::ToTime(p_func()->attribute(name, ""), &error);
    if (error)
    {
      value = defValue;
    }
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElement::hasAttribute(const String& name) const
{
  if (isValid())
  {
    return p_func()->hasAttribute(name);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElement::setAttribute(const String& name, const char* value)
{
  if (isValid())
  {
    return p_func()->setAttribute(name, String(value));
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElement::setAttribute(const String& name, const String& value)
{
  if (isValid())
  {
    return p_func()->setAttribute(name, value);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElement::setAttribute(const String& name, bool value)
{
  if (isValid())
  {
    return p_func()->setAttribute(name, value ? "true" : "false");
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElement::setAttribute(const String& name, s32 value)
{
  if (isValid())
  {
    return p_func()->setAttribute(name, String::Format("%d", value));
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElement::setAttribute(const String& name, s64 value)
{
  if (isValid())
  {
    return p_func()->setAttribute(name, String::Format("%ld", value));
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElement::setAttribute(const String& name, float32 value)
{
  if (isValid())
  {
    return p_func()->setAttribute(name, String::Format("%f", value));
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElement::setAttribute(const String& name, const Time& value)
{
  return setAttribute(name, value.seconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PXmlElement XmlElement::firstChild(const String& name) const
{
  return ege_new XmlElement(isValid() ? p_func()->firstChild(name) : NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PXmlElement XmlElement::nextChild(const String& name) const
{
  return ege_new XmlElement(isValid() ? p_func()->nextChild(name) : NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String XmlElement::name() const
{
  if (isValid())
  {
    return p_func()->name();
  }

  return String();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlElement::appendChildElement(const PXmlElement& element)
{
  if (isValid())
  {
    return p_func()->appendChildElement(element->p_func());
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PXmlAttribute XmlElement::firstAttribute() const
{
  if (isValid())
  {
    return p_func()->firstAttribute();
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END