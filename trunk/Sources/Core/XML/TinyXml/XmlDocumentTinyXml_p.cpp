#include "Core/XML/XmlDocument.h"
#include "Core/XML/TinyXml/XmlDocumentTinyXml_p.h"
#include "Core/XML/XmlElement.h"
#include "Core/XML/TinyXml/XmlElementTinyXml_p.h"
#include "Core/Data/DataBuffer.h"
#include <EGEFile.h>

EGE_NAMESPACE_BEGIN

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
EGEResult XmlDocumentPrivate::load(const String& fileName)
{
  // load file
  if ( ! m_xml.LoadFile(fileName.toAscii()))
  {
    // error!
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult XmlDocumentPrivate::load(const PDataBuffer& buffer)
{
  EGEResult result = EGE_ERROR;

  // parse data buffer
  m_xml.Parse(reinterpret_cast<char*>(buffer->data(0)));
  if ( ! m_xml.Error())
  {
    // success
    result = EGE_SUCCESS;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PXmlElement XmlDocumentPrivate::firstChild(const String& name)
{
  PXmlElement element;

  TiXmlNode* node = m_xml.FirstChild(name.toAscii());
  if (node)
  {
    TiXmlElement* elem = node->ToElement();

    if (elem)
    {
      element = ege_new XmlElement();
      if (element)
      {
        element->p_func()->setElement(elem);
      }
    }
  }

  return element;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult XmlDocumentPrivate::save(const String& fileName)
{
  EGEResult result = EGE_SUCCESS;

  DataBuffer buffer;

  // save XML to buffer
  result = save(buffer);
  if (EGE_SUCCESS == result)
  {
    // open file for writting
    File file(fileName);
    if (EGE_SUCCESS == (result = file.open(EGEFile::MODE_WRITE_ONLY)))
    {
      // write buffer into file
      s64 written = file.write(buffer, -1);
      if (written != buffer.size())
      {
        // error!
        result = EGE_ERROR_IO;
      }
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult XmlDocumentPrivate::save(const PDataBuffer& buffer)
{
  // add declaration to the front
  TiXmlDeclaration decl("1.0", "", "no");
  m_xml.InsertBeforeChild(m_xml.FirstChild(), decl);

  TiXmlPrinter printer;

  // setup feel and look
  printer.SetIndent("  ");

  // parse document and generate XML human-readable stream
  m_xml.Accept(&printer);

  // get reference to stream
  const std::string& str = printer.Str();

  // write stream to buffer (including NULL ending char)
  s64 written = buffer->write((void*) str.c_str(), str.length() + 1);
  if ((str.length() + 1) != written)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool XmlDocumentPrivate::appendElement(const PXmlElement& element)
{
  // NOTE: TiXmlDocument takes ownership of element
  return (NULL != m_xml.LinkEndChild(element->p_func()->element(true)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PXmlElement XmlDocumentPrivate::rootElement()
{
  PXmlElement element;

  TiXmlElement* elem = m_xml.RootElement();
  if (elem)
  {
    element = ege_new XmlElement();
    if (element)
    {
      element->p_func()->setElement(elem);
    }
  }

  return element;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END