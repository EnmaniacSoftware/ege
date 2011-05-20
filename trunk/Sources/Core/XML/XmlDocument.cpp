#include "Core/XML/XMLDocument.h"
#include "Core/XML/XMLElement.h"
#include "Core/Data/DataBuffer.h"

#if EGE_XML_TINYXML
#include "Core/XML/TinyXml/XMLDocumentTinyXML_p.h"
#endif // EGE_XML_TINYXML

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(XmlDocument)
EGE_DEFINE_DELETE_OPERATORS(XmlDocument)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlDocument::XmlDocument() : Object(NULL)
{
  m_p = ege_new XmlDocumentPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlDocument::~XmlDocument()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool XmlDocument::isValid() const
{
  return NULL != m_p;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads document from given file. */
EGEResult XmlDocument::load(const EGEString& fileName)
{
  if (isValid())
  {
    return p_func()->load(fileName);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads document from given buffer. */
EGEResult XmlDocument::load(const PDataBuffer& buffer)
{
  if (isValid())
  {
    return p_func()->load(buffer);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns first child with the given name. */
PXmlElement XmlDocument::firstChild(const EGEString& name)
{
  if (isValid())
  {
    return p_func()->firstChild(name);
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
