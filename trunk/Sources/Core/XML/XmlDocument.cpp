#include "Core/XML/XMLDocument.h"
#include "Core/XML/XMLElement.h"
#include "Core/Data/DataBuffer.h"
#include <EGEFile.h>

#if EGE_XML_TINYXML
#include "Core/XML/TinyXml/XMLDocumentTinyXML_p.h"
#endif // EGE_XML_TINYXML

EGE_NAMESPACE_BEGIN

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
EGEResult XmlDocument::load(const String& fileName)
{
  if (isValid())
  {
    File file(fileName);
    if (!file.exists())
    {
      // error!
      return EGE_ERROR_NOT_FOUND;
    }

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
PXmlElement XmlDocument::firstChild(const String& name)
{
  if (isValid())
  {
    return p_func()->firstChild(name);
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves document to a given file. */
EGEResult XmlDocument::save(const String& fileName)
{
  if (isValid())
  {
    return p_func()->save(fileName);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves document to a given buffer. */
EGEResult XmlDocument::save(const PDataBuffer& buffer)
{
  if (isValid())
  {
    return p_func()->save(buffer);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Appends new element to document. */
bool XmlDocument::appendElement(const PXmlElement& element)
{
  if (isValid())
  {
    return p_func()->appendElement(element);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns root element. */
PXmlElement XmlDocument::rootElement()
{
  if (isValid())
  {
    return p_func()->rootElement();
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END