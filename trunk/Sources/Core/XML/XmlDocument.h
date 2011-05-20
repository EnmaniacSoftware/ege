#ifndef EGE_CORE_XMLDOCUMENT_H
#define EGE_CORE_XMLDOCUMENT_H

#include "EGE.h"
#include "EGEString.h"

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
    virtual ~XmlDocument();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Loads document from given file. */
    EGEResult load(const EGEString& fileName);
    /* Loads document from given buffer. */
    EGEResult load(const PDataBuffer& buffer);
    /* Returns first child with the given name. */
    PXmlElement firstChild(const EGEString& name);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(XmlDocument);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_XMLDOCUMENT_H
