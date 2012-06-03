#include "Core/Xml/XmlAttribute.h"

#if EGE_XML_TINYXML
#include "Core/XML/TinyXml/XMLAttributeTinyXML_p.h"
#endif // EGE_XML_TINYXML

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(XmlAttribute)
EGE_DEFINE_DELETE_OPERATORS(XmlAttribute)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlAttribute::XmlAttribute() : Object(NULL)
{
  m_p = ege_new XmlAttributePrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlAttribute::~XmlAttribute()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if element is valid object */
bool XmlAttribute::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns name. */
String XmlAttribute::name() const
{
  if (isValid())
  {
    return p_func()->name();
  }

  return String();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns value. */
String XmlAttribute::value() const
{
  if (isValid())
  {
    return p_func()->value();
  }

  return String();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns next attribute. NULL if this is last one. */
PXmlAttribute XmlAttribute::next() const
{
  if (isValid())
  {
    return p_func()->next();
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END