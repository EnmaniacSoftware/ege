#include "Core/Xml/Interface/XmlAttribute.h"
#include "EGEDebug.h"

#if EGE_XML_TINYXML
  #include "Core/XML/Implementation/TinyXml/XMLAttributeTinyXML_p.h"
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
bool XmlAttribute::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String XmlAttribute::name() const
{
  if (isValid())
  {
    return p_func()->name();
  }

  return String();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String XmlAttribute::value() const
{
  if (isValid())
  {
    return p_func()->value();
  }

  return String();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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