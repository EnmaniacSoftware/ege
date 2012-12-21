#ifndef EGE_CORE_COMPLEXTYPES_H
#define EGE_CORE_COMPLEXTYPES_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Integer, PInteger)
EGE_DECLARE_SMART_CLASS(Float, PFloat)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Integer : public Object
{
  public:

    Integer(s32 value) : Object(NULL, EGE_OBJECT_UID_INT), m_value(value) {}

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns value. */
    s32 value() const { return m_value; }
    
  private:

    /*! Integer value. */
    s32 m_value;
};

class Float : public Object
{
  public:

    Float(float32 value) : Object(NULL, EGE_OBJECT_UID_FLOAT), m_value(value) {}

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns value. */
    float32 value() const { return m_value; }

  private:

    /*! Float value. */
    float32 m_value;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_COMPLEXTYPES_H