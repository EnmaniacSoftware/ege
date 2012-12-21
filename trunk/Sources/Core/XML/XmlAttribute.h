#ifndef EGE_CORE_XML_XMLATTRIBUTE_H
#define EGE_CORE_XML_XMLATTRIBUTE_H

#include <EGE.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(XmlAttribute, PXmlAttribute)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class XmlAttribute : public Object
{
  friend class XmlElementPrivate;

  public:

    XmlAttribute();
   ~XmlAttribute();

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

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(XmlAttribute);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_XML_XMLATTRIBUTE_H
