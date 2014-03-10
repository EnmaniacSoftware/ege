#ifndef EGE_CORE_COMPLEXTYPES_H
#define EGE_CORE_COMPLEXTYPES_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Integer, PInteger)
EGE_DECLARE_SMART_CLASS(Float, PFloat)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Integer : public Object
{
  public:

    Integer(const Integer& other);
    explicit Integer(s32 value);

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns value. */
    s32 value() const;
    
  private:

    /*! Integer value. */
    s32 m_value;
};

class Float : public Object
{
  public:

    Float(const Float& other);
    explicit Float(float32 value);

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns value. */
    float32 value() const;

  private:

    /*! Float value. */
    float32 m_value;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_COMPLEXTYPES_H