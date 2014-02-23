#ifndef EGE_CORE_XML_XMLATTRIBUTE_PRIVATE_H
#define EGE_CORE_XML_XMLATTRIBUTE_PRIVATE_H

/**
* @file  XmlAttributeTinyXml_p.h
* @brief This is private implementation header file of XMLAttribute class based on TinyXML library. It is meant to be a wrapper around TiXmlAttribute.
*
*/

#include "EGE.h"
#include "EGEString.h"
#include <tinyxml.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(XmlAttribute, PXmlAttribute)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class XmlAttributePrivate
{
  public:

    XmlAttributePrivate(XmlAttribute* base);
   ~XmlAttributePrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns TRUE if element is valid object */
    bool isValid() const;
    /*! Returns name. */
    String name() const;
    /*! Returns value. */
    String value() const;
    /*! Returns next attribute. NULL if this is last one. */
    PXmlAttribute next() const;
    /*! Sets internal TinyXML attribute pointer. */
    void setAttribute(TiXmlAttribute* attribute);

  private:

    /*! Pointer to base object. */
    XmlAttribute* m_base;
    /*! Pointer to TinyXML element object. */
    TiXmlAttribute* m_attribute;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_XML_XMLATTRIBUTE_PRIVATE_H
