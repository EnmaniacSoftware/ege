#ifndef EGE_CORE_XMLDOCUMENT_PRIVATE_H
#define EGE_CORE_XMLDOCUMENT_PRIVATE_H

/**
* @file  XmlDocumentTinyXml_p.h
* @brief This is private implementation header file of XMLDocument class based on TinyXML library. It is meant to be a wrapper around TiXmlDocument.
*
*/

#include "EGE.h"
#include "String.h"
#include <tinyxml.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class XmlElement;

EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class XmlDocumentPrivate
{
  public:

    XmlDocumentPrivate(XmlDocument* base);
   ~XmlDocumentPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Loads document from given file. */
    EGEResult load(const String& fileName);
    /* Loads document from given buffer. */
    EGEResult load(const PDataBuffer& buffer);
    /* Saves document to a given file. */
    EGEResult save(const String& fileName);
    /* Saves document to a given buffer. */
    EGEResult save(const PDataBuffer& buffer);

    /* Returns first child with the given name. */
    PXmlElement firstChild(const String& name);
    /* Appends new element to document. */
    void appendElement(const PXmlElement& element);

  private:

    /*! Pointer to base object. */
    XmlDocument* m_base;
    /*! Underlying XML document object. */
    TiXmlDocument m_xml;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_XMLDOCUMENT_PRIVATE_H
