#include "Core/XML/XmlDocument.h"
#include "Core/XML/TinyXml/XmlDocumentTinyXml_p.h"
#include "Core/XML/XmlElement.h"
#include "Core/XML/TinyXml/XmlElementTinyXml_p.h"
#include "Core/Data/DataBuffer.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(XmlDocumentPrivate)
EGE_DEFINE_DELETE_OPERATORS(XmlDocumentPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlDocumentPrivate::XmlDocumentPrivate(XmlDocument* base) : m_base(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
XmlDocumentPrivate::~XmlDocumentPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads document from given file. */
EGEResult XmlDocumentPrivate::load(const EGEString& fileName)
{
  // load file
  if (!m_xml.LoadFile(fileName.toAscii()))
  {
    // error!
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads document from given buffer. */
EGEResult XmlDocumentPrivate::load(const PDataBuffer& buffer)
{
  // parse data buffer
  m_xml.Parse((const char*) buffer->data(0));
  if (m_xml.Error())
  {
    // error!
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns first child with the given name. */
PXmlElement XmlDocumentPrivate::firstChild(const EGEString& name)
{
  PXmlElement element;

  TiXmlNode* node = m_xml.FirstChild(name.toAscii());

  if (node)
  {
    element = ege_new XmlElement();
    if (element)
    {
      element->p_func()->setElement(node->ToElement());
    }
  }

  return element;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
