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

    XmlElement(const String& name);
   ~XmlElement();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if element is valid object */
    bool isValid() const;
    /* Returns value of given attribute if present */
    String attribute(const String& name) const;
    /* Sets attribute with a given value. 
     * @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
     */
    void setAttribute(const String& name, const String& value);
    /* Returns TRUE if given attribute exists. */
    bool containsAttribute(const String& name) const;
    /* Returns first child element. */
    PXmlElement firstChild() const;
    /* Returns next child element. */
    PXmlElement nextChild() const;
    /* Returns element name. */
    String name() const;
    /* Appends new child element. */
    void appendChildElement(const PXmlElement& element);

  private:

    /* Only intended to be used by XmlDocumentPrivate. */
    XmlElement();
    XmlElement(XmlElementPrivate* p);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(XmlElement);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_XMLELEMENT_H
