#ifndef EGE_CORE_XMLDOCUMENT_H
#define EGE_CORE_XMLDOCUMENT_H

#include <EGE.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class XmlElement;

EGE_DECLARE_SMART_CLASS(XmlDocument, PXmlDocument)
EGE_DECLARE_SMART_CLASS(XmlElement, PXmlElement)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class XmlDocument : public Object
{
  public:

    XmlDocument();
   ~XmlDocument();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Loads document from a given file. */
    EGEResult load(const String& fileName);
    /* Loads document from a given buffer. */
    EGEResult load(const PDataBuffer& buffer);
    /* Saves document to a given file. */
    EGEResult save(const String& fileName);
    /* Saves document to a given buffer. */
    EGEResult save(const PDataBuffer& buffer);

    /* Returns first child with the given name. */
    PXmlElement firstChild(const String& name);
    /* Appends new element to document. */
    bool appendElement(const PXmlElement& element);

    /* Returns root element. */
    PXmlElement rootElement();

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(XmlDocument);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_XMLDOCUMENT_H
