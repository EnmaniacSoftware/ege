#include "Core/Xml/XmlElement.h"
#include "Core/Xml/XmlAttribute.h"

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
/*! Returns TRUE if element is valid object */
bool XmlElement::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns value of given attribute if present. Otherwise, returns default value. */
String XmlElement::attribute(const String& name, const String& defValue) const
{
  if (isValid())
  {
    return p_func()->attribute(name, defValue);
  }

  return defValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns value of given attribute if present. Otherwise, returns default value. */
String XmlElement::attribute(const String& name, const char* defValue) const
{
  if (isValid())
  {
    return p_func()->attribute(name, String(defValue));
  }

  return String(defValue);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns value of given attribute if present. Otherwise, returns default value. */
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
/*! Returns value of given attribute if present. Otherwise, returns default value. */
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
/*! Returns value of given attribute if present. Otherwise, returns default value. */
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
/*! Returns TRUE if given attribute exists. */
bool XmlElement::hasAttribute(const String& name) const
{
  if (isValid())
  {
    return p_func()->hasAttribute(name);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets attribute with a given c-string value. 
 *  @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
 */
bool XmlElement::setAttribute(const String& name, const char* value)
{
  if (isValid())
  {
    return p_func()->setAttribute(name, String(value));
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets attribute with a given string value. 
 * @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
 */
bool XmlElement::setAttribute(const String& name, const String& value)
{
  if (isValid())
  {
    return p_func()->setAttribute(name, value);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets attribute with a given boolean value. 
 * @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
 */
bool XmlElement::setAttribute(const String& name, bool value)
{
  if (isValid())
  {
    return p_func()->setAttribute(name, value ? "true" : "false");
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets attribute with a given integer value. 
 *  @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
 */
bool XmlElement::setAttribute(const String& name, s32 value)
{
  if (isValid())
  {
    return p_func()->setAttribute(name, String::Format("%d", value));
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets attribute with a given 64-bit integer value. 
  * @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
  */
bool XmlElement::setAttribute(const String& name, s64 value)
{
  if (isValid())
  {
    return p_func()->setAttribute(name, String::Format("%ld", value));
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns first child element. If any name is given returned will be first element with given name. */
PXmlElement XmlElement::firstChild(const String& name) const
{
  return ege_new XmlElement(isValid() ? p_func()->firstChild(name) : NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns next child element after given element. If any name is given returned will be next element with given name. */
PXmlElement XmlElement::nextChild(const String& name) const
{
  return ege_new XmlElement(isValid() ? p_func()->nextChild(name) : NULL);
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
/*! Appends new child element. */
bool XmlElement::appendChildElement(const PXmlElement& element)
{
  if (isValid())
  {
    return p_func()->appendChildElement(element->p_func());
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns first attribute. */
PXmlAttribute XmlElement::firstAttribute() const
{
  if (isValid())
  {
    return p_func()->firstAttribute();
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
