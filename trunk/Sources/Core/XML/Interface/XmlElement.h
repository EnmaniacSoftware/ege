#ifndef EGE_CORE_XML_XMLELEMENT_H
#define EGE_CORE_XML_XMLELEMENT_H

#include "EGE.h"
#include "EGEString.h"
#include "EGETime.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class XmlElementPrivate;

EGE_DECLARE_SMART_CLASS(XmlElement, PXmlElement)
EGE_DECLARE_SMART_CLASS(XmlAttribute, PXmlAttribute)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class XmlElement : public Object
{
   friend class XmlDocumentPrivate;

  public:

    XmlElement(const String& name);
   ~XmlElement();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns TRUE if element is valid object */
    bool isValid() const;
    /*! Returns element name. */
    String name() const;

    /*! Returns value of given attribute if present. Otherwise, returns default value. */
    String attribute(const String& name, const String& defValue) const;
    /*! Returns value of given attribute if present. Otherwise, returns default value. */
    String attribute(const String& name, const char* defValue = NULL) const;
    /*! Returns value of given attribute if present. Otherwise, returns default value. */
    s32 attribute(const String& name, s32 defValue) const;
    /*! Returns value of given attribute if present. Otherwise, returns default value. */
    bool attribute(const String& name, bool defValue) const;
    /*! Returns value of given attribute if present. Otherwise, returns default value. */
    s64 attribute(const String& name, s64 defValue) const;
    /*! Returns value of given attribute if present. Otherwise, returns default value. */
    float32 attribute(const String& name, float32 defValue) const;
    /*! Returns value of given attribute if present. Otherwise, returns default value. */
    Time attribute(const String& name, const Time& defValue) const;

    /*! Sets attribute with a given c-string value. 
     *  @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
     */
    bool setAttribute(const String& name, const char* value);
    /*! Sets attribute with a given string value. 
     *  @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
     */
    bool setAttribute(const String& name, const String& value);
    /*! Sets attribute with a given integer value. 
     *  @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
     */
    bool setAttribute(const String& name, s32 value);
    /*! Sets attribute with a given 64-bit integer value. 
     *  @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
     */
    bool setAttribute(const String& name, s64 value);
    /*! Sets attribute with a given boolean value. 
     *  @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
     */
    bool setAttribute(const String& name, bool value);
    /*! Sets attribute with a given float value. 
     *  @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
     */
    bool setAttribute(const String& name, float32 value);
    /*! Sets attribute with a given Time value. 
     *  @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
     */
    bool setAttribute(const String& name, const Time& value);

    /*! Returns TRUE if given attribute exists. */
    bool hasAttribute(const String& name) const;
    /*! Returns first attribute. */
    PXmlAttribute firstAttribute() const;

    /*! Returns first child element. If any name is given returned will be first element with given name. */
    PXmlElement firstChild(const String& name = "") const;
    /*! Returns next child (sibling) element. If any name is given returned will be next element with given name. */
    PXmlElement nextChild(const String& name = "") const;
    /*! Appends new child element. */
    bool appendChildElement(const PXmlElement& element);

  private:

    /*! Only intended to be used by XmlDocumentPrivate. */
    XmlElement();
    XmlElement(XmlElementPrivate* p);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(XmlElement);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_XML_XMLELEMENT_H
