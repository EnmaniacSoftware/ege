#ifndef EGE_CORE_XMLELEMENT_PRIVATE_H
#define EGE_CORE_XMLELEMENT_PRIVATE_H

/**
* @file  XmlElementTinyXml_p.h
* @brief This is private implementation header file of XMLElement class based on TinyXML library. It is meant to be a wrapper around TiXmlElement.
*        Due to fact that TiXmlElement are owned by parent TiXmlDocument this wrapper is not supposed to dispose of TiXmlDocument on its own.
*
*/

#include <EGE.h>
#include <EGEString.h>
#include <tinyxml.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class XmlElement;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class XmlElementPrivate
{
   friend class XmlDocument;

  public:

    XmlElementPrivate(XmlElement* base);
    XmlElementPrivate(XmlElement* base, const String& name);
   ~XmlElementPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if element is valid object */
    bool isValid() const;
    /* Returns value of given attribute if present. Otherwise, returns default value. */
    String attribute(const String& name, const String& defValue) const;
    /* Sets attribute with a given value. 
     * @note  Attribute will be created if does not exists. Otherwise, its value is going to be changed.
     */
    void setAttribute(const String& name, const String& value);
    /* Returns TRUE if given attribute exists. */
    bool containsAttribute(const String& name) const;
    /* Returns first child element. */
    XmlElementPrivate* firstChild() const;
    /* Returns next child element. */
    XmlElementPrivate* nextChild() const;
    /* Returns element name. */
    String name() const;
    /* Appends new child element. */
    void appendChildElement(const XmlElementPrivate* element);
    /* Sets internal TinyXML element pointer. */
    void setElement(TiXmlElement* element);
    /*! Returns pointer to TinyXML element object. */
    inline TiXmlElement* element(bool releaseOwnership) const { if (releaseOwnership) { m_deallocElement = false; } return m_element; }


  private:  

    XmlElementPrivate(TiXmlElement* element, XmlElement* base);

  private:

    /*! Pointer to base object. */
    XmlElement* m_base;
    /*! Pointer to TinyXML element object. */
    TiXmlElement* m_element;
    /*! TiXmlElement deallocation flag. If TRUE m_element was allocated by framework and needs to be deallocated. */
    mutable bool m_deallocElement;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_XMLELEMENT_PRIVATE_H
