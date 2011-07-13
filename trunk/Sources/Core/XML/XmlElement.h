#ifndef EGE_CORE_XMLELEMENT_H
#define EGE_CORE_XMLELEMENT_H

#include <EGE.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class XmlElementPrivate;

EGE_DECLARE_SMART_CLASS(XmlElement, PXmlElement)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class XmlElement : public Object
{
   friend class XmlDocumentPrivate;

  public:

    XmlElement();
   ~XmlElement();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if element is valid object */
    bool isValid() const;
    /* Returns value of given attribute if present */
    EGEString attribute(const EGEString& name) const;
    /* Returns TRUE if given attribute exists. */
    bool containsAttribute(const EGEString& name) const;
    /* Returns first child element. */
    PXmlElement firstChild() const;
    /* Returns next child element. */
    PXmlElement nextChild() const;
    /* Returns element name. */
    EGEString name() const;

  private:

    XmlElement(XmlElementPrivate* p);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(XmlElement);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_XMLELEMENT_H
